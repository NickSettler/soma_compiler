/**
 * Tests for semantic analysis
 * @file: semantic_analysis_tests.cpp
 * @date: 14.12.2022
 */

#include <gtest/gtest.h>
#include <vector>
#include <map>

#include "../src/lexical_analysis.h"
#include "../src/symbol_table.cpp"
#include "../src/semantic_analysis.cpp"

namespace soma {
    namespace tests {
        namespace {
            class SemanticAnalysisTests : public ::testing::Test {
            protected:
                std::istringstream input_stream;

            public:
                SemanticAnalysisTests() { input_stream = std::istringstream(); }

                ~SemanticAnalysisTests() override { input_stream.clear(); }

                void TearDown() override {
                    delete global_symbol_table;
                    global_symbol_table = new SymbolTableTree();
                }

                void CheckSemantics(const std::string &input,
                                    const std::map<std::string, SymbolTableTreeData> &expected_entries) {
                    input_stream = std::istringstream(input);

                    auto lexical_analysis = new LexicalAnalysis(&input_stream);
                    auto syntax_analysis = new SyntaxAnalysis(lexical_analysis);
                    auto syntax_tree = syntax_analysis->build_tree();
                    auto semantic_analysis = new SemanticAnalysis();
                    semantic_analysis->analyze_tree(syntax_tree);

                    for (auto &entry: expected_entries) {
                        auto name = entry.first;
                        auto data = entry.second;

                        auto token = global_symbol_table->find(&name);

                        if (token == nullptr) { FAIL() << "Symbol " << name << " not found"; }

                        EXPECT_EQ(data.get_type(), token->data->get_type())
                                << "Symbol " << name << " type mismatch. Input: " << input;
                        EXPECT_EQ(data.get_flags(), token->data->get_flags())
                                << "Symbol " << name << " value mismatch. Input: " << input;
                    }

                    delete global_symbol_table;
                    global_symbol_table = new SymbolTableTree();
                }
            };

            TEST_F(SemanticAnalysisTests, Empty) {
                CheckSemantics("", {});

                CheckSemantics(" \n   \t \n\n", {});
            }

            TEST_F(SemanticAnalysisTests, VariableDeclaration) {
                SymbolTableTreeData a{};
                a.set_flag(SYM_TABLE_IS_DEFINED | SYM_TABLE_IS_CONSTANT);

                CheckSemantics("const a = 1;", {std::pair<std::string, SymbolTableTreeData>("a", a)});

                a.unset_flag(SYM_TABLE_IS_CONSTANT);
                CheckSemantics("var a = 1;", {std::pair<std::string, SymbolTableTreeData>("a", a)});
            }

            TEST_F(SemanticAnalysisTests, ExpressionsWithVariables) {
                SymbolTableTreeData a{}, b{}, c{};
                a.set_flag(SYM_TABLE_IS_DEFINED | SYM_TABLE_IS_CONSTANT);
                b.set_flag(SYM_TABLE_IS_DEFINED);
                c.set_flag(SYM_TABLE_IS_DEFINED | SYM_TABLE_IS_CONSTANT);

                CheckSemantics("const a = 1;"
                               "var b = a * 1;"
                               "const c = a - b / 3;",
                               {std::pair<std::string, SymbolTableTreeData>("a", a),
                                std::pair<std::string, SymbolTableTreeData>("b", b),
                                std::pair<std::string, SymbolTableTreeData>("c", c)});
            }
        }// namespace
    }    // namespace tests
}// namespace soma