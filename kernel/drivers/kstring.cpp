#include "../../include/kstring.h"
#include "../../include/heap.h"
#include <stdint.h>
#include <stdarg.h>

int Strlen(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

bool checks::isUppercase(const char c) {
    for (int i = 0; i < Strlen(uppercase); i++) {
        if (uppercase[i] == c) return true;
    }
    return false;
}
bool checks::isLowercase(const char c) {
    for (int i = 0; i < Strlen(lowercase); i++) {
        if (lowercase[i] == c) return true;
    }
    return false;
}
bool checks::isOperator(const char c) {
    for (int i = 0; i < Strlen(operators); i++) {
        if (operators[i] == c) return true;
    }
    return false;
}
bool checks::isDigit(const char c) {
    for (int i = 0; i < Strlen(numbers); i++) {
        if (numbers[i] == c) return true;
    }
    return false;
}
bool checks::isSpecial(const char c) {
    if (checks::isDigit(c) or checks::isOperator(c) or checks::isLowercase(c) or checks::isUppercase(c)) return false;
    return true;
}
bool checks::isAlpha(const char c) {
    if (checks::isUppercase(c) || checks::isLowercase(c)) return true;
    return false;
}
bool checks::isAlphaNumerical(const char c) {
    if (checks::isUppercase(c) || checks::isLowercase(c) || checks::isDigit(c)) return true;
    return false;
}
//////////////////////////////////////////////////string
bool checks::isUppercase(const char* str) {
    for (int i = 0; i < Strlen(uppercase); i++) {
        if (uppercase[i] == str[i]) continue;
        else return false;
    }
    return true;
}
bool checks::isLowercase(const char* str) {
    for (int i = 0; i < Strlen(lowercase); i++) {
        if (lowercase[i] == str[i]) continue;
        else return false;
    }
    return true;
}
bool checks::isOperator(const char* str) {
    for (int i = 0; i < Strlen(operators); i++) {
        if (operators[i] == str[i]) continue;
        else return false;
    }
    return true;
}
bool checks::isDigit(const char* str) {
    for (int i = 0; i < Strlen(numbers); i++) {
        if (numbers[i] == str[i]) continue;
    }
    return false;
}
bool checks::isSpecial(const char* str) {
    if (checks::isDigit(str) or checks::isOperator(str) or checks::isLowercase(str) or checks::isUppercase(str)) return false;
    return true;
}
bool checks::isAlpha(const char* str) {
    if (checks::isUppercase(str) || checks::isLowercase(str)) return true;
    return false;
}
bool checks::isAlphaNumerical(const char* str) {
    if (checks::isUppercase(str) || checks::isLowercase(str) || checks::isDigit(str)) return true;
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool character::in(const char* str) const {
    for (int i = 0; i < Strlen(str); i++) {
        if (str[i] == this->c) return true;
    }
    return false;
}
bool character::in(const char* str, const int starterIndex) const {
    for (int i = starterIndex; i < Strlen(str); i++) {
        if (str[i] == this->c) return true;
    }
    return false;
}
int character::inCount(const char* str) const {
    int i = 0;
    int count = 0;
    while (i < Strlen(str)) {
        if (str[i] == this->c) count++;
        i++;
    }
    return count;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
String::String() : len(0) {
    buffer[0] = '\0';
}
String::String(const char* str) {
    len = Strlen(str);
    for (int i = 0; i < len; i++) {
        buffer[i] = str[i];
    }
    buffer[len] = '\0';
}
const char* String::cstr() const {
    return this->buffer;
}
bool String::equals(const char* str) const {
    if (Strlen(str) != this->length()) return false;
    for (int i = 0; i < Strlen(str); i++)  {
        if (!(str[i] == this->buffer[i])) return false;
    }
    return true;
}
bool String::equals(const String& str) const {
    return String::equals(str.cstr());
}
bool String::substr(const char* sub) const {
    int subLen = Strlen(sub);
    int len = Strlen(buffer);
    
    if (subLen > len) return false;
    
    for (int i = 0; i <= len - subLen; i++) {
        bool match = true;
        for (int j = 0; j < subLen; j++) {
            if (buffer[i + j] != sub[j]) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}
void String::append(const char c) {
    if (len < 255) {
        buffer[len] = c;
        len++;
        buffer[len] = '\0';
    }
}
void String::append(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        append(str[i]);
        i++;
    }
}
void String::appendAt(const char c, int place) {
    if (place > Strlen(this->buffer)) return;
    if (place == len) this->append(c);
    if (len < 255) {
        len++;
        int i = place;
        while (i <= Strlen(this->buffer)) {
            this->buffer[i+1] = this->buffer[i];
            i++;
        }
        this->buffer[place] = c;
    }
}
void String::appendAt(const char* str, int place) {
    for (int i = 0; i < Strlen(str); i++) {
        this->appendAt(str[i], place);
        place++;
    }
}
void String::clear() {
    len = 0;
    buffer[0] = '\0';
}
void String::backspace() {
    if (len > 0) {
        len -= 1;
        buffer[len] = '\0';
    }
}
void String::backspace(const int times) {
    if (len - times > 0) {
        len -= times;
        buffer[len] = '\0';
    }
}
String* String::split(const char __splitwith) const {
    if (!has_heap_initted) return nullptr;

    //String* spt = kmalloc
}
bool String::find(const char *__tofind) const {
    
}
String intostr(long long n) {
    if (n == 0) return String("0");

    char buffer[22];
    int i = 21;
    buffer[21] = '\0';

    bool negative = n < 0;
    // Handle INT_MIN edge case: -9223372036854775808 can't be negated
    if (negative) n = -n;

    while (n > 0) {
        buffer[--i] = '0' + (n % 10);
        n /= 10;
    }

    if (negative) buffer[--i] = '-';

    return String(buffer + i);
}
/*String ftostr(double f) {
    if (f != f) return String("NaN");
    if (f == 0.0) return String("0.0");

    char buffer[64];
    int i = 0;
    bool negative = f < 0.0;
    if (negative) { buffer[i++] = '-'; f = -f; }

    long long intPart = (long long)f;
    double fracPart = f - (double)intPart;

    char intBuf[22];
    int j = 21;
    intBuf[21] = '\0';
    if (intPart == 0) { intBuf[--j] = '0'; }
    else { while (intPart > 0) { intBuf[--j] = '0' + (intPart % 10); intPart /= 10; } }
    while (intBuf[j] != '\0') buffer[i++] = intBuf[j++];

    buffer[i++] = '.';

    for (int d = 0; d < 15; d++) {
        fracPart *= 10.0;
        int digit = (int)fracPart;
        buffer[i++] = '0' + digit;
        fracPart -= (double)digit;
    }

    buffer[i] = '\0';
    return String(buffer);
}*/
char* uint_to_str(uint64_t value, char* buffer) {
    char* ptr = buffer + 20;
    *ptr = '\0';

    if (value == 0) {
        *--ptr = '0';
        return ptr;
    }

    while (value > 0) {
        *--ptr = (value % 10) + '0';
        value /= 10;
    }

    return ptr;
}
char* uint_to_hex(uint64_t value, char* buffer) {
    char* ptr = buffer + 18; // "0x" + 16 chars + \0
    *ptr = '\0';
    const char* hex_chars = "0123456789ABCDEF";
    for(int i = 0; i < 16; i++) {
        *--ptr = hex_chars[value & 0xF];
        value >>= 4;
    }
    *--ptr = 'x'; *--ptr = '0';
    return ptr;
}
String fmtString(const char* fmt, ...) {
    String result;
    va_list list;
    va_start(list, fmt);
    int i = 0;
    int stringLength = Strlen(fmt);
    while (i < stringLength) {
        while (i < stringLength && fmt[i] != '%') {
            result.append(fmt[i]);
            i++;
        }
        if (i >= stringLength) { va_end(list); return result; }
        i++;
        switch (fmt[i]) {
            case 's': {
                char* arg = va_arg(list, char*);
                int j = 0;
                while (j < Strlen(arg)) {
                    result.append(arg[j]);
                    j++;
                }
                break;
            }
            case 'i': { //my format my rules (integer)
                int arg = va_arg(list, int);
                const char* argstr = intostr(arg).cstr();
                int j = 0;
                while (j < Strlen(argstr)) {
                    result.append(argstr[j]);
                    j++;
                }
                break;
            }
            case 'u': {
                uint64_t arg = va_arg(list, uint64_t);
                char buf[21];
                char* s = uint_to_str(arg, buf);
                while (*s) {
                    result.append(*s++);
                }
                break;
            }
            case 'x': {
                uint64_t arg = va_arg(list, uint64_t);
                char buf[19];
                char* s = uint_to_hex(arg, buf);
                
                while (*s) {
                    result.append(*s++);
                }
                break;
            }
            /*case 'f': {
                float arg = (float)va_arg(list, float);
                const char* argstr = ftostr(arg).cstr();
                int j = 0;
                while (j < Strlen(argstr)) {
                    result.append(argstr[j]);
                    j++;
                }
                break;
            }*/
            default: break;
        }
        i++;
    }
    va_end(list);
    return result;
}
int strToInt(const String& s) {
    const char* str = s.cstr();
    int result = 0;
    int i = 0;
    bool negative = false;

    if (str[i] == '-') { negative = true; i++; }

    while (str[i] != '\0') {
        if (!checks::isDigit(str[i])) break;
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return negative ? -result : result;
}
/*void strToFloat(const String& s, float& out) {
    const char* str = s.cstr();
    float result = 0.0f;
    float decimal = 0.1f;
    int i = 0;
    bool negative = false;
    bool pastDot = false;

    if (str[i] == '-') { negative = true; i++; }

    while (str[i] != '\0') {
        if (str[i] == '.') { pastDot = true; i++; continue; }
        if (!checks::isDigit(str[i])) break;
        if (!pastDot) {
            result = result * 10.0f + (str[i] - '0');
        } else {
            result += (str[i] - '0') * decimal;
            decimal *= 0.1f;
        }
        i++;
    }

    out = negative ? -result : result;
}*/