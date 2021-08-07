#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PIPE_READ 0
#define PIPE_WRITE 1
#define PIPE_READ_WRITE 2
#define STD_IN 0
#define STD_OUT 1
#define BUFFSIZE 80

#define MSGSIZE 100
#define KEYFILE_PATH "keyfilepath"
#define ID 'M'
#define MSGQ_OK 0
#define MSGQ_NG -1

int main(void)
{
  pid_t pid = 0;                 // process ID
  int pipe_c2p[PIPE_READ_WRITE]; // child to parent
  int num;
  int readSize = 0;

  struct msgbuff
  {
    long mtype;
    int num;
  } message;

  int msqid;
  key_t keyx;
  struct msqid_ds msq;

  memset(pipe_c2p, 0, sizeof(pipe_c2p));

  // Create pipe
  if (pipe(pipe_c2p) == -1)
  {
    perror("processGenerate pipe");
    exit(1);
  }

  keyx = ftok(KEYFILE_PATH, (int)ID);
  msqid = msgget(keyx, 0666 | IPC_CREAT);

  if (msqid == -1)
  {
    perror("msgget");
    exit(1);
  }

  // Create child process
  switch (pid = fork())
  {
  case -1:
    perror("processGenerate fork");
    // close file descriptor for input/output
    close(pipe_c2p[PIPE_READ]);
    close(pipe_c2p[PIPE_WRITE]);
    exit(1);

  case 0:
    // close file descriptor for output
    close(pipe_c2p[PIPE_WRITE]);

    while (1)
    {
      if ((readSize = read(pipe_c2p[PIPE_READ], &num, sizeof(int))) != 0)
      {
        message.mtype = 1;
        message.num = num;
        if ((msgsnd(msqid, (void *)&message, sizeof(int), 0)) == MSGQ_NG)
        {
          perror("msgsnd");
          exit(1);
        }
        if ((msgrcv(msqid, &message, sizeof(int), 1, 0)) == MSGQ_NG)
        {
          perror("msgrcv");
          exit(1);
        }
      }
    }

    // close file descriptor for input
    close(pipe_c2p[PIPE_READ]);
    break;
    
  default:
    // close file descriptor for input
    close(pipe_c2p[PIPE_READ]);

    while (1)
    {
      printf("Enter number :(pid=%d) ", getpid());
      scanf("%d", &num);
      write(pipe_c2p[PIPE_WRITE], &num, sizeof(int));
    }

    // close file descriptor for output
    close(pipe_c2p[PIPE_WRITE]);
    exit(0);
  }
  return 0;
}
