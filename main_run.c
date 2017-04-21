#include <stdlib.h>
#include <stdio.h>
#include "runcmds.c"

void run(){
    char **lines = (char **) malloc(sizeof(char *) * 1024);
    for (int i = 0; i < 1024; i++) { lines[i] = (char *) malloc(sizeof(char) * 1024); }
    int count = 0;
    char line[1024];
    size_t size;
    while (fgets(line, sizeof(line), stdin) && (line[0] != '\n')) {
        strcpy(lines[count++], line);
    }
    for (int i = 0; i < count; i++) {
        int cpid = fork();
        if (cpid < 0) { /* handle error */}
        if (cpid != 0) {/* Parent */
            int stat_loc;
            (void) wait(&stat_loc);

        } else { /* Child */
            runcmds(parse_string(lines[i]), -1);
            exit(78);
        }
    }

}

int main() {
   run();
}



