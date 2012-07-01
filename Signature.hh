#ifndef ELLE_CRYPTOGRAPHY_SIGNATURE_HH
#define ELLE_CRYPTOGRAPHY_SIGNATURE_HH

#include <elle/types.hh>

#include <elle/standalone/Region.hh>

#include <elle/radix/Object.hh>

#include <elle/idiom/Open.hh>

namespace elle
{
  namespace cryptography
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this class represents an asymmetrically encrypted text.
    ///
    class Signature:
      public radix::Object
    {
    public:
      //
      // constants
      //
      static const Signature            Null;

      //
      // constructors & destructors
      //
      Signature();

      //
      // interfaces
      //

      // object
      declare(Signature);
      Boolean           operator==(const Signature&) const;

      // dumpable
      Status            Dump(const Natural32 = 0) const;

      //
      // attributes
      //
      standalone::Region region;
    };

  }
}

#include <elle/cryptography/Signature.hxx>

#endif
