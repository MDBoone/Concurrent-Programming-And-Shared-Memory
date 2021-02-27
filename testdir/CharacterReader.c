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


int main(int argc, char* argv[]){

    //variables to use getline
    char * row = NULL;
    size_t size = 0;
    ssize_t line;

    //write into the pipe using buffer  
    char bufferForWrite[10];
    int sum = 0;

    //open the file to read
    FILE* fp = fopen(argv[1], "r");

    //file failure
    if (fp == NULL)
        exit(EXIT_FAILURE);

    //read each line of the text file and sum it
    while ((line = getline(&row, &size, fp)) != -1) {
        sum = sum + atoi(row);
    }

    //get pipe id
    int writeEnd = atoi(argv[2]);

    //format for the pipe
    sprintf(bufferForWrite, "%d", sum);

    //write to the pipe
    write(writeEnd, bufferForWrite, sizeof(bufferForWrite));

    fclose(fp);
    return sum;
}