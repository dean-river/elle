#ifndef DIARY_DIARY_HH
# define DIARY_DIARY_HH

# include <Infinit.hh>
# include <elle/Elle.hh>
# include <lune/Lune.hh>
# include <hole/Hole.hh>
# include <agent/Agent.hh>
# include <etoile/Etoile.hh>

namespace satellite
{

//
// ---------- classes ---------------------------------------------------------
//

  ///
  /// this class implements the diary satellite.
  ///
  class Diary
  {
  public:
    //
    // enumerations
    //
    enum Operation
      {
        OperationUnknown = 0,

        OperationRecord,
        OperationReplay,
        OperationDump
      };
  };

}

//
// ---------- includes --------------------------------------------------------
//

#include <satellites/diary/Memoirs.hh>

#if defined(INFINIT_LINUX) || defined(INFINIT_MACOSX)
# include <satellites/diary/unix/UNIX.hh>
#elif defined(INFINIT_WINDOWS)
// XXX todo: windows
#else
# error "unsupported platform"
#endif

#endif
