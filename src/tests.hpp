#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include "./tokenization.hpp"

// enum class TokenType {
//     exit,
//     int_lit, 
//     semi,
//     open_paren,
//     close_paren,
//     ident,
//     let,
//     eq
// };


class TokenTest {
    public:
        TokenTest(const std::vector<Token>& tokens)
            : m_tokens(std::move(tokens)) 
        {

        }

        void check_tokens() {
            for (auto& token : m_tokens) {
                int type = static_cast<int>(token.type);    
                if (token.value.has_value()) {
                    std::cout << "Type is: " << type << ", " << "value is " << token.value.value() << std::endl;
                }
                else {
                    std::cout << "Type is: " << type << " no has value" << std::endl;
                }
            }
        }
    private:
        std::vector<Token> m_tokens;
};
