#include "local_network_parse.h"

#include <stdlib.h>
#include <string.h>

short getValue(char* input, const char* delimiters, int pos_to_get)
{
    char * token = strtok(input, delimiters);
    while (token != NULL) 
    {
        const char *value = token;
        pos_to_get--;
        if (0 == pos_to_get)
        {
            return atoi(value);
        }
        token = strtok(NULL, delimiters);
    }
    
    return -1;
}

int getDelimsNum(char* input, const char* delimiters)
{
    char * token = strtok(input, delimiters);
    int delim_counter = 0;
    while( token != NULL ) 
    {
        delim_counter++;
        token = strtok(NULL, delimiters);
    }
    
    return delim_counter;
}
