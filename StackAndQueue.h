// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// Stack and queue header file

#ifndef STACK_AND_QUEUE_H_INCLUDED__
#define STACK_AND_QUEUE_H_INCLUDED__
#pragma once

#include "Error.h"

// Max size of name
#define MAX_NAME_SIZE 100

// Types of token
typedef enum {
  TOKEN_TYPE_NUMBER,
  TOKEN_TYPE_CONST,
  TOKEN_TYPE_FUNCTION,
  TOKEN_TYPE_VARIABLE,
  TOKEN_TYPE_OPEN_BRACKET,
  TOKEN_TYPE_CLOSE_BRACKET,
  TOKEN_TYPE_OPERATION
} TOKEN_TYPE;
// Types declaration
typedef struct variable_t variable_t;
typedef struct function_t function_t;
typedef struct operation_t operation_t;
// Token type definition
typedef struct {
  TOKEN_TYPE type;
  union {
    double number;
    operation_t const* operation;
    variable_t const* constVal;
    function_t const* function;
    variable_t* variable;
  } value;
} token_t;
// List type definition
typedef struct list_t {
  token_t element;
  struct list_t* next;
} list_t;
// Queue type definition
typedef struct queue_t {
  list_t* head;
  list_t* tail;
} queue_t;
// Stack type definition
typedef struct {
  list_t* top;
} stack_t;
//Types definition
typedef struct variable_t {
  char name[MAX_NAME_SIZE];
  double value;
  int isInit;
} variable_t;
typedef struct function_t {
  char name[MAX_NAME_SIZE];
  void (*function)(stack_t*, error_t*);
} function_t;
typedef struct operation_t {
  int prior;
  char assoc;
  void (*operation)(stack_t*, error_t*);
} operation_t;

// Adding an token to the queue
int Put(queue_t* queue, const token_t* newTok, error_t* err);
// Pulling an token from the queue
int Get(queue_t* queue, token_t* oldTok);
// Queue cleanup
void ClearQueue(queue_t* queue);
// Adding an token to the stack
int Push(stack_t* stack, const token_t* newTok, error_t* err);
// Pulling an token from the stack
int Pop(stack_t* stack, token_t* oldTok);
// Seeing the top token in the stack
int Top(stack_t const* stack, token_t* tok);
// Stack cleanup
void ClearStack(stack_t* stack);
// Ñhecking the stack for emptiness
int IsItAnEmptyStack(stack_t const* stack);

#endif /* STACK_AND_QUEUE_H_INCLUDED__ */
