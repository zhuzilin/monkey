//
// Created by zilzhu on 7/25/2019.
//
#include <ctype.h>
#include "../header/lexer.h"

namespace monkey {
    /*
     * utility functions
     */
    bool isLetter(char ch) {
        return isalpha(ch) || ch == '_';
    }

    bool isDigit(char ch) {
        return isdigit(ch);
    }

    /*
     * private functions
     */
    void Lexer::readChar() {
        if(readPosition >= input.size()) {
            ch = 0;
        } else {
            ch = input[readPosition];
        }
        position = readPosition;
        readPosition++;
    }

    char Lexer::peekChar() {
        if(readPosition >= input.size()) {
            return 0;
        } else {
            return input[readPosition];
        }
    }

    std::string Lexer::readIdentifier() {
        int start = position;
        while(isLetter(ch)) {
            readChar();
        }
        return input.substr(start, position - start);
    }

    std::string Lexer::readNumber() {
        int start = position;
        while(isDigit(ch)) {
            readChar();
        }
        return input.substr(start, position - start);
    }

    std::string Lexer::readString() {
        int start = position + 1;
        do {
            readChar();
        } while(ch != '"' && ch != 0);
        return input.substr(start, position - start);
    }

    void Lexer::skipWhitespace() {
        while(ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
            readChar();
        if(ch == '/' && peekChar() == '/') {  // add comment support
            while(ch != '\n')
                readChar();
            skipWhitespace();
        }
    }

    /*
     * public functions
     */
    void Lexer::New(std::string& input) {
        this->input = input;
        readPosition = 0;
        readChar();
    }

    Token Lexer::NextToken() {
        Token tok;
        skipWhitespace();
        switch (ch) {
        case '=':
            if(peekChar() == '=') {
                tok = Token(EQ, "==");
                readChar();
            } else {
                tok = Token(ASSIGN, ch);
            }
            readChar();
            break;
        case '!':
            if(peekChar() == '=') {
                tok = Token(NE, "!=");
                readChar();
            } else {
                tok = Token(BANG, ch);
            }
            readChar();
            break;
        case '<':
            if(peekChar() == '=') {
                tok = Token(LE, "<=");
                readChar();
            } else {
                tok = Token(LT, ch);
            }
            readChar();
            break;
        case '>':
            if(peekChar() == '=') {
                tok = Token(GE, ">=");
                readChar();
            } else {
                tok = Token(GT, ch);
            }
            readChar();
            break;
        case '+':
            tok = Token(PLUS, ch);
            readChar();
            break;
        case '-':
            tok = Token(MINUS, ch);
            readChar();
            break;
        case '*':
            tok = Token(ASTERISK, ch);
            readChar();
            break;
        case '/':
            tok = Token(SLASH, ch);
            readChar();
            break;
        case '%':
            tok = Token(PERCENT, ch);
            readChar();
            break;
        case ',':
            tok = Token(COMMA, ch);
            readChar();
            break;
        case ';':
            tok = Token(SEMICOLON, ch);
            readChar();
            break;
        case '(':
            tok = Token(LPAREN, ch);
            readChar();
            break;
        case ')':
            tok = Token(RPAREN, ch);
            readChar();
            break;
        case '{':
            tok = Token(LBRACE, ch);
            readChar();
            break;
        case '}':
            tok = Token(RBRACE, ch);
            readChar();
            break;
        case '[':
            tok = Token(LBRACKET, ch);
            readChar();
            break;
        case ']':
            tok = Token(RBRACKET, ch);
            readChar();
            break;
        case '"':
            tok = Token(STRING, readString());
            readChar();
            break;
        case 0:
            tok = Token(END, "");
            readChar();
            break;
        default:
            if(isLetter(ch)) {
                tok.literal = readIdentifier();
                tok.type = LookupIdent(tok.literal);
            } else if(isDigit(ch)) {
                tok.type = INT;
                tok.literal = readNumber();
            } else {
                tok = Token(ILLEGAL, ch);
                readChar();
            }
            break;
        }
        return tok;
    }
}