#ifndef ETOILE_PORTAL_APPLICATION_HH
# define ETOILE_PORTAL_APPLICATION_HH

# include <elle/Elle.hh>

namespace etoile
{
  namespace portal
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this class represents an external client triggering operations
    /// on Etoile.
    ///
    /// note that the client must authenticate to Etoile in order to
    /// use the message interface.
    ///
    class Application:
      public elle::Entity
    {
    public:
      //
      // constants
      //
      static const reactor::Duration              Timeout;

      //
      // enumerations
      //
      enum State
        {
          StateConnected = 1,
          StateAuthenticated,
          StateDisconnected
        };

      enum Processing
        {
          ProcessingOff = 0,
          ProcessingOn
        };

      //
      // constructors & destructors
      //
      Application();
      ~Application();

      //
      // methods
      //
      elle::Status      Create(elle::TCPSocket*);

      //
      // callbacks
      //
      elle::Status      Disconnected();
      // XXX: const string&
      elle::Status      Error(elle::String);
      elle::Status      Abort();

      //
      // interfaces
      //

      // dumpable
      elle::Status      Dump(const elle::Natural32 = 0) const;

      //
      // attributes
      //
      State                     state;
      Processing                processing;

      elle::TCPSocket*        socket;
    };

  }
}

#endif
