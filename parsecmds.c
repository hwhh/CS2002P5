#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include "commands.h"

///Takes in the string and returns an instance if the Command struct
Command parse_string(char* input) {
    //Set up variables
    static Command arguments;
    int in = 0, out = 0;
    bool quote = false;
    bool write = false;
    bool read = false;
    //Set up the instance of command stuct
    arguments.total = 0;
    arguments.read = (char*) calloc(strlen(input), sizeof(char));
    arguments.write =(char*) calloc(strlen(input), sizeof(char));
    arguments.prog_args = (char**) malloc(sizeof(char*)*strlen(input));//Set up 2d array
    for(int i=0; i<strlen(input); i++) { arguments.prog_args[i] = (char*)malloc(sizeof(char)*strlen(input)); }

    char * temp = calloc((size_t) strlen(input), sizeof(char)); //Create temp array
    int counter = 0;
    while (input[in] != '\n') { //Loop over characters  in input string
        if(input[in] == '>'){//Write
            write = true;
            arguments.write_c = 1;
            in+=2;
        }else if(input[in] == '<'){//Read
            read = true;
            arguments.read_c = 1;
            in+=2;
        }else if(input[in] == '\"') { //Check for quote
            quote = !quote;
            in++;
        }
        else if(input[in] != ' ' && quote == false){ //Check if space can be ignored
            temp[counter++] =  input[in++]; //Append current character to temp array
        }else if (quote == true){
            temp[counter++] =  input[in++];//Append current character to temp array
        }
        else if (input[in] == ' ' && quote == false){
            if(write || read){ //Check if read or write is set
                write ? (arguments.write = temp) : (arguments.read = temp);
                write ? (write = !write) : (read = !read);
            }else{
                arguments.total++;
                arguments.prog_args[out++] = temp; //Append the temp array to the 2d program arguments array
            }
            temp = calloc((size_t) strlen(input), sizeof(char));
            in++;
            counter = 0;
        }
    }
    if(counter > 0 ){ //If the loop finished and there are characters still in the temp array append them
        if(write || read){//Check if read or write is set
            write ? (arguments.write = temp) : (arguments.read = temp);
        }else {//Append the temp array to the 2d program arguments array
            arguments.total++;
            arguments.prog_args[out] = temp;
        }
    }
    return arguments;
}

///Print out the arguments
void shellparse(Command arguments) {
    arguments.total ==1 ? printf("Run program \"%s\".", arguments.prog_args[0]) : printf("Run program \"%s\" ", arguments.prog_args[0]); //Check if just program name
    if (arguments.total - 1 == 1)//Check for just one argument
        printf("with argument \"%s\".", arguments.prog_args[1]);
    else if (arguments.total - 1 > 1) {//If there are more than one argument
        printf("with arguments ");
        for (int i = 1; i < arguments.total; i++) {
            i == arguments.total - 1 ? printf("\"%s\".", arguments.prog_args[i]) : printf("\"%s\" and ", arguments.prog_args[i]); //Check if last argument
        }
    }//Print write / read information at the end
    if (arguments.read_c == 1)
        printf(" Read the input from file \"%s\".", arguments.read);
    if (arguments.write_c == 1)
        printf(" Write the output to file \"%s\".", arguments.write);
}


