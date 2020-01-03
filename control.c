#include "control.h"

#define SHM_KEY 24602
#define SEM_KEY 11280
#define SEG_SIZE 200

int create(){
  int semd = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT | IPC_EXCL | 0644);
  printf("semaphore created\n");
  if (semd == -1){
    printf("error %d: %s\n", errno, strerror(errno));
  }
  printf("shared memory created\n");
  int fd = open("semaphone.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
  if (fd == -1){
    printf("error %d: %s\n", errno, strerror(errno));
  }
  printf("file created\n");
  close(fd);
  return 0;
}

int remove_story(){
  int semd = semget(SEM_KEY, 1, 0);
  if (semd == -1){
    printf("error %d: %s\n", errno, strerror(errno));
  }
  printf("trying to get in\n");
  semop(semd, &sb, 1);
  int shmd = shmget(SHM_KEY, sizeof(char*), 0);
  if (shmd == -1){
    printf("error %d: %s\n", errno, strerror(errno));
  }
  view();
  shmctl(shmd, IPC_RMID,0);
  printf("shared memory removed\n");
  remove("semaphone.txt");
  printf("file removed\n");
  semctl(semd, IPC_RMID, 0);
  printf("semaphore removed\n");
  return 0;
}

int view(){
	//Open file for reading only...

	FILE *fptr;
	char ch;
	fptr = fopen("semaphone.txt", "r");
	if (fptr == NULL){
		printf("File can't be opened\n");
		return -1;
	}
	printf("The story so far...\n");

	//Print file character by character...
	ch = fgetc(fptr);
	while (ch != EOF){
		printf("%c", ch);
		ch = fgetc(fptr);
	}
	printf("\n");
	fclose(fptr);
	//sb.sem_num = 0;
	//semop(semd, &sb, 1);

	return 0;
}
