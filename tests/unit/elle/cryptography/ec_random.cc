#include <cryptography/random.hh>

#include <elle/types.hh>
#include <elle/Exception.hh>

// XXX
#include <cryptography/KeyPair.hh>

// XXX
#include <elle/serialize/insert.hh>
#include <elle/serialize/extract.hh>

int
main(int, const char *[])
{
  infinit::cryptography::random::setup();

  elle::Natural32 n =
    infinit::cryptography::random::generate<elle::Natural32>(3, 15);

  // XXX[another test]
  {
    infinit::cryptography::KeyPair kp1 =
      infinit::cryptography::KeyPair::generate(
        infinit::cryptography::Cryptosystem::rsa,
        1024);

    std::cout << kp1 << std::endl;

    infinit::cryptography::KeyPair kp2 =
      infinit::cryptography::KeyPair::generate(
        infinit::cryptography::Cryptosystem::rsa,
        2048);

    std::cout << kp2 << std::endl;

    infinit::cryptography::KeyPair kp3 =
      infinit::cryptography::KeyPair::generate(
        infinit::cryptography::Cryptosystem::rsa,
        512);

    std::cout << kp3 << std::endl;
  }

  // XXX[another test]
  {
    infinit::cryptography::KeyPair kp1 =
      infinit::cryptography::KeyPair::generate(
        infinit::cryptography::Cryptosystem::rsa,
        1024);

    elle::String archive;

    elle::serialize::to_string(archive) << kp1;

    infinit::cryptography::KeyPair kp2; // XXX[load construct]
    elle::serialize::from_string(archive) >> kp2;

    // XXX[compare keys]
  }

  std::cout << "tests done." << std::endl;

  return (0);
}
