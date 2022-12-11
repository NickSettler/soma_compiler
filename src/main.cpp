#include <iostream>
#include <sstream>

#include "lexical_analysis.h"

int main() {
    const char *test_input = "1e12 + 1 * (1 + 2)";
    std::istringstream input_stream(test_input);

    LexicalAnalysis analysis(&input_stream);
    std::vector<LexicalToken> tokens;
    LexicalToken *token = analysis.get_token();
    while (token != nullptr && token->get_type() != LEX_TOKEN_EOF) {
        tokens.push_back(*token);
        token = analysis.get_token();
    }

    for (auto &t: tokens) printf("Token: %s (%d)\n", t.get_value().c_str(), t.get_type());

    printf("Done\n");
    return 0;
}
