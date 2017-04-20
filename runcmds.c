//
// Created by Henry Hargreaves on 20/04/2017.
//


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include "commands.h"



void runcmds(Command commands){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    strcat(cwd,"/");
    strcat(cwd, commands.prog_args[0]);
    char *args[commands.total];
    args[0] = commands.prog_args[0];
    for(int i = 1; i < commands.total; i++){
        args[i] = commands.prog_args[i];
    }
    args[commands.total] = NULL;
    execv(cwd,args);
}


int main() {
    char *line = NULL;
    size_t size;
    if (getline(&line, &size, stdin) == -1) {
        printf("No line\n");
    }else {
        runcmds(parse_string(line));
    }
}

