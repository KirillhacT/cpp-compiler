#include <iostream>
#include <fstream>
#include <sstream>

int main() {
    //Чтение файла
    std::stringstream stream;
    std::fstream input("test.txt", std::ios::in);
    stream << input.rdbuf();
    std::string content = stream.str();
    input.close();
    std::cout << content << std::endl;
    
    //Cоздание файла
    std::fstream output("test2.txt", std::ios::out);
    output << content + " lol";
    output.close();
}