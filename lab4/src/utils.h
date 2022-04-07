#ifndef UTILS_H
#define UTILS_H

struct MinMax {
  int min;
  int max;
};

struct SumArgs {
  int *array;
  int begin;
  int end;
};

void GenerateArray(int *array, unsigned int array_size, unsigned int seed);

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end);

long long int Sum(const struct SumArgs *args);

void *ThreadSum(void *args);

#endif
