#include "utilities.h"

int dif(const std::string& s, int max) {
	size_t len{ 0 };
	for (int i{ 0 }; i < s.size(); ++i) {
		len += 1;
		if ((s[i] == 'Ã' || s[i] == 'Å' || s[i] == '€' || s[i] == 'â' || s[i] == 'Ä') && i < max) len -= 1;
	}
	return s.size() - len;
}

int dif(const std::string& s) {
	size_t len{ 0 };
	for (int i{ 0 }; i < s.size(); ++i) {
		len += 1;
		if (s[i] == 'Ã' || s[i] == 'Å' || s[i] == '€' || s[i] == 'â' || s[i] == 'Ä') len -= 1;
	}
	return s.size() - len;
}