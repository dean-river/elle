//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// file          /home/mycure/infinit/elle/misc/Callback.hxx
//
// created       julien quintard   [wed mar 24 23:43:50 2010]
// updated       julien quintard   [thu mar 25 00:27:59 2010]
//

#ifndef ELLE_MISC_CALLBACK_HXX
#define ELLE_MISC_CALLBACK_HXX

namespace elle
{
  namespace misc
  {

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method calls the callback implementation specific Call() method.
    ///
    template <typename... T>
    Status		Callback::Call(T&...			arguments)
    {
      enter();

      // check if the routine is present.
      if (this->routine == NULL)
	escape("unable to call an undefined callback");

      // call the routine.
      if (this->routine->Call(arguments...) == StatusError)
	escape("an error occured in the routine");

      leave();
    }

  }
}

#endif
