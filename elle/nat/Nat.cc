#include <string>
#include <sstream>
#include <elle/nat/Nat.hh>
#include <elle/concurrency/Scheduler.hh>

#include <reactor/network/buffer.hh>
#include <reactor/sleep.hh>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <boost/asio/ip/udp.hpp>

ELLE_LOG_COMPONENT("Elle.NAT");

namespace elle
{
  namespace nat
  {

  namespace {

    namespace ec = elle::concurrency;
    namespace rnet = reactor::network;

  }

  // Class Hole {{{

  Hole::Hole(reactor::Scheduler &sched,
             std::string const &hostname,
             std::string const &port)
      : _handle{new rnet::UDPSocket{sched}}
      , _public_endpoint{"", 0}
      , _endpoint{boost::asio::ip::address::from_string(hostname), std::stoi(port)}
  {
  }

  Hole::Hole(reactor::Scheduler &sched,
             std::string const &hostname,
             int port)
      : _handle{new rnet::UDPSocket{sched}}
      , _public_endpoint{"", 0}
      , _endpoint{boost::asio::ip::address::from_string(hostname), port}
  {
  }

  Hole::Hole(Hole&& other):
    _handle{std::move(other._handle)}
  {}

  Hole::~Hole()
  {
    this->say("quit");
  }

  void
  Hole::sayHello(void)
  {
    this->say("hello");
  }

  void
  Hole::say(std::string const msg)
  {
    if (this->_handle)
      {
        ELLE_TRACE("sending %s to %s", msg, this->_endpoint);
        this->_handle->send_to(rnet::Buffer(msg + "\n"), this->_endpoint);
      }
    else
      {
        ELLE_TRACE("not sending %s to %s because handle is invalid",
                   msg,
                   this->_endpoint);
      }
  }

  std::string const
  Hole::get()
  {
    std::string resp;
    rnet::UDPSocket::EndPoint p;

    resp.resize(512);
    this->_handle->receive_from(rnet::Buffer(resp), p);
    return resp;
  }

  void
  Hole::exchangeIP(void)
  {
    std::string resp;
    std::stringstream ss;
    std::vector<std::string> splited;

    ss << "local " << this->_handle->local_endpoint();
    this->say(ss.str());
    resp = this->get();

    boost::split(splited, resp, boost::is_any_of(" :"));
    this->_public_endpoint = std::make_pair(splited[1], std::stoi(splited[2]));
  }

  void
  Hole::drill(void)
  {
    this->sayHello();
    this->exchangeIP();
  }

  // class Hole }}}

  // class KeepAlive {{{

  KeepAlive::KeepAlive(reactor::Scheduler& sched,
                       s_ptr<rnet::UDPSocket>&& s):
      sched(sched) , handle(std::move(s))
  {}

  KeepAlive::~KeepAlive()
  {}

  void
  KeepAlive::run(void)
  {
    std::string  buf;

    buf.resize(128);
    while (this->running == true)
    {
        reactor::Sleep s(this->sched, boost::posix_time::seconds(60));

        handle->write(rnet::Buffer(std::string("ping\n")));
        handle->read_some(rnet::Buffer(buf));
        s.run();
    }
  }

  void
  KeepAlive::stop(void)
  {
    this->running = false;
  }

  // class KeepAlive }}}

  // class NAT {{{

  NAT::NAT(reactor::Scheduler &s):
      sched(s),
      alive()
  {}

  NAT::~NAT()
  {
    if (this->alive)
      this->alive->terminate();
  }

  Hole
  NAT::punch(std::string const &hostname,
             int port)
  {
    Hole h(sched, hostname, port);

    h.drill();
    return h;
  }


  // class NAT }}}

  } /* nat */
} /* elle */
