/**
 * Errors handling
 * @file: errors.h
 * @date: 12.12.2022
 */

#ifndef SOMA_COMPILER_ERRORS_H
#define SOMA_COMPILER_ERRORS_H

#include <exception>

#define ERROR_CONSTRUCTOR(code)                                                                                        \
    va_list args;                                                                                                      \
    va_start(args, message);                                                                                           \
    vfprintf(stderr, message, args);                                                                                   \
    va_end(args);                                                                                                      \
    exit(code);

#define LEXICAL_ANALYSIS_ERROR_CODE 1

class LexicalAnalysisError : public std::exception {
public:
    explicit LexicalAnalysisError(const char *message, ...);
};

#endif// SOMA_COMPILER_ERRORS_H
