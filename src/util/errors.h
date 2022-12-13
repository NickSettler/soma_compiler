/**
 * Errors handling
 * @file: errors.h
 * @date: 12.12.2022
 */

#ifndef SOMA_COMPILER_ERRORS_H
#define SOMA_COMPILER_ERRORS_H

#include <exception>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#define ERROR_CONSTRUCTOR(code)                                                                                        \
    va_list args;                                                                                                      \
    va_start(args, message);                                                                                           \
    vfprintf(stderr, message, args);                                                                                   \
    va_end(args);                                                                                                      \
    exit(code);

#define CREATE_EXCEPTION(name, code)                                                                                   \
    class name : public std::exception {                                                                               \
    public:                                                                                                            \
        explicit name(const char *message, ...) { ERROR_CONSTRUCTOR(code) }                                            \
    };

#define LEXICAL_ANALYSIS_ERROR_CODE 1

#define SYNTAX_ANALYSIS_ERROR_CODE 2

CREATE_EXCEPTION(LexicalAnalysisError, LEXICAL_ANALYSIS_ERROR_CODE)
CREATE_EXCEPTION(SyntaxAnalysisError, SYNTAX_ANALYSIS_ERROR_CODE)

#endif// SOMA_COMPILER_ERRORS_H
