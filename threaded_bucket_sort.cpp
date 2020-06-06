#include <algorithm>
#include <array>
#include <iostream>
#include <list>
#include <random>
#include <thread>

using namespace std;
static constexpr size_t a_size = 1000;
static constexpr size_t b_size = a_size;
static constexpr size_t range = 100;
using BucketType = std::array<std::list<int>, b_size>;
using ArrayType = std::array<int, a_size>;

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

BucketType assign_to_buckets(const ArrayType &a) {
  BucketType buckets;
  auto put_in_bucket = [&](const int &item) {
    buckets[item / (b_size + 1)].push_back(item);
  };

  std::for_each(a.begin(), a.end(), put_in_bucket);
  // std::for_each(buckets.begin(), buckets.end(), print_list);
  return buckets;
}

// the function that performs the bucket sorting -- runs in parallel
void sort_bucket(BucketType &buckets, int index) {
  // std::cout << "sorting list on index: " << index << std::endl;
  buckets[index].sort();
  // print_list(buckets[index]);
}

ArrayType get_sorted_array(const ArrayType &A) {

  auto buckets = assign_to_buckets(A);

  ArrayType result;
  size_t index = 0;

  // sort all lists using threads
  std::vector<std::thread> ts;
  for (size_t i = 0; i < b_size; i++) {
    std::thread t(sort_bucket, std::ref(buckets), i);
    ts.push_back(std::move(t));
  }
  for (std::thread &t : ts) {
    t.detach();
  }

  std::for_each(buckets.begin(), buckets.end(), [&](auto &l) {
    std::for_each(l.begin(), l.end(),
                  [&](const auto &item) { result[index++] = item; });
  });
  return result;
}

int main() {

  ArrayType A;

  std::random_device rd;
  auto seed = rd();
  std::cout << "SEED: " << seed << std::endl << std::endl;
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> dis(0, range);

  // initialize array with random numbers from 0 till range (=100)
  for (size_t i = 0; i < a_size; i++) {
    A[i] = dis(gen);
  }

  print_array(A);

  auto res = get_sorted_array(A);
  print_array(res);

  return 0;
}
