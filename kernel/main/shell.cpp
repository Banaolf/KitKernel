#include "../../include/shell.h"
#include "../../include/kstring.h"
#include "../../include/out.h"
#include "../../include/shutdown.h"
#include "../../include/vector.h"

static Vector<Token> lex(const char* src) {
    Vector<Token> stream;
    String source(src);

    Vector<String> result = source.split(' '); 

    for (int i = 0; i < result.getLength(); i++) {
        String* current = result.getAt(i);
        
        if (current == nullptr || current->length() == 0) continue;

        if (current->startsWith('-')) {
            stream.push(Token(TokenType::flag, *current));
        } else if (isKeyword<String>(*current)) {
            stream.push(Token(TokenType::keyword, *current));
        } else {
            stream.push(Token(TokenType::word, *current));
        }
    }
    result.push("\xEC");
    String* tmp = nullptr; result.pop(tmp);
    stream.push(Token(TokenType::eoc, *tmp));
    return stream;
}

void shmain(const char* src) {
    Vector<Token> lxd = lex(src);
    int i = 0;

    while (i < lxd.getLength()) {
        Token* tok = lxd.getAt(i);

        if (tok == nullptr || tok->istype(TokenType::eoc)) {
            break;
        }

        if (tok->istype(TokenType::keyword)) {
            if (tok->isstring(keywords[0])) {
                i++;
                Token* flagTok = lxd.getAt(i);
                
                if (flagTok != nullptr && flagTok->istype(TokenType::flag)) {
                    if (flagTok->isflag('o')) shutdown();
                    else if (flagTok->isflag('r')) restart();
                    else if (flagTok->isflag('s')) legacy_tofirmware();
                    else kprint("Unknown flag.\n");
                } else {
                    kprint("Error: 'power' requires a flag (-o, -r, -s).\n");
                }
            } 
            else if (tok->isstring(keywords[1])) {
                scroll();
            }
        } 
        else {
            kprint("Expected command.\n");
            break;
        }
        
        i++;
    }
    kprint_char('\n');
}