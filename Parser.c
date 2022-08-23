// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// Parser sourse file

#include <stdio.h>
#include <assert.h>
#include "Error.h"
#include "StackAndQueue.h"
#include "Operations.h"
#include "Parser.h"

// Check a priority of operation
static _Bool _checkPrior(token_t const* operation, stack_t* stack) {
  token_t top;
  
  Top(stack, &top);
  if (!(_Bool)IsItAnEmptyStack(stack) && top.type == TOKEN_TYPE_OPERATION) {
    
    if (top.value.operation->assoc == '>') {
      return top.value.operation->prior >= operation->value.operation->prior;
    }
    
    else {
      return top.value.operation->prior > operation->value.operation->prior;
    }
  }
  
  return 0;
}
// Displasement operations with higher priority
static void _displacementOperations(token_t operation, stack_t* stack1, stack_t* stack2, error_t* err) {
  token_t tok;
  token_t top;
  
  while ((_Bool)Top(stack2, &top) && (top.type == TOKEN_TYPE_FUNCTION ||
    (top.type == TOKEN_TYPE_OPERATION && _checkPrior(&operation, stack2)))) {
    Pop(stack2, &tok);
    ERROR_CHECKUP_WITHOUT_ACTION(err, );
    Push(stack1, &tok, err);
    ERROR_CHECKUP_WITHOUT_ACTION(err, );
  }
  Push(stack2, &operation, err);
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
}
// Displasement all operations until open bracket
static int _displacementOperationsUntilOpenBracket(stack_t* stack1, stack_t* stack2, error_t* err) {
  token_t tok;
  token_t top;
  
  while ((_Bool)Top(stack2, &top) && (top.type == TOKEN_TYPE_OPERATION || top.type == TOKEN_TYPE_FUNCTION)) {
    Pop(stack2, &tok);
    Push(stack1, &tok, err);
    ERROR_CHECKUP_WITHOUT_ACTION(err, 0);
  }
  
  if (!(_Bool)IsItAnEmptyStack(stack2)) {
    Pop(stack2, &tok);
    return 1;
  }
  
  return 0;
}
// Clear stacks
static void _clearStacks(stack_t* stack1, stack_t* stack2) {
  ClearStack(stack1);
  ClearStack(stack2);
}
// Creating a resulting token queue
static void _createQres(stack_t* stack1, stack_t* stack2, queue_t* Qres, error_t* err) {
  token_t tok;
  
  if ((_Bool)(_displacementOperationsUntilOpenBracket(stack1, stack2, err) == 1)) {
    Error(err, "syntax error");
    return;
  }
  ERROR_CHECKUP_WITHOUT_ACTION(err, );
  // Token queue reverse
  while (Pop(stack1, &tok)) {
    Push(stack2, &tok, err);
    ERROR_CHECKUP_WITHOUT_ACTION(err, );
  }
  while (Pop(stack2, &tok)) {
    Put(Qres, &tok, err);
    ERROR_CHECKUP_WITHOUT_ACTION(err, );
  }
}
// Main part of parser
static void _parseQueue(queue_t* Qres, queue_t* queue, stack_t* stack1, stack_t* stack2, error_t* err) {
  enum {
    STATE_OPERAND,
    STATE_OPERATION
  } state = STATE_OPERAND;
  token_t tok;
  
  // Repeat until tokens run out
  while (Get(queue, &tok)) {
    assert(state == STATE_OPERAND || state == STATE_OPERATION);
    switch (state) {
      case STATE_OPERAND:
        // Handling operand
        if (tok.type == TOKEN_TYPE_NUMBER || tok.type == TOKEN_TYPE_CONST || tok.type == TOKEN_TYPE_VARIABLE) {
          Push(stack1, &tok, err);
          ERROR_CHECKUP_WITHOUT_ACTION(err, );
          state = STATE_OPERATION;
        }
        else if (tok.type == TOKEN_TYPE_OPEN_BRACKET || tok.type == TOKEN_TYPE_FUNCTION) {
          Push(stack2, &tok, err);
          ERROR_CHECKUP_WITHOUT_ACTION(err, );
        }
        else if (tok.type == TOKEN_TYPE_OPERATION && tok.value.operation == GetOperation('-')) {
          tok.value.operation = GetOperation('~');
          Push(stack2, &tok, err);
          ERROR_CHECKUP_WITHOUT_ACTION(err, );
        }
        else {
          assert(tok.type == TOKEN_TYPE_OPERATION || tok.type == TOKEN_TYPE_CLOSE_BRACKET);
          Error(err, "expect operand or '('");
          return;
        }
        break;
      case STATE_OPERATION:
        // Handling operation
        if (tok.type == TOKEN_TYPE_OPERATION) {
          _displacementOperations(tok, stack1, stack2, err);
          ERROR_CHECKUP_WITHOUT_ACTION(err, );
          state = STATE_OPERAND;
        }
        else if (tok.type == TOKEN_TYPE_CLOSE_BRACKET) {
          if ((_Bool)(_displacementOperationsUntilOpenBracket(stack1, stack2, err) == 0)) {
            Error(err, "missing '('");
            return;
          }
          ERROR_CHECKUP_WITHOUT_ACTION(err, );
        }
        else {
          assert(tok.type >= TOKEN_TYPE_NUMBER && tok.type <= TOKEN_TYPE_OPEN_BRACKET);
          Error(err, "expect operation or ')'");
          return;
        }
        break;
      default:
        Error(err, "incorrect state");
        return;
        break;
    }
  }
  // Check state
  if ((_Bool)(state == STATE_OPERAND)) {
    Error(err, "unexpected end");
    return;
  }
  _createQres(stack1, stack2, Qres, err);
}
// Ñonvert the expression to polish reverse notation
void Parser(queue_t* Qres, queue_t* queue, error_t* err) {
  stack_t stack1 = { NULL };
  stack_t stack2 = { NULL };
  
  _parseQueue(Qres, queue, &stack1, &stack2, err);
  ERROR_CHECKUP_WITH_ACTION(err, _clearStacks(&stack1, &stack2), );
}
