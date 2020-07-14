#ifndef MONKEY_EVALUATOR_H_
#define MONKEY_EVALUATOR_H_

#include <unordered_map>
#include <functional>
#include <cstdarg>
#include "object.h"
#include "ast.h"
#include "environment.h"
#include "gc.h"

namespace monkey {
class Evaluator {
 public:
  Object* Eval(Node* node, Environment* env);
 private:
  Object* evalStatements(std::vector<Statement*> statements, Environment* env);
  Object* evalBangOperatorExpression(Object* right);
  Object* evalMinusPrefixExpression(Object* right);
  Object* evalPrefixExpression(std::string op, Object* right);
  Object* evalIntegerInfixExpression(std::string op, Object* left, Object* right);
  Object* evalStringInfixExpression(std::string op, Object* left, Object* right);
  Object* evalInfixExpression(std::string op, Object* left, Object* right);
  Environment* extendedFunctionEnv(Function* fn, std::vector<Object*>& args, Environment* env);
  Object* evalIndexExpression(Object* array, Object* index, Environment* env);
  Object* evalArrayIndexExpression(Array* array, Integer* index);
  Object* evalStringIndexExpression(String* array, Integer* index);
  Object* evalCallExpression(Object* fn, std::vector<Object*>& args, Environment* env);
  Object* evalIdentifier(std::string name, Environment* env);
  Object* evalProgram(Program* program, Environment* env);

  GarbageCollector gc;
  int gcCounter = 0;

};
  
}  // namespace monkey

#endif  // MONKEY_EVALUATOR_H_