#include "../header/evaluator.h"

namespace monkey {
    Null* __NULL = new Null();
    Boolean* __TRUE = new Boolean(true);
    Boolean* __FALSE = new Boolean(false);

    bool isTruthy(Object* condition) {
        if(condition == __TRUE) {
            return true;
        } else if(condition == __FALSE) {
            return false;
        } else if (condition == __NULL) {
            return false;
        } else if (condition->Type() == INTEGER_OBJ && ((Integer*)condition)->value == 0) {
            return false;
        } else {
            return true;
        }
    }

    bool isError(Object* o) {
        if(o != nullptr) {
            return o->Type() == ERROR_OBJ;
        }
        return false;
    }

    Object* Evaluator::evalStatements(std::vector<Statement*> statements, Environment* env) {
        Object* result;
        for(auto stmt : statements) {
            result = Eval(stmt, env);
            gc.Mark(result);  // mark the intermediate result
            if (result->Type() == RETURN_VALUE_OBJ) {
                gc.Mark(((ReturnValue*)result)->value);
            }
            gcCounter++;
            if(gcCounter == 100) {
                gc.Mark(env);
                gc.Sweep();
                gcCounter = 0;
            }
            if (result->Type() == RETURN_VALUE_OBJ || result->Type() == ERROR_OBJ)
                return result;
        }
        return result;
    }

    Object* Evaluator::evalBangOperatorExpression(Object* right) {
        if (right == __TRUE) {
            return __FALSE;
        } 
        else if (right == __FALSE) {
            return __TRUE;
        } 
        else if (right == __NULL) {
            return __TRUE;
        } 
        else if (right->Type() == INTEGER_OBJ) {
            if(((Integer*)right)->value == 0) {
                return __TRUE;
            } else {
                return __FALSE;
            }
        }    
        else {
            return __FALSE;
        }
    }

    Object* Evaluator::evalMinusPrefixExpression(Object* right) {
        if(right->Type() == INTEGER_OBJ) {
            Integer* i = new Integer(-((Integer*)right)->value);
            return i;
        }
        return new Error("unknown operator: -" +  right->Type());
    }

    Object* Evaluator::evalPrefixExpression(std::string op, Object* right) {
        if(op == "!") {
            return evalBangOperatorExpression(right);
        }
        else if (op == "-") {
            return evalMinusPrefixExpression(right);
        }
        else {
            return __NULL;
        }
    }

    Object* Evaluator::evalIntegerInfixExpression(std::string op, Object* left, Object* right) {
        int leftVal = ((Integer*)left)->value;
        int rightVal = ((Integer*)right)->value;
        Object* res;
        if(op == "+") {
            res = new Integer(leftVal + rightVal);
        }
        else if(op == "-") {
            res = new Integer(leftVal - rightVal);
        }
        else if(op == "*") {
            res = new Integer(leftVal * rightVal);
        }
        else if(op == "/" && rightVal != 0) {
            res = new Integer(leftVal / rightVal);
        }
        else if(op == "%" && rightVal != 0) {
            res = new Integer(leftVal % rightVal);
        }
        else if(op == "==") {
            return leftVal == rightVal ? __TRUE : __FALSE;
        }
        else if(op == "!=") {
            return leftVal != rightVal ? __TRUE : __FALSE;
        }
        else if(op == ">") {
            return leftVal > rightVal ? __TRUE : __FALSE;
        }
        else if(op == "<") {
            return leftVal < rightVal ? __TRUE : __FALSE;
        }
        else {
            return new Error("unknown operator: " + left->Type() + " " + op + " " +  right->Type());
        }
        gc.Add(res);
        return res;
    }

    Object* Evaluator::evalStringInfixExpression(std::string op, Object* left, Object* right) {
        std::string leftVal = ((String*)left)->value;
        std::string rightVal = ((String*)right)->value;
        Object* res;
        if(op == "+") {
            res = new String(leftVal + rightVal);
        }
        else {
            return new Error("unknown operator: " + left->Type() + " " + op + " " +  right->Type());
        }
        gc.Add(res);
        return res;
    }

    Object* Evaluator::evalInfixExpression(std::string op, Object* left, Object* right) {
        if (left->Type() == INTEGER_OBJ && right->Type() == INTEGER_OBJ) {
            return evalIntegerInfixExpression(op, left, right);
        }
        else if (left->Type() == STRING_OBJ && right->Type() == STRING_OBJ) {
            return evalStringInfixExpression(op, left, right);
        }
        else if(left == __NULL || right == __NULL) {
            return __NULL;
        }
        else if (left->Type() != right->Type())
            return new Error("type mismatch: " + left->Type() + " " + op + " " +  right->Type());
        else if(op == "==") {
            return left == right ? __TRUE : __FALSE;
        }
        else if(op == "!=") {
            return left != right ? __TRUE : __FALSE;
        }
        else
            return new Error("unknown operator: " + left->Type() + " " + op + " " +  right->Type());
    }

    Environment* Evaluator::extendedFunctionEnv(Function* fn, std::vector<Object*>& args, Environment* outer) {
        Environment* env = outer->NewEnclosedEnvironment();
        for(int i=0; i<args.size(); i++) {
            env->Set(fn->parameters[i]->value, args[i]);
        }
        return env;
    }

    Object* Evaluator::evalFunction(Object* fn, std::vector<Object*>& args, Environment* env) {
        if(fn->Type() != FUNCTION_OBJ) {
            return new Error("not a function: " + fn->Type());
        }
        if(((Function*)fn)->parameters.size() != args.size()) {
            return new Error("argument length(" + std::to_string(args.size()) +
                ") not equal to parameter length (" 
                + std::to_string(((Function*)fn)->parameters.size()) + ")");
        }
        Environment* extendedEnv = extendedFunctionEnv((Function*)fn, args, env);
        Object* evaluated = Eval(((Function*)fn)->body, extendedEnv);
        delete extendedEnv;
        if(evaluated->Type() == RETURN_VALUE_OBJ) {
            return ((ReturnValue*)evaluated)->value;
        }
        return evaluated;
    }

    Object* Evaluator::evalProgram(Program* program, Environment* env) {
        Object* o = evalStatements(program->statements, env);
        if (o->Type() == RETURN_VALUE_OBJ) {  // unwrap return value
            return ((ReturnValue*)o)->value;
        }
        return o;
    }

    Object* Evaluator::Eval(Node* node, Environment* env) {
        std::string type = node->Type();
        if (type == "Program") {
            return evalProgram((Program*)node, env);
        } 
        else if (type == "IntegerLiteral") {
            Integer* i = new Integer(((IntegerLiteral*)node)->value);
            gc.Add(i);
            return i;
        } 
        else if (type == "BooleanLiteral") {
            return ((BooleanLiteral*)node)->value ? __TRUE : __FALSE;
        }
        else if (type == "StringLiteral") {
            String* s = new String(((StringLiteral*)node)->value);
            gc.Add(s);
            return s;
        }
        else if (type == "Identifier") {
            return env->Get(((Identifier*)node)->value);
        }
        else if (type == "FunctionLiteral") {
            Function* f =  new Function(((FunctionLiteral*)node)->parameters, ((FunctionLiteral*)node)->body);
            gc.Add(f);
            return f;
        }
        else if (type == "CallExpression") {
            Object* function = Eval(((CallExpression*)node)->function, env);
            if(isError(function))
                return function;
            
            std::vector<Object*> args;
            for(auto* argument : ((CallExpression*)node)->arguments) {  // for convenience, using pass by value
                Object* arg = Eval(argument, env);
                if(isError(arg))
                    return arg;
                args.push_back(arg);
            }
            return evalFunction(function, args, env);
        }
        else if (type == "PrefixExpression") {
            Object* right = Eval(((PrefixExpression*)node)->right, env);
            if (isError(right))
                return right;
            return evalPrefixExpression(((PrefixExpression*)node)->op, right);
        }
        else if (type == "InfixExpression") {
            Object* left = Eval(((InfixExpression*)node)->left, env);
            if (isError(left))
                return left;
            // to save tmp data
            // TODO: find a better way
            env->Set(std::to_string((intptr_t)left), left);
            Object* right = Eval(((InfixExpression*)node)->right, env);
            if (isError(right))
                return right;
            env->store.erase(std::to_string((intptr_t)left));
            return evalInfixExpression(((InfixExpression*)node)->op, left, right);
        }
        else if (type == "IfExpression") {
            Object* condition = Eval(((IfExpression*)node)->condition, env);
            if(isError(condition))
                return condition;
            if(isTruthy(condition)) {
                return Eval(((IfExpression*)node)->consequence, env);
            } else if(((IfExpression*)node)->alternative != nullptr) {
                return Eval(((IfExpression*)node)->alternative, env);
            } else {
                return __NULL;
            }
        }
        else if (type == "ExpressionStatement") {
            return Eval(((ExpressionStatement*)node)->expression, env);
        }
        else if (type == "BlockStatement") {
            return evalStatements(((BlockStatement*)node)->statements, env);
        }
        else if (type == "ReturnStatement") {
            Object* val = Eval(((ReturnStatement*)node)->returnValue, env);
            if(isError(val))
                return val;
            return new ReturnValue(val);
        }
        else if (type == "LetStatement") {
            Object* val = Eval(((LetStatement*)node)->value, env);
            if(isError(val))
                return val;
            env->Set(((LetStatement*)node)->name.value, val);
            return __NULL;
        }
        else {
            return __NULL;
        }
    }
}