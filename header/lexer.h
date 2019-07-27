#ifndef LEXER_H
#define LEXER_H

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
}


#endif //INTERPRETER_LEXER_H
