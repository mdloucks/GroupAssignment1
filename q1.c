#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int collatz(int n) {
  printf("%d ", n);
  if (n == 1) {
    printf("\n");
    return n;
  }
  else if (n % 2 == 0) return collatz(n/2);
  else return collatz(3 * n + 1);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("error: expected an input number\n");
    return 1;
  }

  int num = atoi(argv[1]);
  if (num <= 0) {
    printf("error: expected a positive input number\n");
    return 1;
  }

  pid_t pid = fork();
  if (pid == 0) {
    // Child
    collatz(num);
  } else {
    // Parent
    wait(NULL);
  }

  return 0;
}
