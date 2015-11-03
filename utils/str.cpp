#include "str.h"
using namespace cHasky;

std::string& trim(std::string &ss) {
    if (ss.empty()) return ss;
    ss.erase(0, ss.find_first_not_of(" \t\n\r"));
    ss.erase(ss.find_last_not_of(" \t\n\r") + 1);
    return ss;
}


/**
 * split string to vector
 *
 * example:
 *
 *    vector<string> cols = split("hello world@bb", " @");
 *
 * Attention: 
 *
 *    use first_not_of
 *    the real delim is either char in a string
 */
std::vector<std::string> split(const std::string &s, const std::string &delim) {
    std::vector<std::string> cols;
    size_t last = 0;
    size_t start = s.find_first_not_of(delim, last);
    while (start != std::string::npos) {
        last = s.find_first_of(delim, start);
        if(last == std::string::npos) {
            cols.push_back(s.substr(start));
        } else {
            cols.push_back(s.substr(start, last - start));
        }
        start = s.find_first_not_of(delim, last);
    }
    return std::move(cols);
}

