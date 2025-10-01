#include "utils.h"
#include <stdlib.h>

int custom_strlen(char* input){ //so that we wont include the whole string.h
    char* start = input;
    while(*input) input++;
    return input - start; //returns bytes, 1 char = 1 byte.
}

char* escape_characters(char* input){
    char* output = malloc(2 * custom_strlen(input) + 1); //worst case, all need to be escaped
    char* start = output; //keep track of the beginning
    while(*input){
        switch(*input){
            case '\n':
                *output++ = '\\';
                *output++ = 'n';
                break;
            case '\t':
                *output++ = '\\';
                *output++ = 't';
                break;
            case '\\':
                *output++ = '\\';
                *output++ = '\\';
                break;
            default:
                *output++ = *input;
        }
        input++;
    }

    *output = '\0';
    return start;
}