// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// Stack and queue sourse file

#include <stdlib.h>
#include <crtdbg.h>
#include "Error.h"
#include "StackAndQueue.h"

// Adding an token to the stack
int Push(stack_t* stack, const token_t* newTok, error_t* err) {
  list_t* node = malloc(sizeof(list_t));
  
  if (node == NULL) {
    Error(err, "not enough memory");
    return 0;
  }
  node->element = *newTok;
  node->next = stack->top;
  stack->top = node;
  
  return 1;
}
// Pulling an token from the stack
int Pop(stack_t* stack, token_t* oldTok) {
  list_t* node = stack->top;
  
  if (stack->top == NULL) {
    return 0;
  }
  *oldTok = node->element;
  stack->top = stack->top->next;
  free(node);
  
  return 1;
}
// Seeing the top token in the stack
int Top(stack_t const* stack, token_t* tok) {
  if (stack->top == NULL) {
    return 0;
  }
  *tok = stack->top->element;
  
  return 1;
}
// Adding an token to the queue
int Put(queue_t* queue, const token_t* newTok, error_t* err) {
  list_t* node = malloc(sizeof(list_t));
  
  if (node == NULL) {
    Error(err, "not enough memory");
    return 0;
  }
  node->element = *newTok;
  node->next = NULL;
  if (queue->tail == NULL) {
    queue->tail = queue->head = node;
  }
  else {
    queue->tail->next = node;
    queue->tail = node;
  }
  
  return 1;
}
// Pulling an token from the queue
int Get(queue_t* queue, token_t* oldTok) {
  list_t* node = queue->head;
  
  if (queue->head == NULL) {
    return 0;
  }
  queue->head = queue->head->next;
  *oldTok = node->element;
  free(node);
  if (queue->head == NULL) {
    queue->tail = NULL;
  }
  
  return 1;
}
// Stack cleanup
void ClearStack(stack_t* stack) {
  list_t* node = NULL;
  
  while (stack->top != NULL) {
    node = stack->top;
    stack->top = stack->top->next;
    free(node);
  }
}
// Queue cleanup
void ClearQueue(queue_t* queue) {
  list_t* node = NULL;
  
  while (queue->head != NULL) {
    node = queue->head;
    queue->head = queue->head->next;
    free(node);
  }
  queue->tail = NULL;
}
// Ñhecking the stack for emptiness
int IsItAnEmptyStack(stack_t const* stack) {
  if (stack->top != NULL) {
    return 0;
  }
  return 1;
}