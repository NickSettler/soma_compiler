/**
 * Errors handling
 * @file: errors.cpp
 * @date: 12.12.2022
 */

#include "errors.h"
#include <cstdarg>
#include <cstdio>

LexicalAnalysisError::LexicalAnalysisError(const char *message, ...) { ERROR_CONSTRUCTOR(LEXICAL_ANALYSIS_ERROR_CODE) }
