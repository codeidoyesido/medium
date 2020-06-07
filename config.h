#include <algorithm>
#include <array>
#include <iostream>
#include <list>

static constexpr size_t BUCKETS = 10;
static constexpr size_t BUCKET_WIDTH = 100;
static constexpr size_t MAX_ARRAY_SIZE = 100000;
static constexpr size_t ARRAY_SIZE =
    std::min(MAX_ARRAY_SIZE, BUCKETS *BUCKET_WIDTH);
static constexpr size_t RANGE = BUCKETS * BUCKET_WIDTH;

using BucketType = std::array<std::list<size_t>, BUCKETS>;
using ArrayType = std::array<size_t, ARRAY_SIZE>;

auto print_item = [](const auto &item) { std::cout << item << ", "; };

auto print_list = [](const auto &l) {
  std::cout << "--------printing elements in list-----" << std::endl;
  std::for_each(l.begin(), l.end(), print_item);
  std::cout << std::endl;
};

void print_array(const ArrayType &a) {
  std::cout << "-------- printing array ----------" << std::endl;
  std::for_each(a.begin(), a.end(), print_item);
  std::cout << std::endl << "----------------------------------" << std::endl;
  std::cout << std::endl << std::endl;
}
