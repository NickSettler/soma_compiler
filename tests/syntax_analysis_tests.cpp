/**
 * Tests for syntax analysis
 * @file: syntax_analysis_tests.cpp
 * @date: 12.12.2022
 */

#include <gtest/gtest.h>

#include "../src/syntax_analysis.cpp"

namespace soma {
    namespace tests {
        namespace {
            class SyntaxAnalysisTests : public ::testing::Test {
            protected:
                std::istringstream input_stream;
                std::string actual_nodes;
                std::string expected_nodes;

            public:
                SyntaxAnalysisTests() {
                    input_stream = std::istringstream();
                    actual_nodes = std::string();
                    expected_nodes = std::string();
                }

                void TearDown() override {
                    input_stream.clear();
                    actual_nodes.clear();
                    expected_nodes.clear();
                }

                void CheckSyntaxTree(const std::string &input, const std::vector<SYNTAX_ANALYSIS_NODE_TYPE> &expected) {
                    input_stream = std::istringstream(input);

                    auto lexical_analysis = new LexicalAnalysis(&input_stream);
                    auto syntax_analysis = new SyntaxAnalysis(lexical_analysis);
                    auto syntax_tree = syntax_analysis->build_tree();

                    syntax_tree->process_tree_using(
                            [&](SyntaxTree *tree) { actual_nodes += std::to_string(tree->type) + " "; }, INORDER);

                    for (auto expected_node: expected) expected_nodes += std::to_string(expected_node) + " ";

                    EXPECT_EQ(actual_nodes, expected_nodes) << "Input: " << input;
                }
            };

            TEST_F(SyntaxAnalysisTests, Empty) {
                CheckSyntaxTree("", {});

                CheckSyntaxTree("   ", {});

                CheckSyntaxTree("   \t  \n  \t   ", {});
            }

            TEST_F(SyntaxAnalysisTests, SingleNumbers) {
                CheckSyntaxTree("1;", {SYN_NODE_SEQUENCE, SYN_NODE_INTEGER_LITERAL});

                CheckSyntaxTree("1.2e-1;", {SYN_NODE_SEQUENCE, SYN_NODE_FLOAT_LITERAL});
            }

            TEST_F(SyntaxAnalysisTests, ArithmeticExpressions) {
                CheckSyntaxTree("1 + 2;",
                                {SYN_NODE_SEQUENCE, SYN_NODE_INTEGER_LITERAL, SYN_NODE_ADD, SYN_NODE_INTEGER_LITERAL});

                CheckSyntaxTree("1 + 2e-2;",
                                {SYN_NODE_SEQUENCE, SYN_NODE_INTEGER_LITERAL, SYN_NODE_ADD, SYN_NODE_FLOAT_LITERAL});

                CheckSyntaxTree("1 * 2 + 3;", {SYN_NODE_SEQUENCE, SYN_NODE_INTEGER_LITERAL, SYN_NODE_MUL,
                                               SYN_NODE_INTEGER_LITERAL, SYN_NODE_ADD, SYN_NODE_INTEGER_LITERAL});

                CheckSyntaxTree("1 * (2 + 3);", {SYN_NODE_SEQUENCE, SYN_NODE_INTEGER_LITERAL, SYN_NODE_MUL,
                                                 SYN_NODE_INTEGER_LITERAL, SYN_NODE_ADD, SYN_NODE_INTEGER_LITERAL});

                CheckSyntaxTree("1 * 2 + 3 / 4;",
                                {SYN_NODE_SEQUENCE, SYN_NODE_INTEGER_LITERAL, SYN_NODE_MUL, SYN_NODE_INTEGER_LITERAL,
                                 SYN_NODE_ADD, SYN_NODE_INTEGER_LITERAL, SYN_NODE_DIV, SYN_NODE_INTEGER_LITERAL});

                CheckSyntaxTree("1 * (2 + 3) / 4;",
                                {SYN_NODE_SEQUENCE, SYN_NODE_INTEGER_LITERAL, SYN_NODE_MUL, SYN_NODE_INTEGER_LITERAL,
                                 SYN_NODE_ADD, SYN_NODE_INTEGER_LITERAL, SYN_NODE_DIV, SYN_NODE_INTEGER_LITERAL});

                CheckSyntaxTree("1 * (2 + (3 / 4) * 5);",
                                {SYN_NODE_SEQUENCE, SYN_NODE_INTEGER_LITERAL, SYN_NODE_MUL, SYN_NODE_INTEGER_LITERAL,
                                 SYN_NODE_ADD, SYN_NODE_INTEGER_LITERAL, SYN_NODE_DIV, SYN_NODE_INTEGER_LITERAL,
                                 SYN_NODE_MUL, SYN_NODE_INTEGER_LITERAL});

                EXPECT_EXIT(CheckSyntaxTree("1 + 2", {}), ::testing::ExitedWithCode(SYNTAX_ANALYSIS_ERROR_CODE),
                            "Unexpected token: . Expected: ;");

                EXPECT_EXIT(CheckSyntaxTree("1 +", {}), ::testing::ExitedWithCode(SYNTAX_ANALYSIS_ERROR_CODE),
                            "Expected expression but found:.*");

                EXPECT_EXIT(CheckSyntaxTree("1 + (", {}), ::testing::ExitedWithCode(SYNTAX_ANALYSIS_ERROR_CODE),
                            "Expected expression but found:");

                EXPECT_EXIT(CheckSyntaxTree("1 + (2))", {}), ::testing::ExitedWithCode(SYNTAX_ANALYSIS_ERROR_CODE),
                            "Unexpected token: ). Expected: ;");
            }

            TEST_F(SyntaxAnalysisTests, Assignment) {
                CheckSyntaxTree("var a = 1;", {SYN_NODE_SEQUENCE, SYN_NODE_IDENTIFIER, SYN_NODE_ASSIGNMENT,
                                               SYN_NODE_INTEGER_LITERAL});

                CheckSyntaxTree("const ball = 1 * 2;",
                                {SYN_NODE_SEQUENCE, SYN_NODE_IDENTIFIER, SYN_NODE_ASSIGNMENT, SYN_NODE_INTEGER_LITERAL,
                                 SYN_NODE_MUL, SYN_NODE_INTEGER_LITERAL});

                CheckSyntaxTree("const a = 1 + 2;"
                                "const b = a * 3;"
                                "const c = b + a;",
                                {SYN_NODE_SEQUENCE, SYN_NODE_IDENTIFIER, SYN_NODE_ASSIGNMENT, SYN_NODE_INTEGER_LITERAL,
                                 SYN_NODE_ADD, SYN_NODE_INTEGER_LITERAL, SYN_NODE_SEQUENCE, SYN_NODE_IDENTIFIER,
                                 SYN_NODE_ASSIGNMENT, SYN_NODE_IDENTIFIER, SYN_NODE_MUL, SYN_NODE_INTEGER_LITERAL,
                                 SYN_NODE_SEQUENCE, SYN_NODE_IDENTIFIER, SYN_NODE_ASSIGNMENT, SYN_NODE_IDENTIFIER,
                                 SYN_NODE_ADD, SYN_NODE_IDENTIFIER});

                CheckSyntaxTree("const a = 1 + 2;"
                                "a = a * 3;",
                                {SYN_NODE_SEQUENCE, SYN_NODE_IDENTIFIER, SYN_NODE_ASSIGNMENT, SYN_NODE_INTEGER_LITERAL,
                                 SYN_NODE_ADD, SYN_NODE_INTEGER_LITERAL, SYN_NODE_SEQUENCE, SYN_NODE_IDENTIFIER,
                                 SYN_NODE_ASSIGNMENT, SYN_NODE_IDENTIFIER, SYN_NODE_MUL, SYN_NODE_INTEGER_LITERAL});

                EXPECT_EXIT(CheckSyntaxTree("const ", {}), ::testing::ExitedWithCode(SYNTAX_ANALYSIS_ERROR_CODE),
                            "Unexpected token: . Expected: ID");

                EXPECT_EXIT(CheckSyntaxTree("const abc", {}), ::testing::ExitedWithCode(SYNTAX_ANALYSIS_ERROR_CODE),
                            "Unexpected token: . Expected: =");

                EXPECT_EXIT(CheckSyntaxTree("const abc =", {}), ::testing::ExitedWithCode(SYNTAX_ANALYSIS_ERROR_CODE),
                            "Expected expression but found: ");

                EXPECT_EXIT(CheckSyntaxTree("const abc = 1", {}), ::testing::ExitedWithCode(SYNTAX_ANALYSIS_ERROR_CODE),
                            "Unexpected token: . Expected: ;");
            }
        }// namespace
    }    // namespace tests
}// namespace soma
