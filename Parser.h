// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// Parser header file

#ifndef PARSER_H_INCLUDED__
#define PARSER_H_INCLUDED__
#pragma once

// Ñonvert the expression to polish reverse notation
void Parser(queue_t* Qres, queue_t* queue, error_t* err);

#endif /* PARSER_H_INCLUDED__ */
