#include <cstdlib>
#include <stdint.h>
#include <inttypes.h>

#include <stdio.h>
#include <cstring>
#include <iostream>

#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "common/snrServiceInfo.h"

union semun{
 int val;
 struct semid_ds *buf;
 unsigned short int *array;
 struct seminfo *__buf;
};

int binary_semaphore_allocation(key_t key, int sem_flags)
{
 return semget(key, 1, sem_flags);
}

int binary_semaphore_deallocate(int semid)
{
 union semun ignored_argument;
 return semctl(semid, 1, IPC_RMID, ignored_argument);
}

int binary_semaphore_initalize(int semid)
{
 union semun argument;
 unsigned short values[1];
 values[0] = 1;
 argument.array = values;
 return semctl(semid, 0, SETALL, argument);
}

int binary_semaphore_wait(int semid)
{
    /* Mode bits for `msgget', `semget', and `shmget'.  */
    //#define IPC_CREAT	    01000		/* Create key if key does not exist. */
    //#define IPC_EXCL	    02000		/* Fail if key exists.  */
    //#define IPC_NOWAIT    04000		/* Return error on wait.  */

 struct sembuf operations[1];
 operations[0].sem_num = 0;
 operations[0].sem_op = -1;

 operations[0].sem_flg = IPC_NOWAIT;//0;

//The fields of struct sembuf are listed here:

// --- sem_num  --- is the semaphore number in the semaphore set on which the operation is performed.
// --- sem_op   --- is an integer that specifies the semaphore operation.
//If sem_op is a positive number, that number is added to the semaphore value
//immediately.

//If sem_op is a negative number, the absolute value of that number is subtracted
//from the semaphore value. If this would make the semaphore value negative, the
//call blocks until the semaphore value becomes as large as the absolute value of
//sem_op (because some other process increments it).

//If sem_op is zero, the operation blocks until the semaphore value becomes zero.
// --- sem_flg --- is a flag value. Specify IPC_NOWAIT to prevent the operation from
//blocking; if the operation would have blocked, the call to semop fails instead.
//If you specify SEM_UNDO, Linux automatically undoes the operation on the
//semaphore when the process exits.


 return semop(semid, operations, 1);
}

int binary_semaphore_post(int semid)
{
 struct sembuf operations[1];
 operations[0].sem_num = 0;
 operations[0].sem_op =  1;
 operations[0].sem_flg = IPC_NOWAIT;//1;//////////////SEM_UNDO;/////////!!!!!!!!!!1
 return semop(semid, operations, 1);
}

int spawn(char * program, char ** arg_list)
{
  //printf("\n11 spawn: program= %s arg_list[0]= %s arg_list[1]= %s arg_list[2]= %s arg_list[3]= %s\n",
   //                   program, arg_list[0], arg_list[1], arg_list[2], arg_list[3]);
 pid_t child_pid;
 child_pid = fork();
 if(child_pid != 0)
  return child_pid;
 else
 {
  execvp(program, arg_list);
  /////fprintf(stderr, "an error occurred in execvp\n");
  abort();
 }
}


void clean_up_up_child_process(int signal_number)
{
 int status;
 wait(&status);
}


pid_t proc_find(const char* name, long secondPid)
{
    DIR* dir;
    struct dirent* ent;
    char buf[512];

    long  pid;
    char pname[100] = {0,};
    char state;
    FILE *fp=NULL;

    if (!(dir = opendir("/proc"))) {
        //perror("can't open /proc");
        return -1;
    }

    while((ent = readdir(dir)) != NULL)
    {
        long lpid = atol(ent->d_name);
        if(lpid < 0 || secondPid >= 0 && lpid == secondPid)
            continue;

        snprintf(buf, sizeof(buf), "/proc/%ld/stat", lpid);

        //qDebug()<<"proc_find:"<<buf;

        //long timeTiks, x;

        fp = fopen(buf, "r");

        if (fp)
        {
            if ( (fscanf(fp, "%ld (%[^)]) %c",       &pid, pname, &state)) != 3 )
//            if ( (fscanf(fp, "%ld (%[^)]) %c %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld ",
//                     &pid, pname, &state, &x, &x, &x, &x, &x, &x, &x, &x, &x, &x, &x, &x, &x, &x, &x, &x, &x, &x, &timeTiks)) != 22 )
            {
                printf("fscanf failed \n");
                fclose(fp);
                closedir(dir);
                return -1;
            }
            if (!strcmp(pname, name))
            {
                //qDebug()<<"proc_find:"<<buf<<timeTiks;

                fclose(fp);
                closedir(dir);
                return (pid_t)lpid;
            }
            fclose(fp);
        }
    }

closedir(dir);
return -1;
}

