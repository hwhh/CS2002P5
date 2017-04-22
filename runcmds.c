
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "parsecmds.c"


//void runWrite(int file, Command commands, char * cwd, char *args[]){
//    if(file < 0) {
//        printf("Write failed: %s\n", commands.write);
//        return;
//    }
//    if(dup2(file,1) < 0)    return;
//    execv(cwd, args);
//    close(file);
//}
//
//void runRead(int file, Command commands, char * cwd, char *args[]){
//    if (file < 0) {
//        printf("Read failed: %s \n", commands.read);
//        return;
//    }
//    if (dup2(file, 1) < 0) return;
//    execv(cwd, args);
//    close(file);
//}

void runParrallel(char* output, int id){
    char filename[1024];
    snprintf(filename, sizeof(filename), "/Users/henryhargreaves/Documents/University/Year_2/CS2002/Practicals/CS2002P5/Practical-SP/temp/output%d.txt", id);
    int file = open(filename,O_CREAT| O_TRUNC |O_WRONLY|O_APPEND, S_IRWXU);
    if(dup2(file,1) < 0)    return;
    printf(output);
    printf("\n");
    close(file);
}

void runcmds(Command commands, int id){
    //char cwd[1024];
    //getcwd(cwd, sizeof(cwd));
    //strcat(cwd,"/");
    //strcat(cwd, commands.prog_args[0]);
    //char *cwd = "/Users/henryhargreaves/Documents/University/Year_2/CS2002/Practicals/CS2002P5/Test/print_args.sh";
    const char *cwd = commands.prog_args[0];
    char *args[commands.total];
    args[0] = commands.prog_args[0];
    for(int i = 1; i < commands.total; i++){
        args[i] = commands.prog_args[i];
    }
    args[commands.total] = NULL;
    char path[1024] = "/Users/henryhargreaves/Documents/University/Year_2/CS2002/Practicals/CS2002P5/Practical-SP/";
    if (strlen (commands.write) > 0){
        char *dir = strcat(path, commands.write);
        int file = open(dir,O_CREAT|O_WRONLY|O_APPEND,S_IRWXU);
        if(file < 0) {
            if(id>=0) {
                char dest[50];
                strcpy(dest, "Write failed: ");
                strcat(dest, commands.write);
                runParrallel(dest, id);
            }else
                printf("Write failed: %s \n", commands.write);
            return;
        }
        if(dup2(file,1) < 0)    return;
        execv(cwd, args);
        close(file);
    }if (strlen (commands.read) > 0) {
        char *dir = strcat(path, commands.read);
        int file = open(dir, O_RDONLY);
        if (file < 0) {
            if (id >= 0) {
                char dest[50];
                strcpy(dest, "Read failed: ");
                strcat(dest, commands.read);
                runParrallel(dest, id);
            } else
                printf("Read failed: %s \n", commands.read);
            return;
        }
        if (dup2(file, 1) < 0) return;
        execv(cwd, args);
        close(file);
    }if(id >= 0){
        char filename[1024];
        char *dir = snprintf(filename, sizeof(filename), "/Users/henryhargreaves/Documents/University/Year_2/CS2002/Practicals/CS2002P5/Practical-SP/temp/output%d.txt", id);
        int file = open(filename,O_CREAT|O_WRONLY|O_TRUNC |O_APPEND, S_IRWXU);
        if(file < 0) { return; }
        if(dup2(file,1) < 0)    return;
        int r = execv(cwd, args);
        if (r < 0 ) { printf("Execute failed: %s", cwd); }
        close(file);
    } else {
        int r = execv(cwd, args);
        if (r < 0){printf("Execute failed: %s", cwd); }
    }

    free(commands.write);
    free(commands.read);
    free(commands.prog_args);
}
