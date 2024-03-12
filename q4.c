#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define arrayLen 10

int array[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};
int sorted[arrayLen];

int partition(int *arr, int lo, int hi) {
  int pivot = arr[lo];

  int i = lo - 1;
  int j = hi + 1;

  while (1) {
    do {
      i += 1;
    } while (arr[i] < pivot);

    do {
      j -= 1;
    } while (arr[j] > pivot);

    if (i >= j) return j;

    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}

void quickSort(int* arr, int lo, int hi) {
  if (lo >= 0 && hi >= 0 && lo < hi) {
    int p = partition(arr, lo, hi);
    quickSort(arr, lo, p);
    quickSort(arr, p + 1, hi);
  }
}

typedef struct {
  int *arr;
  int count;
} sortRunnerParams;

void *sortRunner(void *vargs) {
  sortRunnerParams *args = vargs;
  quickSort(args->arr, 0, args->count - 1);

  return NULL;
}

typedef struct {
  sortRunnerParams *l1;
  sortRunnerParams *l2;
  int *dest;
} mergeRunnerParams;

void *mergeRunner(void *vargs) {
  mergeRunnerParams *args = vargs;

  int l1len = args->l1->count;
  int l2len = args->l2->count;

  int index = 0;
  int idx1 = 0;
  int idx2 = 0;

  while (idx1 < l1len && idx2 < l2len) {
    if (args->l1->arr[idx1] <= args->l2->arr[idx2]) {
      args->dest[index++] = args->l1->arr[idx1++];
    } else {
      args->dest[index++] = args->l2->arr[idx2++];
    }
  }

  // get any remaining elems
  while (idx1 < l1len) {
    args->dest[index++] = args->l1->arr[idx1++];    
  }
  while (idx2 < l2len) {
    args->dest[index++] = args->l2->arr[idx2++];    
  }

  return NULL;
}

int main() {
  printf("unsorted array:\n");
  for (int i = 0; i < arrayLen; i++) {
    printf("%d ", array[i]);
  }
  printf("\n");

  int subListSize = arrayLen / 2;

  // Sorting threads
  pthread_t t1;
  sortRunnerParams t1Args = { array, subListSize };

  pthread_t t2;
  sortRunnerParams t2Args = { array + subListSize, subListSize };

  printf("\nsubarray 1 initial:\n");
  for (int i = 0; i < t1Args.count; i++) {
    printf("%d ", t1Args.arr[i]);
  }
  printf("\n");
  printf("\nsubarray 2 initial:\n");
  for (int i = 0; i < t2Args.count; i++) {
    printf("%d ", t2Args.arr[i]);
  }
  printf("\n");

  pthread_create(&t1, NULL, sortRunner, (void*) &t1Args);
  pthread_create(&t2, NULL, sortRunner, (void*) &t2Args);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  printf("\nsubarray 1 result:\n");
  for (int i = 0; i < t1Args.count; i++) {
    printf("%d ", t1Args.arr[i]);
  }
  printf("\n");
  printf("\nsubarray 2 result:\n");
  for (int i = 0; i < t2Args.count; i++) {
    printf("%d ", t2Args.arr[i]);
  }
  printf("\n");

  // Merging thread
  pthread_t t3;
  mergeRunnerParams t3Args = { &t1Args, &t2Args, sorted };
  pthread_create(&t3, NULL, mergeRunner, (void*) &t3Args);
  pthread_join(t3, NULL);

  printf("\nsorted array:\n");
  for (int i = 0; i < arrayLen; i++) {
    printf("%d ", sorted[i]);
  }
  printf("\n");

  return 0;
}
