//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// author        julien quintard   [wed jan 28 11:22:24 2009]
//

//
// ---------- includes --------------------------------------------------------
//

#include <elle/test/network/local/Test.hh>

namespace elle
{
  namespace test
  {

//
// ---------- functions -------------------------------------------------------
//

    Status              Main(const Natural32                    argc,
                             const Character*                   argv[])
    {
      Server            server;
      Client            client;

      // check the arguments.
      if (argc != 3)
        {
          std::cerr << "[usage] " << argv[0] << " _type{client/server}_ _id_"
                    << std::endl;

          exit(1);
        }

      // init the library.
      if (Elle::Initialize() == Status::Error)
        escape("unable to initialize the Elle library");

      // setup the program.
      if (Program::Setup() == Status::Error)
        escape("unable to set up the program");

      // launch either the client or the server.
      if (String(argv[1]) == String("server"))
        {
          // set up the server.
          if (server.Setup(argv[2]) == Status::Error)
            escape("unable to set up the server");

          // start the server.
          if (server.Run() == Status::Error)
            escape("unable to run the server");
        }
      else if (String(argv[1]) == String("client"))
        {
          // set up the client.
          if (client.Setup(argv[2]) == Status::Error)
            escape("unable to set up the client");

          // start the client.
          if (client.Run() == Status::Error)
            escape("unable to run the client");
        }
      else
        escape("unknown type");

      // launch the program.
      if (Program::Launch() == Status::Error)
        escape("an error occured during the event processing");

      // clean the library.
      if (Elle::Clean() == Status::Error)
        escape("unable to clean the Elle library");

      return Status::Ok;
    }

  }
}

//
// ---------- main ------------------------------------------------------------
//

int                     main(const int                          argc,
                             const char*                        argv[])
{
  if (elle::test::Main(argc, argv) == elle::Status::Error)
    {
      show();

      return (1);
    }

  std::cout << "[success]" << std::endl;

  return (0);
}
