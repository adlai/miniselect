/*          Copyright Danila Kutenin, 2020-.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          https://boost.org/LICENSE_1_0.txt)
 */
#pragma once

#include <algorithm>
#include <cassert>
#include <iterator>
#include <utility>

#include "private/median_common.h"

namespace miniselect {
namespace median_of_medians_detail {

template <class Iter, class Compare>
inline Iter partition(Iter r, Iter end, Compare&& comp) {
  using CompType = typename median_common_detail::CompareRefType<Compare>::type;
  using DiffType = typename std::iterator_traits<Iter>::difference_type;
  const DiffType len = end - r;
  if (len < 5) {
    return median_common_detail::pivot_partition(r, len / 2, len, comp);
  }
  DiffType j = 0;
  for (DiffType i = 4; i < len; i += 5, ++j) {
    median_common_detail::partition5(r, i - 4, i - 3, i, i - 2, i - 1, comp);
    std::swap(r[i], r[j]);
  }
  median_common_detail::quickselect<Iter, CompType, &partition>(r, r + j / 2,
                                                                r + j, comp);
  return median_common_detail::pivot_partition(r, j / 2, len, comp);
}

}  // namespace median_of_medians_detail

template <class Iter, class Compare>
inline void median_of_medians_select(Iter begin, Iter mid, Iter end,
                                     Compare comp) {
  if (mid == end) return;
  using CompType = typename median_common_detail::CompareRefType<Compare>::type;

  median_common_detail::quickselect<
      Iter, CompType, &median_of_medians_detail::partition<Iter, CompType>>(
      begin, mid, end, comp);
}

template <class Iter>
inline void median_of_medians_select(Iter begin, Iter mid, Iter end) {
  using T = typename std::iterator_traits<Iter>::value_type;
  median_of_medians_select(begin, mid, end, std::less<T>());
}

template <class Iter, class Compare>
inline void median_of_medians_partial_sort(Iter begin, Iter mid, Iter end,
                                           Compare comp) {
  if (begin == mid) return;
  using CompType = typename median_common_detail::CompareRefType<Compare>::type;
  median_common_detail::quickselect<
      Iter, CompType, &median_of_medians_detail::partition<Iter, CompType>>(
      begin, mid - 1, end, comp);
  std::sort<Iter, CompType>(begin, mid, comp);
}

template <class Iter>
inline void median_of_medians_partial_sort(Iter begin, Iter mid, Iter end) {
  using T = typename std::iterator_traits<Iter>::value_type;
  median_of_medians_partial_sort(begin, mid, end, std::less<T>());
}

}  // namespace miniselect
