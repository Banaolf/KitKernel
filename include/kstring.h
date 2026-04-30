#pragma once

#include <stdint.h>

#define uppercase "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define lowercase "abcdefghijklmnopqrstuvwxyz"
#define numbers "1234567890"
#define operators "/*-+"

#define NOT_FOUND -2
#define STRING_BAD -3

int Strlen(const char* str);

namespace checks {
    bool isAlphaNumerical(const char c);
    bool isAlpha(const char c);
    bool isUppercase(const char c);
    bool isLowercase(const char c);
    //
    bool isAlphaNumerical(const char* str);
    bool isAlpha(const char* str);
    bool isUppercase(const char* str);
    bool isLowercase(const char* str);
    //
    bool isOperator(const char c);
    bool isDigit(const char c);
    bool isSpecial(const char c);
    //
    bool isOperator(const char* str);
    bool isDigit(const char* str);
    bool isSpecial(const char* str);
}

class character {
private:
    char c;
public:
    character(const char C)
        : c(C) {}
    char get () const { return c; }
    void set (const char C) { c = C; }
    bool equals (const char ch) const { if (c == ch) return true; return false; }
    bool in (const char* string) const;
    bool in (const char* string, const int starterIndex) const;
    int inCount (const char* string) const;
};
#include "vector.h"
class String {
private:
    char* buffer;
    int len;
public:
    String();
    String(const char* str);
    String(const String& other);
    ~String();
    
    int length() const {return len;}
    bool equals(const char* str) const;
    bool equals(const String& other) const;
    bool find(const char* str) const;
    bool find(const String& str) const;
    bool startsWith(const char __c) const;
    bool endsWith(const char __c) const;
    char get(int index) const {if (index < 0 || index >= this->len) return this->back(); else return buffer[index];}
    char back() const {return buffer[len-1];}
    void append(const char c);
    void append(const char* str);
    void appendAt(const char c, int place);
    void appendAt(const char* str, int place);
    void backspace();
    void backspace(const int times);
    void clear();
    const char* cstr() const;
    Vector<String> split(const char __splitwith) const;
    String substr(const int i1, const int i2) const;
};

String fmtString(const char* fmt, ...);
String intostr(long long n);
String ftostr(double f);
char* uint_to_str(uint64_t value, char* buffer);
int strToInt(const String& s);
void strToFloat(const String& s, float& out);