#include "test.hh"

#include <cryptography/Output.hh>
#include <cryptography/Input.hh>
#include <cryptography/random.hh>

#include <elle/serialize/insert.hh>
#include <elle/serialize/extract.hh>
#include <elle/serialize/Base64Archive.hh>

/*----------.
| Represent |
`----------*/

void
test_represent()
{
  // These generate base64-based representations which can be used in
  // other tests.

  // 1)
  {
    infinit::cryptography::Output output(
      infinit::cryptography::random::generate<elle::Buffer>(128));
    elle::String archive;
    elle::serialize::to_string<
      elle::serialize::OutputBase64Archive>(archive) << output;
    elle::printf("[representation 1] %s\n", archive);
  }
}

/*----------.
| Construct |
`----------*/

void
test_construct()
{
  // By size.
  infinit::cryptography::Output output1(64);

  BOOST_CHECK_EQUAL(output1.buffer().size(), 64);

  // By buffer.
  infinit::cryptography::Output output2(
    infinit::cryptography::random::generate<elle::Buffer>(128));

  // Output copy.
  infinit::cryptography::Output output3(output2);

  BOOST_CHECK_EQUAL(output2, output3);

  auto address3 = output3.buffer().contents();

  // Output move.
  infinit::cryptography::Output output4(std::move(output3));

  BOOST_CHECK_EQUAL(output2, output4);

  // Buffer copy.
  infinit::cryptography::Output output5(output4.buffer());

  BOOST_CHECK_EQUAL(output2, output4);
  BOOST_CHECK_EQUAL(output2, output5);

  // Buffer move.
  infinit::cryptography::Output output6(std::move(output4.buffer()));

  BOOST_CHECK_EQUAL(output2, output5);
  BOOST_CHECK_EQUAL(output2, output6);
  BOOST_CHECK_EQUAL(output5, output6);

  auto address6 = output6.buffer().contents();

  BOOST_CHECK(address3 == address6);
}

/*----------.
| Serialize |
`----------*/

void
test_serialize()
{
  // Serialize/deserialize.
  {
    infinit::cryptography::Output output1(
      infinit::cryptography::random::generate<elle::Buffer>(128));

    elle::String archive;
    elle::serialize::to_string(archive) << output1;

    auto extractor = elle::serialize::from_string(archive);
    infinit::cryptography::Output output2(extractor);

    BOOST_CHECK_EQUAL(output1, output2);
  }

  // Deserialize from the hard-coded string [representation 1]: useful
  // for detecting changes in formats.
  {
    elle::String archive1("AAAAAIAAAAAAAAAAYVS/sqT6qs+lo+MdcB1jaZY8gwFx6a10mr1+MjW8to4nvK1hjewkupo66RFvioIZ59hITeuy4CvfQNJSqvdVJ4xLEyWKED9oeuji8jxe1nGOt068HlVyFJ9RkZT4cN6WC8RJXekhokXjI4yr89ad0Vw8KaB5mw5U79CUKaxBVnU=");

    auto extractor =
      elle::serialize::from_string<
        elle::serialize::InputBase64Archive>(archive1);
    infinit::cryptography::Output output(extractor);

    elle::String archive2;
    elle::serialize::to_string<
      elle::serialize::OutputBase64Archive>(archive2) << output;

    BOOST_CHECK_EQUAL(archive1, archive2);
  }
}

/*--------.
| Compare |
`--------*/

void
test_compare()
{
  infinit::cryptography::Output output1(
    infinit::cryptography::random::generate<elle::Buffer>(4096));

  // Output/output.
  infinit::cryptography::Output output2(
    infinit::cryptography::random::generate<elle::Buffer>(4096));

  // With high probabilituy, this should not be the case. Otherwise,
  // the random generator is probably broken.
  BOOST_CHECK(output1 != output2);
  BOOST_CHECK(!(output1 == output2));

  if (output1 < output2)
    {
      BOOST_CHECK(output1 <= output2);
      BOOST_CHECK(!(output1 > output2));
      BOOST_CHECK(!(output1 >= output2));
    }
  else
    {
      BOOST_CHECK(output1 >= output2);
      BOOST_CHECK(!(output1 < output2));
      BOOST_CHECK(!(output1 <= output2));
    }

  // Output/input.
  infinit::cryptography::Input input2(elle::WeakBuffer{output2.buffer()});

  BOOST_CHECK(output1 != input2);
  BOOST_CHECK(!(output1 == input2));

  if (output1 < input2)
    {
      BOOST_CHECK(output1 <= input2);
      BOOST_CHECK(!(output1 > input2));
      BOOST_CHECK(!(output1 >= input2));
    }
  else
    {
      BOOST_CHECK(output1 >= input2);
      BOOST_CHECK(!(output1 < input2));
      BOOST_CHECK(!(output1 <= input2));
    }
}

/*-----.
| Main |
`-----*/

bool
test()
{
  boost::unit_test::test_suite* suite = BOOST_TEST_SUITE("Output");

  // To uncomment if one wants to update the representations.
  //suite->add(BOOST_TEST_CASE(test_represent));

  suite->add(BOOST_TEST_CASE(test_construct));
  suite->add(BOOST_TEST_CASE(test_serialize));
  suite->add(BOOST_TEST_CASE(test_compare));

  boost::unit_test::framework::master_test_suite().add(suite);

  return (true);
}

int
main(int argc,
     char** argv)
{
  return boost::unit_test::unit_test_main(test, argc, argv);
}
