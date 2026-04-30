#include "../../include/shell.h"
#include "../../include/kstring.h"

static Vector<Token> lex(char* src) {
    Vector<Token> stream;
    String source(src);

    for (int i = 0; i < source.length(); i++) {
        char c = source.get(i);
        if (c == '"') {

        }
    }

    return stream;
}

void shmain(char* src) {
    Vector<Token> lxd = lex(src);
}