#ifndef AWS_CREDENTIALS_HH
# define AWS_CREDENTIALS_HH

# include <string>

# include <boost/date_time/posix_time/posix_time.hpp>
# include <boost/optional.hpp>

# include <elle/attribute.hh>
# include <elle/Printable.hh>
# include <elle/serialization/fwd.hh>

# include <aws/Keys.hh>

namespace aws
{
  class Credentials:
    public elle::Printable
  {
      /*-------------.
      | Construction |
      `-------------*/
    public:
      Credentials() = default;
      Credentials(std::string const& access_key_id,
                  std::string const& secret_access_key,
                  std::string const& session_token,
                  std::string const& region,
                  std::string const& bucket,
                  std::string const& folder,
                  std::string const& expiration = "never",
                  std::string const& server_time = "");

      std::string
      credential_string(RequestTime const& request_time,
                        Service const& aws_service);

      bool
      valid();

      ELLE_ATTRIBUTE_R(std::string, access_key_id);
      ELLE_ATTRIBUTE_R(std::string, secret_access_key);
      ELLE_ATTRIBUTE_R(std::string, session_token);
      ELLE_ATTRIBUTE_R(std::string, region);
      ELLE_ATTRIBUTE_R(std::string, bucket);
      ELLE_ATTRIBUTE_R(std::string, folder);
      ELLE_ATTRIBUTE_R(std::string, expiration_str);
      // Amazon-formated current time from server
      ELLE_ATTRIBUTE_R(std::string, server_time);

      ELLE_ATTRIBUTE_R(boost::posix_time::ptime, expiry);
      // Estimated skew between trusted server time and local universal time.
      ELLE_ATTRIBUTE_R(boost::optional<boost::posix_time::time_duration>, skew);

      /*--------------.
      | Serialization |
      `--------------*/
    public:
      Credentials(elle::serialization::SerializerIn& s);
      void
      serialize(elle::serialization::Serializer& s);

      /*----------.
      | Printable |
      `----------*/
    public:
      void
      print(std::ostream& stream) const;

    private:
      void
      _initialize(); // compute skew and expiry from input data
  };
}

#endif
