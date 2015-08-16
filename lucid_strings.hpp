/*
   lucid_strings.hpp
   
   Declarations for my string functions
   
   by Austin Cathey
*/

// Make sure this file hasn't already been included
#ifndef __LUCID_STRINGS_HPP__
#define __LUCID_STRINGS_HPP__

// Includes
#include <string>
#include <iostream>

// Use standard namespace
using namespace std;

// Prototypes
string &string_ltrim(string &s, char c = ' ');
string &string_rtrim(string &s, char c = ' ');
string &string_trim(string &s, char c = ' ');
string string_ltrim_dup(string s, char c = ' ');
string string_rtrim_dup(string s, char c = ' ');
string string_trim_dup(string s, char c = ' ');
bool get_trim_line(istream &ins, string &s, char c = ' ');
int i_strcmp(string s1, string s2);

#endif   // __LUCID_STRINGS_HPP__
