#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/stat.h>

//tar -cf Michael-Boone-HW2.tar makefile Fibb.c Prime.c Total.c Starter.c README.txt

int main(int argc, char* argv[]){
   
   //arrays for sprintf and read()
   char buffer[16];
   char number[10];

   //p[0] = read p[1] = write
   int pi[2];

   //if pipe fails exit
   if (pipe(pi) < 0){
      printf("Pipe failed.\n"); 
      return -1; 
   }

   //format pipe
   sprintf(buffer, "%d", pi[1]);

   //fork process
   pid_t pid = fork();

   //if fork is less than zero, fork failed
   if(pid < 0){
      printf("fork failed.\n");
      return -1;
   }

   //child process
   if(pid == 0){ 
      //close read end of pipe
      close(pi[0]);

      //call charater reader with child process to sum the input.txt
      execlp("./CharacterReader", "./CharacterReader", argv[1], buffer, NULL);
    } 

   //parent process
   else{
      //close write end of pipe
      close(pi[1]);
      //read in the pipe
      read(pi[0], number, sizeof(number));
      //close read end of pipe
      close(pi[0]);
      printf("Starter[%d]: Contents read from the read end pipe: %s \n", getpid(), number);
   }

   //array to store the pids for each child
   int pids[3];

   //array of executable names
   char *exect[3];
   exect[0] = "./Fibb";
   exect[1] = "./Prime";
   exect[2] = "./Total";

   char *name[3];
   name[0] = "F";
   name[1] = "P";
   name[2] = "T";

   char SHM_F[] = "F";
   char SHM_P[] = "P";
   char SHM_T[] = "T";

   //create a shared memory space for fibb
   int fibb_fd = shm_open(SHM_F, O_CREAT | O_RDWR, 0666);
   ftruncate(fibb_fd, 32);
   void *fibbSharedMemPoint = mmap(0, 32, PROT_READ, MAP_SHARED, fibb_fd, 0);

   //create a shared memory space for prime
   int prime_fd = shm_open(SHM_T, O_CREAT | O_RDWR, 0666);
   ftruncate(prime_fd, 32);
   void *primeSharedMemPoint = mmap(0, 32, PROT_READ, MAP_SHARED, prime_fd, 0);

   //create a shared memory space for total
   int total_fd = shm_open(SHM_P, O_CREAT | O_RDWR, 0666);
   ftruncate(total_fd, 32);
   void *totalSharedMemPoint = mmap(0, 32, PROT_READ, MAP_SHARED, total_fd, 0);

   printf("Starter[%u]: Created shared memory \"SHM_Fibb\" with FD: %d \n", getpid(), fibb_fd);
   printf("Starter[%u]: Created shared memory \"SHM_Prime\" with FD: %d \n", getpid(), prime_fd);
   printf("Starter[%u]: Created shared memory \"SHM_Total\" with FD: %d \n", getpid(), total_fd);

   //fork for each program and execute each of the math .c files
   //all children are running concurrently
   for(int i = 0; i < 3; i++){
      pid_t cPID = fork();
      if(cPID < 0){
         printf("Fork failed. \n");
         exit(1);
      }
      else if(cPID == 0){
         execlp(exect[i], exect[i], name[i], number, NULL);
      }
      else{
         pids[i] = cPID;
      }
   }
 
   //wait for children to finish
   int status;
   for(int iter = 0; iter < 3; iter++){
      waitpid(pids[iter], &status, 0);
   }

   //store the results of the child processes
   int fibb_res = atoi(fibbSharedMemPoint);
   int prime_res = atoi(primeSharedMemPoint);
   int total_res = atoi(totalSharedMemPoint);

   printf("Starter[%d]: Fibb last number %d\n", getpid(), fibb_res);
   printf("Starter[%d]: prime last number %d\n", getpid(), total_res);
   printf("Starter[%d]: total last number %d\n", getpid(), prime_res);


   //unlink all the shared memory
   shm_unlink("SHM_Fibb");
   shm_unlink("SHM_Total");
   shm_unlink("SHM_Prime");
}





















   // else if(pid > 0){
   //    close(p[0]);
   //    size_t size = 0;
   //    ssize_t read;
   //    int status; 

   //    while((read = getline(&ptr, &size, fp)) != -1){
   //       write(p[1], c, strlen(c) + 1);
   //       wait(&status);
   //    }
   //    close(p[1]);
   // }

   // else{
   //    close(p[1]);
   //    sprintf(c, "%s", ptr);
   //    execlp(fileName[0], c, NULL);
   // }
   //shm_open() create 3 shared memories for prime total and fibb
   //fork(for every child process)






// int status;                                           //returns the 
//    const char *fileName[3];                              //an array of file names that will be used to call the program after forking
//    //names of the files starter will call
//    fileName[0] = "Fibb";         
//    fileName[1] = "Prime";
//    fileName[2] = "Total";
//    int returnVals[3];                                    //an array used to store the output values of the final calls to fib, prime, total
//    FILE * fp;                                            //a file pointer
//    char *ptr = NULL;                                     //char pointer for getline, used to store the info in the text file on a given line
//    size_t size = 0;                                      //size used in getline
//    ssize_t read;                                         //used in getline to compare against end of file
//    int pid;                                              //used to fetch the pid number of a given process
//    char const* const file = argv[1];                     //store the input.txt file
//    if(argc != 2){
//       printf("Wrong number of arguments given \n");
//       return 1;
//    }
//     fp = fopen(file, "r");                               //open input.txt file to be read
//     while((read = getline(&ptr, &size, fp)) != -1){      //while there is more data in input.txt
//       for(int i = 0; i < 3; i++){                        //loops 3 times, each loop calls either fibb prime or total
//          pid = fork();                                   //fork starter
//          if(pid == 0){                                  
//             if(ptr[read - 1] == '\n'){                    //this if corrects the newline passed into the child processes which resulted in strange newlines
//                ptr[read - 1] = '\0';
//             }
//             execlp(fileName[i], fileName[i], ptr, NULL);    //calls one of the 3 child programs passing in the next line of data from the file
//          }
//          else{
//             printf("%s[%d] : forked process with ID %d \n", argv[0] +2, getpid(), pid);
//             printf("%s[%d] : waiting for proccess [%d] \n", argv[0] +2, getpid(), pid);
//             wait(&status);                                    //waits for the child process to finish 
//             printf("%s Child process %d returned %d. \n", argv[0] +2, pid, WEXITSTATUS(status));
//         }
//         returnVals[i] = WEXITSTATUS(status);                   //stores the return status of the child processes and puts it into the output array
//       } 
//     }

   //print the final stored totals of the child processes
   // printf("fibb: \t%d \n", returnVals[0]);
   // printf("Prime: \t%d \n", returnVals[1]);
   // printf("total Count: \t%d \n", returnVals[2]);


   