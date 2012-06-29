#ifndef  ELLE_CRYPTOGRAPHY_SIGNATURE_HXX
# define ELLE_CRYPTOGRAPHY_SIGNATURE_HXX

# include <cassert>

# include <elle/standalone/Region.hh>

# include <elle/cryptography/Signature.hh>

ELLE_SERIALIZE_SIMPLE(elle::cryptography::Signature,
                      archive,
                      value,
                      version)
{
  assert(version == 0);
  archive & value.region;
}

#endif
