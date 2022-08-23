// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// Operations sourse file

#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <float.h>
#include <assert.h>
#include "Error.h"
#include "StackAndQueue.h"
#include "Operations.h"

#pragma warning(disable: 4996)

// Popping a number
void PopNumber(stack_t* stack, double* num, error_t* err) {
  token_t tok;
  
  if (!(_Bool)Pop(stack, &tok)) {
    Error(err, "not enough operands");
    return;
  }
  // Handling a number
  if ((_Bool)(tok.type == TOKEN_TYPE_NUMBER)) {
    *num = tok.value.number;
  }
  else if ((_Bool)(tok.type == TOKEN_TYPE_CONST)) {
    assert(tok.value.constVal != NULL);
    *num = tok.value.constVal->value;
  }
  else if ((_Bool)(tok.type == TOKEN_TYPE_VARIABLE)) {
    assert(tok.value.variable != NULL);
    if (tok.value.variable->isInit) {
      *num = tok.value.variable->value;
    }
    else {
      Error(err, "uninitialised variable");
      return;
    }
  }
  // Handling operation or function
  else {
    Error(err, "expect a number");
    return;
  }
}
// Pushing a number
void PushNumber(stack_t* stack, double value, error_t* err) {
  token_t tok;
  
  tok.type = TOKEN_TYPE_NUMBER;
  tok.value.number = value;
  Push(stack, &tok, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Addition
static void _add(stack_t* stack, error_t* err) {
  double a, b;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  PopNumber(stack, &b, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  PushNumber(stack, a + b, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Subtraction
static void _sub(stack_t* stack, error_t* err) {
  double a, b;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  PopNumber(stack, &b, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  PushNumber(stack, b - a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Multiplication
static void _mul(stack_t* stack, error_t* err) {
  double a, b;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  PopNumber(stack, &b, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  PushNumber(stack, a * b, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Division
static void _div(stack_t* stack, error_t* err) {
  double a, b;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  PopNumber(stack, &b, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  if (a == 0.0) {
    Error(err, "division by zero");
    return;
  }
  PushNumber(stack, b / a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Exponentiation
static void _power(stack_t* stack, error_t* err) {
  double a, b, c;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  PopNumber(stack, &b, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  c = pow(b, a);
  if ((_Bool)_isnan(c) || !(_Bool)_finite(c)) {
    Error(err, "error with power");
    return;
  }
  PushNumber(stack, c, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Unary minus
static void _unarMinus(stack_t* stack, error_t* err) {
  double a;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  PushNumber(stack, -a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Assignment
static void _assign(stack_t* stack, error_t* err) {
  double a;
  token_t tok;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  Pop(stack, &tok);
  if (tok.type != TOKEN_TYPE_VARIABLE) {
    Error(err, "error with '='");
    return;
  }
  tok.value.variable->isInit = 1;
  tok.value.variable->value = a;
  PushNumber(stack, a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Comma (do nothing)
static void _comma(stack_t* stack, error_t* err) {
  token_t tok;
  
  err->flag = 0;
  Top(stack, &tok);
}
// Semicolon
static void _semicolon(stack_t* stack, error_t* err) {
  token_t tok1;
  token_t tok2;
  
  Pop(stack, &tok1);
  Pop(stack, &tok2);
  Push(stack, &tok1, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Getting operation
operation_t const* const GetOperation(int operation) {
  static const struct {
    int symbol;
    operation_t operation;
  } operations[] = {
    {'+', {2, '>', _add}},
    {'-', {2, '>', _sub}},
    {'*', {3, '>', _mul}},
    {'/', {3, '>', _div}},
    {'^', {5, '<', _power}},
    {'~', {4, '=', _unarMinus}},
    {'=', {1, '<', _assign}},
    {',', {0, '>', _comma}},
    {';', {0, '>', _semicolon}},
  };
  int counter;
  
  // Finding operation in table
  for (counter = 0; (unsigned)counter < sizeof(operations) / sizeof(operations[0]); counter++) {
    if (operation == operations[counter].symbol) {
      return &operations[counter].operation;
    }
  }
  return NULL;
}
// Sin function
static void _sin(stack_t* stack, error_t* err) {
  double a;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  PushNumber(stack, sin(a), err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Cos function
static void _cos(stack_t* stack, error_t* err) {
  double a;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  PushNumber(stack, cos(a), err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Tg function
static void _tg(stack_t* stack, error_t* err) {
  double a, b;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  b = tan(a);
  if ((_Bool)((_Bool)_isnan(b) || !(_Bool)_finite(b))) {
    Error(err, "error with tg");
    return;
  }
  PushNumber(stack, b, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Ctg function
static void _ctg(stack_t* stack, error_t* err) {
  double a, b;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  b = tan(a);
  if ((_Bool)((_Bool)_isnan(b) || (_Bool)(b == 0.0))) {
    Error(err, "error with ctg");
    return;
  }
  if (!(_Bool)_finite(b)) {
    PushNumber(stack, 0, err);
    ERROR_CHECKUP_WITHOUT_ACTION(err, );
  }
  PushNumber(stack, 1.0 / b, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Arcsin function
static void _arcsin(stack_t* stack, error_t* err) {
  double a;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  if ((_Bool)(a < -1.0 || a > 1.0)) {
    Error(err, "error with arcsin");
    return;
  }
  PushNumber(stack, asin(a), err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Arccos function
static void _arccos(stack_t* stack, error_t* err) {
  double a;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  if ((_Bool)(a < -1.0 || a > 1.0)) {
    Error(err, "error with arccos");
    return;
  }
  PushNumber(stack, acos(a), err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Arctg function
static void _arctg(stack_t* stack, error_t* err) {
  double a;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  PushNumber(stack, atan(a), err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Sqrt function
static void _sqrt(stack_t* stack, error_t* err) {
  double a;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  if ((_Bool)(a < 0.0)) {
    Error(err, "error with sqrt");
    return;
  }
  PushNumber(stack, sqrt(a), err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Floor function
static void _floor(stack_t* stack, error_t* err) {
  double a;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  PushNumber(stack, floor(a), err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Ceil function
static void _ceil(stack_t* stack, error_t* err) {
  double a;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  PushNumber(stack, ceil(a), err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Ln function
static void _ln(stack_t* stack, error_t* err) {
  double a;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  if ((_Bool)(a <= 0.0)) {
    Error(err, "error with ln");
    return;
  }
  PushNumber(stack, log(a), err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Log function
static void _log(stack_t* stack, error_t* err) {
  double a, b;
  
  PopNumber(stack, &a, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  PopNumber(stack, &b, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  if ((_Bool)((_Bool)(unsigned)(a <= 0.0) || (_Bool)(unsigned)(b <= 0.0) || (_Bool)(unsigned)(b == 1.0))) {
    Error(err, "error with log");
    return;
  }
  PushNumber(stack, log(a) / log(b), err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Creating a variable repository
variable_repository_t* CreateVarRepository(error_t* err) {
  variable_repository_t* repository = malloc(sizeof(variable_repository_t));
  
  if (repository == NULL) {
    Error(err, "not enough memory");
    return repository;
  }
  repository->list = NULL;
  
  return repository;
}
// Clearing a variable repository
void ClearVarRepository(variable_repository_t* varRepository) {
  var_list_t* node = varRepository->list;
  
  while (node != NULL) {
    node = varRepository->list->next;
    free(varRepository->list);
    varRepository->list = node;
  }
  free(varRepository);
}
// Variable search
variable_t* FindVar(variable_repository_t* varRepository, char const* name) {
  var_list_t* node = varRepository->list;
  
  while (node != NULL) {
    if ((_Bool)(strcmp(name, node->var.name) == 0)) {
      return &node->var;
    }
    node = node->next;
  }
  
  return NULL;
}
// Adding a variable
variable_t* AddVar(variable_repository_t* varRepository, char const* name, error_t* err) {
  variable_t* var = FindVar(varRepository, name);
  
  if ((_Bool)(var == NULL)) {
    var_list_t* node = malloc(sizeof(var_list_t));
    if (node == NULL) {
      Error(err, "not enough memory");
      return NULL;
    }
    node->next = varRepository->list;
    node->var.isInit = 0;
    strcpy(node->var.name, name);
    varRepository->list = node;
    return &node->var;
  }
  
  return var;
}
// Const search
variable_t const* FindConst(char const* name) {
  static const variable_t constVal[] = {
    {"pi", 3.14159265358979323846, 1},
    {"e", 2.71828182845904523536, 1},
  };
  int counter;
  
  for (counter = 0; (unsigned)counter < sizeof(constVal) / sizeof(constVal[0]); counter++) {
    if ((_Bool)(strcmp(name, constVal[counter].name) == 0)) {
      return &constVal[counter];
    }
  }

  return NULL;
}
// Function search
function_t const* FindFunction(char const* name) {
  static const function_t functions[] = {
    {"sin", _sin},
    {"cos", _cos},
    {"tg", _tg},
    {"ctg", _ctg},
    {"arcsin", _arcsin},
    {"arccos", _arccos},
    {"arctg", _arctg},
    {"sqrt", _sqrt},
    {"floor", _floor},
    {"ceil", _ceil},
    {"ln", _ln},
    {"log", _log},
  };
  int counter;
  // Finding function in table
  for (counter = 0; (unsigned)counter < sizeof(functions) / sizeof(functions[0]); counter++) {
    if ((_Bool)(strcmp(name, functions[counter].name) == 0)) {
      return &functions[counter];
    }
  }
  return NULL;
}
