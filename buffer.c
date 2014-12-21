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
  //GString line; 
  GArray* lines; // array of lines
  lines = g_array_new(FALSE,FALSE,sizeof(GString *));
  // can find array size with lines->len which will return a guint
  // a guint is an unsigned int
  
  
  initscr();
  cbreak();
  curs_set(2);
  noecho();
  keypad(stdscr,true);


  // add text to lines
  GString* ar;
  for (int i = 0; i < 100; i++)
    {
      ar = g_string_new("");
      g_string_append_printf(ar, 
			     "%d - lots and lots of lines flowing down the terminal\n",i);
      g_array_append_val(lines, ar);
      refresh();
    }

  // display lines
  int ymax,xmax;
  getmaxyx(stdscr,ymax,xmax);
  for (int i = 0; i < ymax; i++)
    { 
      move(i,0);
      // g_ptr_array_index(lines,i) contains ith line
      // need to extract from it the jth char
      // store ith line as a new GString
      GString* line;
      line = g_array_index(lines,GString*,i);
      for (int j = 0; j < min(xmax,(int)line->len); j++) 
	  wprintw(stdscr,"%c",line->str[j]);
    }

  getch();
  endwin();
}
