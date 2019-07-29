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
    const ObjectType ARRAY_OBJ        = "ARRAY";
    const ObjectType BUILTIN_OBJ      = "BUILTIN";

    class Object {
    public:
        Object() : next(nullptr), mark(0) {}
        virtual ~Object() { }
        virtual ObjectType Type() = 0;
        virtual std::string Inspect() = 0;

        // for GC (mark and sweep)
        Object* next;
        bool mark;
    };

    class Integer : public Object {
    public:
        Integer(int v) : Object(), value(v) { }
        ObjectType Type() { return INTEGER_OBJ; }
        std::string Inspect() { return std::to_string(value); }

        int value;
    };

    class Boolean : public Object {
    public:
        Boolean(bool b) : Object(), value(b) { }
        ObjectType Type() { return BOOLEAN_OBJ; }
        std::string Inspect() { return value ? "true" : "false"; }

        bool value;
    };

    class String : public Object {
    public:
        String(std::string s) : Object(), value(s) { }
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
        ReturnValue(Object* v) : Object(), value(v) { }
        ObjectType Type() { return RETURN_VALUE_OBJ; }
        std::string Inspect() { return value->Inspect(); }
        
        Object* value;
    };

    class Error : public Object {
    public:
        Error(std::string msg) : Object(), message(msg) { }
        ObjectType Type() { return ERROR_OBJ; }
        std::string Inspect() { return "ERROR: " + message; }
        
        std::string message;
    };

    class Environment;

    class Function : public Object {
    public:
        Function(std::vector<Identifier*>& params, BlockStatement* b) :
            Object(), parameters(params), body(b) { }
        ~Function() {
            for(auto param : parameters)
                delete param;
            delete body;
        }
        ObjectType Type() { return FUNCTION_OBJ; }
        std::string Inspect() {
            std::string res =  "fn (";
            for (auto ident : parameters) {
                res += ident->String() + ", ";
            }
            res += ")" + body->String();
            return res;
        }

        std::vector<Identifier*> parameters;
        BlockStatement* body;
    };

    class Array : public Object {
    public:
        Array(std::vector<Object*>& elems) :
            Object(), elements(elems) { }
        ~Array() {
            for(auto elem : elements)
                delete elem;
        }
        ObjectType Type() { return ARRAY_OBJ; }
        std::string Inspect() {
            std::string res =  "[";
            for (auto elem : elements) {
                res += elem->Inspect() + ", ";
            }
            res += "]";
            return res;
        }

        std::vector<Object*> elements;
    };

    class Builtin : public Object {
    public:
        Builtin(Object* (*fn)(std::vector<Object*>&)) : Object(), function(fn) { }
        ~Builtin() {
        }
        ObjectType Type() { return BUILTIN_OBJ; }
        std::string Inspect() { return "builtin function"; }

        Object* (*function)(std::vector<Object*>&);
    };

    extern Null* __NULL;
    extern Boolean* __TRUE;
    extern Boolean* __FALSE;
}


#endif