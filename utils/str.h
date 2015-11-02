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
void format_string(std::string& s, const char* format, ARGS... args);

template<typename... ARGS>
std::string format_string(const char* format, ARGS... args);



};  // end namespace cHasky
