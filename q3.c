#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *runner(void *param);

int fibArray[1000];

int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stderr, "usage: a.out <integer value>\n");
    /*exit(1);*/
    return -1;
  }
  if (atoi(argv[1]) < 0) {
    fprintf(stderr, "Argument %d must be non-negative\n", atoi(argv[1]));
    /*exit(1);*/
    return -1;
  }

  pthread_t tid;       /* the thread identifier */
  pthread_attr_t attr; /* set of attributes for the thread */
  if (argc != 2) {
    fprintf(stderr, "usage: a.out <integer value>\n");
    /*exit(1);*/
    return -1;
  }
  if (atoi(argv[1]) < 0) {
    fprintf(stderr, "Argument %d must be non-negative\n", atoi(argv[1]));
    /*exit(1);*/
    return -1;
  }
  /* get the default attributes */
  pthread_attr_init(&attr);
  /* create the thread */
  pthread_create(&tid, &attr, runner, argv[1]);
  /* now wait for the thread to exit */
  pthread_join(tid, NULL);

  // now print the generated sequence
  int n = atoi(argv[1]);
  for (int i = 0; i < n; i++) {
    printf("%i ", fibArray[i]);
  }
}

int fib(int i) {
  if (i == 0) {
    return 0;
  } else if (i == 1) {
    return 1;
  } else {
    return fib(i - 1) + fib(i - 2);
  }
}

void *runner(void *param) {
  // calculate fibbonacci sequence

  int _, n = atoi(param);

  if (n >= 1000) {
    printf("Input number too big! Must be less than 1000.");
  }

  for (int i = 0; i < n; i++) {
    fibArray[i] = fib(i);
  }

  pthread_exit(0);
}
