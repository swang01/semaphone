#include "control.h"

#define SEM_KEY 24602
#define SHM_KEY 24601
#define SEG_SIZE 500
#define FILE_NAME "semaphone.txt"

union semun {
    int val;
    struct semid_ds * buf;
    unsigned short * array;
    struct seminfo *__buf;
};

int control(){
  int semd;
  int r;
  int v;
  semd = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  if (semd == -1) {
    printf("error %s\n", strerror(errno));
    semd = semget(SEM_KEY,1,0);
    v = semctl(semd, 0, GETVAL, 0);
  }
  else{
    union semun usl;
    usl.val = 1;
    r = semctl(semd, 0, SETVAL, usl);
  }
  printf("semaphore created\n");
  return semd;
}

int viewing(){
  printf("The story so far:\n");
  char story[SEG_SIZE];
  int fd;
  fd = open(FILE_NAME, O_RDONLY);
  int bytes = read(fd, story, sizeof(story));
  if (bytes < 0){
    printf("Reading file errno: %d\nerror: %s\n",errno, strerror(errno));
    return 0;
  }
  while(bytes > 0){
    printf("%s\n", story);
    bytes = read(fd, story, sizeof(story));
  }
  close(fd);
  printf("\n");
}

int main(int argc, char * argv[]) {
  int semd;
  int shmd;
  int fd;
  if (argc == 2) {
    if(strcmp(argv[1], "-c") == 0) {
      semd = control();
      shmd = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT|0600);
      printf("shared memory created\n");
      fd = open(FILE_NAME, O_CREAT | O_EXCL | O_RDWR | O_TRUNC, 0644);
      if (fd < 0) {
        fd = open(FILE_NAME, O_RDWR | O_TRUNC);
      }
      printf("file created\n");
    }
    else if (strcmp(argv[1], "-r") == 0){
      viewing();
      semd = semget(SEM_KEY, 1, 0);
      shmd = shmget(SHM_KEY, SEG_SIZE, 0600);
      shmctl(shmd, IPC_RMID, 0);
      printf("shared memory removed\n");
      if (remove(FILE_NAME) == 0) {
        printf("file removed\n");
      } else {
        printf("file not removed");
      }
      semctl(semd, IPC_RMID, 0);
      printf("semaphore removed\n");
    }
    else if (strcmp(argv[1], "-v") == 0) {
      viewing();
    }
  }
  else if (argc == 1){
    printf("Argument needed\n");
  }
  else if (argc > 2){
    printf("Too many arguments\n");
  }
  return 0;
}
