#include <stdio.h>
#include "parsecmds.c"

int main() {
    char *line = NULL;
    size_t size;
    if (getline(&line, &size, stdin) == -1) {
        printf("No line\n");
    }else {
        shellparse(parse_string(line));
    }
}