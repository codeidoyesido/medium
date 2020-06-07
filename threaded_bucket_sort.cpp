#include "config.h"

#include <random>
#include <thread>

BucketType assign_to_buckets(const ArrayType &a) {
  BucketType buckets;

  for (auto &item : a) {
    auto bucket_index = std::min(item / BUCKET_WIDTH, (BUCKETS - 1));
    buckets[bucket_index].push_back(item);
  }

  return buckets;
}

// the function that performs the bucket sorting -- runs in parallel
void sort_bucket(BucketType &buckets, size_t index) { buckets[index].sort(); }

ArrayType get_sorted_array(const ArrayType &A) {

  // create the buckets placing each item into the correct bucket
  auto buckets = assign_to_buckets(A);

  // sort all buckets using threads
  std::vector<std::thread> ts;
  for (size_t i = 0; i < BUCKETS; i++) {
    std::thread t(sort_bucket, std::ref(buckets), i);
    ts.push_back(std::move(t));
  }
  for (std::thread &t : ts) {
    t.join();
  }

  // iterate over the buckets and copy their sorted items into the result array
  ArrayType result;
  size_t index = 0;

  for (const auto &bucket_list : buckets) {
    for (const auto &item : bucket_list) {
      result[index] = item;
      index++;
    }
  }
  return result;
}

int main() {

  ArrayType A;

  std::random_device rd;
  auto seed = rd();
  std::cout << "SEED: " << seed << std::endl << std::endl;
  std::mt19937 gen(seed);
  std::uniform_int_distribution<size_t> dis(0, RANGE);

  // initialize array with random numbers from 0 till RANGE (=100)
  for (size_t i = 0; i < ARRAY_SIZE; i++) {
    A[i] = dis(gen);
  }

  print_array(A);

  auto res = get_sorted_array(A);
  print_array(res);

  return 0;
}
