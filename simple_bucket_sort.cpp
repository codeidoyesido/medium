#include "config.h"

#include <random>

BucketType assign_to_buckets(const ArrayType &a) {
  BucketType buckets;
  auto put_in_bucket = [&](const size_t &item) {
    buckets[std::min(BUCKETS - 1, item / BUCKET_WIDTH)].push_back(item);
  };

  std::for_each(a.begin(), a.end(), put_in_bucket);
  // std::for_each(buckets.begin(), buckets.end(), print_list);
  return buckets;
}

// the function that performs the bucket sorting -- to be parallelized
void sort_bucket(BucketType &buckets, int index) {
  // std::cout << "sorting list on index: " << index << std::endl;
  buckets[index].sort();
  // print_list(buckets[index]);
}

ArrayType get_sorted_array(const ArrayType &A) {

  // create the buckets placing each item into the correct bucket
  auto buckets = assign_to_buckets(A);
  // sort each bucket
  for (size_t i = 0; i < BUCKETS; i++) {
    sort_bucket(buckets, i);
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

  // initialize array with random numbers from 0 till range (=100)
  for (size_t i = 0; i < ARRAY_SIZE; i++) {
    A[i] = dis(gen);
  }

  print_array(A);

  auto res = get_sorted_array(A);
  print_array(res);

  return 0;
}
