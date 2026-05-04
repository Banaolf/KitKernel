#include "../../include/kstring.h"
#include "../../include/vector.h"
#include <stdint.h>
#include <stdarg.h>

int Strlen(const char *str) {
	int i = 0;
	while (str[i] != '\0') {
		i++;
	}
	return i;
}
String Strcpy(const String& toCopy) {
	String result;
	for (int i = 0; i < toCopy.length(); i++) {
		result.append(toCopy.get(i));
	}
	return result;
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
	buffer = (char*)kmalloc(1);
	if (buffer) {
		buffer[0] = '\0';
	}
}
String::String(const char* str) {
	if (!str) {
		len = 0;
		buffer = (char*)kmalloc(1);
		buffer[0] = '\0';
		return;
	}

	len = Strlen(str);
	// Allocate space for characters + null terminator
	buffer = (char*)kmalloc(len + 1);

	if (buffer) {
		// Use your new kmcopy!
		kmcopy((void*)str, (void*)buffer, len);
		buffer[len] = '\0';
	}
}
String::String(const String& other) {
	this->len = other.len;
	this->buffer = (char*)kmalloc(this->len+1);

	if (this->buffer && other.buffer) {
		kmcopy((void*)other.buffer, (void*)buffer, this->len+1);
	}
}
String::~String() {
	if (buffer) {
		kfree(buffer);
		buffer = nullptr;
	}
	len = 0;
}
String& String::operator=(const String& other) {
	if (this == &other) return *this;

	if (this->buffer) {
		kfree(this->buffer);
	}

	this->len = other.len;
	this->buffer = (char*)kmalloc(len + 1);
	
	if (this->buffer && other.buffer) {
		kmcopy((void*)other.buffer, (void*)this->buffer, len + 1);
	}

	return *this;
}
String String::operator+(const String& other) {
	String s = Strcpy(*this);
	s.append(other);
	return s;
}
void String::operator+=(const String& other) {this->append(other);}
bool String::operator==(const String& other) {return this->equals(other);}
bool String::operator==(const int& other) {return this->length() == other;}
bool String::operator!=(const String& other) {return !this->equals(other);}
bool String::operator!=(const int& other) {return this->length() != other;}
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
bool String::find(const char* sub) const {
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
bool String::startsWith(const char __c) const {
	return this->buffer[0] == __c;
}
bool String::endsWith(const char __c) const {
	return  this->buffer[this->len-1] == __c;
}
void String::append(const char c) {
	char* new_buffer = (char*)krealloc(buffer, len + 2);
	
	if (new_buffer) {
		buffer = new_buffer;
		buffer[len] = c;
		buffer[len + 1] = '\0';
		len++;
	}
}
void String::append(const char* str) {
	int len = Strlen(str);
	for (int i = 0; i < len; i++)
		append(str[i]);
}
void String::append(const String& str) {
	int len = Strlen(str.cstr());
	for (int i = 0; i < len; i++)
		this->append(str.get(i));
}
void String::appendAt(const char c, int place) {
	if (place < 0 || place > len) return;

	char* new_buffer = (char*)krealloc(this->buffer, len + 2);
	if (!new_buffer) return;
	this->buffer = new_buffer;

	for (int i = len; i >= place; i--) {
		this->buffer[i + 1] = this->buffer[i];
	}

	this->buffer[place] = c;
	len++;
}
void String::appendAt(const char* str, int place) {
	if (!str || place < 0 || place > len) return;
	
	size_t insert_len = Strlen(str);
	char* new_buffer = (char*)krealloc(this->buffer, len + insert_len + 1);
	if (!new_buffer) return;
	this->buffer = new_buffer;

	for (int i = len; i >= place; i--) {
		this->buffer[i + insert_len] = this->buffer[i];
	}

	kmcopy((void*)str, (void*)(this->buffer + place), insert_len);
	
	len += insert_len;
}
void String::appendAt(const String& str, int place) {
	if (place < 0 || place > len) return;
	
	size_t insert_len = Strlen(str.cstr());
	char* new_buffer = (char*)krealloc(this->buffer, len + insert_len + 1);
	if (!new_buffer) return;
	this->buffer = new_buffer;

	for (int i = len; i >= place; i--) {
		this->buffer[i + insert_len] = this->buffer[i];
	}

	kmcopy((void*)&str, (void*)(this->buffer + place), insert_len);
	
	len += insert_len;
}
void String::clear() {
	len = 0;
	
	char* new_buffer = (char*)krealloc(this->buffer, 1);
	if (new_buffer) {
		this->buffer = new_buffer;
	}
	this->buffer[0] = '\0';
}
void String::backspace() {
	if (len > 0) {
		len -= 1;
		char* new_buffer = (char*)krealloc(this->buffer, len + 1);
		if (new_buffer) {
			this->buffer = new_buffer;
		}
		this->buffer[len] = '\0';
	}
}
void String::backspace(const int times) {
	if (times <= 0) return;
	
	if (times >= len) {
		this->clear();
		return;
	}

	len -= times;
	char* new_buffer = (char*)krealloc(this->buffer, len + 1);
	if (new_buffer) {
		this->buffer = new_buffer;
	}
	this->buffer[len] = '\0';
}
String String::substr(int i1, int i2) const {
	if (i1 < 0 || i1 >= this->len) i1 = 0;
	if (i2 < 2 || i2 >= this->len) i2 = this->len-1;
	String rslt;
	for (int idx = i1; idx <= i2; idx++) {
		rslt.append(this->buffer[idx]);
	}
	return rslt;
}
Vector<String> String::split(char delimiter) const {
    Vector<String> result;
    String current;

    for (int i = 0; i < this->len; i++) {
        if (this->buffer[i] == delimiter) {
            if (current.length() > 0) {
                result.push(current);
                current = String("");
            }
        } else {
            current.append(this->buffer[i]);
        }
    }

    // Don't forget the last part!
    if (current.length() > 0) {
        result.push(current);
    }

    return result;
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
			case 'c': {
				int arg = va_arg(list, int);
				result.append((char)arg);
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