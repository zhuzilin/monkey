#include "./header/parser.h"
#include <string>
#include <iostream>

void sh() {
    const std::string PROMPT = ">> ";
    monkey::Lexer l;
    monkey::Parser p;
    while(true) {
        std::string line;
        std::cout << PROMPT;
        std::getline(std::cin, line);
        l.New(line);
        p.New(l);
        monkey::Program* program = p.ParseProgram();
        if (p.Errors().size()) {
            for (auto error : p.Errors()) {
                std::cout << error << std::endl;
            }
        } else
            std::cout << program->String() << std::endl;
    }
}

int main() {
    sh();
    return 0;
}