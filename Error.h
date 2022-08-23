// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// The header file which used to handling error

#ifndef ERROR_H_INCLUDED__
#define ERROR_H_INCLUDED__
#pragma once

// Maximum number of characters in error message
#define MAX_MESSAGE_SIZE 100

// Error Checkups
#define ERROR_CHECKUP_WITH_ACTION(ERROR, ACTION, RETURN) if ((_Bool)ERROR->flag) {ACTION; return RETURN;}
#define ERROR_CHECKUP_WITHOUT_ACTION(ERROR, RETURN) if ((_Bool)ERROR->flag) {return RETURN;}

// Error type 
typedef struct {
  int flag;
  char message[MAX_MESSAGE_SIZE];
} error_t;

// Function to create a complete error message
void Error(error_t* err, char const* const str);

#endif /* ERROR_H_INCLUDED__ */
