#include "tgstring.hpp"

const char* find_replace(char* source, char* tofind, char* replace_with) {
	size_t len = std::strlen(source);
	size_t find_len = std::strlen(tofind);
	std::string str = std::string();
	size_t last = 0;
	for (size_t i = 0; i <= len - find_len; i++)
	{
		if (std::strncmp(&source[i], tofind, find_len) == 0) {
			str.append(&source[last], i - last);
			str.append(replace_with);
			last = i + 1;
		}
	}
	str.append(&source[last], len - last);
	char* buf = new char[str.size() + 1];
	str.copy(buf, str.size());
	buf[str.size()] = '\0';
	return buf;
}