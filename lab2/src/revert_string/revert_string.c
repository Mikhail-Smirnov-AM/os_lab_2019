#include "revert_string.h"

void RevertString(char *str)
{
	char *str_copy = (char*)malloc(sizeof(char) * (strlen(str) + 1));
  strcpy(str_copy,str);
  
  for(int i = 0; i < strlen(str); i++)
    str[i] = str_copy[strlen(str)-1-i];
  
  free(str_copy);
  
}

