#include "../../include/shell.h"
#include "../../include/kstring.h"
#include "../../include/out.h"
#include "../../include/shutdown.h"
#include "../../include/vector.h"


typedef unsigned char uint8_t;

static Vector<Token> lex(char* src) {
    Vector<Token> stream;
    String source(src);

    Vector<String> result = source.split(' '); //No string support yet

    for (int i = 0; i < result.getLength(); i++) {
        String current = result.getAt(i);
        if (current.startsWith('-')) {
            stream.push(Token(TokenType::flag, current));
        } else if (isKeyword<String>(current)) {
            stream.push(Token(TokenType::keyword, current));
        } else {
            stream.push(Token(TokenType::word, current));
        }
    }
    String eoc = String("\xEC");
    stream.push(Token(TokenType::eoc, eoc));

    return stream;
}

#define kwat(i) keywords[i]
#define lxdat lxd.getAt(i)
void shmain(char* src) {
    Vector<Token> lxd = lex(src);
    
    for (int i = 0; i < lxd.getLength(); i++) {
        if (lxdat.istype(TokenType::keyword)) {
            i++;
            if (lxdat.istype(TokenType::keyword)) {kprint("Cannot have more than two commands."); break;}
            i--;
            if (lxdat.isstring(kwat(0))) {
                i++;
                if (!lxdat.istype(TokenType::flag)) {kprint("Expected one flag."); break;}
                if (lxdat.)
            }
        }
        else if (lxdat.istype(TokenType::eoc)) {continue;}
        else {kprint("Expected command."); break;}
    }
    kprint_char('\n');
}