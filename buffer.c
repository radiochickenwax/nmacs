/*
  Sat Dec 20, 2014 22:03:02 
  Really striving for maximum simplicity here. 

  Just an array of strings that can be printed on ncurses to test scrolling.

  This version uses a plain GArray
 */
#include <ncurses.h>
#include <glib.h>
#include <stdlib.h>

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif



// // a line has a number and text
// typedef struct Line {  
//   int number;
//   // string text;
//   //GArray* text;
//   GString* text;
// } line;

// typedef struct Location {
//   int x;
//   int y;
// } location;

// print2vec(const char cstring[]){
  
// }

int main()
{
  // init curses
  // -----------
  initscr();
  cbreak();
  curs_set(2);
  noecho();
  keypad(stdscr,true);
  // end curses init

  // setup structures
  // ------------------
  //GString line; 
  int ymax,xmax;
  getmaxyx(stdscr,ymax,xmax);
  GArray* lines; // array of lines
  lines = g_array_new(FALSE,FALSE,sizeof(GString *));
  // can find array size with lines->len which will return a guint
  // a guint is an unsigned int

  int startLine = 0;
  int finishLine = ymax;
  


  // add text to lines
  // -------------------
  GString* ar;
  for (int i = 0; i < 100; i++)
    {
      ar = g_string_new(""); // initialize string
      // push text to string
      g_string_append_printf(ar, 
			     "%d - lots and lots of lines flowing down the terminal\n",i);
      // push string into array
      g_array_append_val(lines, ar);
    }

  // display lines
  // --------------
  for (int i = 0; i < ymax; i++) // get ith line
    { 
      move(i,0);
      // g_ptr_array_index(lines,i) contains ith line
      // need to extract from it the jth char
      // store ith line as a new GString
      GString* line;
      line = g_array_index(lines,GString*,i);
      // print jth char of ith line
      for (int j = 0; j < min(xmax,(int)line->len); j++) 
	  wprintw(stdscr,"%c",line->str[j]);
    }

  getch();
  endwin();
}
