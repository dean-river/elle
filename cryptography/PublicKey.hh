
#ifndef ELLE_CRYPTOGRAPHY_PUBLICKEY_HH
#define ELLE_CRYPTOGRAPHY_PUBLICKEY_HH


#include <elle/types.hh>

#include <elle/radix/Object.hh>

#include <elle/serialize/Uniquable.hh>

#include <elle/cryptography/Plain.hh>
#include <elle/cryptography/Code.hh>
#include <elle/cryptography/Clear.hh>
#include <elle/cryptography/Signature.hh>

#include <elle/utility/Buffer.hh>

#include <elle/idiom/Close.hh>
# include <openssl/rsa.h>
# include <openssl/bn.h>
# include <openssl/err.h>
# include <openssl/evp.h>
#include <elle/idiom/Open.hh>

namespace elle
{

  using namespace radix;

  namespace cryptography
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this class represents a public key.
    ///
    class PublicKey:
        public Object
      , public elle::serialize::Uniquable<PublicKey>
    {
    public:
      //
      // constants
      //
      static const PublicKey            Null;

      //
      // constructors & destructors
      //
      PublicKey();
      PublicKey(const PublicKey&);
      ~PublicKey();

      //
      // methods
      //
      Status            Create(const ::EVP_PKEY*);
      Status            Create(Large*,
                               Large*);

      // XXX harmoniser les methodes encrypt/decrypt/sign/verify
      // pour prendre des Plain sans les serializer d'abord.

    public:
      ///
      /// this method encrypts the given data with the public key.
      ///
      /// since the public key size limits the size of the data that can be
      /// encrypted and raising large data to large exponent is very slow; the
      /// algorithm below consists in (i) generating a secret key, (ii)
      /// ciphering the plain text with this key, (iii) encrypting the secret
      /// key with the public key and finally (iv) returning an archive
      /// containing the asymetrically-encrypted secret key with the
      /// symmetrically-encrypted data.
      ///
      /// note however that should the plain be small enough, (1) a direct
      /// computation is performed. otherwise, (2) the data is symmetrically
      /// encrypted.
      ///
      Status
        Encrypt(elle::utility::WeakBuffer const& buffer, Code& out) const;
      template <typename T> Status
        Encrypt(T const& in, Code& out) const;

    public:
      ///
      /// this method decrypts a code which should actually be an archive
      /// containing both a secret key and some data.
      ///
      /// note that, although it may sound strange to 'decrypt' with a public
      /// key, it does not matter as both keys act as the other's opposite.
      ///
      /// therefore the public key can be used to encrypt in which case the
      /// private key will be used for decrypting or the other way around,
      /// which is what this function is for.
      ///
      /// this method starts by (i) extracting the key and data in their
      /// encrypted forms (ii) decrypt the symmetric key with the public key
      /// and (iii) decipher the data with the symmetric key.
      ///
      Status
        Decrypt(Code const& in, elle::utility::Buffer& out) const;
      template <typename T> Status
        Decrypt(Code const& in, T& out) const;

    public:
      ///
      /// this method verifies that the non-signed text is equal to the plain
      /// text.
      ///
      /// note that, as for the Sign() method, this method computes the plain's
      /// digest before forwarding to the other Verify() method.
      ///
      //Status
      //  Verify(Signature const& signature,
      //         Code const& code) const;
      //Status
      //  Verify(Signature const& signature,
      //         Plain const& plain) const;
      Status
        Verify(Signature const& signature,
               elle::utility::WeakBuffer const& plain) const;
      template <typename T> Status
        Verify(Signature const& signature, T const& any) const;

      //
      // interfaces
      //

      // object
      declare(PublicKey);
      Boolean           operator==(const PublicKey&) const;

      // dumpable
      Status            Dump(const Natural32 = 0) const;

      // archivable
      // XXX
      //Status            Serialize(Archive&) const;
      //Status            Extract(Archive&);

      //
      // attributes
      //
    private:
      ::EVP_PKEY*       _key;

      struct
      {
        ::EVP_PKEY_CTX* encrypt;
        ::EVP_PKEY_CTX* verify;
        ::EVP_PKEY_CTX* decrypt;
      }                 _contexts;

    public:
      //
      // properties
      //
      ::EVP_PKEY const* key() const { return this->_key; }


      // verify

    };

  }
}

//
// ---------- templates -------------------------------------------------------
//

#include <elle/cryptography/PublicKey.hxx>

#endif
