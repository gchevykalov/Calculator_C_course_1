// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// Main program file

#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <ctype.h>
#include <assert.h>
#include "Error.h"
#include "GetResult.h"

#pragma warning(disable:4996)

#define START_SIZE 10

// Function to read line
static char* _readExpression(FILE* inputStream, int* last) {
  int symbol, lengthStr = START_SIZE, position = 0;
  char character;
  char* auxiliary;
  char* string = malloc(sizeof(char) * (unsigned)START_SIZE);
  
  // If there is not enough memory will skip the string and will check the end of the stream
  if (string == NULL) {
    while ((symbol = fgetc(inputStream)) != (int)'\n' && symbol != EOF) {
    }
    if (symbol == EOF) {
      *last = 1;
    }
    return string;
  }
  // Read character by character until the string ends
  while ((symbol = fgetc(inputStream)) != (int)'\n') {
    // Check the end of the stream
    if (symbol == EOF) {
      *last = 1;
      assert(position < lengthStr);
      // Write the end of the string
      string[position] = '\0';
      return string;
    }
    // Type conversion of variable
    character = (char)symbol;
    if (position == lengthStr - 2) {
      auxiliary = realloc(string, sizeof(char) * (unsigned)lengthStr * (unsigned)2);
      // If there is not enough memory will skip the rest of the string and will check the end of the stream
      if (auxiliary == NULL) {
        free(string);
        while ((symbol = fgetc(inputStream)) != (int)'\n' && symbol != EOF) {
        }
        if (symbol == EOF) {
          *last = 1;
        }
        return NULL;
      }
      lengthStr *= 2;
      string = auxiliary;
    }
    assert(position < lengthStr);
    string[position++] = character;
  }
  assert(position < lengthStr);
  // Write the end of the string
  string[position] = '\0';
  
  return string;
}
// Function to check comment or a blank line
static int _isCommentOrBlankLine(char const* expression) {
  int answer = 1;
  // Check characters in the string
  while (*expression) {
    if (!(_Bool)isspace(*expression)) {
      if (*expression == '/' && *(expression + 1) == '/') {
        break;
      }
      // If a character other than space or '/' is found, then the string is an expression
      else {
        answer = 0;
        break;
      }
    }
    // Skip spaces
    else {
      expression++;
    }
  }

  return answer;
}
// Main function
int main(int argV, char* argC[]) {
  FILE* inputStream = stdin;
  char* expression;
  double result = 1;
  int isLast = 0, isFile = 0;
  error_t isError = { 0, "success" };

  _CrtSetDbgFlag((unsigned)_CRTDBG_ALLOC_MEM_DF | (unsigned)_CRTDBG_LEAK_CHECK_DF);

  // Check the conditions for launching the program
  switch (argV) {
    case 1:
      inputStream = stdin;
      break;
    case 2:
      // Open file
      isFile = 1;
      inputStream = fopen(argC[1], "r");
      if (inputStream == NULL) {
        printf("ERROR: stream error\n");
        return 0;
      }
      break;
    default:
      printf("ERROR: too many arguments\n");
      return 0;
      break;
    }
  // Repeat until the end of the stream
  while (!(_Bool)isLast) {
    // Read line
    expression = _readExpression(inputStream, &isLast);
    if ((_Bool)(expression == NULL)) {
      if (!(_Bool)isLast) {
        printf("ERROR: not enough memory\n");
        continue;
      }
      else {
        break;
      }
    }
    // Ñheck the end of the stream
    if ((_Bool)(expression[0] == '\0') && (_Bool)isLast) {
      free(expression);
      break;
    }
    // Ñheck comment or a blank line
    if ((_Bool)_isCommentOrBlankLine(expression)) {
      printf("%s\n", expression);
    }
    // Calculate the value of the expression
    else {
      printf("%s == ", expression);
      result = GetResult(expression, &isError);
      // Print result
      if (!(_Bool)isError.flag) {
        printf("%g", result);
      }
      else {
        printf("%s", isError.message);
        isError.flag = 0;
      }
      if (!(_Bool)isLast) {
        printf("\n");
      }
    }
    // Back to the original state
    free(expression);
  }
  if ((_Bool)isFile) {
    fclose(inputStream);
  }

  return 0;
}