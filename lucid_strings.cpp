/*
   lucid_strings.cpp
   
   String Functions
   
   by Austin Cathey
*/

// Includes
#include <string>
#include <cstring>
#include <cctype>
#include "lucid_strings.hpp"

// Use standard namespace
using namespace std;

// string_ltrim ///////////////////////////////////////////
string &string_ltrim(string &s, char c)
{ /*
   This function removes all leading occurrances of c from s.
*/
   size_t index;
   
   // Find the first occurance in s that is not c
   if ((index = s.find_first_not_of(c)) == string::npos) {
      
      // If it doesn't exist clear the string.
      s.clear();
   }
   
   // Trim the characters
   else {
      s.erase(0, index);
   }
   
   // Return a reference to the string
   return s;
}

// string_rtrim ///////////////////////////////////////////
string &string_rtrim(string &s, char c)
{ // Trims all trailing occurrances of c from s
   size_t index;
   
   // Find the last occurrance in s that is not c
   if ((index = s.find_last_not_of(c)) == string::npos) {
      
      // If it doesn't exist, make the string empty.
      s.clear();
   }
   
   // Remove the trailing spaces.
   else {
      s.erase(index + 1);
   }
   
   // Return a reference to the string.
   return s;
}

// string_trim ////////////////////////////////////////////
string &string_trim(string &s, char c)
{ // Trims all leading and trailing occurrances of c from s
   return string_ltrim(string_rtrim(s, c), c);
}

// string_ltrim_dup ///////////////////////////////////////
string string_ltrim_dup(string s, char c)
{ /*
   This function removes the leading occurances of c from
   string s.
   
   Algorithm:
      * Search for the first occurance that is not c in s
      * If there is no such occurance, return an empty string.
      * Return the substring past that index.
*/
   return string_ltrim(s, c);
}

// string_rtrim_dup ///////////////////////////////////////
string string_rtrim_dup(string s, char c)
{ /*
   This function removes the trailing occurances of c from
   string s.
   
   Algorithm:
      * Search for the last occurance that is not c in s
      * If there is no such occurance, return an empty string.
      * Return the substring before that index.
*/
   return string_rtrim(s, c);
}

// string_trim_dup ////////////////////////////////////////
string string_trim_dup(string s, char c)
{ /*
   This function trims the leading and trailing occurances of c
   from the string s.
   
   Algorithm:
      * Trim both sides of character c.
*/
   return string_trim(s, c);
}

// get_trim_line //////////////////////////////////////////
bool get_trim_line(istream &ins, string &s, char c)
{ /*
  Gets the next line from the input stream and trims
  the leading and trailing occurrances of c

  Algorithm:
     * Get the line from the input stream
	 * Trim the leading and trailing occurrances of c
	 * Return false on an empty string, true otherwise.
*/

	// Get the line
	getline(ins, s);

	// Return false if the string was empty and true otherwise
	return !(string_trim(s, c)).empty();
}

// i_strcmp ///////////////////////////////////////////////
int i_strcmp(string s1, string s2)
{ // This function performs a case-insensitive comparison of two strings.
   size_t index, len, len1, len2;
   int result;
   
   // Get the lengths of both strings
   len1 = s1.length();
   len2 = s2.length();
   
   // Get the length of the shorter string.
   len = (len1 < len2) ? (len1) : (len2);
   //if (len1 < len2)
	  // len = len1;
   //else
	  // len = len2;
   
   // Go through the string
   for (index = 0; index < len; index++) {
      
      // Convert the character to lowercase and subtract.
      result = tolower(s1[index]) - tolower(s2[index]);
      
      // If they are not equal, the result will not be zero.
      if (result) {
         return result; // Return the result of the subtraction.
      }
   }
   
   // We got here if the strings are equal thus far.
   // In this case, the longer string is the greater one.
   if (len1 > len2) {
      return tolower(s1[index]);    // Return > 0 on s1 > s2
   }
   else if (len1 < len2) {
      return -tolower(s2[index]);   // Return < 0 on s1 < s2
   }
   
   // We got here if the strings are equal.
   return 0;                        // Return 0 on s1 == s2
}
