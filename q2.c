#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  // n/ 2 if n is even
  // 3n + 1 if n is odd

  char *shm_name = "MyMemory";
  int fd = shm_open(shm_name, O_RDWR | O_CREAT);
  int n = atoi(argv[1]);

  if (fd == -1) {
    printf("Could not create shared memory!");
  }

  int mem_siz = 4096;

  // 2 4 8 16 32 64 128 256 512 1024 2048 5096
  if (ftruncate(fd, mem_siz) == -1) {
    printf("Could not set size of shared memory!");
  }

  int *ptr;
  ptr = mmap(NULL, mem_siz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (ptr == MAP_FAILED) {
    printf("nmap failed.");
  } /* Handle error */;

  pid_t pid = fork();

  int i = 0;

  // child
  if (pid == 0) {
    // Starting number for collatz

    while (n != 1) {

      // write to shared memory
      ptr[i] = n;
      i++;
      if (n % 2 == 0) {
        n = n / 2;
      } else {
        n = (3 * n) + 1;
      }
    }

    // insert 1 manually
    ptr[i++] = 1;
  }

  // when child completes, parent outputs the contents of the shared
  // memory

  waitpid(pid, NULL, 0);

  int j = 0;
  while (j < i) {
    printf("%i ", ptr[j]);
    j++;
  }

  // close shared memory
  shm_unlink(shm_name);
  return 0;
}
