#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void GenerateArray(int *array, unsigned int array_size, unsigned int seed) {
  srand(seed);
  for (int i = 0; i < array_size; i++) {
    array[i] = rand();
  }
}


struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) {
  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;


  for(int i = begin; i < end; i++)
    {
      //printf("%d ", array[i]);
      if (array[i] <= min_max.min) min_max.min = array[i];
      if (array[i] >= min_max.max) min_max.max = array[i];
    }
  //printf("\n");
  
  return min_max;
}

long long int Sum(const struct SumArgs *args) {
  long long int sum = 0;
  for(int i = args->begin; i < args->end; i++)
    sum += args->array[i];
  return sum;
}

void *ThreadSum(void *args) 
{
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}