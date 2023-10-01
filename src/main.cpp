#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

#include "./tokenization.hpp"
#include "./parser.hpp"
#include "./generation.hpp"
#include "./tests.hpp"



std::string get_contents(char* filename) {
    std::string contents; 
    {
        std::stringstream contents_stream;
        std::fstream input(filename, std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }   
    return contents;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect usage" << std::endl;
        std::cerr << "hydro <input.hy>" << std::endl;
        return EXIT_FAILURE;
    }
    // std::cout << get_contents(argv[1]) << std::endl;
    std::string contents = get_contents(argv[1]);
    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens = tokenizer.tokenize();  //Лексический анализ (токенизация) 
    // TokenTest test(tokens);
    // test.check_tokens();

    Parser parser(std::move(tokens));
    std::optional<NodeProg> prog = parser.parse_prog(); //Парсинг (создание абстрактного дерева)
    if (!(prog.has_value())) {
        std::cerr << "Invalid Program" << std::endl;
        exit(EXIT_FAILURE);
    } 
    Generator generator(prog.value()); 
    {
        std::fstream file("out.asm", std::ios::out);
        file << generator.gen_prog(); //Генерация кода
    }
    system("nasm -felf64 out.asm");
    system("ld -o out out.o");
    system("rm out.o");
    return EXIT_SUCCESS;
}