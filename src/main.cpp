#include <sstream>

#include "lexical_analysis.h"
#include "syntax_analysis.h"
#include "semantic_analysis.h"
#include "symbol_table.h"

extern SymbolTableTree *global_symbol_table;

int main() {
    const char *test_input = "const a = 1; var b = 1 + 1; var b = a + b;";
    std::istringstream input_stream(test_input);

    auto *analysis = new LexicalAnalysis(&input_stream);

    auto syntax_analysis = new SyntaxAnalysis(analysis);
    auto *syntax_tree = syntax_analysis->build_tree();

    auto *semantic_analysis = new SemanticAnalysis();
    semantic_analysis->analyze_tree(syntax_tree);

    printf("Done\n");

    delete global_symbol_table;
    return 0;
}
