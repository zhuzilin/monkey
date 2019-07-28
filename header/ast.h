#ifndef AST_H
#define AST_H
#include <string>
#include <vector>
#include "token.h"

namespace monkey {
    /*
     * Interfaces
     * use pure class to imitate interface.
     */
    class Node {
    public:
        virtual ~Node() { };
        virtual std::string TokenLiteral() = 0;
        virtual std::string String() = 0;
        virtual std::string Type() = 0;
    };

    // here I haven't found any good solution to nesting interface.
    class Statement : public Node {
    public:
        virtual void statementNode() = 0;
        virtual std::string TokenLiteral() = 0;
        virtual std::string String() = 0;
        virtual std::string Type() = 0;
    };

    class Expression : public Node {
    public:
        virtual void expressionNode() = 0;
        virtual std::string TokenLiteral() = 0;
        virtual std::string String() = 0;
        virtual std::string Type() = 0;
    };

    /*
     * Whole Program
     */
    class Program : public Node {
    public:
        ~Program() {
            for(auto stmt : statements) {
                delete stmt;
            }
        }
        std::string TokenLiteral();
        std::string String();
        std::string Type() { return "Program"; }

        std::vector<Statement*> statements;
        
    };

    /*
     * Expressions
     */
    class BlockStatement;

    class Identifier : public Expression {
    public:
        void expressionNode() { }
        std::string TokenLiteral() { return token.literal; }
        std::string String() { return value; }
        std::string Type() { return "Identifier"; }
        
        Token token;
        std::string value;
    };

    class IntegerLiteral : public Expression {
    public:
        void expressionNode() { }
        std::string TokenLiteral() { return token.literal; }
        std::string String() { return std::to_string(value); }
        std::string Type() { return "IntegerLiteral"; }
        
        Token token;
        int value;
    };

    class BooleanLiteral : public Expression {
    public:
        void expressionNode() { }
        std::string TokenLiteral() { return token.literal; }
        std::string String() { return value ? "true" : "false"; }
        std::string Type() { return "BooleanLiteral"; }
        
        Token token;
        bool value;
    };

    class StringLiteral : public Expression {
    public:
        void expressionNode() { }
        std::string TokenLiteral() { return token.literal; }
        std::string String() { return value; }
        std::string Type() { return "StringLiteral"; }

        Token token;
        std::string value;
    };

    class FunctionLiteral : public Expression {
    public:
        ~FunctionLiteral() {
            for(auto param : parameters) {
                delete param;
            }
        }
        void expressionNode() { }
        std::string TokenLiteral() { return token.literal; }
        std::string String();
        std::string Type() { return "FunctionLiteral"; }
        
        Token token;
        std::vector<Identifier*> parameters;
        BlockStatement* body;
    };

    class ArrayLiteral : public Expression {
    public:
        ~ArrayLiteral() {
            for(auto elem : elements) {
                delete elem;
            }
        }
        void expressionNode() { }
        std::string TokenLiteral() { return token.literal; }
        std::string String();
        std::string Type() { return "ArrayLiteral"; }
        
        Token token;
        std::vector<Expression*> elements;
    };

    class CallExpression : public Expression {
    public:
        ~CallExpression() {
            for(auto arg : arguments) {
                delete arg;
            }
            delete function;
        }
        void expressionNode() { }
        std::string TokenLiteral() { return token.literal; }
        std::string String();
        std::string Type() { return "CallExpression"; }
        
        Token token;
        Expression* function;
        std::vector<Expression*> arguments;
    };

    class IndexExpression : public Expression {
    public:
        void expressionNode() { }
        std::string TokenLiteral() { return token.literal; }
        std::string String();
        std::string Type() { return "IndexExpression"; }

        Token token;
        Expression* array;
        Expression* index;
    };

    class PrefixExpression : public Expression {
    public:
        void expressionNode() { }
        std::string TokenLiteral() { return token.literal; }
        std::string String();
        std::string Type() { return "PrefixExpression"; }
        
        Token token;
        std::string op;
        Expression* right;
    };

    class InfixExpression : public Expression {
    public:
        void expressionNode() { }
        std::string TokenLiteral() { return token.literal; }
        std::string String();
        std::string Type() { return "InfixExpression"; }
        
        Token token;
        Expression* left;
        std::string op;
        Expression* right;
    };

    class IfExpression : public Expression {
    public:
        void expressionNode() { }
        std::string TokenLiteral() { return token.literal; }
        std::string String();
        std::string Type() { return "IfExpression"; }
        
        Token token;  // if
        Expression* condition;
        BlockStatement* consequence;
        BlockStatement* alternative;
    };

    class WhileExpression : public Expression {
    public:
        void expressionNode() { }
        std::string TokenLiteral() { return token.literal; }
        std::string String();
        std::string Type() { return "WhileExpression"; }
        
        Token token;  // if
        Expression* condition;
        BlockStatement* consequence;
    };

    /*
     * Statements
     */
    // for code like: let x = 5;
    class LetStatement : public Statement {
    public:
        ~LetStatement() {
            delete value;
        }
        void statementNode() { }
        std::string TokenLiteral() { return token.literal; }
        std::string String();
        std::string Type() { return "LetStatement"; }

        Token token;  // token LET
        Identifier name;
        Expression* value;
    };

    // for code like: return 10;
    class ReturnStatement : public Statement {
    public:
        ~ReturnStatement() {
            delete returnValue;
        }
        void statementNode() { }
        std::string TokenLiteral() { return token.literal; }
        std::string String();
        std::string Type() { return "ReturnStatement"; }

        Token token;  // token RETURN
        Expression* returnValue;
    };

    // for code like: x + 10;
    class ExpressionStatement : public Statement {
    public:
        ~ExpressionStatement() {
            delete expression;
        }
        void statementNode() {}
        std::string TokenLiteral() { return token.literal; }
        std::string String();
        std::string Type() { return "ExpressionStatement"; }

        Token token;  // the first token of the expression
        Expression* expression;
    };

    class BlockStatement : public Statement {
    public:
        ~BlockStatement() {
            for(auto stmt : statements) {
                delete stmt;
            }
        }
        void statementNode() {}
        std::string TokenLiteral() { return token.literal; }
        std::string String();
        std::string Type() { return "BlockStatement"; }

        Token token; // "{"
        std::vector<Statement*> statements;
    };
}

#endif