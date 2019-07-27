#include "../header/token.h"

namespace monkey {
    std::unordered_map<std::string, TokenType> keywords({
        {"let", LET},
        {"fn", FUNCTION},
        {"true", TRUE},
        {"false", FALSE},
        {"if", IF},
        {"else", ELSE},
        {"return", RETURN},
    });

    TokenType LookupIdent(std::string ident) {
        if(keywords.find(ident)== keywords.end())
            return IDENT;
        return keywords[ident];
    }
}
