#include "utilities.h"

int dif(const std::string& s, int max) {
	size_t len{ 0 };
	for (int i{ 0 }; i < s.size(); ++i) {
		len += 1;
		if ((s[i] == '�' || s[i] == '�' || s[i] == '�' || s[i] == '�' || s[i] == '�') && i < max) len -= 1;
	}
	return s.size() - len;
}

int dif(const std::string& s) {
	size_t len{ 0 };
	for (int i{ 0 }; i < s.size(); ++i) {
		len += 1;
		if (s[i] == '�' || s[i] == '�' || s[i] == '�' || s[i] == '�' || s[i] == '�') len -= 1;
	}
	return s.size() - len;
}