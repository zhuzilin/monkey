#ifndef MONKEY_LEXER_H_
#define MONKEY_LEXER_H_

#include "token.h"
#include <string>

namespace monkey {

class Lexer {
 public:
  void New(std::string& input);
  Token NextToken();
 private:
  void readChar();
  char peekChar();
  std::string readIdentifier();
  std::string readNumber();
  std::string readString();
  void skipWhitespace();

  std::string input;
  int position;
  int readPosition;
  char ch;
};

}  // namespace monkey


#endif //MONKEY_LEXER_H_
