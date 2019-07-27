#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "object.h"
#include "ast.h"
#include "environment.h"

namespace monkey {
    Object* Eval(Node* node, Environment* env);
}

#endif