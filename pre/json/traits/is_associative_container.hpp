#ifndef PRE_JSON_TRAITS_IS_ASSOCIATIVE_CONTAINER_HPP
#define PRE_JSON_TRAITS_IS_ASSOCIATIVE_CONTAINER_HPP

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>


namespace pre { namespace json { namespace traits {

  /**
   * Credits to relaxx
   * See http://stackoverflow.com/a/31532326/271781 
   *
   */
  template<typename T>
  struct to_void
  {
      typedef void type;
  };

  template <typename Container, typename dummy = void>
  struct is_associative_container : std::false_type {};

  template <typename Container>
  struct is_associative_container<Container, typename to_void<typename Container::key_type>::type > : std::true_type{};

  template <typename Container>
  struct is_multi_container : std::false_type {};

  template <typename Container>
  struct is_unique_container : std::conditional<is_multi_container<Container>::value, std::false_type, std::true_type>::type{};

  template <typename Container>
  struct is_unordered_container : std::false_type {};

  template <typename Container>
  struct is_ordered_container : std::conditional<is_unordered_container<Container>::value, std::false_type, std::true_type>::type{};

  template <typename T, typename Compare, typename Alloc>
  struct is_multi_container<std::multiset<T,Compare, Alloc>> : std::true_type {};

  template <typename T, typename Compare, typename Alloc>
  struct is_multi_container<std::multimap<T, Compare, Alloc>> : std::true_type{};

  template <typename T, typename Hash, typename Pred, typename Alloc>
  struct is_multi_container<std::unordered_multimap<T, Hash, Pred, Alloc>> : std::true_type{};

  template <typename T, typename Hash, typename Pred, typename Alloc>
  struct is_multi_container<std::unordered_multiset<T, Hash, Pred, Alloc>> : std::true_type{};

  template <typename T, typename Hash, typename Pred, typename Alloc>
  struct is_unordered_container<std::unordered_multimap<T, Hash, Pred, Alloc>> : std::true_type{};

  template <typename T, typename Hash, typename Pred, typename Alloc>
  struct is_unordered_container<std::unordered_map<T, Hash, Pred, Alloc>> : std::true_type{};

  template <typename T, typename Hash, typename Pred, typename Alloc>
  struct is_unordered_container<std::unordered_multiset<T, Hash, Pred, Alloc>> : std::true_type{};

  template <typename T, typename Hash, typename Pred, typename Alloc>
  struct is_unordered_container<std::unordered_set<T, Hash, Pred, Alloc>> : std::true_type{};

}}}

#endif
