#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "runcmds.c"


struct arg_struct{
    int id;
    Command command;
} arg_struct;

void *run(void *arguments){
    struct arg_struct *args = arguments;
    int cpid = fork();
    if (cpid < 0) { /* handle error */}
    if (cpid != 0) {/* Parent */
        int stat_loc;
        (void) wait(&stat_loc);

    } else { /* Child */
        runcmds(args->command, args->id);
        exit(78);
    }
    pthread_exit((void*)"Goodbye!");
}


int main(int argc, char *argv[]){
    int no_of_threads;
    argc < 3 ? (no_of_threads = 3):(no_of_threads = atoi(argv[2]));
    char **lines = (char **) malloc(sizeof(char *) * 1024);
    for (int i = 0; i < 1024; i++) { lines[i] = (char *) malloc(sizeof(char) * 1024); }
    int count = 0;
    char line[1024];
    while (fgets(line, sizeof(line), stdin) && (line[0] != '\n')) {
        strcpy(lines[count++], line);
    }
    pthread_t thread_id[no_of_threads];
    void *tr1;

    struct arg_struct *com;
    for(int no_of_args = 0; no_of_args < count; no_of_args+=no_of_threads) {
        for(int i=0;i<no_of_threads;i++) {
            if(no_of_args+i >= count)
                break;
            com = malloc(sizeof(struct arg_struct));
            (*com).command = parse_string(lines[no_of_args+i]);
            (*com).id = no_of_args+i;
            pthread_create (&thread_id[i], NULL , run, (void *) com);
        }
        for(int i=0;i<no_of_threads;i++) {
            if(no_of_args+i >= count)
                break;
            pthread_join(thread_id[no_of_args+i], &tr1);
        }
    }
    free(com);
    char buf[1024];
    for(int i = 0; i < count; i++){
        char filename[1024];
        char *dir = snprintf(filename, sizeof(filename), "/Users/henryhargreaves/Documents/University/Year_2/CS2002/Practicals/CS2002P5/Practical-SP/temp/output%d.txt", i);
        int file = open(filename,O_RDONLY);
        ssize_t bytes = read(file, &buf, sizeof(buf) - 1); buf[bytes]='\0';
        printf("%s",buf);
    }

}

