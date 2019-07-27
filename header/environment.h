#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <string>
#include "object.h"

namespace monkey {
    class Environment {
    public:

        Environment* NewEnclosedEnvironment() {
            Environment* inner = new Environment();
            inner->outer = this;
            return inner;
        }

        Object* Get(std::string name) {
            if(store.find(name) == store.end()) {
                if (outer != nullptr)
                    return outer->Get(name);
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