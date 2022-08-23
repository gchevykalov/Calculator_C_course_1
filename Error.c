// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// The source file which used to handling error

#include <stdio.h>
#include "Error.h"

// Function to create a complete error message
void Error(error_t* err, char const* const str) {
  char const errDecor[] = "ERROR: ";
  
  err->flag = 1;
  sprintf_s(err->message, sizeof(errDecor), "%s", errDecor);
  sprintf_s(err->message + (int)sizeof(errDecor) - 1, MAX_MESSAGE_SIZE - (int)sizeof(errDecor), "%s", str);
}