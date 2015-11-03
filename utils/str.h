#pragma once
/*
 * str.h
 *
 *  Created on: Nov 1, 2015
 *      Author: Superjom
 *      Email: yanchunwei@outlook.com
 */
#include "common.h"
namespace cHasky {


std::string& trim(std::string &ss);
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
std::vector<std::string> split(const std::string &s, const std::string &delim);

template<typename... ARGS>
void format_string(std::string& s, const char* format, ARGS... args) {
    // get target size
    int len = snprintf(NULL, 0, format, args...);
    CHECK(len >= 0);
    size_t oldlen = s.length();
    s.resize(oldlen + len + 1);
    CHECK(snprintf(&s[oldlen], (size_t)len+1, format, args...) == len);
    s.resize(oldlen + len);
}

template<typename... ARGS>
std::string format_string(const char* format, ARGS... args) {
    std::string tmp_s;
    int len = snprintf(NULL, 0, format, args...);
    CHECK(len >= 0);
    tmp_s.resize(len + 1);
    CHECK(snprintf(&tmp_s[0], (size_t)len+1, format, args...) == len);
    tmp_s.resize(len);
    return std::move(tmp_s);
}



};  // end namespace cHasky
