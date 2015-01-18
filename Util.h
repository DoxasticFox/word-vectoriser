#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>
#include <vector>

namespace Util {
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &s, char delim);
	std::string separator();
}

#endif
