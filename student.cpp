/*
   Student.cpp
   
   Source file for utility functions for Student structure
   
   by Austin Cathey
   
   -----
   
   Computer Science 1
   Final Project
   Due Date: 12/12/2007
   
   Algorithms for each function will be beside their definitions.
*/

// Includes
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <conio.h>
#include "student.hpp"
#include "lucid_strings.hpp"

// Use standard namespace
using namespace std;

/*
   The LETTER_GRADES constant
   
   The method behind this madness is that the function
   I will create that will return the student's grade,
   will actually be an integer that can be used to index
   this array.
   
   U represents unknown.
*/
#define LETTER_GRADES_SIZE ((int)(6))
const char LETTER_GRADES[] = {'A', 'B', 'C', 'D', 'F', 'U'};

// The type for a compare function (for easy typecasting)
typedef int (*compare_func)(const void *p1, const void *p2);

// Static constants
static const int NEEDED_TEST_SCORES = MAX_TESTS - 1;  // We need all of the test scores except one.

// Static prototypes
// Afterall, the main source file doesn't need to know EVERYTHING...
static void Student_highest_scores(const Student &s, double *highest_grades);
static int Student_name_compare(const Student *s1, const Student *s2);
static int Student_id_compare(const Student *s1, const Student *s2);
static int Student_average_compare(const Student *s1, const Student *s2);
static bool __read_score__(istream &ins, double &score);

/////////////////////////////////
//                             //
// Static Function Definitions //
//                             //
/////////////////////////////////

// Student_highest_scores /////////////////////////////////
static void Student_highest_scores(const Student &s, double *highest_scores)
{ /*
   This function stores the student's highest four test grades into
   the highest_scores array.
   
   With the five test scores, there are numbers score1, score2, score3,
   score4, and score5, such that the following holds true.
   
      score1 >= score2 >= score3 >= score4 >= score5
      
   Considering this fact, let numbers s1, s2, s3, s4, and s5 be the
   student's test scores, where each of these numbers is equal to one
   of the previously defined numbers.  Also assume we do not know exactly
   which of those numbers s1 through s5 is equal to.
   
   Let us assume that s1 through s4 are the highest test scores.  Since
   we don't know exactly what s1 through s5 are equal to, this may or may
   not be true.  If we take the first 4 test scores, then the following is
   true about the fifth score:
      
      The fifth score is either greater than at least ONE test score,
      or less than or equal to the other four test scores.
      
   My algorithm then becomes this:
      
      * Store the first 4 test scores in the array.
      * Find the element of the lowest test score in this array.
      * Compare this test score with the fifth score.
      * If the fifth score is less than the score we are comparing
        against, then the fifth score is the lowest test score, and
        we therefore do not need it.  Otherwise, treat it as the
        fourth highest test score.
*/

   // Variables
   int index, low_index;
   double fifth_score;
   
   // Get the last test score (MAX_TESTS - 1 is the last element)
   fifth_score = s.test_scores[NEEDED_TEST_SCORES];
   
   // Copy the first 4 test scores into the array
   memcpy(highest_scores, s.test_scores, NEEDED_TEST_SCORES * sizeof(double));
   
   // Search for the lowest score in the list of assumed low scores
   for (index = low_index = 0; index < NEEDED_TEST_SCORES; index++) {
      
      // If the current score is less than what we think
      // the lowest score is, set the lowest score element
      // to the current element.
      if (highest_scores[index] < highest_scores[low_index]) {
         low_index = index;
      }
   }
   
   // Compare the fifth score with the lowest score currently
   // in the array.  If it is greater than the lowest score
   // in the array, then the fifth score is actually the fourth
   // lowest score, and that change needs to be reflected
   // accordingly.
   if (fifth_score > highest_scores[low_index]) {
      highest_scores[low_index] = fifth_score;
   }
}

//////////////////////////////////
//                              //
// Sorting/Comparison functions //
//                              //
//////////////////////////////////

// Student_name_compare ///////////////////////////////////
static int Student_name_compare(const Student *s1, const Student *s2)
{ /*
   A comparison function that compares the names of two students
   
   To be passed to qsort()
   
   This does a case-insensitive compare.
   
   Returns:
        0 if name1 == name2
      > 0 if name1 > name2
      < 0 if name1 < name2
      
   For this function, I'm going to convert the C++ strings
   to the equivalent C null-terminated strings, due to the
   fact that the standard C string library has routines for
   converting strings to lowercase.  I will make copies of
   the C++ strings to avoid tampering with the originals.
   
   Algorithm:
      
      * Do a case-insensitive comparison for all comparisons
      * Compare the last names (the comparison will return zero if they are equal)
      * If the result is not zero, return the result.
      * Compare the first names and return the result of that comparison.
*/
   int result;
   
   // Compare the last names
   result = i_strcmp(s1->last_name, s2->last_name);
   
   // If the last names aren't equal,
   if (result) {
      return result; // then return the result of the comparison.
   }
      
   // Compare the first names and return the result of the comparison.
   return i_strcmp(s1->first_name, s2->first_name);
}

// Student_id_compare /////////////////////////////////////
static int Student_id_compare(const Student *s1, const Student *s2)
{ /*
   A comparison function that compares the IDs of two students
   
   To be passed to qsort()
   
   Returns:
        0 if id1 == id2
      > 0 if id1 > id2
      < 0 if id1 < id2
      
   Algorithm:
      
      * Copy both strings so the originals are untampered
      * Ensure both ID strings follow the 123-45-6789 format.
      * Compare both strings, adhering to the preceding return values.
*/
   string id1, id2;
   
   // Get the respective IDs
   id1 = s1->id;
   id2 = s2->id;
   
   // Ensure that it follows the 123-45-6789 format
   id1[3] = id1[6] = '-';
   id2[3] = id2[6] = '-';
   
   // Return greater than, less than, or equality.
   return id1.compare(id2);
}

// Student_average_compare ////////////////////////////////
static int Student_average_compare(const Student *s1, const Student *s2)
{ /*
   A comparison function that compares the averages of two students
   
   Returns:
        0 if avg1 == avg2
      > 0 if avg1 > avg2
      < 0 if avg1 < avg2
      
   Invalid averages are treated as less than valid averages.
   
   Algorithm:
      * Multiply both averages by 100, due to the fact that we're only dealing
        with real numbers between 0 and 100.  The -1 in the error is okay.
      * Return the result of the subtraction.  This will make the return value
        adhere to the preceding information.
*/
   return (int)(Student_get_average(*s1) * 100.0) - (int)(Student_get_average(*s2) * 100.0);
}

/*
// bubble_sort ////////////////////////////////////////////
void bubble_sort(void *buffer, size_t num_elements, size_t element_size, bubble_sort_compare compare)
{ // A generic bubble sort function (not needed, but written just for kicks)
   void *temp;
   bool switched;
   int result;
   size_t i, j, addr1, addr2;
   
   // Allocate temporary space
   temp = (void *)malloc(element_size);
   if (!temp) {
      return;
   }
   
   // Initialize variables
   j = num_elements - 1;
   
   // Go through every element
   do {
      switched = false;
      
      // Go through the array
      for (i = 0; i < j; i++) {
         
         // Get the address of the current element
         addr1 = (size_t)buffer + (i * element_size);
         
         // Get the address of the next element
         addr2 = addr1 + element_size;
         
         // Compare the two elements
         result = compare((void *)addr1, (void *)addr2);
         
         // If the current element is greater than the next element,
         if (result > 0) {
            
            // then swap them.
            memcpy(temp, (void *)addr1, element_size);
            memcpy((void *)addr1, (void *)addr2, element_size);
            memcpy((void *)addr2, temp, element_size);
            switched = true;
         }
      }
   } while (switched);
   
   // Free the allocated memory
   free(temp);
}
*/

// Student_sort_by_key ////////////////////////////////////
void Student_sort_by_key(Student *sa, int size, int key)
{ /*
   This function sorts the array of students by the given key.  If
   an invalid key was passed, then no sort is performed.  Since qsort()
   is a standard C function, I don't have to worry about providing my
   own sorting routine.  All I have to do is pass the proper comparison
   function and be done with it.
*/

   // Sort the array by the given key
   switch (key) {
      
      // By ID number
      case (SORT_KEY_ID) : {
         qsort(sa, size, sizeof(Student), (compare_func)Student_id_compare);
         break;
      }
      
      // By name
      case (SORT_KEY_NAME) : {
         qsort(sa, size, sizeof(Student), (compare_func)Student_name_compare);
         break;
      }
      
      // By average
      case (SORT_KEY_AVERAGE) : {
         qsort(sa, size, sizeof(Student), (compare_func)Student_average_compare);
         break;
      }
   }
}

//////////////////////////
//                      //
// Operator definitions //
//                      //
//////////////////////////

// conv_score /////////////////////////////////////////////
inline double conv_score(double score)
{ // Converts a score to a number in range
   if (score < 0.0) {            // Self-explanatory
      return 0.0;
   }
   else if (score > 100.0) {     // Self-explanatory
      return 100.0;
   }
   return score;                 // Self-explanatory
}

// __read_score__ /////////////////////////////////////////
static bool __read_score__(istream &ins, double &score)
{ /*
  This function reads a score from an input stream,
  and rounds it to the nearest hundredth.

  If the read was successful, we will return true, otherwise,
  return false.

  Algorithm:

    * Read the score
	 * Return false on a bad read
	 // * Convert the score to a value within range
	 * Round the score to the nearest hundredth
	 * Return true for success.
*/

	// Read the score from the stream
	ins >> score;
	if (ins.bad()) {
		return false;	// Return false for failure
	}
	
	// Convert the score to a value within bounds
	// score = conv_score(score);

	// Round the score to the nearest hundredth
	score = floor((score * 100.0) + 0.5) / 100.0;

	// Return true for success
	return true;
}

// operator >> ////////////////////////////////////////////
istream &operator >>(istream &ins, Student &s)
{ /*
   Here, I have redefined the >> operator so it can handle the Student type.
   
   Since I am dealing with multiple fields, I have to check the
   integrity of each read.
   
   Algorithm:
      
      * Note: If we are reading from cin, then display a prompt before each read.
      * Get the first name
      * Leave on bad read
      * Ensure that the first character is uppercase by converting it to
        uppercase
      * Get the last name
      * Leave on bad read
      * Ensure that the first character is uppercase by converting it to
        uppercase
      * Get the social security number (in 123-45-6789 format)
      * Leave on bad read
      * If the length is not exactly 11 characters, assume it is a bad read
      * Explicitly change the 4th and 7th characters to dashes so
        it follows the 123-45-6789 format
      * Check to see if the other characters are numbers.
         * If any are not numbers, then treat this as a bad read.
      * Get the quiz score
      * Leave on bad read
      * Get the number of tests taken and save it into another variable
	  * Leave on a bad read
      * Set the field in the record to this number if it is less than 5,
        otherwise, explicitly set it to five.
      * Read however many numbers in the other variable, from the file,
        and store it in their respective elements
        * Read the score
        * If the read was good
          * Store the number
        * Leave if anyting else went wrong
*/
   int index, num_tests;
   double score;
   
   // Display a prompt if we are reading from standard input
   if (&ins == &cin) {
      cout << "Enter first name: ";
   }

   // Read in the first name
   ins >> s.first_name;
   if (ins.bad()) {
      return ins;    // Exit on a bad read
   }
   
   // Ensure that the first letter is uppercase
   s.first_name[0] = toupper(s.first_name[0]);
   
   // Display a prompt if we are reading from standard input
   if (&ins == &cin) {
      cout << "Enter last name: ";
   }

   // Read in the last name
   ins >> s.last_name;
   if (ins.bad()) {
      return ins;    // Exit on a bad read
   }
   
   // Ensure that the first letter is uppercase
   s.last_name[0] = toupper(s.last_name[0]);
   
   // Display a prompt if we are reading from standard input
   if (&ins == &cin) {
      cout << "Enter social security number: ";
   }

   // Read in the ID number
   ins >> s.id;
   
   // Exit on a bad read
   if (ins.bad()) {
      return ins;
   }
   
   //
   // Ensure that the ID number follows the 123-45-6789 format
   //
   
   // We'll take care of the dashes explicitly.
   if (!__is_valid_id(s.id)) {
      
      // Explicitly set the badbit in the current stream
      // and return a reference to itself.
      //
      // This will cause ins.bad() to return true, since
      // if we got here, the last read technically wasn't
      // a bad read, but I want to treat it like it was.
      //
      ins.clear(ins.rdstate() | ifstream::badbit);
      return ins;
   }
   
   // Ensure these characters are dashes.
   s.id[3] = s.id[6] = '-';
   
   // Display a prompt if we are reading from standard input
   if (&ins == &cin) {
      cout << "Enter quiz score: ";
   }

   // Read in the quiz score
   if (!__read_score__(ins, s.quiz)) {
      return ins;    // Exit on a bad read
   }
   
   // Display a prompt if we are reading from standard input
   if (&ins == &cin) {
      cout << "Enter number of tests taken: ";
   }

   // Read in the number of tests taken
   ins >> num_tests;

   // Exit on a bad read
   if (ins.bad()) {
	   return ins;
   }
   
   // Ensure that the number of tests taken is never negative
   if (num_tests < 0) {
      num_tests = 0;
   }
   
   // Ensure that the student record stays within bounds,
   // While saving the number we actually read.
   s.num_tests = (num_tests > MAX_TESTS) ? MAX_TESTS : num_tests;
   
   // Leave if the last read was bad.
   if (ins.bad()) {
      return ins;
   }
   
   // Read in the test scores
   for (index = 0; index < num_tests; index++) {

      // Display a prompt if we are reading from standard input
      if (&ins == &cin) {
         cout << "Enter test score #" << (index + 1) << ": ";
      }
      
      // If reading the score went okay,
      if (__read_score__(ins, score)) {
         
         // Store it into the array if it's within bounds
         if (index < MAX_TESTS) {
            s.test_scores[index] = score;
         }
      }
      
      // Leave if anything else went wrong
      if (!ins) {
         return ins;
      }
   }

   // Return a reference to the stream
   return ins;
}

// operator << ////////////////////////////////////////////
ostream &operator <<(ostream &outs, Student &s)
{ /*
   Here, I am redefining the << operator to work with the Student type.

   The algorithm here is self explanatory, simply write each field to
   the specified output stream.
*/
   int index;
   
   // Check to see if this is going to cout or cerr
   if ((&outs == &cout) || (&outs == &cerr)) {
      outs << "First name:             " << s.first_name << endl;
      outs << "Last name:              " << s.last_name << endl;
      outs << "Social security number: " << s.id << endl;
      outs << "Quiz score:             " << s.quiz << endl;
      outs << "Number of tests taken:  " << s.num_tests << endl;
      for (index = 0; index < s.num_tests; index++) {
         outs << "   Test #" << (index + 1) << ":             " << s.test_scores[index] << endl;
      }
   }
   
   // Otherwise, simply write it to the file as is.
   else {
      outs << s.first_name << ' ' << s.last_name << ' ';
      outs << s.id << ' ';
      outs << s.quiz << ' ';  // Don't convert the scores.  Allow bad data to be written.
      outs << s.num_tests;
      for (index = 0; index < s.num_tests; index++) {
         outs << ' ' << s.test_scores[index];
      }
      outs << endl;
   }
   
   // Return a reference to itself
   return outs;
}

//////////////////////////
//                      //
// Function Definitions //
//                      //
//////////////////////////

// __is_valid_id //////////////////////////////////////////
bool __is_valid_id(string id)
{ /*
   Checks to see if the given ID is valid, disregarding
   the need for dashes at the 4th and 7th positions.
*/
   size_t index, len = id.length();

   // If the ID doesn't have 11 characters, then it's not valid.
   if (len != 11) {
      return false;
   }
   
   // Look through the ID number to make sure all
   // non-separators are numeric.
   for (index = 0; index < len; index++) {
      
      // These characters should be dashes.
      // Move to the next character.
      if ((index == 3) || (index == 6)) {
         continue;
      }
      
      // If the character is not a number, then
      // treat this like it was a bad read.
      if (!isdigit(id[index])) {
         return false;
      }
   }
   
   // We got here if all tests were passed.
   return true;
}

// is_valid_id ////////////////////////////////////////////
bool is_valid_id(string id)
{ /*
   Checks to see if the given ID is valid

   Algorithm:
      Do preliminary ID test
	  Check for dashes in 4th and 7th positions
*/

   // If the preceding tests failed, then return false.
   if (!__is_valid_id(id)) {
      return false;
   }
   
   // If the 4th and 7th characters are dashes, then we're in business.
   return (id[3] == '-') && (id[6] == '-');
}

// Student_init_array /////////////////////////////////////
void Student_init_array(Student *sa, int size)
{ /*
   This function initializes an array of students.
   
   By initialize, I mean setting all strings to empty strings,
   and setting all numeric values to zero.
   
   sa is the array of students
   size is the number of elements to initialize.
   
   All students from element 0 to element (size - 1) will be
   initialized.
*/
   int count, test_index;
   
   // Initialize the students
   for (count = 0; count < size; count++) {
      sa[count].first_name = "";
      sa[count].last_name = "";
      sa[count].id = "";
      sa[count].quiz = 0.0;
      sa[count].num_tests = 0;
      
      // Initialize the test scores
      for (test_index = 0; test_index < MAX_TESTS; test_index++) {
         sa[count].test_scores[test_index] = 0.0;
      }
   }
}

// Student_error_test_scores //////////////////////////////
bool Student_error_test_scores(const Student &s)
{ /*
  Returns true if any of the given scores are out of range

  Algorithm
     Loop through all students
	    If any score is bad, return false.
	 Return true if we make it outside of the loop.
*/

   // Iterate through each score
   for (int index = 0; index < MAX_TESTS; index++) {
      
      // Return true if it's a bad score
      if (Student_error_test_score(s, index)) {
         return true;
      }
   }
   
   // We got here if no scores were bad.
   return false;
}

// Student_test_average ///////////////////////////////////
double Student_test_average(const Student &s)
{ /*
   This function will get the student's test average.
   
   This takes into consideration that we only need the highest 4
   test scores.
   
   Note that as long as all test scores are within range, the
   test average will never be negative.  Therefore, this function
   will return a negative number in the event an error occurred.
   
   Algorithm:
      * If any scores are not within range, return -1.0
      * Get the highest four test scores.
      * Get the sum of them, and divide it by four.
*/
   double highest_scores[NEEDED_TEST_SCORES];
   double sum = 0.0;
   
   // Return a negative number if any score is not within range.
   if (Student_error_test_scores(s)) {
      return -1.0;
   }
   
   // Get the highest four scores
   Student_highest_scores(s, highest_scores);
   
   // Sum all four test scores
   for (int index = 0; index < NEEDED_TEST_SCORES; index++) {
      sum += highest_scores[index];
   }
   
   // Return the test average
   return sum / NEEDED_TEST_SCORES;
}

// Student_get_average ////////////////////////////////////
double Student_get_average(const Student &s)
{ /*
   This function returns the student's overall average.
   
   It takes into consideration that the quiz is 10% of their grade,
   and the tests make up the other 90%.
   
   Therefore, the formula needed would be this:
      
      average = quiz * 0.1 + tests * 0.9
      
   However, we must consider that we could have a bad test score,
   or a bad quiz score, so if that happens, we must handle that as well.
   As long as all numbers are good, the result will never be negative,
   so return a negative number on error.
   
   Algorithm:
      
      * If the quiz score is bad, return -1.0
      * Get test average via Student_test_average()
      * If test average < 0.0, return -1.0
      * return (quiz * 0.1) + (test average * 0.9)
*/
   // Variables
   double average;

   // Leave on a bad quiz score
   if (Student_error_quiz(s)) {
      return -1.0;
   }
   
   // Get the test average and leave on error
   average = Student_test_average(s);
   if (average < 0.0) {
      return -1.0;
   }
   
   // Return the test average
   return (s.quiz * 0.1) + (average * 0.9);
}

// GetGradeIndex //////////////////////////////////////////
int GetGradeIndex(double average)
{ /*
  Converts the average to an index for use with the
  LETTER_GRADES array, which gives us a letter grade.

  Algorithm:
     If the average is bad, return index to U.
	 If the average is >= 90, return index to A.
	 If average >= 80, return index to B.
	 If average >= 70, return index to C.
	 If average >= 60, return index to D.
	 Otherwise, return index to fail.
*/

   // If we couldn't get the average, return the index to U.
   if (is_bad_score(average)) {
      return 5;
   }
   
   //
   // At this point, I know FOR SURE that average
   // holds a value between 0.0 and 100.0.
   //
   
   // Check the average and return the required value
   if (average >= 90.0) {
      return 0;
   }
   else if (average >= 80.0) {
      return 1;
   }
   else if (average >= 70.0) {
      return 2;
   }
   else if (average >= 60.0) {
      return 3;
   }
   
   // We got here otherwise.
   return 4;
}

// Student_grade_index ////////////////////////////////////
int Student_grade_index(const Student &s)
{ /*
   This function takes a student and returns an integer
   in which the student can index the LETTER_GRADES array
   to convert it to a letter grade.
   
   Algorithm:
      
      If the average is:
         
         Greater than 100 or less than zero return 5 (to access the U in the array)
         
         ....At this point, I know average is between 0 and 100.
         
         Between 90 and 100 return 0
         Between 80 and 89.9 return 1
         Between 70 and 79.9 return 2
         Between 60 and 69.9 return 3
         Else return 4
*/
   return GetGradeIndex(Student_get_average(s));
}

// Student_search_by_id ///////////////////////////////////
int Student_search_by_id(Student *sa, int size, string id)
{ /*
   Finds a student by their ID number and returns its index
   in the array.
   
   For this function, I'm going to use a standard linear search.
   I.e. search through each element until the first occurrance of ID is found.
   
   If I can't find the student, I'll return a -1.
*/

   // Variables
   int index;
   
   // Perform a standard linear search.
   for (index = 0; index < size; index++) {
      
      // Return the index of the very first match.
      if (id == sa[index].id) {
         return index;
      }
   }
   
   // If we got here, that means the ID wasn't found.
   // Return a -1 for error.
   return -1;
}

// Student_search_names ///////////////////////////////////
int Student_search_names(Student *sa, int size, string first_name, string last_name, bool *table)
{ /*
   Searches through an entire list of students by name

   Algorithm:
      Iterate through every student.
	     Assume neither name matches the current student.
		 Use a case insensitive compare for the following.
		 If the given first name is empty, it matches all names, else check to see that it matches.
		 If the given last name is empty, it matches all names, else check to see that it matches.
		 Set the corresponding index in the table to true or false depending on the match.
		 Increment the counter if the match was good.
	  Return the number of matches.
*/
   bool match_first, match_last;
   int index, count;
   
   // Go through the entire array
   for (count = index = 0; index < size; index++) {
      
      // Assume the names don't match.
      match_first = false;
      match_last = false;
      
      // If the first name is an empty string, then it's a match.
      if (first_name.empty()) {
         match_first = true;
      }
      
      // Otherwise, check to see that it matches.
      else {
         match_first = !i_strcmp(first_name, sa[index].first_name);
      }
      
      // If the last name is an empty string, then it's a match.
      if (last_name.empty()) {
         match_last = true;
      }
      
      // Otherwise, check to see that it matches.
      else {
         match_last = !i_strcmp(last_name, sa[index].last_name);
      }
      
      // Update the truth value in the table
      table[index] = (match_first && match_last);
      count += (int)table[index];   // Increment the counter
   }
   
   // Return the number of matches
   return count;
}

// Student_add ////////////////////////////////////////////
int Student_add(Student *sa, int &size, Student &s)
{ /*
   Adds a student
   
   Algorithm:
      Put the student at the end of the array
      Return that element, and increment size by one
*/

   // Put that student at the end
   sa[size] = s;
   
   // Return the element and increment the size
   return size++;
}

// Student_drop ///////////////////////////////////////////
int Student_drop(Student *sa, int &size, int index)
{ /*
   Drops a student
   
   Algorithm:
      If the index is out of bounds, abort.
      Copy all students after the given index to the given index.
      Decrement the size by one.
      Zero out the final element.
      Return the new size.
*/
   int count, num_writes;

   // If there are no students to process, then get out.
   if (size <= 0) {
      return (size = 0);
   }

   // Leave if the index is out of bounds
   if ((index < 0) || (index >= size)) {
      return size;
   }
   
   // 01234x6789 - size 10 - index 5
   for (count = index + 1; count < size; count++) {
	   sa[count - 1] = sa[count];
   }
   
   // Decrement the size and zero out the final element
   Student_init(sa[--size]);
   
   // Return the new size
   return size;
}

// Student_drop_all ///////////////////////////////////////
void Student_drop_all(Student *sa, int &size)
{ /*
   Drops all students
   
   Algorithm:
      Reset the entire array.
      Set the size back to zero.
*/
   Student_init_array(sa, size);
   size = 0;
}

// GetBadStudentAverageNum ////////////////////////////////
int GetBadStudentAverageNum(Student *sa, int size)
{ /*
   This function returns the number of student records
   that have bad data in their averages.
   
   Algorithm:
      
      * Start the counter at zero
      * Loop through all students
         * If a student has bad data in their average
            * Add one to that counter
      * Return the counter
*/
   int counter = 0;
   
   // Go through each student, counting backwards
   // to use only one variable
   while (--size >= 0) {
      
      // False type casted to int returns zero
      // True type casted to int returns one
      counter += (int)Student_error_average(sa[size]);
   }
   
   // Return the counter
   return counter;
}

// GetClassAverage ////////////////////////////////////////
double GetClassAverage(Student *sa, int size)
{ /*
   This function returns the average of the class, not counting
   the student records that contain bad data.
   
   On error, it will return a negative number.
   
   Algorithm:
      
      * Start the sum at zero
      * Start a bad data counter at zero
      * Loop through all students
         * If this student has bad average data
            * Add one to bad data counter
         * Else
            * Add their average to the sum
      * If the number of bad data records is less than the number
        of students, then the class average is equal to the sum
        divided by (the number of students - bad data records),
        else return a negative number to signify an error.
*/
   int count, bad_data_count;
   double average, sum = 0.0;
   
   // Start the bad data counter at zero
   bad_data_count = 0;
   
   // Get the sum of the averages
   for (count = 0; count < size; count++) {
      
      // Get the average
      average = Student_get_average(sa[count]);
      
      // See if this student has bad data in the average
      if (average < 0.0) {
         
         // Increment the bad data count
         bad_data_count++;
         continue;         // Move to the next student
      }
      
      // Otherwise, add this average to the sum
      sum += average;
   }
   
   // If the number of bad data records is less than the number of
   // students, then..............
   //
   // (This is to avoid a division by zero.)
   if (bad_data_count < size) {
   
      // The class average is the sum divided by the number of
      // students with good data.
      return sum / (size - bad_data_count);
   }
   
   // If we got here, all of the data was bad.
   // Return a negative number to signify an error.
   return -1.0;
}

// GetLowestAverageIndex //////////////////////////////////
int GetLowestAverageIndex(Student *sa, int size)
{ /*
   This function returns the index of the student record holding
   the lowest average, not considering students with bad data.
   
   Returns a negative number if ALL students have bad data.
   
   Algorithm:
      
      * Assume the first student with GOOD data has the lowest average
         * Search through every element of the array
            * Start the count at zero.
            * If the data is bad, then increment the counter.
         * If the counter exceeds the bounds of the array, then
           no good data was found, so return a negative number.
      * Iterate through all students
         * If this student has bad data in its average,
           then move to the next student.
         * If this student's average is lower than the
           average of the student who we think has the lowest
           average, then assume the current student has the
           lowest average.
      * At this point, we now have the index of the student with
        the lowest average.  I want the index because this way, I
        can access the entire record.
*/
   int index, low_index;
   double average, low_average;
   
   // Since we are going to assume that
   // the first student with good data has
   // the lowest average, then first, we
   // shall look for the first student with
   // good data.
   
   // Search through every element
   for (low_index = 0;
        (low_index < size) && Student_error_average(sa[low_index]);
        low_index++) {}
        
   // If I have made it past the end of the array, then no good data was found.
   if (low_index >= size) {
      return -1;        // Return a negative number on error.
   }
   
   //
   // Since I know that low_index contains the first
   // student record with good data, there is no
   // reason for me to start my search at the first element.
   //
   // In fact, I can actually start at the element right after,
   // since I know comparing a record of one element to the
   // same record of the same element will always be equal.
   //
   // In the event that the next element exceeds the bounds
   // of my array, the for loop is automatically skipped
   // due to the test in my for loop.
   //
   
   // Get the assumed lowest average
   low_average = Student_get_average(sa[low_index]);
   
   // Search through every student
   for (index = low_index + 1; index < size; index++) {
      
      // Get the average of this student
      average = Student_get_average(sa[index]);
      
      // If this average is invalid, move to the next student
      if (average < 0.0) {
         continue;
      }
      
      // Compare this average with the assumed lowest average
      if (average < low_average) {
         low_index = index;
         low_average = Student_get_average(sa[low_index]);  // Assume this is now the lowest average
      }
   }
   
   // Return the index.
   return low_index;
}

// GetHighestAverageIndex /////////////////////////////////
int GetHighestAverageIndex(Student *sa, int size)
{ /*
   This function returns the index of the student record holding
   the highest average, not considering students with bad data.
   
   Returns a negative number if ALL students have bad data.
   
   Algorithm:
      
      * Assume the first student with GOOD data has the highest average
         * Search through every element of the array
            * Start the count at zero.
            * If the data is bad, then increment the counter.
         * If the counter exceeds the bounds of the array, then
           no good data was found, so return a negative number.
      * Iterate through all students
         * If this student has bad data in its average,
           then move to the next student.
         * If this student's average is higher than the
           average of the student who we think has the highest
           average, then assume the current student has the
           highest average.
      * At this point, we now have the index of the student with
        the highest average.  I want the index because this way, I
        can access the entire record.
*/
   int index, high_index;
   double average, high_average;
   
   // Since we are going to assume that
   // the first student with good data has
   // the highest average, then first, we
   // shall look for the first student with
   // good data.
   
   // Search through every element
   for (high_index = 0;
        (high_index < size) && Student_error_average(sa[high_index]);
        high_index++) {}
        
   // If I have made it past the end of the array, then no good data was found.
   if (high_index >= size) {
      return -1;        // Return a negative number on error.
   }
   
   //
   // Since I know that high_index contains the first
   // student record with good data, there is no
   // reason for me to start my search at the first element.
   //
   // In fact, I can actually start at the element right after,
   // since I know comparing a record of one element to the
   // same record of the same element will always be equal.
   //
   // In the event that the next element exceeds the bounds
   // of my array, the for loop is automatically skipped
   // due to the test in my for loop.
   //
   
   // Get the assumed highest average
   high_average = Student_get_average(sa[high_index]);
   
   // Search through every student
   for (index = high_index + 1; index < size; index++) {
      
      // Get the average of this student
      average = Student_get_average(sa[index]);
      
      // If this average is invalid, move to the next student
      if (average < 0.0) {
         continue;
      }
      
      // Compare this average with the assumed highest average
      if (average > high_average) {
         high_index = index;
         high_average = Student_get_average(sa[high_index]);   // Assume this is now the highest average
      }
   }
   
   // Return the index.
   return high_index;
}

// GetEqualAverages ///////////////////////////////////////
int GetEqualAverages(Student *sa, int size, int compare_index, bool *table)
{ /*
   Returns the number of students that have the same average
   as the student indexed by compare_index, and returns a
   boolean truth table of which students contain those equal values
*/
   int index, count = 0;
   double avg;
   
   // Return zero on out of bounds
   if ((compare_index < 0) || (compare_index >= size)) {
      return 0;
   }
   
   // Initialize the table to false
   memset(table, false, sizeof(bool) * size);

   // Get the student's average of which we are trying to compare
   avg = Student_get_average(sa[compare_index]);
   
   // Iterate through every student
   for (index = 0; index < size; index++) {
      
      // If the current student's average is equal
      // to the comparing average, then set the value
      // to true in the table, and increment the counter.
      if (Student_get_average(sa[index]) == avg) {
         table[index] = true;
         count++;
      }
   }
   
   // Return the number of students that have an
   // average equal to the one we were looking for
   return count;
}

// NumStudentsUnderClassAverage ///////////////////////////
int NumStudentsUnderClassAverage(Student *sa, int size, bool *table)
{ /*
   Returns the number of students under the class average,
   not counting any students with bad data.
   
   Algorithm:
      
      * Get the class average
      * Start the counter at zero
      * Iterate through all students
         * If this student has bad data, then move to the next student.
         * If the student's average is less than the class average,
           set the corresponding spot in the table to true then
           increment the counter
         * Otherwise set the corresponding spot in the table to false.
      * The counter is now set to the number of students below the class average.
*/

   // Variables
   double class_average, average;
   int count;
   
   // Get the class average
   class_average = GetClassAverage(sa, size);
   
   // Initialize counter to zero since we haven't counted anyone yet.
   count = 0;
   
   // Loop through all students
   while (--size >= 0) {
      
      // Get this student's average
      average = Student_get_average(sa[size]);
      
      // If this student has bad data, move to the next student.
      if (average < 0.0) {
         table[size] = false; // Be sure to set it to false.
         continue;
      }
      
      // If this student's average is less than the class average,
      // then increment the counter.
      table[size] = (average < class_average);  // Mark as true or false
      count += (int)table[size];
   }
   
   // Return the number of students
   return count;
}

// TallyGrades ////////////////////////////////////////////
void TallyGrades(int *tally, Student *sa, int size)
{ /*
   This keeps track of how many students have what grade.
   
   This function takes a 6-element integer array, where each element
   represents the following:
      
      tally[0] == number of A's
      tally[1] == number of B's
      tally[2] == number of C's
      tally[3] == number of D's
      tally[4] == number of F's
      tally[5] == number of unknown grades (most likely due to bad data)
      
   Algorithm:
      
      * Initialize the array to all zeroes.
      * Iterate through all students
         * Pass the student to the grade index function
         * Increment the array at that index.
*/

   // First thing's first.  Let's initialize the array
   // tallying the grade count to zero.
   //
   // Note: This trick works ONLY for initializing
   // arrays to zero.
   memset(tally, 0, LETTER_GRADES_SIZE * sizeof(int));
   
   // Go through each student (backwards)
   while (--size >= 0) {
      tally[Student_grade_index(sa[size])]++;   // Tally the grade.
   }
}
