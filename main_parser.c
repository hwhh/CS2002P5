#include <stdio.h>
#include "parsecmds.c"

///Main function to read a line in from stdin and parse it
int main() {
    char *line = NULL;
    size_t size;
    if (getline(&line, &size, stdin) == -1) { //Read in lines until blank line
        printf("No line\n");
    }else {
        shellparse(parse_string(line)); //Parse each line
    }
}