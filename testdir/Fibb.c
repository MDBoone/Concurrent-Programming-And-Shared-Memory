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

int fibb(int n)
{
   //vars that keep track of current num and previous num
   int firstNum = 0;                   
   int secondNum = 1;

   //store the result num
   int result;
 
   if (n < 1){
      return 0;
   }
   printf("%d, ", firstNum);
   for (int i = 1; i < n; i++) {
      printf("%d, ", secondNum);
      int next = firstNum + secondNum; //adds the current and previous nums together which will be stored in second num
      firstNum = secondNum;            //update first number to second number
      result = secondNum;              //result keeps getting updated with newest second num until for loop ends, returns last fib number
      secondNum = next;
   }
   printf("\n");
   return result;
}

int main(int argc, char* argv[]){

   //convert the argument to an int
   char argument = atoi(argv[2]);

   printf("Fibb[%u]: Number of terms in fibonacii series is %u: \n", getpid(), argument);
   printf("Fibb[%u]: The first %u numbers of the Fibonacii sequence are: ", getpid(), argument);

   //call the fibb function passing in argv[2] int
   int sum = fibb(argument);
   int size = 32;

   //communicate with the shared memory space for fibb
   int fd = shm_open(argv[1], O_CREAT | O_RDWR, 0666);
   ftruncate(fd, size);
   void *ptr = mmap(0, size, PROT_WRITE, MAP_SHARED, fd,0);
   sprintf(ptr, "%d", sum);
   
   return(sum);
}

