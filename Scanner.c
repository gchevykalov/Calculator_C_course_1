// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// String to token queue convertion source file

#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "Error.h"
#include "StackAndQueue.h"
#include "Operations.h"
#include "Scanner.h"

// Creating number token function
static void _number(char const** str, token_t* tok, error_t* err) {
  char* end = NULL;
  double number = strtod(*str, &end);
  
  if (*str == end) {
    Error(err, "wrong number format");
    return;
  }
  *str = end;
  tok->type = TOKEN_TYPE_NUMBER;
  tok->value.number = number;
}
// Creating name token function
static void _name(char const** str, token_t* tok, variable_repository_t* varRepository, error_t* err) {
  int counter = 0;
  char name[MAX_NAME_SIZE];
  function_t const* function;
  variable_t const* constVal;
  
  // Name reading
  while (isalpha(**str)) {
    if (counter == MAX_NAME_SIZE - 2) {
      Error(err, "huge name");
      return;
    }
    name[counter++] = *(*str)++;
  }
  name[counter] = '\0';
  // Finding function with this name
  if ((function = FindFunction(name)) != NULL) {
    assert(function != NULL);
    tok->type = TOKEN_TYPE_FUNCTION;
    tok->value.function = function;
  }
  // Finding const variable with this name
  else if ((constVal = FindConst(name)) != NULL) {
    assert(constVal != NULL);
    tok->type = TOKEN_TYPE_CONST;
    tok->value.constVal = constVal;
  }
  // Adding variable with this name
  else {
    tok->type = TOKEN_TYPE_VARIABLE;
    tok->value.variable = AddVar(varRepository, name, err);
  }
}
// Creating operation token function
static void _operation(char const** str, token_t* tok, error_t* err) {
  int operation;
  
  switch (**str) {
  case '+':
  case '-':
  case '*':
  case '/':
  case '^':
  case '=':
  case ',':
  case ';':
    operation = (int)**str;
    tok->type = TOKEN_TYPE_OPERATION;
    tok->value.operation = GetOperation(operation);
    // Check operation
    if (tok->value.operation == NULL) {
      Error(err, "incorrect operation");
      return;
    }
    break;
  case '(':
    tok->type = TOKEN_TYPE_OPEN_BRACKET;
    break;
  case ')':
    tok->type = TOKEN_TYPE_CLOSE_BRACKET;
    break;
  default:
    tok->type = TOKEN_TYPE_OPERATION;
    Error(err, "unknown token");
    return;
    break;
  }
  (*str)++;
}
// Splitting a string into tokens function
void Scanner(char const* str, queue_t* queue, variable_repository_t* varRepository, error_t* err) {
  token_t tok;
  
  ERROR_CHECKUP_WITH_ACTION(err, ClearQueue(queue), );
  // Iterate all string
  while (*str != '\0') {
    // Skip space
    if (isspace(*str)) {
      str++;
      continue;
    }
    // Number handling
    if ((_Bool)((_Bool)isdigit(*str) || *str == '.')) {
      _number(&str, &tok, err);
      ERROR_CHECKUP_WITH_ACTION(err, ClearQueue(queue), );
    }
    // Name handling
    else if (isalpha(*str)) {
      _name(&str, &tok, varRepository, err);
      ERROR_CHECKUP_WITH_ACTION(err, ClearQueue(queue), );
    }
    // Operation handling
    else {
      _operation(&str, &tok, err);
      ERROR_CHECKUP_WITH_ACTION(err, ClearQueue(queue), );
    }
    Put(queue, &tok, err);
    ERROR_CHECKUP_WITH_ACTION(err, ClearQueue(queue), );
  }
}
