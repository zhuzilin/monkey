#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

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
            store[name] = val;
        }

        std::unordered_map<std::string, Object*> store;
        Environment* outer;
    };
}

#endif