#include <algorithm>
#include <utility>
using std::pair;
using std::rotate;
using std::stable_partition;

template <typename RandomAccessIterator>
// return new first' and last'
auto slide(RandomAccessIterator first, RandomAccessIterator last,
           RandomAccessIterator new_pos)
    -> pair<RandomAccessIterator, RandomAccessIterator> {
  // move to lower bound
  if (new_pos < first) return {new_pos, rotate(new_pos, first, last)};
  // move to upper bound
  if (last < new_pos) return {rotate(first, last, new_pos), new_pos};
  // new_pos within [first, last)
  return {first, last};
}

template <typename BidirectionIterator, typename UnaryPredicate>
auto gather(BidirectionIterator first, BidirectionIterator last,
            BidirectionIterator middle, UnaryPredicate p)
    -> pair<BidirectionIterator, BidirectionIterator> {
  return {stable_partition(first, middle, not1(p)),
          stable_partition(first, middle, p)};
}
