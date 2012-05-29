//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// author        julien quintard   [fri nov 27 22:04:36 2009]
//

//
// ---------- includes --------------------------------------------------------
//

#include <elle/test/network/udp/Node.hh>

namespace elle
{
  namespace test
  {

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method initializes the node.
    ///
    Status              Node::Setup(const String&               name,
                                    const String&               host,
                                    const Port                  port)
    {
      // set the attributes.
      this->name = name;
      this->host = host;
      this->port = port;

      return Status::Ok;
    }

    ///
    /// this is the thread entry locus.
    ///
    Status              Node::Run()
    {
      Host              local;
      Locus             remote;

      // create an host.
      if (local.Create(this->host) == Status::Error)
        escape("unable to create an host");

      // create the socket.
      if (this->socket.Create() == Status::Error)
        escape("unable to create the socket");

      std::cout << "[port] " << this->socket.port << std::endl;

      // register the probe message.
      if (Network::Register(
            Procedure<TagProbe>(
              Callback<>::Infer(&Node::Handle, this))) == Status::Error)
        escape("unable to register the probe message");

      // create the table.
      if (this->table.Create(this) == Status::Error)
        escape("unable to create the table");

      // create an locus.
      if (remote.Create(local, this->port) == Status::Error)
        escape("unable to create a location");

      // probe the first peer.
      if (this->socket.Send(remote,
                            Inputs<TagProbe>(this->name,
                                             this->table)) == Status::Error)
        escape("unable to send the probe");

      return Status::Ok;
    }

//
// ---------- callbacks -------------------------------------------------------
//

    ///
    /// this method handles probe packets.
    ///
    Status              Node::Handle(const String&              name,
                                     const Table&               table)
    {
      Session*          session;

      // retrieve the current session.
      if (Session::Instance(session) == Status::Error)
        escape("unable to retrieve the instance of the current session");

      // simply add the sender to the table.
      if (this->table.Update(session->locus, name) == Status::Error)
        escape("unable to add the new neighbour");

      // refresh the sender.
      if (this->table.Refresh(session->locus) == Status::Error)
        escape("unable to refresh the sender's entry");

      // merge the table with the received one.
      if (this->table.Merge(table) == Status::Error)
        escape("unable to update the table");

      return Status::Ok;
    }

  }
}
