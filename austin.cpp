/*
   austin.cpp
   
   Austin Cathey
   Computer Science #1
   Final Project
   Due Date: 12/12/2007
*/

/*
   Problem
   -------
   
   Write a program to access files that contain student records.
   It should be a menu-driven program which provides the user
   many options in which the data can be accessed/manipulated.
*/

/*
   Algorithm
   ---------
   
   This algorithm would be too complex for pseudocode.
   
   **Menu Design**
   
   For the menus, it would be best to write a function that gives each
   menu option their own unique ID, where the ID that the functinon
   returns can be reset at any time.  I have decided that this function
   will only return integers greater than zero, since it would look
   funny to ask the user to select a menu option of zero, and this enables
   me to use integers less than 1 as menu options that aren't enabled.
   
   Instead of putting the entire menu in a loop, I feel that it would be best
   to put the menus inside of their own functions, and put these functions
   inside of while loops, so that the return values can dictate whether
   the loops should continue or not.  Also, in the event that the loop
   should continue, this forces the function to be called again, refreshing
   certain data as needed.  Because of this, the necessary data that I need
   should be passed by reference so that the data doesn't get lost.
   
   For example, if I have a function that has dislpays limited options when
   there is no file being worked on, but displays the options when there is
   a file being worked on, it can take a file name as a reference parameter,
   and update that file name, and since it would be in a loop, the updated
   file name would then be passed, refreshing the data inside of my function,
   and displaying those menu options.
   
   Inside of the menu function, there is a loop for getting the CHOICE only.
   At the beginning of the loop, the screen will be refreshed just to keep
   things looking nice.  No options will be processed until we exit this loop.
   The loop of the calling routine is what will keep us in the current menu.
   
   This method will also allow the use of submenus.
   
   **Main Menu**
   
   For the main menu, there are four options:
      
      1) Load File
      2) Access Data
      3) Save Data
      4) Close File
      5) Quit
      
   When the program starts, everything is initialized to zero, false, empty
   string, etc.  In my program, there is a string variable that holds the
   name of the file that I am working on.  If this string is empty, then that
   means that we are not working on any file.
   
   If a file is not being worked on, then only the Load File and Quit options
   are shown.
   
   If a file IS being worked on, then all options are shown except for Load
   File.
   
   However, if a file is not being worked on, you will not see the menu
   look like this:
      
      1) Load File
      5) Quit
   
   But instead, like this:
      
      1) Load File
      2) Quit
      
   In order to accomplish this, I initialize each menu option to a negative
   number, meaning that the menu is not enabled.  In order to get an ID for
   the menu option, it has to get it from my ID assigning function.  For a
   description of how it works, check the NewMenuOption() function.
   
   I also will need to have a function that displays each menu option, and
   quietly leaves if a menu option is disabled.
   
   Load File option:
      
      This option will ask the user for a file name.  If the file exists,
      then the data will be loaded.  If it doesn't exist, it will ask the
      user if they would like to create the file.  If they don't want to
      create the file, then we have no working file, otherwise, we do, and
      we then go back to the main menu, which will be refreshed, and show
      the new options.
      
   Access Data option:
      
      This option takes the user to the Access Data submenu.
      
   Save Data option:
      
      This option takes the user to the Save File submenu.
      
   Close File option:
      
      This option "closes" the file that the user is working on.  Technically,
      the file is just not left hanging around open.  If any changes have
      been made to the student data, then the user will be given the option
      to save the file.  If no changes have been made, or if the file has
      been saved beforehand, then all of the application data is reset, i.e.
      the student array, the working file, the number of students, and
      the "changes made" variable.
      
   Quit option:
      
      This quits the program.
      
   **Access Data Menu**
   
   This menu provides everything that the user needs to access the student
   data.  The user is given the ability to search for a student using a
   primitive search engine that can determine whether a user is looking
   for a student by name, ID, or index number.  The user has options to sort
   the data based on the ID, name, and test average.  Also, you can get the
   grade count, add students, drop students, among many other things.
   
   In this menu, there are 14 options:
      
      1)  Show/Edit Students
      2)  Get Class Average
      3)  Get Student(s) With the Highest Average
      4)  Get Student(s) With the Lowest Average
      5)  Get Student(s) Below Class Average
      6)  Get Grade Count
      7)  Search for a Student
      8)  Sort Students by ID
      9)  Sort Students by Name
      10) Sort Students by Average
      11) Drop Student
      12) Drop All Students
      13) Add Student
      14) Go Back
      
   Show/Edit Students:
      
      This option is pretty self-explanatory.  It will display the students
      in a nicely formatted table, only displaying so many students per
      screen, so the user can actually view what data they choose.  The user
      can page through each screen, and they have the ability to select a
      student so they can see further details on them, such as their test
      scores.  If the user wishes to edit this information, they are free to
      do so.
      
   Get Class Average:
      
      Self-explanatory.  However, this only calculates the averages based
      on the student records that DO NOT contain bad data.
*/

// Disable deprecation
#define _CRT_SECURE_NO_WARNINGS

// Includes
#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <cctype>
#include <ctime>
#include <cstdio>
#include <cmath>
#include <conio.h>
#include "student.hpp"
#include "lost_conio.hpp"
#include "lucid_strings.hpp"

// Use standard namespace
using namespace std;

// Constants
const int MENU_OPTION_MINIMUM = 1;
const int ENTRIES_PER_SCREEN = 15;
const int STUDENT_MAX = 50;
const double DEFAULT_TIME = 0.03;

// Color constants
const int
   NORMAL_COLOR = 10,
   ERROR_COLOR = 12,
   HIGHLIGHT_COLOR = 15,
   TITLE_COLOR = 14,
   MENU_COLOR = 11;
   
// The input_info structure
struct input_info {
   int px, py;
   string prompt;
   string allowed;
   int x, y;
   int attr, grayed;
   int vis_len, max_len;
   bool visible, is_id;
   bool numeric, integer;
   bool good;
};

// Prototypes
void Application_Init(Student *sa, int size);
void outchar_slick(char c, double seconds = DEFAULT_TIME);
void outtext_slick(string s, double seconds = DEFAULT_TIME);
void Application_Shutdown(void);
int wait_key_msg(string message);
int NewMenuOption(bool reset);
void DisplayMenuOption(int width, int option, string text);
void Reset_Data(Student *student, const int ARRAY_MAX, int &num_students, string &work_file, bool &changes_made);
bool GetMenuSelection(int &choice, int low, int high);
int Main_Menu(Student *student, const int ARRAY_MAX, int &num_students, string &work_file, bool &changes_made);
bool GetNewSaveFile(string &work_file);
bool Save_Menu(Student *student, int num_students, string &work_file, bool &changes_made);
bool Access_Menu(Student *student, const int ARRAY_MAX, int &num_students, string work_file, bool &changes_made);
void Load_File(Student *student, const int ARRAY_MAX, int &num_students, string &work_file, bool &changes_made);
int Save_Data(Student *student, int num_students, string work_file, bool &changes_made);
bool Show_Save_Message(bool test, string work_file);
void Close_File(Student *student, const int ARRAY_MAX, int &num_students, string &work_file, bool &changes_made);
void Show_Class_Average(Student *student, int num_students);
void Show_Grade_Count(Student *student, int num_students);
void Sort_Data(Student *student, int num_students, int sort_key);
void Search_Student(Student *student, int num_students);
void Display_Student(Student *student, int count, bool highlight);
int __find_true(bool *table, int start, int end, int after);
int __find_true_back(bool *table, int end, int start, int after);
int Display_Student_Table_E(string title, Student *student, int num_students, int start, int end, bool select, int start_choice);
int Display_Student_Table_T(string title, Student *student, int num_students, bool *table, bool select);
void Show_Students_Under_Class_Average(Student *student, const int ARRAY_MAX, int num_students);
bool Input_Student(Student &s);
void Show_Edit_Students(Student *student, int num_students, bool &changes_made);
void Add_Student(Student *student, int &num_students, bool &changes_made);
void Drop_Student(Student *student, int &num_students, bool &changes_made);
void Drop_All_Students(Student *student, int &num_students, bool &changes_made);

//////////////////////
//                  //
// Inline functions //
//                  //
//////////////////////

// setup_screen ///////////////////////////////////////////
inline void setup_screen(void)
{ // Sets up the screen by setting the default text attribute and clearing the screen
   textattr(7);
   clrscr();
}

//////////
//      //
// Main //
//      //
//////////

// Main
int main(void)
{
   /**/
   // The maximum size for the student array
   const int ARRAY_MAX = STUDENT_MAX;
   
   // Variables
   int error_code = -1;                   // The error code main() will return.
                                          // Set this to negative to put us in the main loop.
   string work_file = "";                 // The name of the file we are working on.
                                          // If this is empty, we currently aren't working on
                                          // a file.
   bool changes_made = false;             // Boolean variable to say whether changes have been made
                                          // to the student data or not.
   Student student[ARRAY_MAX];            // The array of students.  This is where the magic happens.
   int num_students = 0;                  // Of course, we just started, so there are no students.
   
   ////////////////////////////////////////
   //
   // On with the executable statements!!
   //
   ////////////////////////////////////////
   
   // Initialize the application
   // This also zeroes out our student array.
   Application_Init(student, ARRAY_MAX);

   // Start the main menu
   while (error_code < 0) {
      
      // Put this function in a loop.
      // The necessary data is passed by reference so it is
      // preserved between each call.
      error_code = Main_Menu(student, ARRAY_MAX, num_students, work_file, changes_made);
   }
   
   // Shut down the application
   Application_Shutdown();
   
   // Return the error code
   return error_code;
   /**/
}

//////////////////////////
//                      //
// Function Definitions //
//                      //
//////////////////////////


// Application_Init ///////////////////////////////////////
void Application_Init(Student *sa, int size)
{ /*
   This function initializes the application
   
   Algorithm:
      Initialize console routines
      Initialize the student array
      Set desired color
      Clear the screen
*/

   // Initialize the console routines so we can use them
   lost_conio_init();
   
   // Zero out the student array
   Student_init_array(sa, size);
   
   // Set the desired colors and clear the screen
   textattr(7);
   clrscr();
}

// outchar_slick //////////////////////////////////////////
void outchar_slick(char c, double seconds)
{ /*
  Just playing around

  This just outputs a character by rotating slashes before
  the actual character itself gets outputted.

  Algorithm:
     * Rotate slashes for however many seconds
	    * Output current look of slash
		* Backspace it out
	 * If the character happens to be a newline
	    * Backspace out the last slash
	 * Output the actual character
*/
   char rotate[] = {'\\', (char)179, '/', '-'};
   int index;
   clock_t t;
   
   // Display the rotating thingies for however many seconds
   index = 0;
   t = clock();
   while (((clock() - t) / (double)CLOCKS_PER_SEC) < seconds) {
      cout << rotate[index++ % 4];
      cout << '\b';
   }
   
   // If the character is a newline, backspace the last dash.
   if (c == '\n') {
      cout << " \b";
   }
   
   // Display the character
   cout << c;
}

// outtext_slick //////////////////////////////////////////
void outtext_slick(string s, double seconds)
{ /*
  Playing around some more

  Algorithm:
     * Iterate through the entire string
	 * Output each individual character...the "slick" way.
*/
   for (unsigned int index = 0; index < s.length(); index++) {
      outchar_slick(s[index], seconds);
   }
}

// Application_Shutdown ///////////////////////////////////
void Application_Shutdown(void)
{ /*
   This function shuts down the application
   
   Algorithm:
      Go back to default output color attribute
      Clear the screen
*/
   static const int
      COLOR_DIM = 12,
      COLOR_MID = 14,
      COLOR_BRIGHT = 15;
   int count, len;
   
   // Go to default attribute and clear the screen
   setup_screen();
   
   // Just to play around
   cout << endl;
   textcolor(8);  outtext_slick(" Com");     // Computer Science 1
   textcolor(7);  outtext_slick("put");
   textcolor(15); outtext_slick("er Sci");
   textcolor(7);  outtext_slick("enc");
   textcolor(8);  outtext_slick("e 1\n\n ");
   textcolor(4);  outchar_slick('F');        // Final Project
   textcolor(12); outchar_slick('i');
   textcolor(14); outchar_slick('n');
   textcolor(12); outchar_slick('a');
   textcolor(4);  outtext_slick("l P");
   textcolor(12); outchar_slick('r');
   textcolor(14); outchar_slick('o');
   textcolor(15); outchar_slick('j');
   textcolor(14); outchar_slick('e');
   textcolor(12); outchar_slick('c');
   textcolor(4);  outtext_slick("t\n\n ");
   textcolor(15); outtext_slick("by ");      // by Austin Cathey
   textcolor(9);  outchar_slick('A');
   textcolor(11); outchar_slick('u');
   textcolor(15); outtext_slick("st");
   textcolor(11); outchar_slick('i');
   textcolor(9);  outchar_slick('n');
   textcolor(2);  outtext_slick(" C");
   textcolor(10); outchar_slick('a');
   textcolor(15); outtext_slick("th");
   textcolor(10); outchar_slick('e');
   textcolor(2);  outchar_slick('y');
   
   //
   // Draw a box :)
   //
   
   // Get the length of the longest line
   len = (int)string("computer science 1").length();
   
   // Draw the top and bottom
   for (count = 0; count < len; count++) {
      
      // Set the cursor position and the correct color
      gotoxy(2 + count, 1); textattr(count ? COLOR_DIM : COLOR_MID);
      outchar_slick((char)196);  // Display the character
      
      // Set the cursor position and the correct color
      gotoxy(2 + count, 7); textattr((count == len - 1) ? COLOR_MID : COLOR_DIM);
      outchar_slick((char)196);  // Display the character
   }
   
   // Display the top corners
   gotoxy(1, 1); textattr(COLOR_BRIGHT); outchar_slick((char)218);
   gotoxy(20, 1); textattr(COLOR_DIM); outchar_slick((char)191);
   
   // Draw the left and right sides
   for (count = 0; count < 5; count++) {
      
      // Set the cursor position and the correct color
      gotoxy(1, 2 + count); textattr(count ? COLOR_DIM : COLOR_MID);
      outchar_slick((char)179);     // Display the character
      
      // Set the cursor position and the correct color
      gotoxy(2 + len, 2 + count); textattr((count == 4) ? COLOR_MID : COLOR_DIM);
      outchar_slick((char)179);     // Display the character
   }
   
   // Display the bottom corners
   gotoxy(1, 7); textattr(COLOR_DIM); outchar_slick((char)192);
   gotoxy(20, 7); textattr(COLOR_BRIGHT); outchar_slick((char)217);

   // Go to the next line
   cout << endl;
}

// wait_key_msg ///////////////////////////////////////////
int wait_key_msg(string message)
{ /*
   Displays a message and waits for a keypress
*/
   cout << message;
   return wait_key();
}

// NewMenuOption //////////////////////////////////////////
int NewMenuOption(bool reset)
{ /*
   This function returns an integer value for a menu option.
   If reset is true, restart the count.  If it is false, then don't.
   
   Algorithm:
      Start the internal count at one
      If reset is true, then reset the internal count to 1 and return zero.
      Return the current value of the counter, while incrementing it.
*/
   static int internal_counter;
   
   // If we should reset the count, then do just that.
   if (reset) {
      internal_counter = MENU_OPTION_MINIMUM;
      return 0;
   }
   
   // Return the current count value and move to the next.
   return internal_counter++;
}

// DisplayMenuOption //////////////////////////////////////
void DisplayMenuOption(int width, int option, string text)
{ /*
   This function displays a menu option if it's valid.

   Algorithm:
      * If the menu option is disabled, then leave quietly.
	  * Display the menu option number
	  * Display the caption
*/

   // Leave if the menu option is invalid
   if (option < MENU_OPTION_MINIMUM) {
      return;
   }
   
   // Display the menu option
   textcolor(HIGHLIGHT_COLOR);
   if (width > 0) {
      cout << setw(width);          // Set the width if necessary
   }
   cout << option << ") ";          // Highlight the option
   
   textcolor(MENU_COLOR);
   //outtext_slick(text + '\n', 0.001);
   cout << text << endl;            // Display the caption in the proper color
}

// Reset_Data /////////////////////////////////////////////
void Reset_Data(Student *student, const int ARRAY_MAX, int &num_students, string &work_file, bool &changes_made)
{ /*
   This function resets the data as if the program just started.
*/
   Student_init_array(student, ARRAY_MAX);
   num_students = 0;
   work_file = "";
   changes_made = false;
}

// GetMenuSelection ///////////////////////////////////////
bool GetMenuSelection(int &choice, int low, int high)
{ /*
   Gets the menu selection from the user
   
   Returns true on success, false on failure.
*/

   // Display the prompt
   textcolor(NORMAL_COLOR);
   cout << "Please enter your choice: ";
   textcolor(HIGHLIGHT_COLOR);
   cin >> choice;
   cin.ignore(100, '\n');        // Ensure only ONE value is retrieved.
   textcolor(NORMAL_COLOR);
   
   // IMMEDIATELY check for an error, and fix any.
   if (cin.fail()) {
      
      // Clear the buffer and error state.
      cin.clear();
      cin.ignore(100, '\n');
   }
      
   // Otherwise, if the choice is valid, then return true.
   else if ((choice >= low) && (choice <= high)) {
      return true;
   }
   
   // We didn't get a valid value.
   return false;
}

// Main_Menu //////////////////////////////////////////////
int Main_Menu(Student *student, const int ARRAY_MAX, int &num_students, string &work_file, bool &changes_made)
{ // The application's main menu
   int choice, retval;
   bool done;
   
   // Menu options
   //
   // Menu options are initialized to a negative number
   // because NewMenuOption() always returns a positive number.
   //
   // Even though they may be disabled at this point,
   // I'm still setting them to unique values in the
   // event I need to access their code blocks.
   int OPTION_LOAD_FILE   = -1,
       OPTION_ACCESS_DATA = -2,
       OPTION_SAVE_DATA   = -3,
       OPTION_CLOSE_FILE  = -4,
       OPTION_QUIT        = -5,
       MAX_OPTIONS;
       
   //
   // Get the menu options
   //
   
   // Reset the menu options
   NewMenuOption(true);
   
   // Load file option
   if (work_file.empty()) {
      OPTION_LOAD_FILE = NewMenuOption(false);
   }
   else {
      OPTION_ACCESS_DATA = NewMenuOption(false);
      OPTION_SAVE_DATA = NewMenuOption(false);
      OPTION_CLOSE_FILE = NewMenuOption(false);
   }
   
   // Quit
   OPTION_QUIT = NewMenuOption(false);
   
   // Set the maximum number of options
   MAX_OPTIONS = OPTION_QUIT;
   
   //
   // Display the menu
   //
   
   // Since we just started, we aren't done.
   done = false;
   
   // Display the menu.
   // In this loop, we are only getting the CHOICE.
   // We won't process until AFTER the loop is completed.
   while (!done) {
   
      // Clear the screen
      setup_screen();
   
      //
      // Menu title
      //
      
      // Display the title
      textcolor(TITLE_COLOR);
      cout << "Main Menu";
      
      // Display the file we are working with
      if (!work_file.empty()) {
         cout << " - ";
         textcolor(HIGHLIGHT_COLOR);
         cout << work_file;
      }
      cout << endl << endl;
      
      //
      // Menu options
      //
      
      // Load file
      textcolor(NORMAL_COLOR);
      
      // Display the menu options
      DisplayMenuOption(0, OPTION_LOAD_FILE,   "Load File");
      DisplayMenuOption(0, OPTION_ACCESS_DATA, "Access Student Data");
      DisplayMenuOption(0, OPTION_SAVE_DATA,   "Save Student Data");
      DisplayMenuOption(0, OPTION_CLOSE_FILE,  "Close File");
      DisplayMenuOption(0, OPTION_QUIT,        "Exit");
      
      // Skip a line
      cout << endl;
      
      // Get the user's choice
      done = GetMenuSelection(choice, MENU_OPTION_MINIMUM, MAX_OPTIONS);
   }
   
   // Initialize the return value to a negative number to
   // keep us in the main loop.
   retval = -1;
   
   // If the user decides to quit, then set the return value to zero.
   if (choice == OPTION_QUIT) {
      
      // A return value of zero or greater will kick us out
      // of the main loop.  This is also the value that main()
      // will return.
      retval = 0;
      choice = OPTION_CLOSE_FILE;   // Run the code for closing the file
   }
   
   //
   // Note how the preceding and the following
   // are in separate if blocks.  It makes the
   // trick I did in the previous if block
   // possible.  Pretty sweet, eh? :)
   //
   
   // If the user decides to load a file, then ask for that file.
   if (choice == OPTION_LOAD_FILE) {
      
      // Ask the user for a filename to load.
      Load_File(student, ARRAY_MAX, num_students, work_file, changes_made);
   }
   
   // If the user decides to access the data,
   // then take them to the data access menu.
   else if (choice == OPTION_ACCESS_DATA) {
      
      // Go to the access menu.
      while (Access_Menu(student, ARRAY_MAX, num_students, work_file, changes_made)) {}
   }
   
   // If the user decides to save the data
   else if (choice == OPTION_SAVE_DATA) {
      
      // Go to the save file menu.
      while (Save_Menu(student, num_students, work_file, changes_made)) {}
   }
   
   // If the user decides to close the file
   else if (choice == OPTION_CLOSE_FILE) {
      
      // Attempt to close the file.
      // If no changes were made, this function processes quietly.
      //
      // If there is no working file, the code is written so
      // changes_made can never be true, so in that case, this
      // function does nothing significant.
      //
      Close_File(student, ARRAY_MAX, num_students, work_file, changes_made);
   }
   
   // Return to main
   return retval;
}

// GetNewSaveFile /////////////////////////////////////////
bool GetNewSaveFile(string &work_file)
{ /*
   Gets a new save file name
*/
   string s;
   char yn;
   ifstream fin;
   bool exists;
   
   // Prompt the user for the filename
   textcolor(NORMAL_COLOR);
   cout << "Enter file name: ";
   textcolor(HIGHLIGHT_COLOR);
   getline(cin, s);
   
   // Check for existence.
   fin.open(s.c_str());
   exists = fin.is_open();
   fin.close();
   
   // If it exists, ask the user if they want to overwrite it.
   if (exists) {
      
      // Display the prompt
      textcolor(HIGHLIGHT_COLOR);
      cout << s;
      textcolor(ERROR_COLOR);
      cout << " already exists.  Overwrite (y/N)? ";
      textcolor(HIGHLIGHT_COLOR);
      cin >> yn;
      cin.ignore(100, '\n');
      textcolor(NORMAL_COLOR);
      
      // If they don't want to overwrite it, then notify the caller of that.
      if (tolower(yn) != 'y') {
         return false;
      }
   }
   
   // Set the new work file and let the caller know it's okay.
   work_file = s;
   return true;
}

// Save_Menu //////////////////////////////////////////////
bool Save_Menu(Student *student, int num_students, string &work_file, bool &changes_made)
{ /*
   This function is the save file menu
*/
   int choice, n;
   bool done;

   // Menu options
   int OPTION_SAVE    = -1,
       OPTION_SAVE_AS = -2,
       OPTION_BACK    = -3,
       MAX_OPTIONS;
       
   // Reset the menu options
   NewMenuOption(true);
   
   // Get the menu options
   OPTION_SAVE = NewMenuOption(false);
   OPTION_SAVE_AS = NewMenuOption(false);
   OPTION_BACK = NewMenuOption(false);
   MAX_OPTIONS = OPTION_BACK;
   
   // Since we just started, we aren't done.
   done = false;
   
   // Display the menu
   while (!done) {
      
      // Clear the screen
      setup_screen();
      
      // Display the title
      textcolor(TITLE_COLOR);
      cout << "Save File Menu - ";
      textcolor(HIGHLIGHT_COLOR);
      cout << work_file;
      textcolor(NORMAL_COLOR);
      cout << endl << endl;
      
      // Display the menu options
      DisplayMenuOption(0, OPTION_SAVE,    "Save");
      DisplayMenuOption(0, OPTION_SAVE_AS, "Save As");
      DisplayMenuOption(0, OPTION_BACK,    "Go Back");
      
      // Skip a line
      cout << endl;
      
      // Get the user's choice
      done = GetMenuSelection(choice, MENU_OPTION_MINIMUM, MAX_OPTIONS);
   }
   
   // If the user chose Save As,
   if (choice == OPTION_SAVE_AS) {
      
      // then get another save file name.
      if (GetNewSaveFile(work_file)) {
         
         // If we successfully retrieved a different
         // name, then just do the save option as normal.
         choice = OPTION_SAVE;
      }
   }
   
   // If we are to go back, then return a value to
   // kill the loop in the calling routine.
   if (choice == OPTION_BACK) {
      return false;
   }
   
   // Save the data
   else if (choice == OPTION_SAVE) {
      
      // Attempt to save the file
      n = Save_Data(student, num_students, work_file, changes_made);
      
      // Show the message
      n = (int)Show_Save_Message((n == num_students), work_file);
      
      // Wait for a keypress
      textcolor(NORMAL_COLOR);
      cout << "Press any key to continue . . .";
      wait_key();
      
      // There's no need to stay in this menu if the file has been saved.
      if (n) {
         return false;
      }
   }

   // Keep the loop going
   return true;
}

// Access_Menu ////////////////////////////////////////////
bool Access_Menu(Student *student, const int ARRAY_MAX, int &num_students, string work_file, bool &changes_made)
{ /*
   This is where the magic happens.  This is where the data is accessed and edited.
*/
   int choice, index, num;
   bool done, compare_table[STUDENT_MAX];
   char message[50];
   
   // This is so I can form my sorting message without an if statement.
   static string what[] = {
      "ID",
      "name",
      "average"
   };

   // Menu options
   int OPTION_GET_CLASS_AVERAGE    = -1,
       OPTION_GET_HIGHEST_AVERAGE  = -2,
       OPTION_GET_LOWEST_AVERAGE   = -3,
       OPTION_GET_GRADE_COUNT      = -4,
       OPTION_SORT_BY_AVERAGE      = -5,
       OPTION_SORT_BY_NAME         = -6,
       OPTION_SORT_BY_ID           = -7,
       OPTION_SEARCH_STUDENT       = -8,
       OPTION_GET_LT_CLASS_AVERAGE = -9,
       OPTION_SHOW_EDIT_STUDENTS   = -10,
       OPTION_ADD_STUDENT          = -13,
       OPTION_DROP_STUDENT         = -14,
       OPTION_DROP_ALL_STUDENTS    = -15,
       OPTION_BACK                 = -16,
       MAX_OPTIONS;
       
   // Initialize my boolean table
   memset(compare_table, false,  sizeof(compare_table));
       
   // Reset the menu options
   NewMenuOption(true);
   
   // Only make these options available if there ARE
   // students that we can actually process.
   if (num_students > 0) {
      OPTION_SHOW_EDIT_STUDENTS = NewMenuOption(false);			// Show a table of the students

      OPTION_GET_CLASS_AVERAGE = NewMenuOption(false);			// Get the class average
      OPTION_GET_HIGHEST_AVERAGE = NewMenuOption(false);		// Get students with the highest average
      OPTION_GET_LOWEST_AVERAGE = NewMenuOption(false);			// Get stuents with the lowest average
      OPTION_GET_LT_CLASS_AVERAGE = NewMenuOption(false);		// Get the students who are lower than the class average
      OPTION_GET_GRADE_COUNT = NewMenuOption(false);			// Get a grade count

      OPTION_SEARCH_STUDENT = NewMenuOption(false);				// Search a student

      OPTION_SORT_BY_ID = NewMenuOption(false);					// Sort students by ID
      OPTION_SORT_BY_NAME = NewMenuOption(false);				// Sort students by name
      OPTION_SORT_BY_AVERAGE = NewMenuOption(false);			// Sort students by average

      OPTION_DROP_STUDENT = NewMenuOption(false);				// Drop a student
      OPTION_DROP_ALL_STUDENTS = NewMenuOption(false);			// Drop all students
   }
   
   // Only make this option available if there are less than 50 students.
   if (num_students < ARRAY_MAX) {
      OPTION_ADD_STUDENT = NewMenuOption(false);				// Add a student
   }
   
   // Go back option
   OPTION_BACK = NewMenuOption(false);
   MAX_OPTIONS = OPTION_BACK;
   
   //
   // Menu time!
   //
   
   // We just started
   done = false;
   
   // Display the menu
   while (!done) {
      
      // Clear screen
      setup_screen();

      // Display the menu title
      textcolor(TITLE_COLOR);
      cout << "Access Student Data - ";
      textcolor(HIGHLIGHT_COLOR);
      cout << work_file;
      cout << endl << endl;
      
      // Display the menu options.
      textcolor(NORMAL_COLOR);
      DisplayMenuOption(2, OPTION_SHOW_EDIT_STUDENTS,   "Show/Edit Students\n");
      DisplayMenuOption(2, OPTION_GET_CLASS_AVERAGE,    "Get Class Average");
      DisplayMenuOption(2, OPTION_GET_HIGHEST_AVERAGE,  "Get Student(s) With the Highest Average");
      DisplayMenuOption(2, OPTION_GET_LOWEST_AVERAGE,   "Get Student(s) With the Lowest Average");
      DisplayMenuOption(2, OPTION_GET_LT_CLASS_AVERAGE, "Get Student(s) Below Class Average");
      DisplayMenuOption(2, OPTION_GET_GRADE_COUNT,      "Get Grade Count\n");
      DisplayMenuOption(2, OPTION_SEARCH_STUDENT,       "Search for a Student\n");
      DisplayMenuOption(2, OPTION_SORT_BY_ID,           "Sort Students by ID");
      DisplayMenuOption(2, OPTION_SORT_BY_NAME,         "Sort Students by Name");
      DisplayMenuOption(2, OPTION_SORT_BY_AVERAGE,      "Sort Students by Average\n");
      DisplayMenuOption(2, OPTION_DROP_STUDENT,         "Drop Student");
      DisplayMenuOption(2, OPTION_DROP_ALL_STUDENTS,    "Drop All Students");
      DisplayMenuOption(2, OPTION_ADD_STUDENT,          "Add Student\n");
      DisplayMenuOption(2, OPTION_BACK,                 "Go Back");
      
      // Skip a line
      cout << endl;
      
      // Get the user's choice
      done = GetMenuSelection(choice, MENU_OPTION_MINIMUM, MAX_OPTIONS);
   }

   // If the user has decided to go back, then stop this menu.
   if (choice == OPTION_BACK) {
      return false;
   }
   
   // Calculate the class average
   else if (choice == OPTION_GET_CLASS_AVERAGE) {
      
      // Show it to the user
      Show_Class_Average(student, num_students);
   }
   
   // Get the students with the lowest/highest average
   else if ((choice == OPTION_GET_LOWEST_AVERAGE) || (choice == OPTION_GET_HIGHEST_AVERAGE)) {
      
      //
      // I kept it like this because the code blocks are similar.
      //
      
      // Get the index of the student with the lowest/highest average
      // and get the indexes of the students that match this average
      //
      // Lowest
      if (choice == OPTION_GET_LOWEST_AVERAGE) {
         index = GetLowestAverageIndex(student, num_students);
         num = GetEqualAverages(student, num_students, index, compare_table);
         
         // Set the title
         if (num > 1) {
            sprintf(message, "%d Students With the Lowest Average", num);
         }
         else {
            sprintf(message, "Student With the Lowest Average");
         }
      }
      
      // Highest
      else {
         index = GetHighestAverageIndex(student, num_students);
         num = GetEqualAverages(student, num_students, index, compare_table);
         
         // Set the title
         if (num > 1) {
            sprintf(message, "%d Students With the Highest Average", num);
         }
         else {
            sprintf(message, "Student With the Highest Average");
         }
      }
      
      // Display the table
      Display_Student_Table_T(message, student, num_students, compare_table, false);
   }
   
   // Get the number of students that have what grade
   else if (choice == OPTION_GET_GRADE_COUNT) {
      Show_Grade_Count(student, num_students);
   }
   
   // Sort student data
   else if ((choice >= OPTION_SORT_BY_ID) && (choice <= OPTION_SORT_BY_AVERAGE)) {
      Sort_Data(student, num_students, choice - OPTION_SORT_BY_ID);
   }
   
   // Search for a student
   else if (choice == OPTION_SEARCH_STUDENT) {
      Search_Student(student, num_students);
   }
   
   // Get the number of students whose average is less
   // than the class average
   else if (choice == OPTION_GET_LT_CLASS_AVERAGE) {
      Show_Students_Under_Class_Average(student, ARRAY_MAX, num_students);
   }
   
   // Display students in a table format
   else if (choice == OPTION_SHOW_EDIT_STUDENTS) {
      Show_Edit_Students(student, num_students, changes_made);
   }
   
   // Add a student
   else if (choice == OPTION_ADD_STUDENT) {
      Add_Student(student, num_students, changes_made);
   }
   
   // Drop a student
   else if (choice == OPTION_DROP_STUDENT) {
      Drop_Student(student, num_students, changes_made);
   }
   
   // Drop all students
   else if (choice == OPTION_DROP_ALL_STUDENTS) {
      Drop_All_Students(student, num_students, changes_made);
   }

   // Keep the menu going
   return true;
}

// Load_File //////////////////////////////////////////////
void Load_File(Student *student, const int ARRAY_MAX, int &num_students, string &work_file, bool &changes_made)
{ // Loads the student data into a file
   string s;
   ifstream fin;
   bool failure;
   char yn;

   // Skip a line
   cout << endl;
   
   // Set the proper color
   textcolor(NORMAL_COLOR);
   
   // Ask the user for the name of the file
   cout << "Enter file name (";
   textcolor(TITLE_COLOR);
   cout << "leave blank to cancel";
   textcolor(NORMAL_COLOR);
   cout << "): ";
   textcolor(HIGHLIGHT_COLOR);
   getline(cin, s);
   textcolor(NORMAL_COLOR);
   
   // If the user didn't enter a file name,
   // then just go back quietly.
   if (s.empty()) {
      return;
   }
   
   // No changes have been made yet
   changes_made = false;
   
   // Start at zero students
   num_students = 0;
   
   // Try to open that file
   fin.open(s.c_str());
   if (!fin) {
      
      // Display an error message
      textcolor(ERROR_COLOR);
      cout << "I couldn't seem to open "; textcolor(HIGHLIGHT_COLOR);
      cout << s; textcolor(ERROR_COLOR);
      cout << " for input." << endl;
      
      // Ask if the user would like to create that file.
      cout << "Would you like to create it (y/N)? ";
      textcolor(HIGHLIGHT_COLOR);
      cin >> yn;
      cin.ignore(100, '\n');  // Ensure only ONE character is retrieved
      
      // Since N is default, I have to
      // explicitly check for Y.
      if (tolower(yn) == 'y') {
         work_file = s;
      }
      
      // Return to the caller.
      return;
   }
   
   // Set this as the new work file
   work_file = s;
   
   /* cout << "before first data read" << endl; wait_key(); */
   
   // Read the first student
   fin >> student[num_students];
   
   // Keep reading students until an error occurs.
   while (fin) {
      
      // Count the student that we read last
      num_students++;
      
      // If it goes past our boundaries,
      if (num_students >= ARRAY_MAX) {
         break;   // then get out.
      }
      
      // Attempt to read the next student
      /* cout << "before read " << (num_students + 1) << endl; wait_key(); */
      fin >> student[num_students];
      /* cout << "after read " << (num_students + 1) << endl; wait_key(); */
   }
   
   /* cout << "after entire file read" << endl; wait_key(); */

   // If we haven't reached the end of the file, an error occurred.
   failure = !fin.eof();   // Cheap trick :)
   
   // Close the input file
   fin.close();
   
   // Display the information
   textcolor(HIGHLIGHT_COLOR);
   cout << num_students << ' '; textcolor(NORMAL_COLOR);
   cout << ((num_students == 1) ? "student was" : "students were");  // Let's make sure we're grammatically correct ;)
   cout << " read from "; textcolor(HIGHLIGHT_COLOR);
   cout << work_file; textcolor(NORMAL_COLOR);
   cout << '.' << endl;
   
   // If no students were read from this file,
   // then notify the user, and ask if they still
   // want to use this file.
   if (!num_students) {
      
      // Display the message
      textcolor(ERROR_COLOR);
      cout << "Would you still like to use this file (y/N)? ";
      textcolor(HIGHLIGHT_COLOR);
      cin >> yn;
      cin.ignore(100, '\n');
      
      // Reset the data.
      //Reset_Data(student, ARRAY_MAX, num_students, work_file, changes_made);
      if (tolower(yn) != 'y') {
         work_file = "";   // No working file.
      }
   }
   
   // If there were problems reading from this file,
   // notify the user.
   else if (failure) {
      textcolor(ERROR_COLOR);
      cout << "There were problems reading this file." << endl;
      cout << "Please check its contents." << endl;
      
      // Ask the user if they still would like to use this data.
      cout << "Would you still like to use this data (y/N)? ";
      textcolor(HIGHLIGHT_COLOR);
      cin >> yn;
      cin.ignore(100, '\n');
      
      // If it's not equal to Y, then the answer is N
      // by default, meaning they don't want to use this data.
      if (tolower(yn) != 'y') {
         
         // Reset all values
         Reset_Data(student, ARRAY_MAX, num_students, work_file, changes_made);
      }
   }
   
   // Wait for the user to press a key
   textcolor(NORMAL_COLOR);
   cout << "Press any key to continue . . .";
   wait_key();
}

// Save_Data //////////////////////////////////////////////
int Save_Data(Student *student, int num_students, string work_file, bool &changes_made)
{ /*
   This function saves the data to the file,
   and returns the number of records written.
*/
   int index;
   
   // Open the working file for output.
   ofstream fout(work_file.c_str());
   if (!fout) {
      return 0;
   }
   
   // Write the data
   for (index = 0; index < num_students; index++) {
      
      // Yes, this is valid.
      // The convention is that the following expression:
      //
      //   stream << anything_you_want
      //
      // Should evaluate to a reference to stream.
      //
      // Kids, don't try this at home! :D
      //
      if (!(fout << student[index])) {
         break;   // Leave if we couldn't write the data.
      }
   }
   
   // Close the file
   fout.close();
   
   // If all of the records have been written, then no
   // changes were made.  Otherwise, leave it at true.
   changes_made = (index != num_students);
   
   // Return the number of records written.
   return index;
}

// Show_Save_Message //////////////////////////////////////
bool Show_Save_Message(bool test, string work_file)
{ // Shows the save message.

   // Display success message.
   if (test) {
      
      // RULE #1 - Assume NOTHING.
      textcolor(NORMAL_COLOR);
      
      // Notify the user on success
      cout << "Data successfully written to "; textcolor(HIGHLIGHT_COLOR);
      cout << work_file; textcolor(NORMAL_COLOR);
      cout << '.' << endl;
   }
   
   // Display an error message.
   else {
      textcolor(ERROR_COLOR);
      cout << "Error writing to "; textcolor(HIGHLIGHT_COLOR);
      cout << work_file; textcolor(ERROR_COLOR);
      cout << '.' << endl;
      cout << "Try saving this data to a different file." << endl;
      cout << "If that fails, you may be running out of disk space." << endl;
   }
   
   // Return the test that was sent.
   return test;
}

// Close_File /////////////////////////////////////////////
void Close_File(Student *student, const int ARRAY_MAX, int &num_students, string &work_file, bool &changes_made)
{ /*
   This function "closes the file."
   
   The file isn't really opened, but the user doesn't need to know
   ALL of the technical details.
   
   Basically this just resets the data and saves the file if we need to save it.
*/
   char yn;
   bool okay = true;
   int n;

   // If changes have been made to the file, ask the user if they want to save it.
   if (changes_made) {
      
      // Let the user know the file hasn't been saved.
      textcolor(HIGHLIGHT_COLOR);
      cout << work_file;
      textcolor(NORMAL_COLOR);
      cout << " has not been saved." << endl;
      
      // Since this has to do with saving, I
      // want the user to explicitly tell me
      // YES or NO.
      do {
         cout << "Would you like to save it (Y/N)? ";
         
         // Get the input from the user
         textcolor(HIGHLIGHT_COLOR);
         cin >> yn;
         cin.ignore(100, '\n');  // Clear the buffer
         textcolor(NORMAL_COLOR);
         
         // Convert to lowercase
         yn = tolower(yn);
      } while ((yn != 'y') && (yn != 'n'));
      
      // If the user wishes to save it, then do so.
      if (yn == 'y') {
         
         // Attempt to save the data
         n = Save_Data(student, num_students, work_file, changes_made);
         
         // Display an error message on failure.
         if (!Show_Save_Message((n == num_students), work_file)) {
            
            // Keep the error color
            textcolor(ERROR_COLOR);

            // Find out if the user would like to close the file anyway.
            cout << "Would you like to close this file anyway (y/N)? ";
            cin >> yn;
            cin.ignore(100, '\n');
            
            // If the user says no (default), then it's
            // not okay to reset the data.
            if (tolower(yn) != 'y') {
               okay = false;
            }
            
            // Go back to normal settings
            textcolor(NORMAL_COLOR);
         }
         
         // Wait for a keypress.
         cout << "Press any key to continue . . .";
         wait_key();
      }
   }
   
   // Reset the data if it's okay to do so
   if (okay) {
      Reset_Data(student, ARRAY_MAX, num_students, work_file, changes_made);
   }
}

// Show_Class_Average /////////////////////////////////////
void Show_Class_Average(Student *student, int num_students)
{ // Shows the class average to the user
   double average;
   
   // Get the class average
   average = GetClassAverage(student, num_students);
   
   // Clear the screen
   setup_screen();
   
   // If the average can't be shown, display an error message.
   if (average < 0.0) {
      textcolor(ERROR_COLOR);
      cout << "The class average cannot be calculated due to bad data.";
   }
   
   // Otherwise, display the class average
   else {
      textcolor(MENU_COLOR);
      cout << "The class average is: ";
      textcolor(HIGHLIGHT_COLOR);
      cout << setprecision(2) << average;
   }
   
   // Wait for a keypress
   textcolor(NORMAL_COLOR);
   wait_key_msg("\n\nPress any key to continue . . .");
}

// Show_Grade_Count ///////////////////////////////////////
void Show_Grade_Count(Student *student, int num_students)
{ // Shows the number of students that have what grade
   int len = (int)string("unknown grade").length();

   // To save me the trouble of using if statements
   static string what[2][6] = {
      {"an ", "a  ", "a  ", "a  ", "an ", "an "},
      {"A", "B", "C", "D", "F", "unknown grade"}
   };
   
   // The grade count
   int tally[6], count1, count2;
   static int LAST_ELEMENT = (sizeof(tally) / sizeof(tally[0])) - 1;
   
   // Get the grade count
   TallyGrades(tally, student, num_students);
   
   // Clear the screen
   setup_screen();
   
   // Display the grades
   for (count1 = 0; count1 <= LAST_ELEMENT; count1++) {
      
      // If we're at the last element, skip it
      // if we didn't count anything.
      if ((count1 == LAST_ELEMENT) && (!tally[count1])) {
         continue;   // A break would be fine here, as well.
      }
      
      // Display the string
      textcolor(MENU_COLOR);
      cout << "Number of students with ";
      
      // Simply loop through the array here
      for (count2 = 0; count2 < (sizeof(what) / sizeof(what[0])); count2++) {
         if (count2 == 1) {
			 textcolor(TITLE_COLOR);
			 cout << right << setw(len);
		 }
         cout << what[count2][count1];
         if (count2 == 1) { textcolor(MENU_COLOR); }
      }
	  cout << " - ";
      
      // Display the number
      textcolor(HIGHLIGHT_COLOR);
      cout << tally[count1] << endl;
   }
   
   // Wait for a keypress
   textcolor(NORMAL_COLOR);
   clear_key();
   wait_key_msg("\nPress any key to continue . . .");
}

// Sort_Data //////////////////////////////////////////////
void Sort_Data(Student *student, int num_students, int sort_key)
{ // Sorts the data by name, average, or ID
   static string what[] = {
      "ID",
      "name",
      "average"
   };
   
   // Sort the data
   Student_sort_by_key(student, num_students, sort_key);
   
   // Display a message
   textcolor(NORMAL_COLOR);
   cout << endl << "Data sorted by ";
   textcolor(HIGHLIGHT_COLOR);
   cout << what[sort_key];
   textcolor(NORMAL_COLOR);
   cout << '.' << endl;
   
   // Wait for the user to press a key.
   wait_key_msg("Press any key to continue . . .");
}

// Search_Student /////////////////////////////////////////
void Search_Student(Student *student, int num_students)
{ /*
  Searches for a student.

  You can search for a student by their index number, their
  ID number, or their name.
*/
   string search_key;
   string first_name, last_name;
   char title[81];
   bool table[STUDENT_MAX];
   int x, y;
   int index;
   int num;
   int key_type;
   static const int
	   NOT_DONE = -1,
	   SEARCH_KEY_CANCEL = 0,
	   SEARCH_KEY_ID = 1,
	   SEARCH_KEY_NAME = 2,
	   SEARCH_KEY_INDEX = 3;

   // Clear the screen
   setup_screen();

   // Initialize the entire boolean table to false
   memset(table, false, sizeof(table));
   
   // Display the number of students.
   textcolor(NORMAL_COLOR);
   cout << "There are currently ";
   textcolor(HIGHLIGHT_COLOR);
   cout << num_students;
   textcolor(NORMAL_COLOR);
   cout << " students." << endl << endl;

   // Give the user instructions on what to do.
   cout << "Please enter a search key." << endl << endl;
   cout << "Your search key can be a ";
   textcolor(HIGHLIGHT_COLOR);
   cout << "name";
   textcolor(NORMAL_COLOR);
   cout << ',' << endl;
   cout << "an ";
   textcolor(HIGHLIGHT_COLOR);
   cout << "index";
   textcolor(NORMAL_COLOR);
   cout << " (";
   textcolor(TITLE_COLOR);
   cout << "a number from ";
   textcolor(HIGHLIGHT_COLOR);
   cout << '1';
   textcolor(TITLE_COLOR);
   cout << " through ";
   textcolor(HIGHLIGHT_COLOR);
   cout << num_students;
   textcolor(NORMAL_COLOR);
   cout << ")," << endl;
   cout << "or an ";
   textcolor(HIGHLIGHT_COLOR);
   cout << "ID ";
   textcolor(NORMAL_COLOR);
   cout << '(';
   textcolor(TITLE_COLOR);
   cout << "SSN";
   textcolor(NORMAL_COLOR);
   cout << ")." << endl << endl;
   
   // Give the user additional instructions on searching by name
   cout << "If you would like to search by name, you can enter it" << endl;
   cout << "in the following formats:" << endl << endl;
   textcolor(HIGHLIGHT_COLOR);
   cout << "\tFirstName LastName" << endl;
   cout << "\tLastName"; textcolor(NORMAL_COLOR);
   cout << ','; textcolor(HIGHLIGHT_COLOR);
   cout << " FirstName" << endl;
   cout << "\tFirstName" << endl;
   cout << "\tLastName"; textcolor(NORMAL_COLOR);
   cout << ", "; textcolor(MENU_COLOR);
   cout << "<--- Note the trailing comma" << endl << endl;
   textcolor(ERROR_COLOR);
   cout << "\tSearching for a partial name is not supported." << endl;
   cout << "\tAn omitted first or last name means all first or last names." << endl << endl;
   
   // Prompt the user for input
   textcolor(NORMAL_COLOR);
   cout << "Enter search key (";
   textcolor(TITLE_COLOR);
   cout << "leave blank to cancel";
   textcolor(NORMAL_COLOR);
   cout << "): ";

   // Save the current x and y position
   wherexy(x, y);
   
   // Initialize variables
   key_type = NOT_DONE;
      
   // Get the search key from the user
   textcolor(HIGHLIGHT_COLOR);
   getline(cin, search_key);
   textcolor(NORMAL_COLOR);		// Reset to the original color

   // Remove all leading and trailing spaces.
   string_trim(search_key);

   // Check the length of the search key.
   // If it is less than or equal to two,
   // check for the possibility of an index.
   if (search_key.length() <= 2) {

      // If the user entered an empty string,
      // then the user wishes to cancel.
      if (search_key.empty()) {
         key_type = SEARCH_KEY_CANCEL;
	  }

	  // Otherwise, this string has data, so we must process it.
	  else {

   	     // Assume that it's an index.
         key_type = SEARCH_KEY_INDEX;

         // Go through the string
         for (index = 0; index < (int)search_key.length(); index++) {

            // If any characters are not digits,
            // then this is NOT an index.
            if (!isdigit(search_key[index])) {
               key_type = NOT_DONE;		// In that case, we aren't done.
               break;
            }
         }
	  }
   }

   // If we haven't found anything, then keep comparing data.
   if (key_type == NOT_DONE) {

      // If the data looks like it can be an ID, then...
      if (__is_valid_id(search_key)) {

         // Ensure these characters are dashes
         search_key[3] = search_key[6] = '-';

         // Display the correct input
         gotoxy(x, y);
         cout << search_key << endl;

         // We have confirmed that it's an ID number.
         key_type = SEARCH_KEY_ID;
      }

      // Otherwise, we will assume it's a name.
      else {
	     key_type = SEARCH_KEY_NAME;
      }
   }
   
   // Do the search based on the type of search key
   switch (key_type) {

	   // By ID number
	   case (SEARCH_KEY_ID) : {
		   strcpy(title, "ID Matches");

		   // Search for the student.
		   // I'm only searching for the first occurrance
		   // due to the fact that two students should
		   // not have the same Social Security Number.
		   index = Student_search_by_id(student, num_students, search_key);

		   // Display an error message on failure
		   if (index < 0) {
			   textcolor(ERROR_COLOR);
			   cout << "There is no student with an ID of ";
			   textcolor(HIGHLIGHT_COLOR);
			   cout << search_key;
			   textcolor(ERROR_COLOR);
			   cout << " in this file." << endl;

			   // Wait for a keypress
			   textcolor(NORMAL_COLOR);
			   wait_key_msg("\nPress any key to continue . . .");

			   // We're not displaying the table.
			   key_type = SEARCH_KEY_CANCEL;
		   }

		   // Set the element on success.
		   else {
			   table[index] = true;
		   }
		   break;
      }

	   // By name
	   case (SEARCH_KEY_NAME) : {

		   /*
		      Algorithm
		      ---------
		      
		      This search is a little bit more difficult, so
			   I'll have to explain this one a bit.

			   Since I know that all leading and trailing spaces have
			   been removed, that makes my search much easier.

			   If there is a comma in the string, then I know that
			   everything before the comma is part of the last name,
			   and everything after is part of the first name.

			   If there is a space in the string, then I know that
			   everything before the first space is the first name,
			   and everything after the last space is the last name.
			   
			   Searching for a comma will take precedence over searching for a
            space.
			   
			   If a comma is found, everything before the comma is the last name,
			   and everything after the comma is the first name.
			   
			   Otherwise, if a space is found, everything before the space is the
			   first name, and everything after the space is the last name.
			   
			   If neither are found, then the search key assumes that this is the
			   first name.
			   
			   Once these names are found, we then remove all leading and trailing
			   spaces from these strings.
		   */
		   
		   // Initialize variables
		   first_name = "";
		   last_name = "";
		   
		   // Look for a comma
		   index = (int)search_key.find(',');
		   
		   // If a comma was found, get the first and last name.
		   if (index != (int)string::npos) {
            
            // The last name is before the comma
            last_name = search_key.substr(0, index);
            
            // Increment the index for easier calculations
            index++;
            
            // Remove that portion from the search key
            if ((int)search_key.length() >= index) {
               search_key = search_key.substr(index);
            }
            else {
               search_key.clear();  // To ensure I don't get an out of bounds exception
            }
            
            // Anything left over is the first name.
            first_name = search_key;
         }
         
         // Otherwise, try to find a space.
         else if ((index = (int)search_key.find(' ')) != (int)string::npos) {
            
            // The first name is before the space
            first_name = search_key.substr(0, index);
            
            // Increment the index for easier calculations
            index++;
            
            // Remove that portion from the search key
            if ((int)search_key.length() >= index) {
               search_key = search_key.substr(index);
            }
            else {
               search_key.clear();  // To ensure I don't get an out of bounds exception.
            }
            
            // Anything left over is the last name.
            last_name = search_key;
         }
         
         // Otherwise, we have the first name.
         else {
            first_name = search_key;
         }
         
         // Trim leading and trailing spaces from first and last names.
         string_trim(first_name);
         string_trim(last_name);
         
         // Capitalize the first and last names
         first_name[0] = toupper(first_name[0]);
         last_name[0] = toupper(last_name[0]);
         
         // Search for equal names
         num = Student_search_names(student, num_students, first_name, last_name, table);
         
         // If we found some students, then set up the title.
         if (num > 0) {
            
            // Set up the title
            if (first_name.empty()) {
               sprintf(title, "%d match(es) of the last name %s", num,
                       last_name.c_str());
            }
            else if (last_name.empty()) {
               sprintf(title, "%d match(es) of the first name %s", num,
                       first_name.c_str());
            }
            else {
               sprintf(title, "%d match(es) of the name %s", num,
                       (first_name + ' ' + last_name).c_str());
            }
         }
         
         // Otherwise, don't bother showing the data.
         else {
            textcolor(ERROR_COLOR);
            cout << "Name not found." << endl;
            wait_key_msg("Press any key to continue . . .");
            key_type = SEARCH_KEY_CANCEL;
         }         
  		   break;
      }

	   // By index
	   case (SEARCH_KEY_INDEX) : {

		   // This one's easy.  All we have to do
		   // is convert the string to a number.
		   sscanf(search_key.c_str(), " %d", &index);
		   sprintf(title, "Student at Index %d", index);
		   
		   // Decrement the index for easy calculation
		   index--;

		   // Set that element to true.
		   if ((index >= 0) && (index < num_students)) {
            table[index] = true;
         }
         
         // Display an error message if out of range
         else {
            textcolor(ERROR_COLOR);
            cout << "Index out of range." << endl;
            wait_key_msg("Press any key to continue . . .");
            key_type = SEARCH_KEY_CANCEL;
         }
		   break;
      }
   }
   
   // Display the data if we need to.
   if (key_type != SEARCH_KEY_CANCEL) {
      Display_Student_Table_T(title, student, num_students, table, false);
   }
}

// display_table_header ///////////////////////////////////
inline void display_header(void)
{ // Displays the header for the table

   // Title
   cout << left;
   textcolor(TITLE_COLOR);
   cout << setw(4)  << '#';
   cout << setw(35) << "Name";
   cout << setw(15) << "ID Number";
   cout << setw(10) << "Average";
   cout << setw(10) << "Grade" << endl;
   
   // Separator
   textcolor(NORMAL_COLOR);
   cout << setfill('-') << setw(79) << '-' << setfill(' ') << endl;
   
   // Clean up my mess
   cout << right;
}

// Displays a student
void Display_Student(Student *student, int count, bool highlight)
{ // Displays a single student in table format
   static const int NAME_HIGHLIGHT = 14 + (1 << 4);
   string name;
   double average;
            
   // Display the student number
   textattr(highlight ? NAME_HIGHLIGHT : HIGHLIGHT_COLOR);
   cout << showpoint << setw(4) << setprecision(0) << fixed;
   cout << (float)(count + 1) << noshowpoint;
   
   // Get the name
   name = student[count].last_name;
   name += ", ";
   name += student[count].first_name;
   name.resize(32);     // Restrict the name to 30 characters, considering the space and comma
   
   // Display the name
   textattr(highlight ? NAME_HIGHLIGHT : MENU_COLOR);
   cout << setw(35) << name;
   
   // Display the ID
   cout << setw(15) << student[count].id;
   
   // Get the average
   average = Student_get_average(student[count]);
   
   // If there's an error, display an error message
   if (average < 0.0) {
      textattr(highlight ? NAME_HIGHLIGHT : ERROR_COLOR);
      cout << "Error in test score data";
   }
   
   // Otherwise, display the average and grade.
   else {
      
      // Set the precision to two decimal places
      cout << setprecision(2);
      
      // Display the average
      textattr(highlight ? NAME_HIGHLIGHT : HIGHLIGHT_COLOR);
      cout << setw(10) << average;
      
      // Display the grade
      cout << setw(10) << LETTER_GRADES[GetGradeIndex(average)];
   }
   
   // Set the background back to black if we were highlighting
   if (highlight) {
      textbackground(0);
   }
   
   // Go to the next line
   cout << endl;
}

// Display_Student_Table_E ////////////////////////////////
int Display_Student_Table_E(string title, Student *student, int num_students, int start, int end, bool select, int start_choice)
{ /*
   Displays the students in a table format by element
   
   This isn't a really basic function, so I'll begin by explaining its purpose.
   
   Basically, you have two options.  An option to just display the students
   as is, and another option to treat the list as a menu in which you can
   select an individual student.
   
   I have two flags for updating the screen.  One to indicate that the
   data on the screen needs to be refreshed, and another flag to explicitly
   state that the screen needs to be cleared.  The reason for the screen
   clearing flag is because for each page, I know the same data is going to
   be displayed, and if I'm not going to change anything, then there is no
   point to clear the screen, because all that will do is cause the screen
   to flicker, and that just looks ugly.
   
   The order for the starting and ending elements is IMPORTANT, so I want to
   make sure that start is always less than end.  Since I'm dealing with
   an array, I also want to make sure that these values are always within
   bounds of the elements of the array.
   
   The list will be displayed in a loop, in which the program will wait
   for the user to press a key.  There are only two ways out of this function,
   by the user pressing Enter or Escape.  If the select flag is false, then
   the Enter, Up, and Down keys are disabled.
   
   During each update of the screen, the title, the header, and the students
   are displayed.  If the select flag is true, then the current choice is
   highlighted.  The user can then use the Up, Down, and Enter keys to make
   a selection this way.
   
   If the user presses Enter, then the element of the given student is
   returned, and if Escape is pressed, then -1 is returned.
*/
   bool done, refresh, clear_screen;
   int key, sub_start, sub_end, count, choice;
   int LAST_ELEMENT = num_students - 1;
   string message;
   
   // Ensure start is less than end
   if (start > end) {
      key = start;
      start = end;
      end = key;
   }
   
   // Keep the bounds in check
   start = (start < 0) ? (0) : start;
   end = (end > LAST_ELEMENT) ? (LAST_ELEMENT) : end;
   
   // Get the starting choice
   if (select) {
	   if (start_choice < start) {
		   start_choice = start;
	   }
	   else if (start_choice > end) {
		   start_choice = end;
	   }
   }
   else {
	   start_choice = -1;
   }
   
   // Set the selection cursor
   choice = start_choice;

   // Make sure the right start and end are chosen.
   //
   // The lazy way!!! ^_^
   do {

	   // Set the beginning and ending elements
	   sub_start = start;
	   sub_end = sub_start + (ENTRIES_PER_SCREEN - 1);
	   
	   // Make sure the ending is in bounds
	   if (sub_end > end) {
		   sub_end = end;
	   }
   } while ((choice < sub_start) || (choice > sub_end));

   // Initialize flags
   done = false;           // This puts us in the loop
   refresh = true;         // I want to refresh the screen.
   clear_screen = true;    // I want to clear the screen as well.
   
   // Display the data
   while (!done) {
      
      // If we need to refresh the screen, then do it.
      if (refresh) {

         // Clear the screen
         if (clear_screen) {
            setup_screen();
            clear_screen = false;
         }
         
         // Otherwise, position the cursor at the upper left hand corner
         else {
            gotoxy(1, 1);
         }
   
         // Display the title if we have one
         if (!title.empty()) {
            textcolor(TITLE_COLOR);
            cout << title << endl << endl;
         }
         
         // Display the table header
         display_header();
         
         // Display the table entries
         cout << left;
         for (count = sub_start; count <= sub_end; count++) {
            Display_Student(student, count, choice == count);
         }
         cout << right;
         
         // Set up the message
         message = "Esc - Back to Menu";
         if (sub_start != start) {
            message += " / Left - Previous";
         }
         if (sub_end != end) {
            message += " / Right - Next";
         }
         if (select) {
            message += '\n';
            message += "Up or Down - Select Student / Enter - Confirm Selection";
         }
         
         // Display the message
         textcolor(NORMAL_COLOR);
         cout << endl << message;
         
         // Okay, the screen has been refreshed,
         // so we don't need to refresh it.
         refresh = false;
      }
      
      // Wait for a keypress
      clear_key();
      key = wait_key();
      
      // If the user presses Escape, then we're done
      if (key == 27) {
         choice = -1;      // To signify that the user pressed escape
         done = true;
      }
      
      // If the user presses left, then go back
      else if (key == ('K' << 8)) {
         
         // Only do this if the start is past the first element.
         if (sub_start > start) {
            
            // Move the elements
            sub_start -= ENTRIES_PER_SCREEN;
            if (sub_start < start) {
               sub_start = start;
            }
            sub_end = sub_start + (ENTRIES_PER_SCREEN - 1);
            
            // Reset the choice
            choice = (select) ? (sub_start) : (-1);
            
            // Yes, the screen needs to be refreshed.
            refresh = true;
            clear_screen = true;
         }
      }
      
      // If the user presses right, then go forward
      else if (key == ('M' << 8)) {
         
         // Only do this if the end before the last element.
         if (sub_end < end) {
            
            // Move the elements
            sub_end += ENTRIES_PER_SCREEN;
            if (sub_end > end) {
               sub_end = end;
            }
            sub_start = sub_end - (ENTRIES_PER_SCREEN - 1);
            
            // Reset the choice
            choice = (select) ? (sub_start) : (-1);
            
            // Yes, the screen needs to be refreshed.
            refresh = true;
            clear_screen = true;
         }
      }
      
      // If the user is allowed to make a selection, then look for up, down, and enter.
      else if (select) {
         
         // Enter
         if (key == 13) {
            done = true;
         }
         
         // Up
         else if (key == ('H' << 8)) {
            
            // Decrement the choice and keep it in bounds
            choice--;
            if (choice < sub_start) {
               choice = sub_start;
            }
            else {
               refresh = true;
            }
         }
         
         // Down
         else if (key == ('P' << 8)) {
            
            // Increment the choice and keep it in bounds
            choice++;
            if (choice > sub_end) {
               choice = sub_end;
            }
            else {
               refresh = true;
            }
         }
      }
   }
   
   // Return the user's selection.
   return choice;
}

// __find_true ////////////////////////////////////////////
int __find_true(bool *table, int start, int end, int after)
{ // Returns the next true element
   int index, count;
   
   // We want after the after-th occurrance.
   after++;
   
   // Search through the array
   count = 0;
   for (index = start; index <= end; index++) {
      
      // If we find a true, increment the counter.
      count += table[index];
      
      // If we made it to the correct occurrance,
      if (count == after) {
         break;   // then we have found our index!
      }
   }
   
   // If we made it past the array, then return
   // a negative number to indicate the search failed.
   if (index > end) {
      return -1;
   }
   
   // We got here if everything was okay.
   return index;
}

// __find_true_back ///////////////////////////////////////
int __find_true_back(bool *table, int end, int start, int after)
{ // Returns the next true element, going backwards from end to start
   int index, count;
   
   // We want after the after-th occurrance.
   after++;
   
   // Search through the array
   count = 0;
   for (index = end; index >= start; index--) {
      
      // If we find a true, increment the counter.
      count += table[index];
      
      // If we made it to the correct occurrance,
      if (count == after) {
         break;   // then we have found our index!
      }
   }
   
   // If we made it past the array, then return
   // a negative number to indicate the search failed.
   if (index < start) {
      return -1;
   }
   
   // We got here if everything was okay.
   return index;
}

// Display_Student_Table_T ////////////////////////////////
int Display_Student_Table_T(string title, Student *student, int num_students, bool *table, bool select)
{ // Displays the students in a table format by a boolean table
   bool done, refresh;
   int key, start, end, sub_start, sub_next, count, index;
   int LAST_ELEMENT = num_students - 1;
   string message;
   
   // Search for a starting element
   sub_start = start = __find_true(table, 0, LAST_ELEMENT, 0);
   if (start < 0) {
      return -1;     // If there are no true values, we have nothing to display.
   }
   
   // Search for the ending element
   end = __find_true_back(table, LAST_ELEMENT, start, 0);
   
   // Put us in the loop
   refresh = true;      // To ensure that the screen gets refreshed
   done = false;   
   
   // Display the data
   while (!done) {
      
      // If we need to refresh the screen, then do it.
      if (refresh) {

         // Clear the screen
         setup_screen();
   
         // Display the title if we have one
         if (!title.empty()) {
            textcolor(TITLE_COLOR);
            cout << title << endl << endl;
         }
         
         // Display the table header
         display_header();
         
         // Start our counter at zero.
         count = 0;
         
         // Display the table entries
         cout << left;
         for (index = sub_start; index <= end; index++) {
            
            // Display this entry if it should be displayed
            if (table[index]) {
               Display_Student(student, index, false);
               count++;    // Keep track of it
               
               // If we've reached our limit, then leave.
               if (count == ENTRIES_PER_SCREEN) {
                  break;
               }
            }
         }
         cout << right;
         
         // Set up the message
         message = "Esc - Back to Menu";
         if (sub_start != start) {
            message += " / Left - Previous";
         }
         if (index < end) {
            message += " / Right - Next";
         }
         
         // Display the message
         textcolor(NORMAL_COLOR);
         cout << endl << message;
         
         // Okay, the screen has been refreshed,
         // so we don't need to refresh it.
         refresh = false;
      }
      
      // Wait for a keypress
      clear_key();
      key = wait_key();
      
      // If the user presses Escape, then we're done
      if (key == 27) {
         done = true;
      }
      
      // If the user presses left, then go back
      else if (key == ('K' << 8)) {
         
         // Only do this if the start is past the first element.
         if (sub_start > start) {
            
            // Move the start element backwards
            sub_start = __find_true_back(table, sub_start, start, ENTRIES_PER_SCREEN - 1);
            
            // Yes, the screen needs to be refreshed.
            refresh = true;
         }
      }
      
      // If the user presses right, then go forward
      else if (key == ('M' << 8)) {
         
         // Look for the next true
         sub_next = __find_true(table, sub_start, LAST_ELEMENT, ENTRIES_PER_SCREEN - 1);
         
         // If we received a valid element, then update it.
         if (sub_next >= 0) {
            
            // Update the element
            sub_start = sub_next;
         
            // Yes, the screen needs to be refreshed.
            refresh = true;
         }
      }
   }
   
   // Return -1 to signify we pressed Escape.
   return -1;
}

// Show_Students_Under_Class_Average //////////////////////
void Show_Students_Under_Class_Average(Student *student, const int ARRAY_MAX, int num_students)
{ // Shows the students that are under the class average
   int num;
   bool compare_table[STUDENT_MAX];
   char message[40];
   
   // Get the number of students under the class average
   num = NumStudentsUnderClassAverage(student, num_students, compare_table);

   // If we have students, then process them.
   if (num > 0) {

      // Make sure everything is grammatically correct
      if (num == 1) {
         sprintf(message, "Student Under Class Average");
      }
      else {
         sprintf(message, "%d Students Under Class Average", num);
      }

      // Display the table
      Display_Student_Table_T(message, student, num_students, compare_table, false);
   }
   
   // On the RARE occurrance that we don't, display a message.
   else {
      textcolor(ERROR_COLOR);
      cout << "No students are below the class average." << endl << endl;
      textcolor(NORMAL_COLOR);
      wait_key_msg("Press any key to continue . . .");
   }
}

// Input_Student //////////////////////////////////////////
bool Input_Student(Student &s)
{ // Gets a student from standard input
   int count, count2, num_tests, num_decimals, current_field;
   int x, y, result;
   bool done, refresh, clear_screen;
   static const int GRAYED = 8, GRAYED_BAD = 4, INFO_SIZE = 5 + MAX_TESTS;
   static const int INPUT_GOOD = HIGHLIGHT_COLOR + (1 << 4), INPUT_BAD = 14 + (4 << 4);
   float fl;
   
   // To make things easier on me, I'll use a parallel array.
   input_info info[INFO_SIZE];
   string buffer[INFO_SIZE];
   char text[15];
   
   // Initialize each buffer.
   buffer[0] = s.first_name;
   buffer[1] = s.last_name;
   buffer[2] = s.id;
   sprintf(text, "%.2f", (s.quiz < 1000.00) ? (s.quiz) : (999.99));
   buffer[3] = text;
   sprintf(text, "%d", s.num_tests);
   buffer[4] = text;
   for (count = 0; count < MAX_TESTS; count++) {
      sprintf(text, "%.2f", (s.test_scores[count] < 1000.00) ? (s.test_scores[count]) : (999.99));
      buffer[5 + count] = text;
   }
   
   // Get the number of tests
   num_tests = s.num_tests;

   // Save the x and y position.
   x = 1;
   y = 3;
   
   // Initialize the input info.
   for (count = 0; count < INFO_SIZE; count++) {
      
      // Initialize the fields
      info[count].px      = x;
      info[count].py      = y + count;
      info[count].x       = x + 30;
      info[count].y       = y + count;
      info[count].attr    = INPUT_GOOD;
      info[count].numeric = (count >= 3);
      info[count].integer = (count == 4);
      info[count].is_id   = (count == 2);
      info[count].grayed  = GRAYED;
      info[count].good    = true;
      
      // Initialize the test score prompts
      if (count >= 5) {
         info[count].prompt = "Test #";
         info[count].prompt += (char)('0' + (count - 4));   // Convert number to character
         info[count].prompt += ':';
      }
      
      // Initialize the visible fields for the scores
      if (info[count].numeric) {
         info[count].vis_len = (info[count].integer) ? 1 : 6;
         info[count].max_len = info[count].vis_len;
      }

      // Set the visibility
      info[count].visible = !(info[count].numeric && (count >= 5));
      
      // Set the allowed characters
      if (info[count].numeric) {
         if (info[count].integer) {
            info[count].allowed = "012345";
         }
         else {
            info[count].allowed = ".0123456789";
         }
      }
      else if (info[count].is_id) {
         info[count].allowed = "-0123456789";
      }
      else {
         info[count].allowed = "";
      }
   }   

   // Set up the input info.  
   info[0].prompt  = "First Name:";
   info[0].vis_len = 30;
   info[0].max_len = 30;
   
   info[1].prompt  = "Last Name:";
   info[1].vis_len = info[0].vis_len;
   info[1].max_len = info[0].max_len;
   
   info[2].prompt  = "Student ID (";
   info[2].vis_len = 11;
   info[2].max_len = 11;
   
   info[3].prompt  = "Quiz Score:";
   
   info[4].prompt  = "Number of Tests:";
   
   // Update the visibility of the tests
   for (count = 0; count < MAX_TESTS; count++) {
      if (count < num_tests) {
         info[5 + count].visible = true;
      }
      else {
         info[5 + count].visible = false;
         info[5 + count].good = true;
      }
   }
      
   // Edit the fields
   done = false;
   refresh = true;
   clear_screen = true;
   current_field = 0;
   while (!done) {
      
      // Refresh the screen if necessary
      if (refresh) {
         
         // If the screen needs to be cleared
         if (clear_screen) {
            setup_screen();
            clear_screen = false;
         }
         else {
            gotoxy(1, 1);
         }
         
         // Give the user instructions
         textcolor(NORMAL_COLOR);
         cout << "Up or Down - Navigate through fields / Enter - Confirm / Escape - Cancel";
         
         // Display the prompts
         for (count = 0; count < INFO_SIZE; count++) {
            
            // Display the field if it's visible
            if (info[count].visible) {
               gotoxy(info[count].px, info[count].py);
               textcolor(MENU_COLOR); cout << info[count].prompt;
               gotoxy(info[count].x, info[count].y);
               textcolor(info[count].grayed); cout << left << setw(info[count].vis_len) << buffer[count];
               
               // Finish the rest of the SSN field
               if (info[count].is_id) {
                  gotoxy(info[count].px + (int)info[count].prompt.length(), info[count].py);
                  textcolor(TITLE_COLOR); cout << "SSN";
                  textcolor(MENU_COLOR);  cout << "):";
               }
            }
         }
         
         // The screen has been refreshed.
         refresh = false;
      }
      
      // Set the cursor position and the proper attribute
      gotoxy(info[current_field].x, info[current_field].y);
      textattr(info[current_field].attr);
         
      // Get the input for the current field
      result = get_text_line(
         false,
         buffer[current_field],
         info[current_field].vis_len,
         info[current_field].max_len,
         ' ', GTL_ABORT_ESCAPE_UP_DOWN,
         info[current_field].allowed
      );
      
      // Change the visibility of test scores if needed
      if (info[current_field].numeric && info[current_field].integer) {
         
         // If it's an empty string, assume zero.
         if (buffer[current_field].empty()) {
            num_tests = 0;
         }
         
         // Otherwise, get the number of tests.
         else {
            sscanf(buffer[current_field].c_str(), " %d", &num_tests);
         }
         
         // The screen needs to be cleared now.
         clear_screen = true;
      }
      
      // Reset the background back to black
      textbackground(0);
      
      //
      // Ensure validity in each field
      //
      
      // Search through each field
      for (count = 0; count < INFO_SIZE; count++) {
         
         // If the string is not empty, let's see what it has.
         if (!buffer[count].empty()) {
            
            // If it's the social security number, ensure that it's valid
            if (info[count].is_id) {
               info[count].good = is_valid_id(buffer[count]);
            }
            else {
               
               // Start by assuming it's good if it's not empty
               info[count].good = !buffer[count].empty();
               
               // If it's a number that could possibly hold
               // non-integer data, make sure there is only
               // ONE decimal point at most.
               if (info[count].numeric && !info[count].integer) {
                  
                  // We haven't counted anything yet
                  num_decimals = 0;
                  
                  // Go through the entire string
                  for (count2 = 0; count2 < (int)buffer[count].length(); count2++) {
                     num_decimals += (buffer[count][count2] == '.');
                  }
                  
                  // If there is more than one, then we have a problem.
                  info[count].good = (num_decimals <= 1);
               }
            }
         }
         
         // If it's empty, the data is bad.
         else {
            info[count].good = false;
         }
         
         // Change colors accordingly
         if (info[count].good) {
            info[count].grayed = GRAYED;
            info[count].attr = INPUT_GOOD;
         }
         else {
            info[count].grayed = GRAYED_BAD;
            info[count].attr = INPUT_BAD;
         }
      }
      
      // Update the visibility of the tests
      for (count = 0; count < MAX_TESTS; count++) {
         if (count < num_tests) {
            info[5 + count].visible = true;
         }
         else {
            info[5 + count].visible = false;
            info[5 + count].good = true;
         }
      }
      
      // If the user presses escape, there's no point in doing anything.
      if (result == 27) {
         return false;
      }
      
      // If the user presses enter, then we are done.
      else if (result == 13) {
         
         // Assume that we are done.
         done = true;
         
         // Don't allow the user to leave if they're on the quiz scores
         if (info[current_field].numeric && info[current_field].integer) {
            done = false;
         }
         
         // Don't leave if anything is bad
         for (count = 0; count < INFO_SIZE; count++) {
            
            // If something is bad,
            if (!info[count].good) {
               done = false;  // then don't let the user leave.
            }
         }
      }
      
      // If the user presses up, then move to the previous field.
      else if (result == ('H' << 8)) {
         
         // Wrap around.
         do {
            current_field--;
            if (current_field < 0) {
               current_field = INFO_SIZE - 1;
            }
         } while (!info[current_field].visible);
         refresh = true;
      }
      
      // If the user presses down, then move to the next field.
      else if (result == ('P' << 8)) {
         
         // Wrap around
         do {
            current_field++;
            if (current_field >= INFO_SIZE) {
               current_field = 0;
            }
         } while (!info[current_field].visible);
         refresh = true;
      }
   }
   
   // Update the student
   s.first_name = buffer[0];
   s.last_name = buffer[1];
   s.id = buffer[2];
   sscanf(buffer[3].c_str(), " %f", &fl); s.quiz = floor(((double)fl * 100.0) + 0.5) * 0.01;
   s.quiz = (s.quiz < 1000.00) ? (s.quiz) : (999.99);
   sscanf(buffer[4].c_str(), " %d", &s.num_tests);
   for (count = 0; count < MAX_TESTS; count++) {
      
      // If we're in the range of tests taken, update the score.
      if (count < s.num_tests) {
         sscanf(buffer[5 + count].c_str(), " %f", &fl);
         s.test_scores[count] = floor(((double)fl * 100.0) + 0.5) * 0.01;
         s.test_scores[count] = (s.test_scores[count] < 1000.00) ? (s.test_scores[count]) : (999.99);
      }
      
      // Otherwise, reset it to zero.
      else {
         s.test_scores[count] = 0.0;
      }
   }
   
   // Return success
   return true;
}

// Show_Edit_Students /////////////////////////////////////
void Show_Edit_Students(Student *student, int num_students, bool &changes_made)
{ // Shows the user a table of students while giving the option of editing a student
   int index;
   
   // Stay in here until the user decides to cancel
   index = 0;  // To put us in the loop
   while (index >= 0) {
      
      // Get a student
      index = Display_Student_Table_E("Current List of Students - Select a student to edit / Esc to cancel", student, num_students, 0, num_students - 1, true, index);
      
      // Edit the student if we have made a selection
      if (index >= 0) {
         
         // If we want to keep the input,
         if (Input_Student(student[index])) {
            
            // Then record that changes have been made.
            changes_made = true;
         }
      }
   }
}

// Add_Student ////////////////////////////////////////////
void Add_Student(Student *student, int &num_students, bool &changes_made)
{ // Adds a student to the list
   Student s;
   
   // Initialize the student
   Student_init(s);
   
   // Get the student
   if (Input_Student(s)) {
      
      // Add the student
      Student_add(student, num_students, s);
      
      // Okay, changes have been made.
      changes_made = true;
   }
}

// Drop_Student ///////////////////////////////////////////
void Drop_Student(Student *student, int &num_students, bool &changes_made)
{ // Drops a student from the list
   int index, num_before_drop = num_students;

   // Keep dropping students while the user wants the action to be done.
   index = 0;  // To put us in the loop
   while (index >= 0 && num_students) {

	   // Make sure the index stays in bounds
	   if (index >= num_students) {
		   index = num_students - 1;
	   }
      
      // Get the choice from the user
      index = Display_Student_Table_E("Drop a Student - Enter to Drop - Escape to Cancel", student, num_students, 0, num_students - 1, true, index);
      
      // Drop the student
      // This function quietly does nothing if the index is out of bounds.
      Student_drop(student, num_students, index);
   }
   
   // Notify the program that changes have
   // been made, if they have been.
   if (num_before_drop > num_students) {
      changes_made = true;    // Explicitly set it to true to avoid flip-flopping
   }
}

// Drop_All_Students //////////////////////////////////////
void Drop_All_Students(Student *student, int &num_students, bool &changes_made)
{ // Drops all students
   int yn;

   // Give the user a warning message
   setup_screen();
   textcolor(ERROR_COLOR);
   cout << "WARNING!!!!" << endl << endl;
   textcolor(TITLE_COLOR);
   cout << "This will drop "; textcolor(HIGHLIGHT_COLOR);
   cout << "ALL "; textcolor(TITLE_COLOR);
   cout << "students from this data file." << endl;
   
   // Ask the user for input.
   yn = wait_key_msg("Are you sure you want to do this (y/N)? ");
   
   // Only drop the students if the user explicitly says to do so.
   if (tolower(yn) == 'y') {
      
      // Drop all students
      Student_drop_all(student, num_students);
      changes_made = true;    // Since changes HAVE been made.
   }
}
