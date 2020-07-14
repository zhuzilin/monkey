#include <iostream>
#include <string>
#include "./header/lexer.h"

void sh() {
  const std::string PROMPT = ">> ";
  monkey::Lexer l;
  while(true) {
    std::string line;
    std::cout << PROMPT;
    std::getline(std::cin, line);
    l.New(line);
    monkey::Token t = l.NextToken();
    while(t.type != monkey::END) {
      std::cout << "Type: " << t.type << ", Literal: " << t.literal << std::endl;
      t = l.NextToken();
    }
  }
}

int main() {
  sh();
  return 0;
}