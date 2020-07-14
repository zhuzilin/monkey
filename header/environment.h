#ifndef MONKEY_ENVIRONMENT_H_
#define MONKEY_ENVIRONMENT_H_

#include <unordered_map>
#include <string>
#include "object.h"

namespace monkey {
// environment should only detach object pointer from store
// never delete in environment
class Environment {
public:
  Environment() : outer(nullptr) { }
  ~Environment() {
    store.clear();
  }
  Environment* NewEnclosedEnvironment() {
    Environment* inner = new Environment();
    inner->outer = this;
    return inner;
  }

  Object* Get(std::string name) {
    if(store.find(name) == store.end()) {
      if (outer != nullptr) {
        Object* obj = outer->Get(name);
        return obj;
      }
      return new Error("identifier not found: " + name);
    }
    return store[name];
  }

  // we can only use let to set.
  // no assignments for outer variables.
  Object* Set(std::string name, Object* val) {
    if (val->Type() == RETURN_VALUE_OBJ) 
      store[name] = ((ReturnValue*)val)->value;
    else
      store[name] = val;
    return __NULL;
  }

  Object* RefSet(std::string name, Object* val) {
    if(store.find(name) == store.end())
      return new Error(Error("identifier not defined: " + name));
    Object* old = Get(name);
    if(old->Type() != val->Type())
      return new Error("does not support different type reference assign yet");
    // we cannot have virtual constructor function in C++
    // therefore, we could not use virtual copy constructor
    // TODO: find a better way

    if (val->Type() == INTEGER_OBJ)
      *(Integer*)old = Integer(((Integer*)val)->value);
    else if (val->Type() == STRING_OBJ)
      *(String*)old = String(((String*)val)->value);
    else if (val->Type() == RETURN_VALUE_OBJ)
      *(ReturnValue*)old = RefSet(name, ((ReturnValue*)val)->value);
    else if (val->Type() == FUNCTION_OBJ)
      *(Function*)old = Function(((Function*)val)->parameters, ((Function*)val)->body);
    else if (val->Type() == ARRAY_OBJ)
      *(Array*)old = Array(((Array*)val)->elements);
    else  // boolean and null
      Set(name, val);
    return __NULL;
  }

  std::unordered_map<std::string, Object*> store;
  Environment* outer;
};

}  // namespace monkey

#endif  // MONKEY_ENVIRONMENT_H_