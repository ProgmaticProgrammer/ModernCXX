#ifndef MATRIX_HEADER
#define MATRIX_HEADER
#include <array>
#include <functional>
#include <iterator>
#include <utility>
using std::array;
using std::iterator;

template <class T, std::size_t Row, std::size_t Col>
struct Matrix {
  static_assert(Row >= 1);
  static_assert(Col >= 1);

  const array<array<T, Col>, Row> data_;

  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;

  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  // member typedefs provided through inheriting from std::iterator
  // Iterate as below for a 3X3 matrix, 1->2->3...->9
  //    col 0 1 2
  // row     | | |
  // 0  --  1 2 4
  // 1  --  3 5 7
  // 2  --  6 8 9
  //
  // NOTE: track num is the sum of any elements' (in this track) row and col
  // 1 is in track 0 because 1's row/col is (0,0)
  // 2 and 3 are in track 1 because 2's row/col is (0,1) and 3's row/col is
  // (1,0) 4, 5 and 6 are in track 2 7 and 8 are in track 3 9 is in track 4

  class const_iterator
      : public std::iterator<std::input_iterator_tag,  // iterator_category
                             value_type,               // value_type
                             difference_type,          // difference_type
                             pointer,                  // pointer
                             reference                 // reference
                             > {
    using Point = std::pair<size_type, size_type>;
    using Track = std::size_t;

    std::reference_wrapper<const Matrix> container_;
    std::size_t where_ = 0;  // 0, 1, ...,  Row*Col-1, Row*Col
    Point cur_ = {0, 0};
    /////
    auto left_bottom_() const -> Point {
      return {cur_.first + 1, cur_.second - 1};
    }
    auto first_in_track_(Track track) const -> Point {
      if (track < Col) {
        return {0, track};
      } else {
        return {track - (Col - 1), (Col - 1)};
      }
    }
    auto next_track_() const -> Track { return cur_track_() + 1; }
    auto cur_track_() const -> Track { return cur_.first + cur_.second; }
    /////
    bool is_valid_(const Point& pt) const {
      return pt.first < Row && pt.second < Col;
    }
    bool is_last_intrack_() const { return !is_valid_(left_bottom_()); }
    bool is_last_() const { return cur_.first + cur_.second == Row + Col - 2; }
    /////
    void next_() {
      cur_ =
          is_last_intrack_() ? first_in_track_(next_track_()) : left_bottom_();
    }
    bool done_() const { return is_last_(); }

   public:
    explicit const_iterator(std::reference_wrapper<const Matrix> data,
                      size_type sentinel = 0)
        : container_(data), where_(sentinel) {}

    const_iterator& operator++() {
      if (!done_()) next_();
      if (where_ < Row * Col) ++where_;
      return *this;
    }
    const_iterator operator++(int) {
      iterator retval = *this;
      ++(*this);
      return retval;
    }
    bool operator==(const_iterator other) const { return where_ == other.where_; }
    bool operator!=(const_iterator other) const { return !(*this == other); }

    auto operator*() const -> const_reference {
      auto row = cur_.first;
      auto col = cur_.second;
      //   static_assert(row < Row);
      //   static_assert(col < Col);
      return container_.get().at(row, col);
    }
  };

//   typedef const iterator const_iterator;
  //   iterator begin() noexcept { return iterator(std::ref(*this)); }
  //   iterator end() noexcept { return iterator(std::ref(*this), size()); }
  const_iterator cbegin() const noexcept { return const_iterator(std::cref(*this)); }
  const_iterator cend() const noexcept {
    return const_iterator(std::cref(*this), size());
  }

  constexpr size_type size() const { return Row * Col; }
  constexpr bool empty() const { return size() == (size_type)0u; }

  const_reference at(size_type row, size_type col) const {
    if (!(row < Row) || !(col < Col))
      throw std::out_of_range("row|col out of rang!");
    return data_[row][col];
  }
};

#endif  // !MATRIX_HEADER
