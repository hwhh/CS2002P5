#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>

typedef struct{
    int total;
    int write_c;
    int read_c;
    char *write;
    char *read;
    char **prog_args;
} Arguments;


Arguments parse_string(char* input) {
    static Arguments arguments;
    int in = 0, out = 0;
    bool quote = false;
    bool write = false;
    bool read = false;
    arguments.total = 0;
    arguments.read = (char*) calloc(strlen(input), sizeof(char));
    arguments.write =(char*) calloc(strlen(input), sizeof(char));
    arguments.prog_args = (char**) malloc(sizeof(char*)*strlen(input));
    for(int i=0; i<strlen(input); i++) { arguments.prog_args[i] = (char*)malloc(sizeof(char)*strlen(input)); }

    char * temp = calloc((size_t) strlen(input), sizeof(char));
    int counter = 0;
    while (input[in] != '\n') {//Write
        if(input[in] == '>'){
            write = true;
            arguments.write_c = 1;
            in+=2;
        }else if(input[in] == '<'){//Read
            read = true;
            arguments.read_c = 1;
            in+=2;
        }else if(input[in] == '\"') {
            quote = !quote;
            in++;
        }
        else if(input[in] != ' ' && quote == false){
            temp[counter++] =  input[in++];
        }else if (quote == true){
            temp[counter++] =  input[in++];
        }
        else if (input[in] == ' ' && quote == false){
            if(write || read){
                write ? (arguments.write = temp) : (arguments.read = temp);
                write ? (write = !write) : (read = !read);
            }else{
                arguments.total++;
                arguments.prog_args[out++] = temp;
            }
            temp = calloc((size_t) strlen(input), sizeof(char));
            in++;
            counter = 0;
        }
    }
    if(counter > 0 ){
        if(write || read){
            write ? !write : !read;
            write ? (arguments.write = temp) : (arguments.read = temp);
        }else {
            arguments.total++;
            arguments.prog_args[out] = temp;
        }
    }
    return arguments;
}


void shellparse(Arguments arguments) {
    arguments.total ==1 ? printf("Run program \"%s\".", arguments.prog_args[0]) : printf("Run program \"%s\" ", arguments.prog_args[0]);
    if (arguments.total - 1 == 1)
        printf("with argument \"%s\".", arguments.prog_args[1]);
    else if (arguments.total - 1 > 1) {
        printf("with arguments ");
        for (int i = 1; i < arguments.total; i++) {
            i == arguments.total - 1 ? printf("\"%s\".", arguments.prog_args[i]) : printf("\"%s\" and ", arguments.prog_args[i]);
        }
    }
    if (arguments.read_c == 1)
        printf(" Read the input from file \"%s\".", arguments.read);
    if (arguments.write_c == 1)
        printf(" Write the output to file \"%s\".", arguments.write);

}


int main() {
    char *line = NULL;
    size_t size;
    if (getline(&line, &size, stdin) == -1) {
        printf("No line\n");
    }else {
        shellparse(parse_string(line));


    }
}


//if (input[in] == '>' || input[in] == '<') {
//input[in] == '>' ? (arguments.write_c = 1) : (arguments.read_c = 1);
//int c = 0;
//in += 2;
//while (input[in] != ' ' && input[in] != '\n') {
//arguments.write_c == 1 ? (arguments.write[c++] = input[in++]) : (arguments.read[c++] = input[in++]);
//}
//} else if (input[in] == '\"') {
//quote_count++;
//in++;
//} else if (input[in] == ' ')
//in++;
//else {
//if (quote_count % 2 == 0) {
//char *inter = calloc((size_t) strlen(input), sizeof(char));
//int inter_count = 0;
//while (input[in] != ' ' && input[in] != '\n') {
//if (input[in] == '\"') {
//quote_count++;
//break;
//}
//inter[inter_count++] = input[in++];
//}
//arguments.total++;
//arguments.prog_args[out] = inter;
//arguments.count[out] = inter_count;
//out++;
//} else {
//char *inter = calloc((size_t) strlen(input), sizeof(char));
//int inter_count = 0;
//while (input[in] != ' ' && quote_count % 2 != 0) {
//if (input[in] == '\"'){
//quote_count++;
//in++;
//}
//else
//inter[inter_count++] = input[in++];
//}
//arguments.total++;
//arguments.prog_args[out] = inter;
//arguments.count[out] = inter_count;
//out++;
//}
//}
