/*
  Sat Dec 20, 2014 22:03:02 
  Really striving for maximum simplicity here. 

  Just an array of strings that can be printed on ncurses to test scrolling.

  This version uses a plain GArray
 */
#include <ncurses.h>
#include <glib.h>
//#include <stdlib.h>

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

void displayLines(GArray* lines,
		  int startLine,
		  int finishLine){
  int xmax,ymax,ty,tx;
  getmaxyx(stdscr,ymax,xmax);
  getyx(stdscr,ty,tx);
  clear();
  //refresh();
  for (int i = startLine; i < finishLine; i++) // get ith line
    { 
      move(startLine-i,0);
      // g_ptr_array_index(lines,i) contains ith line
      // need to extract from it the jth char
      // store ith line as a new GString
      GString* line;
      line = g_array_index(lines,GString*,i);
      // print jth char of ith line
      for (int j = 0; j < min(xmax,(int)line->len); j++) 
	//for (int j = 0; j < line->len; j++)
	wprintw(stdscr,"%c",line->str[j]);
    }
  wmove(stdscr,ty,tx);
  refresh();
}

int getStartLine(GArray* lines,
		  int currentLine)
{
  int cy,cx;
  getyx(stdscr,cy,cx);
  int minBoundary = currentLine - cy;
  // startLine must be >= 0 
  return (minBoundary >= 0) ? minBoundary : 0;
}

int getFinishLine(GArray* lines,
		  int currentLine)
{
  int cy,cx, xmax,ymax;
  getyx(stdscr,cy,cx);
  getmaxyx(stdscr,ymax,xmax);
  int maxBoundary = currentLine + ymax - cy - 1;
  return (maxBoundary < lines->len) ? maxBoundary : lines->len;
}


void scrollBufferREPL(GArray* lines){
  int xmax,ymax,cx,cy;
  getmaxyx(stdscr,ymax,xmax);
  int key;

  int startLine = 0;
  int currentLine = 0;
  int finishLine = ymax-1;

  displayLines(lines,startLine,finishLine);

  wmove(stdscr,0,0);


  bool editorRunning = true;
  

  while (editorRunning)
    {
      getmaxyx(stdscr,ymax,xmax);
      getyx(stdscr,cy,cx);
      key = getch();  
      switch (key)
	{
	case KEY_UP:
	  {
	    if (cy-1 >= 0) // don't scroll display 
	      {
	    	wmove(stdscr,cy-1,cx);
	    	currentLine--;
	      }
	    else // scroll display
	      {
	    	if (currentLine-1 >= 0)
	    	  {
	    	    wmove(stdscr,cy-1,cx);
	    	    currentLine--;
	    	    //startLine = getStartLine(lines,currentLine);
	    	    //finishLine = getFinishLine(lines,currentLine);
	    	    startLine--;
	    	    finishLine--;
	    	  }
	      }
	    //printw("key_up pressed");
	    break;
	  } // end key_up
	case KEY_DOWN:
	  {
	    //if (currentLine+1 < ymax-1)
	    if (cy+1 < ymax-1) // don't scroll display
	      {
		if (cy+1 < lines->len)
		  {
		    wmove(stdscr,cy+1,cx);
		    currentLine++;
		    //refresh();
		  }
		wmove(stdscr,cy+1,cx);
	      }
	    else // scroll display
	      {
		if (currentLine+1 < lines->len)
		  {
		    currentLine++;
		    startLine = getStartLine(lines,currentLine);
		    finishLine = getFinishLine(lines,currentLine);
		    //startLine++;
		    //finishLine++;
		  }
	      }
	    //displayLines(lines,startLine,finishLine);
	    break;
	  } // end key_down
	case KEY_RIGHT: 
	  {
	    GString* line;
	    line = g_array_index(lines,GString*,currentLine);
	    if (cx+1 < line->len)
	      wmove(stdscr,cy,cx+1);
	    else
	      {
		if (cy+1 < ymax)
		  if (currentLine+1 < lines->len)
		    {
		      wmove(stdscr,cy+1,0);
		      currentLine++;
		    }
	      }
	    break;
	  } // end key_right
	case KEY_LEFT: 
	  {
	    if (cx-1 >= 0) // stay on same line
	      wmove(stdscr,cy,cx-1);
	    else // move to prev line
	      {
		if (currentLine-1 >= 0)
		  {
		    currentLine--;
		    GString* line;
		    line = g_array_index(lines,GString*,currentLine);
		    wmove(stdscr,cy-1,
			  line->len-1
			  );
		  }
	      }
	    break;
	  } // end key_left
	default: 
	  {
	    break;
	  }
	} // end switch
      displayLines(lines,startLine,finishLine);
    } // end REPL
}

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


  //displayLines(lines);
  // REPL starts here
  scrollBufferREPL(lines);

  endwin();
}
