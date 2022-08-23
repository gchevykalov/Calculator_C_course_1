// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// String to token queue convertion header file

#ifndef SCANNER_H_INCLUDED__
#define SCANNER_H_INCLUDED__
#pragma once

#include "Error.h"
#include "StackAndQueue.h"
#include "Operations.h"

// Splitting a string into tokens function
void Scanner(char const* str, queue_t* queue, variable_repository_t* varRepository, error_t* err);

#endif /* SCANNER_H_INCLUDED__ */
