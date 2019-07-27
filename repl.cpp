#include <iostream>
#include <string>
#include "./header/lexer.h"
#include "./header/parser.h"
#include "./header/evaluator.h"
#include "./header/environment.h"

void eval() {
    const std::string PROMPT = ">> ";
    monkey::Lexer l;
    monkey::Parser p;
    monkey::Environment* env = new monkey::Environment();
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
            continue;
        }
        monkey::Object* o = monkey::Eval(program, env);
        std::cout << "type: " << o->Type() << std::endl;
        std::cout << o->Inspect() << std::endl;
    }
}

int main() {
    eval();
    return 0;
}