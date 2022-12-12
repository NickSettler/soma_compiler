/**
 * Tests for lexical analysis
 * @file: lexical_analysis_tests.cpp
 * @date: 11.12.2022
 */

#include <gtest/gtest.h>

#include "../src/errors.cpp"
#include "../src/lexical_analysis.cpp"

namespace soma {
    namespace tests {
        namespace {
            class LexicalAnalysisTests : public ::testing::Test {
            protected:
                std::vector<LexicalToken> actual_tokens;
                std::istringstream input_stream;

            public:
                LexicalAnalysisTests() {
                    actual_tokens = std::vector<LexicalToken>();
                    input_stream = std::istringstream();
                }

                void TearDown() override {
                    actual_tokens.clear();
                    input_stream.clear();
                }

                void ProcessInput(const std::string &input, const std::vector<LexicalToken> &expected_tokens) {
                    input_stream = std::istringstream(input);
                    LexicalAnalysis analysis(&input_stream);
                    LexicalToken *token = analysis.get_token();

                    while (token != nullptr && token->get_type() != LEX_TOKEN_EOF) {
                        actual_tokens.push_back(*token);
                        token = analysis.get_token();
                    }

                    EXPECT_EQ(actual_tokens.size(), expected_tokens.size()) << "Input: " << input;
                    for (auto expected_token: expected_tokens) {
                        auto actual_token = actual_tokens.front();

                        if (expected_token.get_type())
                            EXPECT_EQ(actual_token.get_type(), expected_token.get_type()) << "Input: " << input;

                        EXPECT_EQ(actual_token.get_value(), expected_token.get_value()) << "Input: " << input;

                        actual_tokens.erase(actual_tokens.begin());
                    }
                }
            };

            TEST_F(LexicalAnalysisTests, Empty) {
                ProcessInput("", {});

                ProcessInput("    ", {});

                ProcessInput("    \n   ", {});

                ProcessInput("    \n\t\n   ", {});

                ProcessInput("    \n \t  \n   ", {});
            }

            TEST_F(LexicalAnalysisTests, Brackets) {
                ProcessInput("()", {LexicalToken("(", LEX_TOKEN_LEFT_PARENTHESIS),
                                    LexicalToken(")", LEX_TOKEN_RIGHT_PARENTHESIS)});

                ProcessInput("[]", {LexicalToken("[", LEX_TOKEN_LEFT_SQUARE_BRACKET),
                                    LexicalToken("]", LEX_TOKEN_RIGHT_SQUARE_BRACKET)});

                ProcessInput("{}", {LexicalToken("{", LEX_TOKEN_LEFT_CURLY_BRACKET),
                                    LexicalToken("}", LEX_TOKEN_RIGHT_CURLY_BRACKET)});

                ProcessInput("(())", {LexicalToken("(", LEX_TOKEN_LEFT_PARENTHESIS),
                                      LexicalToken("(", LEX_TOKEN_LEFT_PARENTHESIS),
                                      LexicalToken(")", LEX_TOKEN_RIGHT_PARENTHESIS),
                                      LexicalToken(")", LEX_TOKEN_RIGHT_PARENTHESIS)});

                ProcessInput("([])", {LexicalToken("(", LEX_TOKEN_LEFT_PARENTHESIS),
                                      LexicalToken("[", LEX_TOKEN_LEFT_SQUARE_BRACKET),
                                      LexicalToken("]", LEX_TOKEN_RIGHT_SQUARE_BRACKET),
                                      LexicalToken(")", LEX_TOKEN_RIGHT_PARENTHESIS)});

                ProcessInput("([{}])", {LexicalToken("(", LEX_TOKEN_LEFT_PARENTHESIS),
                                        LexicalToken("[", LEX_TOKEN_LEFT_SQUARE_BRACKET),
                                        LexicalToken("{", LEX_TOKEN_LEFT_CURLY_BRACKET),
                                        LexicalToken("}", LEX_TOKEN_RIGHT_CURLY_BRACKET),
                                        LexicalToken("]", LEX_TOKEN_RIGHT_SQUARE_BRACKET),
                                        LexicalToken(")", LEX_TOKEN_RIGHT_PARENTHESIS)});

                ProcessInput(" ( [ \n ] ) ", {LexicalToken("(", LEX_TOKEN_LEFT_PARENTHESIS),
                                              LexicalToken("[", LEX_TOKEN_LEFT_SQUARE_BRACKET),
                                              LexicalToken("]", LEX_TOKEN_RIGHT_SQUARE_BRACKET),
                                              LexicalToken(")", LEX_TOKEN_RIGHT_PARENTHESIS)});
            }

            TEST_F(LexicalAnalysisTests, Operators) {
                ProcessInput("+", {LexicalToken("+", LEX_TOKEN_PLUS)});

                ProcessInput("-", {LexicalToken("-", LEX_TOKEN_MINUS)});

                ProcessInput("*", {LexicalToken("*", LEX_TOKEN_MULTIPLY)});

                ProcessInput("/", {LexicalToken("/", LEX_TOKEN_DIVIDE)});

                ProcessInput("/ *", {LexicalToken("/", LEX_TOKEN_DIVIDE), LexicalToken("*", LEX_TOKEN_MULTIPLY)});

                ProcessInput("/ *   -  \n+",
                             {LexicalToken("/", LEX_TOKEN_DIVIDE), LexicalToken("*", LEX_TOKEN_MULTIPLY),
                              LexicalToken("-", LEX_TOKEN_MINUS), LexicalToken("+", LEX_TOKEN_PLUS)});

                EXPECT_EXIT(ProcessInput("+*+", {}), ::testing::ExitedWithCode(LEXICAL_ANALYSIS_ERROR_CODE),
                            "Unknown operator: .*");

                EXPECT_EXIT(ProcessInput("/-", {}), ::testing::ExitedWithCode(LEXICAL_ANALYSIS_ERROR_CODE),
                            "Unknown operator: .*");
            }

            TEST_F(LexicalAnalysisTests, Integers) {
                ProcessInput("1", {LexicalToken("1", LEX_TOKEN_INTEGER_LITERAL)});

                ProcessInput("1 2", {LexicalToken("1", LEX_TOKEN_INTEGER_LITERAL),
                                     LexicalToken("2", LEX_TOKEN_INTEGER_LITERAL)});

                ProcessInput("1 234", {LexicalToken("1", LEX_TOKEN_INTEGER_LITERAL),
                                       LexicalToken("234", LEX_TOKEN_INTEGER_LITERAL)});
            }

            TEST_F(LexicalAnalysisTests, Floats) {
                ProcessInput("1.1", {LexicalToken("1.1", LEX_TOKEN_FLOAT_LITERAL)});

                ProcessInput("1.1 2e1", {LexicalToken("1.1", LEX_TOKEN_FLOAT_LITERAL),
                                         LexicalToken("2e1", LEX_TOKEN_FLOAT_LITERAL)});

                ProcessInput("1.1 2e1 3e-12 4.52e+13", {LexicalToken("1.1", LEX_TOKEN_FLOAT_LITERAL),
                                                        LexicalToken("2e1", LEX_TOKEN_FLOAT_LITERAL),
                                                        LexicalToken("3e-12", LEX_TOKEN_FLOAT_LITERAL),
                                                        LexicalToken("4.52e+13", LEX_TOKEN_FLOAT_LITERAL)});

                EXPECT_EXIT(ProcessInput("1.1e", {}), ::testing::ExitedWithCode(LEXICAL_ANALYSIS_ERROR_CODE),
                            "Invalid float: .*");

                EXPECT_EXIT(ProcessInput("1.1e-", {}), ::testing::ExitedWithCode(LEXICAL_ANALYSIS_ERROR_CODE),
                            "Invalid float: .*");

                EXPECT_EXIT(ProcessInput("1.1e+", {}), ::testing::ExitedWithCode(LEXICAL_ANALYSIS_ERROR_CODE),
                            "Invalid float: .*");

                EXPECT_EXIT(ProcessInput("1.1e1.1", {}), ::testing::ExitedWithCode(LEXICAL_ANALYSIS_ERROR_CODE),
                            "Invalid float: .*");
            }
        }// namespace
    }    // namespace tests
}// namespace soma
