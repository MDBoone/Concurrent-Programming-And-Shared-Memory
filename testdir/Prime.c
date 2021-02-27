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

int primeCheck(long long int number)                //tells prime whether or not the number being looked at is prime or not
{
    for (int i = 2; i*i <= number; i++) {         
        if (number % i == 0) return 0;              //if number can be devided then it is not prime
    }
    return 1;                                       //else it is prime
}

int prime(int n){
    int counter = 0;
    int result = 0;
    if (n == 1){                    // if number = 1 it would only print 2 because its the first prime #
        printf("%d, ", 2);
        return 2;
    }
    printf("%d, ", 2);              //print the first prime number because we are starting on the second one
    for (result = 3; ; result += 2)
    {
        if (primeCheck(result))
        {
            printf("%d, ", result);  //print each prime found after 2
            counter++;
            if (counter == (n - 1))
                return result;
        }
    }
}

int main(int argc, char* argv[]){

    //change from char* to int
    int argument = atoi(argv[2]);          
    printf("%s[%d] : First %s prime numbers are: \n",  argv[0] +2, getpid(), argv[2]);

    //store the result of the prime function in sum
    int sum = prime(argument);
    printf("\n");
    int size = 32;

    //communicate with the shared memory space for prime
   int fd = shm_open(argv[1], O_CREAT | O_RDWR, 0666);
   ftruncate(fd, size);
   void *ptr = mmap(0, size, PROT_WRITE, MAP_SHARED, fd,0);
   sprintf(ptr, "%d", sum);
   return(sum);
}