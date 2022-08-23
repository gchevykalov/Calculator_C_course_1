// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// Calculator module file

#include <string.h>
#include <assert.h>
#include "StackAndQueue.h"
#include "Error.h"
#include "Operations.h"
#include "Calculator.h"

// Calculate value of the expression
double Calculate(queue_t* Qres, error_t* err) {
  double res = 0;
  token_t tok;
  stack_t stack = { NULL };
  
  // Repeat until tokens run out
  while (Get(Qres, &tok)) {
    // Handling numbers
    if (tok.type == TOKEN_TYPE_NUMBER || tok.type == TOKEN_TYPE_CONST || tok.type == TOKEN_TYPE_VARIABLE) {
      Push(&stack, &tok, err);
      ERROR_CHECKUP_WITH_ACTION(err, ClearStack(&stack), 0);
    }
    // Handling operations
    else if (tok.type == TOKEN_TYPE_OPERATION) {
      assert(tok.value.operation->operation != NULL);
      tok.value.operation->operation(&stack, err);
      ERROR_CHECKUP_WITH_ACTION(err, ClearStack(&stack), 0);
    }
    // Handling functions
    else {
      assert(tok.type == TOKEN_TYPE_FUNCTION && tok.value.function->function != NULL);
      tok.value.function->function(&stack, err);
      ERROR_CHECKUP_WITH_ACTION(err, ClearStack(&stack), 0);
    }
  }
  // Get token with result value
  PopNumber(&stack, &res, err);
  ERROR_CHECKUP_WITH_ACTION(err, ClearStack(&stack), 0);
  // Check the stack
  if ((_Bool)Pop(&stack, &tok)) {
    Error(err, "syntax error");
    ERROR_CHECKUP_WITH_ACTION(err, ClearStack(&stack), 0);
    return 0;
  }
  
  return res;
}
