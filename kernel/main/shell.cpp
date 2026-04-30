#include "../../include/shell.h"
#include "../../include/kstring.h"
#include "../../include/vector.h"

typedef unsigned char uint8_t;

static uint8_t currentmod = 0;
static bool hasmd(int idx) {if (idx > 7 || idx < 0) return false; return currentmod & (1 << idx);}
static void toggleMd(int idx) {if (idx > 7 || idx < 0) return; currentmod ^= (1 << idx);}
static void turnoffMd(int idx) {if (idx > 7 || idx < 0) return; currentmod &= ~(1 << idx);}

static Vector<Token> lex(char* src) {
    Vector<Token> stream;
    String source(src);

    Vector<String> result = source.split(' ');

    for (int i = 0; i < result.getLength(); i++) {
        String current = result.getAt(i);
        if (!hasmd(0) && current.startsWith('"')) {toggleMd(0); toggleMd(1);}
        if (hasmd(0) && current.endsWith('"')) turnoffMd(0);

        if (hasmd(0)) {

        }
        turnoffMd(1);
    }

    return stream;
}

void shmain(char* src) {
    Vector<Token> lxd = lex(src);
}