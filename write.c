#include "control.h"

#define SEM_KEY 24602
#define SHM_KEY 24601
#define SEG_SIZE 500
#define FILE_NAME "semaphone.txt"

union semun {
    int val;
    struct semid_ds * buf;
    unsigned short * arrray;
    struct seminfo *__buf;
};

int main() {
  int semd, shmd, fd;
  char * line;
  struct sembuf	sb;
  sb.sem_op = -1;
  sb.sem_num = 0;
  semd = semget(SEM_KEY, 1, 0);
  printf("trying to get in\n");
  semop(semd, &sb, 1);
  shmd = shmget(SHM_KEY, SEG_SIZE, 0600);
  if (shmd < 0){
    printf("error: %s\n",strerror(errno));
    return 0;
  }
  line = shmat(shmd, 0, 0);
  printf("Last addition: %s\n", line);
  printf("Your addition: ");
  fgets(line, SEG_SIZE, stdin);
  line[strlen(line)-1] = 0;
  fd = open(FILE_NAME, O_WRONLY | O_APPEND);
  if (fd < 0){
    printf("error: %s\n", strerror(errno));
    return 0;
  }
  write(fd, line, SEG_SIZE);
  shmdt(line);
  sb.sem_op = 1;
  semop(semd, &sb, 1);
  close(fd);
  return 0;
}
