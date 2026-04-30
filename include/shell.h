#pragma once

#include "kstring.h"
enum class TokenType {
    keyword, word, flag, eoc
};

class Token {
protected:
    TokenType type;
    String contents;
public:
    Token(TokenType ty, char* value) : type(ty), contents(value) {}
    ~Token() = default;

    inline bool istype(TokenType __expected) {return this->type == __expected;}
    inline bool isstring(char *__expected) {return this->contents.equals(__expected);}
};

void shmain();