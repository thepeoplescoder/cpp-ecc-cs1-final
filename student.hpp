/*
   student.hpp
   
   Header file containing the Student structure definition
   and utility function prototypes
   
   by Austin Cathey
   
   -----
   
   Computer Science 1
   Final Project
   Due Date: 12/12/2007
*/

// Make sure this file wasn't already included
#ifndef __STUDENT_HPP__
#define __STUDENT_HPP__

// Includes
#include <string>

// Use standard namespace
using namespace std;

// The maximum test scores
// #define is a DIRECT text substitution,
// so ensure that this symbol evaluates to
// an int.  I'm using a #define macro here
// so I don't get a compiler error having
// to do with a duplicate symbol declaration.
#define MAX_TESTS ((int)(5))

// Defines
#define SORT_KEY_ID      ((int)(0))
#define SORT_KEY_NAME    ((int)(1))
#define SORT_KEY_AVERAGE ((int)(2))

// External constants
extern const char LETTER_GRADES[];

// The Student structure
struct Student {
   string first_name;                     // First name of student
   string last_name;                      // Last name of student
   string id;                             // Student ID number (SSN)
   double quiz;                           // Quiz score
   int num_tests;                         // Number of tests
   double test_scores[MAX_TESTS];         // Test scores
};

// Create a compare function typedef
typedef int (*bubble_sort_compare)(const void *p1, const void *p2);

// Redefine the extraction and insertion operators to work with the Student type
istream &operator >>(istream &ins, Student &s);
ostream &operator <<(ostream &outs, Student &s);

// Prototypes
bool __is_valid_id(string id);
bool is_valid_id(string id);
void bubble_sort(void *buffer, size_t num_elements, size_t element_size, bubble_sort_compare compare);
void Student_init_array(Student *sa, int size);
double Student_test_average(const Student &s);
double Student_get_average(const Student &s);
bool Student_error_test_scores(const Student &s);
int GetGradeIndex(double average);
int Student_grade_index(const Student &s);
void Student_sort_by_key(Student *sa, int size, int key);
int Student_search_by_id(Student *sa, int size, string id);
int Student_search_names(Student *sa, int size, string first_name, string last_name, bool *table);
int Student_add(Student *sa, int &size, Student &s);
int Student_drop(Student *sa, int &size, int index);
void Student_drop_all(Student *sa, int &size);
int GetBadStudentAverageNum(Student *sa, int size);
double GetClassAverage(Student *sa, int size);
int GetLowestAverageIndex(Student *sa, int size);
int GetHighestAverageIndex(Student *sa, int size);
int GetEqualAverages(Student *sa, int size, int compare_index, bool *table);
int NumStudentsUnderClassAverage(Student *sa, int size, bool *table);
void TallyGrades(int *tally, Student *sa, int size);

//////////////////////
//                  //
// Inline functions //
//                  //
//////////////////////

// Student_init ///////////////////////////////////////////
inline void Student_init(Student &s)
{ /*
   This initializes a single student.
   
   I'm taking advantage of the fact that the address of a variable
   can be treated as a one-element array.
*/
   Student_init_array(&s, 1);
}

// is_bad_score ///////////////////////////////////////////
inline bool is_bad_score(double score)
{ // Returns true if the given score is out of range
   return ((score < 0.0) || (score > 100.0));
}

// Student_error_num_tests ////////////////////////////////
inline bool Student_error_num_tests(const Student &s)
{ // Returns true if the number of tests taken is out of range
   return ((s.num_tests < 0) || (s.num_tests > MAX_TESTS));
}

// Student_error_quiz /////////////////////////////////////
inline bool Student_error_quiz(const Student &s)
{ // Returns true if the quiz score is out of range
   return is_bad_score(s.quiz);
}

// Student_error_test_score ///////////////////////////////
inline bool Student_error_test_score(const Student &s, int num)
{ // Returns true if the given score is out of range
   return is_bad_score(s.test_scores[num]);
}

// Student_error_average //////////////////////////////////
inline bool Student_error_average(const Student &s)
{ // Returns true if there is any bad data in the average
   return Student_error_quiz(s) || Student_error_test_scores(s);
}

// Student_error //////////////////////////////////////////
inline bool Student_error(const Student &s)
{ // Returns true if there is any bad data in the record, e.g. bad scores, etc.
   return Student_error_num_tests(s) || Student_error_average(s);
}

#endif      // __STUDENT_HPP__
