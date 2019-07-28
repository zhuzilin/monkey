#ifndef GC_H
#define GC_H
#include "object.h"
#include "environment.h"

namespace monkey {

    class Environment;

    class GarbageCollector {
    public:
        GarbageCollector() { head = new String("head"); }
        ~GarbageCollector() { delete head; }
        void Add(Object* obj) {
            obj->next = head->next;
            head->next = obj;
        }

        void Mark(Object* obj) {
            if(obj->mark)
                return;
            obj->mark = true;
            if (obj->Type() == ARRAY_OBJ) {
                for (auto elem : ((Array*)obj)->elements) {
                    Mark(elem);
                }
            }
        }

        void Mark(Environment* env) {
            for(std::pair<std::string, Object*> p : env->store) {
                Integer tmp = *(Integer*)(p.second);
                Object* obj = p.second;
                Mark(obj);
            }
            if(env->outer != nullptr) {
                Mark(env->outer);
            }
        }

        void Sweep() {
            Object* node = head;
            while(node->next != nullptr) {
                if (!node->next->mark) {
                    Object* tmp = node->next;
                    node->next = tmp->next;
                    delete tmp;
                } else {
                    node = node->next;
                }
            }
            node = head;
            while(node->next != nullptr) {
                node->next->mark = false;
                node = node->next;
            }
        }

        Object* head;
    };
}

#endif