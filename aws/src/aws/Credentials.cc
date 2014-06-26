#include <aws/Credentials.hh>

#include <elle/log.hh>
#include <elle/printf.hh>
#include <elle/serialization/Serializer.hh>
#include <elle/serialization/SerializerIn.hh>

#include <aws/Exceptions.hh>

ELLE_LOG_COMPONENT("aws.Credentials");

namespace aws
{
  Credentials::Credentials(std::string const& access_key_id,
                           std::string const& secret_access_key,
                           std::string const& session_token,
                           std::string const& region,
                           std::string const& bucket,
                           std::string const& folder,
                           std::string const& expiration,
                           std::string const& server_time)
    : _access_key_id(access_key_id)
    , _secret_access_key(secret_access_key)
    , _session_token(session_token)
    , _region(region)
    , _bucket(bucket)
    , _folder(folder)
    , _expiration_str(expiration)
    , _server_time(server_time)
    , _skew()
  {
    this->_initialize();
  }

  void
  Credentials::_initialize()
  {
    if (!this->_server_time.empty())
    { // Assume creation time is now, and compute skew with local clock
      ELLE_TRACE("Parsing server time: %s", this->_server_time);
      std::string time_str;
      for (auto c: this->_server_time)
      {
        if (c != '-' && c != ':' && c != 'Z')
        {
          time_str += c;
        }
      }
      auto creation_time = boost::posix_time::from_iso_string(time_str);
      this->_skew = boost::posix_time::second_clock::universal_time() - creation_time;
      ELLE_TRACE("Computed clock skew of %s", *this->_skew);
    }
    // For debugging.
    if (this->_expiration_str == "never")
      return;

    // Take the AWS server response and convert it to a ptime.
    // eg: 2014-02-26T23:34:19Z -> 20140226T233419
    std::string time_str;
    for (auto c: this->_expiration_str)
    {
      if (c != '-' && c != ':' && c != 'Z')
      {
        time_str += c;
      }
    }
    this->_expiry = boost::posix_time::from_iso_string(time_str);
    if (!this->valid())
    {
      // Maybe the local clock is wrong. AWS will tell us if creds are
      // realy expired.
      ELLE_WARN("Credentials appear to be expired: %s", *this);
    }
  }

  std::string
  Credentials::credential_string(RequestTime const& request_time,
                                 Service const& aws_service)
  {
    std::string date_str = boost::posix_time::to_iso_string(request_time);
    date_str = date_str.substr(0, 8);
    std::string res = elle::sprintf(
      "%s/%s/%s/%s/aws4_request",
      this->_access_key_id,
      date_str,
      this->_region,
      aws_service
    );
    return res;
  }

  bool
  Credentials::valid()
  {
    // For debugging.
    if (this->_expiration_str == "never")
      return true;

    using namespace boost::posix_time;
    ptime now = second_clock::universal_time();
    bool res;
    if (this->_expiry < now)
    {
      ELLE_DEBUG("%s: credentials have expired", *this);
      res = false;
    }
    else
    {
      ELLE_DEBUG("%s: credentials are valid", *this);
      res = true;
    }
    return res;
  }

  Credentials::Credentials(elle::serialization::SerializerIn& s)
  {
    this->serialize(s);
  }

  void
  Credentials::serialize(elle::serialization::Serializer& s)
  {
    s.serialize("access_key_id", this->_access_key_id);
    s.serialize("secret_access_key", this->_secret_access_key);
    s.serialize("session_token", this->_session_token);
    s.serialize("region", this->_region);
    s.serialize("bucket", this->_bucket);
    s.serialize("folder", this->_folder);
    s.serialize("expiration", this->_expiration_str);
    s.serialize("current_time", this->_server_time);
    if (s.in())
      this->_initialize();
  }

  void
  Credentials::print(std::ostream& stream) const
  {
    stream << "AWS credentials: access_id=" << this->_access_key_id
           << "expiry=" << this->_expiration_str;
  }
}
