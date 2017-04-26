
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "parsecmds.c"
///Gets the current directory and appends a black slash to end
char *get_dir(){
    static char cwd[1024]; //Define static array to store result
    getcwd(cwd, sizeof(cwd)); //Get current directory
    strcat(cwd,"/"); //Append backslash
    return cwd;
}


///Write the output of parallel calls to separate files
void run_parrallel(char* output, const char *cwd, char * args[],int id){
    char filename[1024]; //Creates array to hold path
    snprintf(filename, sizeof(filename), "output%d.txt", id); //Creates path to file
    int file = open(filename,O_CREAT| O_TRUNC |O_WRONLY|O_APPEND, S_IRWXU); //Opens the file
    if(dup2(file,1) < 0) return; //If file opened successfully divert output
    if(output != NULL) { //If not a command to be executed
        printf("%s", output); //Write input to file
        printf("\n");
    }else{ //Else execute the command
        int r = execv(cwd, args);
        if (r < 0 ) { printf("Execute failed: %s", cwd); }
    }
    close(file);// Close file
}
///Write output to a file, given the directory and the program arguments
void run_write(char * dir, int id, Command commands, const char * cwd, char *args[]){
    int file = open(dir,O_CREAT|O_WRONLY|O_APPEND,S_IRWXU); //Opens / creates file with correct permissions
    if(file < 0) { //Error checking
        if(id>=0) { //If call from parallel output to correct file
            char dest[50];
            strcpy(dest, "Write failed: ");
            strcat(dest, commands.write);
            run_parrallel(dest,cwd, args ,id);
        }else//Otherwise print to stdout
            printf("Write failed: %s \n", commands.write);
        return;
    }
    if(dup2(file,1) < 0) return;//If file opened successfully divert output
    execv(cwd, args);//Execute command
    close(file);// Close file
}

///Read input from a file, given the directory and the program arguments
void run_read(char * dir, int id, Command commands,const char * cwd, char *args[]){
    int file = open(dir, O_RDONLY); //Opens file in read only mode
    if (file < 0) {
        if (id >= 0) { //Error checking
            char dest[50]; //If call from parallel output to correct file
            strcpy(dest, "Read failed: ");
            strcat(dest, commands.read);
            run_parrallel(dest,cwd, args ,id);
        } else //Otherwise print to stdout
            printf("Read failed: %s \n", commands.read);
        return;
    }
    if (dup2(file, 1) < 0) return; //If file opened successfully divert output
    execv(cwd, args); //Execute command
    close(file); // Close file
    return;
}

///Given a command and an id run the command
void runcmds(Command commands, int id){
    char *path = get_dir(); //Get the current directory
    const char *cwd = commands.prog_args[0]; //The name of the program to be executed
    char *args[commands.total]; //Create the arguments array
    args[0] = commands.prog_args[0]; //The name of the program to be executed
    for(int i = 1; i < commands.total; i++){ //Add all of the arguments from the command to the arguments array
        args[i] = commands.prog_args[i];
    }
    args[commands.total] = NULL; //Add null to end of arguments array
    if (strlen (commands.write) > 0){ //Write
        char *dir = strcat(path, commands.write); //Add the file name to the path
        run_write(dir,id ,commands, cwd, args);
        return;
    }if (strlen (commands.read) > 0) { //Read
        char *dir = strcat(path, commands.read); //Add the file name to the path
        run_read(dir,id ,commands, cwd, args);
        return;
    }if(id >= 0){//If call from parallel
        run_parrallel(NULL,cwd, args ,id);
        return;
    } else {
        int r = execv(cwd, args); //If standard call
        if (r < 0){printf("Execute failed: %s", cwd); }
    }
    //Free arrays
    free(commands.write);
    free(commands.read);
    free(commands.prog_args);
}

