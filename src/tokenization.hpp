#pragma once 
#include <iostream>
#include <optional>
#include <vector>

enum class TokenType {
    exit,
    int_lit, 
    semi,
    open_paren,
    close_paren,
    ident,
    let,
    eq
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

class Tokenizer {
    public:
        inline explicit Tokenizer(const std::string& src) //! 
            : m_src(std::move(src))
        {
        }

        inline std::vector<Token> tokenize() {
            std::vector<Token> tokens;
            std::string buffer;
            while (peek().has_value()) {
                if (std::isalpha(peek().value())) { //Если является ключевым словом (переменная или команда)
                    buffer.push_back(consume());
                    while (peek().has_value() && std::isalnum(peek().value())) {
                        buffer.push_back(consume());
                    }
                    if (buffer == "exit") {
                        tokens.push_back({ .type = TokenType::exit });
                        buffer.clear();
                        continue;
                    }
                    else if (buffer == "let") {
                        tokens.push_back({.type = TokenType::let});
                        buffer.clear(); 
                        continue;
                    }
                    else {
                        tokens.push_back({.type = TokenType::ident, .value = buffer});
                        buffer.clear();
                        continue;
                    }
                }
                else if (std::isdigit(peek().value())) { //если является числом
                    buffer.push_back(consume());
                    while (peek().has_value() && std::isdigit(peek().value())) {
                        buffer.push_back(consume());
                    }
                    tokens.push_back({.type = TokenType::int_lit, .value = buffer});
                    buffer.clear();
                    continue;
                }
                else if (peek().value() == '(') { //если является (
                    consume();
                    tokens.push_back({ .type = TokenType::open_paren});
                    continue;
                }
                else if (peek().value() == ')') { //если является )
                    consume();
                    tokens.push_back({ .type = TokenType::close_paren});
                    continue;
                }

                else if (peek().value() == ';') { //если является ;
                    consume();
                    tokens.push_back({ .type = TokenType::semi });
                    continue;
                }
                else if (peek().value() == '=') { //если является =
                    consume();
                    tokens.push_back({ .type = TokenType::eq });
                    continue;
                }
                else if (std::isspace(peek().value())) { //если является пробелом
                    consume();
                    continue;
                } else { //в противном случае получаем ошибку
                    std::cerr << "You messed up!" << std::endl;
                    exit(EXIT_FAILURE);
                }
                m_index = 0;
            }
            return tokens;
        }

    private:
        [[nodiscard]] inline std::optional<char> peek(int offset = 0) const {
            if (m_index + offset >= m_src.length()) {
                return {};
            } else {
                return m_src.at(m_index + offset);
            }
        }
        inline char consume() {
            return m_src.at(m_index++);            
        }
         
        const std::string m_src;
        int m_index = 0;
};