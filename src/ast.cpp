#include "../header/ast.h"

namespace monkey{
    std::string Program::TokenLiteral() {
        if(statements.size()) {
            return statements[0]->TokenLiteral();
        } else {
            return "";
        }
    }

    std::string Program::String() {
        std::string res;
        for(auto stmt : statements) {
            res += stmt->String() + "\n";
        }
        return res;
    }

    std::string LetStatement::String() {
        std::string res = TokenLiteral() + " " + 
            name.String() + " = ";
        if(value != nullptr) {
            res += value->String();
        }
        res += ";";
        return res;
    }

    std::string ReturnStatement::String() {
        std::string res = TokenLiteral() + " ";
        if(returnValue != nullptr) {
            res += returnValue->String();
        }
        res += ";";
        return res;
    }

    std::string ExpressionStatement::String() {
        std::string res = "";
        if(expression != nullptr) {
            res += expression->String();
        }
        res += ";";
        return res;
    }

    std::string BlockStatement::String() {
        std::string res = "{ ";
        for (auto stmt : statements) {
            res += stmt->String() + " ";
        }
        res += "}";
        return res;
    }

    std::string PrefixExpression::String() {
        std::string res = "(" + op + right->String() + ")";
        return res;
    }

    std::string InfixExpression::String() {
        std::string res = "(" + left->String() + op + right->String() + ")";
        return res;
    }

    std::string IfExpression::String() {
        std::string res = "if " + condition->String() + " " + consequence->String();
        if (alternative != nullptr)
            res += " else  " + alternative->String();
        return res;
    }

    std::string WhileExpression::String() {
        std::string res = "while " + condition->String() + " " + consequence->String();
        return res;
    }

    std::string FunctionLiteral::String() {
        std::string res = TokenLiteral() + " (";
        for(auto param : parameters) {
            res += param->String() + ", ";
        }
        res += ")";
        res += body->String();
        return res;
    }

    std::string ArrayLiteral::String() {
        std::string res = "[";
        for(auto elem : elements) {
            res += elem->String() + ", ";
        }
        res += "]";
        return res;
    }

    std::string CallExpression::String() {
        std::string res = function->String() + "(";
        for(auto arg : arguments) {
            res += arg->String() + ", ";
        }
        res += ")";
        return res;
    }

    std::string IndexExpression::String() {
        return "(" + array->String() + "[" + index->String() + "])";
    }
}