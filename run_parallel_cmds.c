#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "runcmds.c"


struct arg_struct{
    int id;
    Command command;
} arg_struct;

///Process each command
void *run(void *arguments){
    struct arg_struct *args = arguments;
    int cpid = fork();//Create process
    if (cpid < 0)
        printf("Error creating child"); //Error
    if (cpid != 0) {/* Parent */
        int stat_loc;
        (void) wait(&stat_loc);

    } else { /* Child */
        runcmds(args->command, args->id); //Use child process to process the current line
        exit(78);//Stop child process
    }
    pthread_exit((void*)"Goodbye!");
}

///Prints out the contents of all the files created by each thread
void print_out(int count){
    char buf[1024];
    for(int i = 0; i < count; i++){ //Loop number of lines input times
        //Create the paths to the output files
        char filename[1024];
        snprintf(filename, sizeof(filename), "/Users/henryhargreaves/Documents/University/Year_2/CS2002/Practicals/CS2002P5/Practical-SP/temp/output%d.txt", i);
        //Read in file and output contents to stdin
        int file = open(filename,O_RDONLY);
        ssize_t bytes = read(file, &buf, sizeof(buf) - 1); buf[bytes]='\0';
        printf("%s",buf);
    }
}

////Main function, which is responsible  for reading in n number of lines
///and storing each line in 2d array. Then processing the lines in parrallel.
int main(int argc, char *argv[]){
    int no_of_threads;
    argc < 3 ? (no_of_threads = 3):(no_of_threads = atoi(argv[2])); //Set the no_of_threads variable based on the arguments given in
    char **lines = (char **) malloc(sizeof(char *) * 1024);//2d array for storing lines
    for (int i = 0; i < 1024; i++) { lines[i] = (char *) malloc(sizeof(char) * 1024); }
    int count = 0;
    char line[1024];
    while (fgets(line, sizeof(line), stdin) && (line[0] != '\n')) {//Read in lines until blank line is seen
        strcpy(lines[count++], line);
    }
    pthread_t thread_id[no_of_threads];//Create an array of threads
    void *tr1;
    struct arg_struct *com;
    for(int no_of_args = 0; no_of_args < count; no_of_args+=no_of_threads) { //Loop until the number of threads created is equal to the number of lines read in
        for(int i=0;i<no_of_threads;i++) { //Create threads
            if(no_of_args+i >= count)
                break;
            com = malloc(sizeof(struct arg_struct)); //Array to store the arg_struct structures
            (*com).command = parse_string(lines[no_of_args+i]); //Set the fields of the arg_struct
            (*com).id = no_of_args+i;
            pthread_create (&thread_id[i], NULL , run, (void *) com); //Create the thread
        }
        for(int i=0;i<no_of_threads;i++) {
            if(no_of_args+i >= count)
                break;
            pthread_join(thread_id[no_of_args+i], &tr1); //Terminate the threads
        }
    }
    free(com);
    print_out(count);

}

