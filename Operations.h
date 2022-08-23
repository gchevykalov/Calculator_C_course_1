// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// Operations header file

#ifndef OPERATIONS_H_INCLUDED__
#define OPERATIONS_H_INCLUDED__
#pragma once

// Defining variable list
typedef struct var_list_t {
  variable_t var;
  struct var_list_t* next;
} var_list_t;
// Defining variable repository type
typedef struct variable_repository_t {
  var_list_t* list;
} variable_repository_t;
// Popping a number
void PopNumber(stack_t* stack, double* num, error_t* err);
// Pushing a number
void PushNumber(stack_t* s, double value, error_t* err);
// Creating a variable repository
variable_repository_t* CreateVarRepository(error_t* err);
// Clearing a variable repository
void ClearVarRepository(variable_repository_t* varRepository);
// Variable search
variable_t* FindVar(variable_repository_t* varRepository, char const* name);
// Adding a variable
variable_t* AddVar(variable_repository_t* varRepository, char const* name, error_t* err);
// Const search
variable_t const* FindConst(char const* name);
// Function search
function_t const* FindFunction(char const* name);
// Getting operation
operation_t const* const GetOperation(int operation);

#endif /* OPERATIONS_H_INCLUDED__ */
