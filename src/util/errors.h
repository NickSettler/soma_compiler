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

#define LEXICAL_ANALYSIS_ERROR_CODE 0x101

#define SYNTAX_ANALYSIS_ERROR_CODE 0x201

#define SEMANTIC_ANALYSIS_UNDEFINED_VARIABLE_ERROR_CODE 0x301

#define SEMANTIC_ANALYSIS_REDEFINE_VARIABLE_ERROR_CODE 0x302

#define SEMANTIC_ANALYSIS_REASSIGN_CONSTANT_ERROR_CODE 0x303

CREATE_EXCEPTION(LexicalAnalysisError, LEXICAL_ANALYSIS_ERROR_CODE)
CREATE_EXCEPTION(SyntaxAnalysisError, SYNTAX_ANALYSIS_ERROR_CODE)
CREATE_EXCEPTION(SemanticAnalysisUndefinedVariableError, SEMANTIC_ANALYSIS_UNDEFINED_VARIABLE_ERROR_CODE)
CREATE_EXCEPTION(SemanticAnalysisRedefineVariableError, SEMANTIC_ANALYSIS_REDEFINE_VARIABLE_ERROR_CODE)
CREATE_EXCEPTION(SemanticAnalysisReassignConstantError, SEMANTIC_ANALYSIS_REASSIGN_CONSTANT_ERROR_CODE)

#endif// SOMA_COMPILER_ERRORS_H
