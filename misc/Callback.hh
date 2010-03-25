//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// file          /home/mycure/infinit/elle/misc/Callback.hh
//
// created       julien quintard   [wed mar 24 15:49:05 2010]
// updated       julien quintard   [thu mar 25 01:38:45 2010]
//

#ifndef ELLE_MISC_CALLBACK_HH
#define ELLE_MISC_CALLBACK_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/misc/Status.hh>
#include <elle/misc/Routine.hh>

namespace elle
{
  namespace misc
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this class represents an callback i.e a pointer-based routine.
    ///
    class Callback:
      public Entity,
      public Dumpable
    {
    public:
      //
      // constructors & destructors
      //
      Callback();
      Callback(const Routine&);
      Callback(const Callback&);
      ~Callback();

      //
      // methods
      //
      template <typename... T>
      Status		Call(T&...);

      //
      // interfaces
      //

      // entity
      declare(Entity, Callback);
      // XXX

      // dumpable
      Status		Dump(const Natural32 = 0) const;

      //
      // attributes
      //
      Routine*		routine;
    };

  }
}

//
// ---------- templates -------------------------------------------------------
//

#include <elle/misc/Callback.hxx>

#endif
