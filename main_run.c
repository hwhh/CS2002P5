#include <stdlib.h>
#include <stdio.h>
#include "runcmds.c"


////Main function, which is responsible  for reading in n number of lines
///and storing each line in 2d array. Then processing the lines one at a time.
int main() {
    char **lines = (char **) malloc(sizeof(char *) * 1024);
    for (int i = 0; i < 1024; i++) { lines[i] = (char *) malloc(sizeof(char) * 1024); } //2d array for storing lines
    int count = 0;
    char line[1024];
    size_t size;
    //Read in lines until blank line is seen
    while (fgets(line, sizeof(line), stdin) && (line[0] != '\n')) { //Read in lines
        strcpy(lines[count++], line);
    }
    for (int i = 0; i < count; i++) { //Loop over all the lines
        int cpid = fork(); //Create process
        if (cpid < 0)
            printf("Error creating child"); //Error
        if (cpid != 0) {/* Parent */
            int stat_loc;
            (void) wait(&stat_loc);
        } else { /* Child */
            runcmds(parse_string(lines[i]), -1); //Use child process to process the current line
            exit(78); //Stop child process
        }
    }
    return 0;
}



