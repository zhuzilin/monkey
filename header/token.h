#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <unordered_map>

namespace monkey {
    typedef std::string TokenType;
    class Token {
    public:
        Token() { }
        Token(TokenType type, std::string literal) : type(type), literal(literal) {}
        Token(TokenType type, char ch) : type(type), literal(std::string(1, ch)) {}

        TokenType type;
        std::string literal;
    };

    const TokenType ILLEGAL = "ILLEGAL";  // a token we don't know
    const TokenType END     = "END";  // end of file, notice that the origin EOF has been taken in C++

    const TokenType IDENT   = "IDENT";  // identifier
    const TokenType INT     = "INT";
    const TokenType STRING  = "STRING";

    const TokenType ASSIGN   = "=";
    const TokenType PLUS     = "+";
    const TokenType MINUS    = "-";
    const TokenType BANG     = "!";
    const TokenType ASTERISK = "*";
    const TokenType SLASH    = "/";
    const TokenType PERCENT  = "%";

    const TokenType LT       = "<";
    const TokenType GT       = ">";

    const TokenType EQ       = "==";
    const TokenType NE       = "!=";

    const TokenType COMMA     = ",";
    const TokenType SEMICOLON = ";";

    const TokenType LPAREN  = "(";
    const TokenType RPAREN  = ")";
    const TokenType LBRACE  = "{";
    const TokenType RBRACE  = "}";

    // keywords
    const TokenType FUNCTION = "FUNCTION";
    const TokenType LET      = "LET";
    const TokenType TRUE     = "TRUE";
    const TokenType FALSE    = "FALSE";
    const TokenType IF       = "IF";
    const TokenType ELSE     = "ELSE";
    const TokenType RETURN   = "RETURN";

    extern std::unordered_map<std::string, TokenType> keywords;

    TokenType LookupIdent(std::string ident);
}

#endif //INTERPRETER_TOKEN_H
