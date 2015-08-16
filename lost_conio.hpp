/*
   lost_conio.hpp
   
   Header file for prototypes containing functions
   that emulate the old DOS conio.h routines.
   
   Austin Cathey
*/

// Make sure this file wasn't already included
#ifndef __LOST_CONIO_HPP__
#define __LOST_CONIO_HPP__

// Include <cstdlib> for NULL
#include <cstdlib>
#include <string>

// Use standard namespace
using namespace std;

// Yes, we are using my Lost Conio.h routines
#define USING_LOST_CONIO

// Special arrays
extern const int GTL_ABORT_ESCAPE[];
extern const int GTL_ABORT_ESCAPE_UP_DOWN[];

// Constants
const int GTL_SUCCESS = 13;   // Code for the Enter key
const int GTL_ESCAPE = 27;    // Code for the Escape key

// Initialization/Shutdown Prototypes
void lost_conio_init(void);
void lost_conio_shutdown(void);

// Prototypes
void clrscr(void);
void gotoxy(int x, int y);
void wherexy(int *x, int *y);
int wherex(void);
int wherey(void);
int get_textattr(void);
void textattr(int attr);
void textcolor(int color);
void textbackground(int color);

// Functions that build on conio.h
// Just things I've added
void show_console_cursor(bool flag);
int get_key(void);
int clear_key(void);
int wait_key(void);
int get_text_line(bool newline, string &buffer, int vis_len, int max_len, char space_char, const int *abort_table = GTL_ABORT_ESCAPE, string allowed = "");

//////////////////////
//                  //
// Inline Functions //
//                  //
//////////////////////

// wherexy ////////////////////////////////////////////////
inline void wherexy(int &x, int &y)
{ // Gets the current x and y coordinates - (1, 1) is upper left
   wherexy(&x, &y);
}

#endif      // __LOST_CONIO_HPP__
