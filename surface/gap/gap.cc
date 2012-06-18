#include <cassert>

#include <elle/log.hh>

#include "gap.h"
#include "State.hh"

extern "C" {

#define __TO_C(st)    reinterpret_cast<gap_state_t*>(st)
#define __TO_CPP(st)  reinterpret_cast<surface::gap::State*>(st)

    gap_state_t* gap_new()
    {
      return __TO_C(new (std::nothrow) surface::gap::State());
    }

    void gap_free(gap_state_t* state)
    {
      delete __TO_CPP(state);
    }

    int gap_refresh_networks(gap_state_t* state)
    {
      assert(state != nullptr);
      try
        {
          __TO_CPP(state)->refresh_networks();
        }
      catch (std::exception const& err)
        {
          elle::log::warn("Cannot refresh networks:", err.what());
          return -1;
        }
      return 0;
    }

    char const* gap_path_to_network(gap_state_t* state, char const* path)
    {
      assert(state != nullptr);
      try
        {
          std::string const& network = __TO_CPP(state)->path_to_network(path);
          return network.c_str();
        }
      catch (std::exception const& err)
        {
          elle::log::warn("Cannot convert path", path, "to network:", err.what());
        }
      return nullptr;
    };

} // ! extern "C"
