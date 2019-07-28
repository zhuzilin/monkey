#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <unordered_map>
#include "lexer.h"
#include "ast.h"

namespace monkey {
    class Parser;

    typedef Expression* (Parser::*PrefixParseFn)();  // prefix operator example: ++i
    typedef Expression* (Parser::*InfixParseFn)(Expression* );  // infix operator example: 1 + 1

    //  const qualifier is not allowed on enum
    enum Precedence{
        LOWEST,
        EQUALS,      // ==
        LESSGREATER, // > or <
        SUM,         // +
        PRODUCT,     // *
        PREFIX,      // -X or !X
        CALL         // myFunction(X)
    };

    extern std::unordered_map<TokenType, Precedence> precedences;

    class Parser {
    public:
        void New(Lexer& l);
        std::vector<std::string> Errors() { return errors; }
        Program* ParseProgram();
    private:
        void nextToken();
        bool expectPeek(const TokenType& t);
        Precedence peekPrecedence();
        Precedence curPrecedence();

        Statement* parseStatement();
        LetStatement* parseLetStatement();
        ReturnStatement* parseReturnStatement();
        ExpressionStatement* parseExpressionStatement();
        BlockStatement* parseBlockStatement();

        Expression* parseExpression(Precedence precedence);
        // prefix parser functions
        Expression* parseIdentifier();  // cannot return Identifier* to match the PrefixParseFn
        Expression* parseIntegerLiteral();
        Expression* parseStringLiteral();
        Expression* parsePrefixExpression();
        Expression* parseBoolean();
        std::vector<Identifier*> parseFunctionParameters();
        Expression* parseFunctionLiteral();
        Expression* parseGroupedExpression();
        Expression* parseIfExpression();
        Expression* parseForExpression();
        // infix parser functions
        std::vector<Expression*> parseCallArguments();
        Expression* parseCallExpression(Expression* function);
        Expression* parseInfixExpression(Expression* left);

        void noPrefixParseFnError(TokenType type);

        void peekError(const TokenType& t);
        
        Lexer l;

        Token curToken;
        Token peekToken;
        
        std::unordered_map<TokenType, PrefixParseFn> prefixParseFns;
        std::unordered_map<TokenType, InfixParseFn> infixParseFns;

        std::vector<std::string> errors;
    };
}


#endif