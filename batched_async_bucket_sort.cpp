#include "config.h"

#include <future>
#include <random>

static constexpr size_t NUM_OF_THREADS = 4;

BucketType assign_to_buckets(const ArrayType &a) {
  BucketType buckets;
  auto put_in_bucket = [&](const size_t &item) {
    buckets[std::min(BUCKETS - 1, item / BUCKET_WIDTH)].push_back(item);
  };

  std::for_each(a.begin(), a.end(), put_in_bucket);
  return buckets;
}

// the function that performs the bucket sorting -- runs in parallel
void sort_bucket(BucketType &buckets, int index) { buckets[index].sort(); }

void batch_task(BucketType &buckets, int from, int to) {
  for (size_t i = from; i < to; i++) {
    sort_bucket(buckets, i);
  }
}

ArrayType get_sorted_array(const ArrayType &A) {

  auto buckets = assign_to_buckets(A);

  // sort all lists using threads
  auto chunk_size = BUCKETS / NUM_OF_THREADS;
  std::vector<std::future<void>> fs;

  for (size_t i = 0; i < BUCKETS; i += chunk_size) {
    auto f = std::async(std::launch::async, batch_task, std::ref(buckets), i,
                        std::min(i + chunk_size, BUCKETS));
    fs.push_back(std::move(f));
  }

  for (auto &f : fs) {
    f.wait();
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
