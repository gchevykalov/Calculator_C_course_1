// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// Module to get the result

#include <stdlib.h>
#include "Error.h"
#include "Scanner.h"
#include "Parser.h"
#include "StackAndQueue.h"
#include "Calculator.h"
#include "Operations.h"
#include "GetResult.h"

// Clear queues and varRepository
static void _clearAll(queue_t* Qres, queue_t* queue, variable_repository_t* varRepository) {
  ClearQueue(Qres);
  ClearQueue(queue);
  ClearVarRepository(varRepository);
}
// Function to get the result
double GetResult(char const* expression, error_t* err) {
  double res = 0;
  queue_t Qres = { NULL, NULL }, queue = { NULL, NULL };
  variable_repository_t* varRepository = CreateVarRepository(err);
  
  ERROR_CHECKUP_WITHOUT_ACTION(err, 0);
  // Splitting a string into tokens
  Scanner(expression, &queue, varRepository, err);
  ERROR_CHECKUP_WITH_ACTION(err, _clearAll(&Qres, &queue, varRepository), 0);
  // Ñonvert the expression to polish reverse notation
  Parser(&Qres, &queue, err);
  ERROR_CHECKUP_WITH_ACTION(err, _clearAll(&Qres, &queue, varRepository), 0);
  // Calculate value of the expression
  res = Calculate(&Qres, err);
  ERROR_CHECKUP_WITH_ACTION(err, _clearAll(&Qres, &queue, varRepository), 0);
  // Clear all variables
  ClearVarRepository(varRepository);

  return res;
}
