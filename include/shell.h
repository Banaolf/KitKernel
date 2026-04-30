#pragma once

#define _KEYWORD_AMOUNT_ 5

#include "kstring.h"

enum TokenType {
    keyword, word, flag, eoc
};

inline String keywords[_KEYWORD_AMOUNT_] {
    "power",
    "scroll",
    "wash",
    "minfo",
    "pinfo"
};
template<typename T>
bool isKeyword(T val) {for (int i = 0; i < _KEYWORD_AMOUNT_; i++) if (keywords[i].equals(val)) return true; return false;}

class Token {
protected:
    TokenType type;
    String contents;
public:
    Token(TokenType ty, char* value) : type(ty), contents(value) {}
    Token(TokenType ty, String& value) : type(ty), contents(value) {}
    ~Token() = default;

    inline bool istype(TokenType __expected) {return this->type == __expected;}
    inline bool isstring(char *__expected) {return this->contents.equals(__expected);}
    inline bool isstring(String &__expected) {return this->contents.equals(__expected);}
    inline boot isflag(const char __c) {return this->contents.equals(fmtString("-%c", ))}
};

void shmain();