#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main(int argc, char *argv[]){
  if(argc != 3) {
    fprintf(stderr, "usage: %s key_name message\n", argv[0]);
    return 1;
  }

  key_t key = ftok(argv[1], 'a');

  if(key < 0) {
    fprintf(stderr, "Error creating key\n");
    return 2;
  }

  int shmid = shmget(key, 1024, 0666 | IPC_CREAT | IPC_EXCL);

  if(shmid < 0) {
    fprintf(stderr, "Error getting shared memory\n");
    return 3;
  }

  void *seg = shmat(shmid, NULL, 0);

  if(seg == (void *)-1) {
    fprintf(stderr, "Error attaching shared memory segment\n");
    return 4;
  }

  strcpy(seg, argv[2]);
  puts("Press <ENTER> to continue...");
  getchar();

  if(shmdt(seg) == -1) {
    fprintf(stderr, "Error detaching shared memory segment\n");
    return 5;
  }

  if(shmctl(shmid, IPC_RMID, NULL) == -1){
    fprintf(stderr, "Error removing sharem memory\n");
     return 6;
   }

  return 0;
}
