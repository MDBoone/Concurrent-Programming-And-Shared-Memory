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

int total(int n){
   int result = 0;                        //return variable
   for(int i = 0; i <= n; i++){           //iterates through adding all numbers starting from 0 to n
      result += i;                        //add next increment to return var
   }
   return result;
}

int main(int argc, char* argv[]){

   //change argv from char* to int
   int argument = atoi(argv[2]);  

   //call the total function and store the value of sum      
   int sum = total(argument);           
   printf("%s[%d] : sum =  %d \n",  argv[0] +2, getpid(), total(argument));
   int size = 32;

   //communicate with the shared memory space for total
   int fd = shm_open(argv[1], O_CREAT | O_RDWR, 0666);
   ftruncate(fd, size);
   void *ptr = mmap(0, size, PROT_WRITE, MAP_SHARED, fd,0);
   sprintf(ptr, "%d", sum);
   return(sum);
}