#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include "ast.h"

namespace monkey {
    typedef std::string ObjectType;

    const ObjectType INTEGER_OBJ      = "INTEGER";
    const ObjectType BOOLEAN_OBJ      = "BOOLEAN";
    const ObjectType STRING_OBJ       = "STRING";
    const ObjectType NULL_OBJ         = "NULL";
    const ObjectType RETURN_VALUE_OBJ = "RETURN_VALUE";
    const ObjectType ERROR_OBJ        = "ERROR";
    const ObjectType FUNCTION_OBJ     = "FUNCTION";

    class Object {
    public:
        virtual ObjectType Type() = 0;
        virtual std::string Inspect() = 0;
    };

    class Integer : public Object {
    public:
        Integer(int v) : value(v) { }
        ObjectType Type() { return INTEGER_OBJ; }
        std::string Inspect() { return std::to_string(value); }

        int value;
    };

    class Boolean : public Object {
    public:
        Boolean() { }
        Boolean(bool b) : value(b) { }
        ObjectType Type() { return BOOLEAN_OBJ; }
        std::string Inspect() { return value ? "true" : "false"; }

        bool value;
    };

    class String : public Object {
    public:
        String() { }
        String(std::string s) : value(s) { }
        ObjectType Type() { return STRING_OBJ; }
        std::string Inspect() { return value; }

        std::string value;
    };

    class Null : public Object {
    public:
        ObjectType Type() { return NULL_OBJ; }
        std::string Inspect() { return "NULL"; }
    };

    class ReturnValue : public Object {
    public:
        ReturnValue(Object* v) : value(v) { } 
        ObjectType Type() { return RETURN_VALUE_OBJ; }
        std::string Inspect() { return value->Inspect(); }
        
        Object* value;
    };

    class Error : public Object {
    public:
        Error(std::string msg) : message(msg) { }
        ObjectType Type() { return ERROR_OBJ; }
        std::string Inspect() { return "ERROR: " + message; }
        
        std::string message;
    };

    class Environment;

    class Function : public Object {
    public:
        Function(std::vector<Identifier*>& params, BlockStatement* b, Environment* e) :
            parameters(params), body(b), env(e) { }
        ObjectType Type() { return FUNCTION_OBJ; }
        std::string Inspect() {
            std::string res =  "fn (";
            for (auto ident : parameters) {
                res += ident->String() + ", ";
            }
            res += body->String();
            return res;
        }

        std::vector<Identifier*> parameters;
        BlockStatement* body;
        Environment* env;
    };
}


#endif