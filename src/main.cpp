#include <sstream>

#include "lexical_analysis.h"
#include "syntax_analysis.h"

int main() {
    const char *test_input = "1 + 2 - 3 * 4";
    std::istringstream input_stream(test_input);

    auto *analysis = new LexicalAnalysis(&input_stream);
    auto syntax_analysis = new SyntaxAnalysis(analysis);

    SyntaxTree *syntax_tree = syntax_analysis->build_tree();

    printf("Done\n");
    return 0;
}
