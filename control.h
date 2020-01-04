#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>

int crt_file(char * name);
void rem_file(char * name);
int crt_sem();
void rem_sem(int semd);
int crt_shm();
void rem_shm(int shmd);
int print_story();
