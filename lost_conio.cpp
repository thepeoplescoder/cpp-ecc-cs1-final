/*
   lost_conio.cpp
   
   Functions to emulate the conio.h functions
   from back in the day :)

   Everything is done by making direct Windows API calls.
   
   Austin Cathey
*/

// Disable deprecation

// Includes
#include <windows.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "lost_conio.hpp"

// Use standard namespace
using namespace std;

// Keys that you can use to leave get_text_line.
const int GTL_ABORT_ESCAPE[] = {1, 27};
const int GTL_ABORT_ESCAPE_UP_DOWN[] = {3, 27, ('H' << 8), ('P' << 8)};

// This is so none of my other routines can readily access this variable
static /* volatile const */ bool __initialized = false;

//////////////////////
//                  //
// Inline functions //
//                  //
//////////////////////

// StdOut__ ///////////////////////////////////////////////
inline HANDLE StdOut__(void)
{ // Returns the handle to standard output
   return GetStdHandle(STD_OUTPUT_HANDLE);
}

//////////////////////////
//                      //
// Function Definitions //
//                      //
//////////////////////////

// lost_conio_init ////////////////////////////////////////
void lost_conio_init(void)
{ // Initializes my Lost Conio package

   // Leave if we have already initialized
   if (__initialized) {
      return;
   }
   
   // Register the shutdown routine as an exit() function
   atexit(lost_conio_shutdown);
   
   // Okay, we have been initialized
   __initialized = true;
}

// lost_conio_shutdown ////////////////////////////////////
void lost_conio_shutdown(void)
{ // The shutdown routine for my Lost Conio package
   
   // Leave if we have already de-initialized
   if (!__initialized) {
      return;
   }
   
   // Set the text attribute to white
   textattr(7);
   
   // Okay, we have de-initialized
   __initialized = false;
}

// clrscr /////////////////////////////////////////////////
void clrscr(void)
{ // Clears the screen
   HANDLE hStdOut;
   CONSOLE_SCREEN_BUFFER_INFO csbi;
   DWORD num;
   int size;
   static COORD upper_left = {0, 0};
   
   // Get the handle to standard output
   hStdOut = StdOut__();
   
   // Get the console information
   GetConsoleScreenBufferInfo(hStdOut, &csbi);
   size = csbi.dwSize.X * csbi.dwSize.Y;
   
   // Clear the screen with spaces
   FillConsoleOutputCharacter(hStdOut, ' ', size, upper_left, &num);
   FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, size, upper_left, &num);
   
   // Position the cursor at the upper left hand corner of the screen
   SetConsoleCursorPosition(hStdOut, upper_left);
}

// gotoxy /////////////////////////////////////////////////
void gotoxy(int x, int y)
{ /*
   Sets the cursor position to column x, and row y.
   
   (1, 1) is the upper left hand corner.

   Technically, (0, 0) is the upper left hand corner,
   so the necessary conversion must be made.
*/
   COORD coords;
   
   // Convert the position
   coords.X = x - 1;
   coords.Y = y - 1;
   
   // Set the cursor position
   SetConsoleCursorPosition(StdOut__(), coords);
}

// wherexy ////////////////////////////////////////////////
void wherexy(int *x, int *y)
{ /*
  Gets the current x and y coordinates - (1, 1) is upper left

  Technically, (0, 0) is the upper left hand corner,
  so the necessary conversion must be made.
*/
   CONSOLE_SCREEN_BUFFER_INFO csbi;
   
   // Get the console information
   GetConsoleScreenBufferInfo(StdOut__(), &csbi);
   
   // Store the cursor position
   if (x) { *x = csbi.dwCursorPosition.X + 1; }
   if (y) { *y = csbi.dwCursorPosition.Y + 1; }
}

// wherex /////////////////////////////////////////////////
int wherex(void)
{ // Returns the current x position (column)
   int x;
   
   // Get the x position and return it
   wherexy(&x, NULL);
   return x;
}

// wherey /////////////////////////////////////////////////
int wherey(void)
{ // Returns the current y position (row)
   int y;
   
   // Get the y position and return it
   wherexy(NULL, &y);
   return y;
}

// get_textattr ///////////////////////////////////////////
int get_textattr(void)
{ /*
  Returns the current text attribute

  This has to be retrieved from the system's console buffer information.
*/
   CONSOLE_SCREEN_BUFFER_INFO csbi;
   
   // Get the information
   GetConsoleScreenBufferInfo(StdOut__(), &csbi);
   
   // Return the information
   return csbi.wAttributes;
}

// textattr ///////////////////////////////////////////////
void textattr(int attr)
{ // Changes the text color attribute
   SetConsoleTextAttribute(StdOut__(), attr & 0xFF);
}


// textcolor //////////////////////////////////////////////
void textcolor(int color)
{ // Sets the foreground color

   // The bitmask needed to preserve the background color
   static int MASK = 7 << 4;
   
   // Variables
   int attr;
   
   // Get the color attribute and save the background color
   attr = get_textattr() & MASK;
   
   // Ensure that we only have the 5 least significant bits.
   // This turns off the blink bit, and disregards the background.
   color &= 31;
   
   // If the fifth bit is set, then set the eigth bit.
   // (i.e. turn ON the blink bit.)
   if (color & 16) {
      color |= 128;  // Turn on the eighth bit.
      color ^= 16;   // Turn off the fifth bit since we moved it to the eigth.
   }
   
   // Set the new text color.
   textattr(color | attr);
}

// textbackground /////////////////////////////////////////
void textbackground(int color)
{ // Sets the background color

   // The bitmask needed to preserve the foreground color
   static int MASK = 128 + 15;
   
   // Variables
   int attr;
   
   // Get the color attribute and save the foreground color
   attr = get_textattr() & MASK;

   // We only need the first 3 bits.
   color &= 7;
   
   // Shift this color past the foreground bits.
   color <<= 4;
   
   // Set the new attribute
   textattr(color | attr);
}

// show_console_cursor ////////////////////////////////////
void show_console_cursor(bool flag)
{ // Shows the cursor
   CONSOLE_CURSOR_INFO cci;
   
   // Get the cursor information for standard output
   // This maintains the cursor size.
   GetConsoleCursorInfo(StdOut__(), &cci);
   
   // Set the visible flag to false
   cci.bVisible = flag;
   
   // Set the new information
   SetConsoleCursorInfo(StdOut__(), &cci);
}

// get_key ////////////////////////////////////////////////
int get_key(void)
{ // Gets a key, considering extended keys
   union {
      int w;
      unsigned char b[sizeof(int)];
   } key;
   
   // Initialize the value to zero
   key.w = 0;
   
   // If a key was pressed,
   if (kbhit()) {
      key.b[0] = getch();        // then get it.
      if (kbhit()) {             // If there is still a key in the buffer,
         key.b[1] = getch();     // then it's an extended key.
         key.b[0] = 0;           // For compatibility with DOS key codes
      }
   }
   
   // Return the key
   return key.w;
}

// clear_key //////////////////////////////////////////////
int clear_key(void)
{ // Clears the keyboard buffer, returning how many keys were cleared
   int cleared_keys = 0;
   
   // get_key() returns a nonzero value if a key is waiting in the buffer.
   while (get_key()) {
      cleared_keys++;   // Increment the count
   }
   
   // Return the number of keys cleared
   return cleared_keys;
}

// wait_key ///////////////////////////////////////////////
int wait_key(void)
{ // Waits for a key to be pressed
   int k;
   while (!(k = get_key())) {}
   return k;
}

// get_text_line //////////////////////////////////////////
int get_text_line(bool newline, string &buffer, int vis_len, int max_len, char space_char, const int *abort_table, string allowed)
{ // This function reads a line of text from the keyboard
   int x, y;               // This will save the current cursor position.
   int abort_table_size;   // This will tell me how many elements are in my abort table.
   bool done;              // This will let us know if we're done or not.
   int key;                // This holds the key that was currently pressed.
   int pos;                // The position in the string.
   int len;                // The current length of the string.
   char old_fill;          // This saves the old fill character.
   
   // Get the current cursor position
   wherexy(&x, &y);
   
   // Get the size of the abort table
   if (abort_table) {
      abort_table_size = *abort_table;
      abort_table++;                      // Point to the first key in the table
   }
   else {
      abort_table_size = 0;
   }
   
   // At least ONE character must be shown.
   if (vis_len < 1) {
      vis_len = 1;
   }
   
   // Restrict visibility to 50 characters.
   // I just feel like choosing 50.
   else if (vis_len > 50) {
      vis_len = 50;
   }
   
   // The maximum length has to at least be the visible length
   if (max_len < vis_len) {
      max_len = vis_len;
   }
   
   // Convert non-printable space_characters to a space.
   if (isspace(space_char)) {
      space_char = ' ';
   }
   
   // Get the old fill character
   old_fill = cout.fill();
   
   // We just started
   done = false;
   
   // Clear the keyboard buffer
   clear_key();
   
   // Get the input!
   while (!done) {
      
      // Position the cursor
      gotoxy(x, y);
      
      // Get the length of the string
      len = buffer.length();
      
      // Hide the cursor
      show_console_cursor(false);
      
      // If it's less than the visible length, then display it as is.
      if (len < vis_len) {
         cout << buffer;
         cout << setfill(space_char) << setw(vis_len - len) << space_char;
         gotoxy(x + len, y);
      }
      
      // Otherwise, display the appropriate substring.
      else {
         
         // Use a for loop to keep from wasting memory
         for (int count = 0; count < vis_len; count++) {
            cout << buffer[(len - vis_len) + count];
         }
      }
      
      // Show the cursor
      show_console_cursor(true);
      
      // Wait for a keypress
      key = wait_key();
      
      // Backspace key
      if (key == 8) {
         
         // Only process this key if the length of the string is greater than 0.
         if (len > 0) {
            buffer.resize(len - 1);
         }
      }
      
      // Any printable character
      else if (
         (allowed.empty() && ((key >= ' ') && (key <= '~'))) ||
         (!allowed.empty() && (allowed.find((char)key) != string::npos))
      ) {
         
         // Only process this key if the length is less than the maximum.
         if (len < max_len) {
            buffer += (char)key;
         }
      }
      
      // If the user can only 
      
      // The enter key
      else if (key == 13) {
         done = true;
      }
      
      // The abort keys
      else if (abort_table_size > 0) {
         
         // Go through the table
         for (int count = 0; count < abort_table_size; count++) {
            
            // If we found one of the abort keys, then we're done.
            if (key == abort_table[count]) {
               done = true;
            }
         }
      }
   }
   
   // Reset the fill character
   cout << setfill(old_fill);
   
   // If we are to output a newline, then do so.
   if (newline) {
      cout << endl;
   }
   
   // Return the key that was pressed to get out of here.
   return key;
}
