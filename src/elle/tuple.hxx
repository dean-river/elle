#pragma once

#include <elle/print-fwd.hh>

namespace std
{
  namespace
  {
    template <int idx, typename T>
    void
    print_tuple(std::ostream& s, T const& v, bool first)
    {
      if constexpr(idx < tuple_size<T>::value)
      {
        elle::print(s, first ? "{}" : ", {}", get<idx>(v));
        print_tuple<idx + 1, T>(s, v, false);
      }
    };
  }

  template <typename ... Elts>
  ostream&
  operator <<(ostream& out, tuple<Elts...> const& v)
  {
    elle::print(out, "(");
    print_tuple<0, tuple<Elts...>>(out, v, true);
    elle::print(out, ")");
    return out;
  }

  template <typename F, typename T>
  [[deprecated("prefer std::apply")]]
  auto
  forward_tuple(F const& f, T&& tuple)
    -> decltype(std::apply(f, std::forward<T>(tuple)))
  {
    return std::apply(f, std::forward<T>(tuple));
  }

  /*----------.
  | Operators |
  `----------*/

  template <typename ... T1, typename ... T2, typename F>
  auto
  tuple_zip(std::tuple<T1...> const& lhs, std::tuple<T2...> const& rhs,
            F const& f)
  {
    return std::apply(
      [&] (auto&& ... l)
      {
        return std::apply(
          [&] (auto&& ... r)
          {
            return std::tuple(f(l, r)...);
          },
          rhs);
      },
          lhs);
  }

#define ELLE_TUPLE_OP(Op)                                               \
  template <typename ... T1, typename ... T2>                           \
  auto                                                                  \
  operator Op(std::tuple<T1...> const& lhs,                             \
              std::tuple<T2...> const& rhs)                             \
  {                                                                     \
    return tuple_zip(                                                   \
      lhs, rhs,                                                         \
      [] (auto&& l, auto&& r)                                           \
      {                                                                 \
        return std::forward<decltype(l)>(l) Op                          \
          std::forward<decltype(r)>(r);                                 \
      });                                                               \
  }
  ELLE_TUPLE_OP(+);
  ELLE_TUPLE_OP(-);
  ELLE_TUPLE_OP(*);
  ELLE_TUPLE_OP(/);
  ELLE_TUPLE_OP(%);
#undef ELLE_TUPLE_OP

  /*----.
  | Any |
  `----*/

  namespace _details
  {
    template <int i, typename ... T>
    std::enable_if_t<i == 0, bool>
    any(std::tuple<T...> const& v)
    {
      return false;
    }

    template <int i, typename ... T>
    std::enable_if_t<0 < i, bool>
    any(std::tuple<T...> const& v)
    {
      if (std::get<i - 1>(v))
        return true;
      else
        return any<i - 1>(v);
    }
  }

  template <typename ... T>
  bool
  any(std::tuple<T...> const& v)
  {
    return _details::any<sizeof ... (T)>(v);
  }
}
