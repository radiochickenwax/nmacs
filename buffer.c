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

#define ENTER 10
#define TAB 9
#define ESCAPE 27


GString* g_substring (GString *str, int index, int len)
{
  GString* returnVal = g_string_new(NULL);
  if (len <= str->len)
    for (int i = index; i < len; i++)
      {
	gchar j = str->str[i];
	g_string_append_printf(returnVal, "%c",j);
      }
    //  return g_string_new_len (str->str, min (str->len - index, len));
  return returnVal;
}


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

void stats(GArray* lines,
	   int currentLine,
	   int startLine,
	   int finishLine)
  {
    wattron(stdscr,A_REVERSE);
    int ty,tx;
    getyx(stdscr,ty,tx); // store point
    int cy = ty;
    int cx = tx;
    int ymax,xmax;
    getmaxyx(stdscr,ymax,xmax);
    
    wmove(stdscr,ymax-1,0); 
    int lineNumber = currentLine + 1;

    //wmove(stdscr,finishLine,0); 
    wclrtoeol(stdscr); // clear line
    wmove(stdscr,ymax-1,0);
    wattron(stdscr,A_BOLD);
    // wprintw(stdscr,"%s: ",name.c_str());
    wattroff(stdscr,A_BOLD);
    wprintw(stdscr,"cx:%d,cy:%d,sl:%d,cl:%d,fl:%d,xmax:%d,ymax:%d,ln:%d,lines:%d",
	    cx,cy,startLine,currentLine,finishLine,xmax,ymax,lineNumber,lines->len);
    wrefresh(stdscr);
    wmove(stdscr,ty,tx); // restore point
    wattroff(stdscr,A_REVERSE);
  }


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
      wmove(stdscr,startLine-i,0);
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
		// wmove(stdscr,cy+1,cx);
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
	    else // duplicate key_down functionality
	      {
		// if (cy+1 < ymax-1)
		//   if (currentLine+1 < lines->len)
		//     {
		//       wmove(stdscr,cy+1,0);
		//       currentLine++;
		//     }
		
		// need to duplicate key_down functionality.
		// key_down(lines,currentLine,cy,cx);
		// this doesn't warrant a broken out function just yet
		
		/*
		  Sun Dec 21, 2014 21:05:22
		  Actually, this is a slightly different function that
		  wouldn't work correctly by simply calling key_down
		  if it existed.
		 */

		if (cy+1 < ymax-1) // don't scroll display
		  {
		    if (cy+1 < lines->len)
		      {
			wmove(stdscr,cy+1,0);
			currentLine++;
			//refresh();
		      }
		    wmove(stdscr,cy+1,0);
		  }
		else // scroll display
		  {
		    if (currentLine+1 < lines->len)
		      {
			currentLine++;
			startLine = getStartLine(lines,currentLine);
			finishLine = getFinishLine(lines,currentLine);
			wmove(stdscr,cy,0); // put cursor at beginning of line
			//startLine++;
			//finishLine++;
		      }
		  }

	      } // 
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
	case KEY_BACKSPACE: 
	  {
	    getyx(stdscr,cy,cx);
	    GString *before;
	    GString *after;
	    GString *current;

	    if (cx-1 >= 0) // stay on same line
	      {
		// // split current line text into two strings
		current = g_array_index(lines,GString*,currentLine);
		before = g_substring(current,0,cx-1); 
		after = g_substring(current,cx,current->len);
		
		// // push character into string "before" point
		///g_string_append_printf(before, "%c",(char)key);
		
		// // concat "before" and "after" strings
		g_string_assign(current, before->str); // push before into current
		g_string_append_printf(current,"%s",after->str); 

		// // restore line in structure
		g_array_insert_vals(lines,currentLine,&current,0);
		// note the ampersand for "current".  I'm not sure of these details 
		
		// // update cursor
		wmove(stdscr,cy,cx-1);

	      }
	    else // move to prev line
	      {
		if (currentLine > 0)
		  {
		    currentLine--;
		    current = g_array_index(lines,GString*,currentLine);
		    before = g_substring(current,0,current->len-1); 
		    after = g_array_index(lines,GString*,currentLine+1);
		    g_string_assign(current,before->str);
		    g_string_append_printf(current,"%s",after->str);
		    wmove(stdscr,cy-1,before->len);
		    g_array_remove_index(lines,currentLine+1);
		  }
	      }
	    refresh();
	    startLine = getStartLine(lines,currentLine);
	    finishLine = getFinishLine(lines,currentLine);

	    break;
	  } // end key_backspace
	  
	  //case KEY_ENTER:
	case ENTER:
	  {
	    /*
	      Tue Dec 30, 2014 12:24:45
	      Note that g_array_insert_vals()  needs to use the ampersand &
	      to address the value to insert for some reason. 

	      Took me hours to realize this.
	     */
	    if (cx == 0)
	      {
		GString *emptyLine;
		emptyLine = g_string_new("\n");
		g_array_insert_vals(lines,(guint)currentLine,&emptyLine,1);
		currentLine++;      
		wmove(stdscr,cy+1,0);
		//startLine = getStartLine(lines,currentLine);
		//finishLine = getFinishLine(lines,currentLine);
	      }
	    else
	      {
		GString* before;
		GString* after;
		GString* current;
		current = g_array_index(lines,GString*,currentLine);
		before = g_substring(current,0,cx); 
		g_string_append(before,"\n");
		after = g_substring(current,cx,current->len); 

		g_array_remove_index(lines,currentLine);
		g_array_insert_vals(lines,currentLine,&before,1);
		g_array_insert_vals(lines,currentLine+1,&after,1);  // should get pushed to next line in next step



		currentLine++;
		wmove(stdscr,cy+1,0);
		startLine = getStartLine(lines,currentLine);
		finishLine = getFinishLine(lines,currentLine);
	      }
	    break;
	  }
	default:  // line gap
	  {
	    getyx(stdscr,cy,cx);
	    GString *before;
	    GString *after;
	    GString *current;
	    // current = lines[currentLine].text;
	    current = g_array_index(lines,GString*,currentLine);
	    // // split current line text into two strings
	    // // 1 before cursor, 1 after cursor

	    // before = current.substr(0,point.x); // includes current cursor
	    // before = g_strndup(current,cx); // includes current cursor
	    before = g_substring(current,0,cx); // includes current cursor

	    // after = current.substr(point.x,current.length());
	    // after = g_strndup(current+cx,current->len);
	    after = g_substring(current,cx,current->len);

	    // // push character into string "before" point
	    // before.push_back(thisChar);
	    // g_string_append(before, (gchar) key);
	    g_string_append_printf(before, "%c",(char)key);

	    // // concat "before" and "after" strings
	    // current = before + after;
	    g_string_assign(current, before->str); // push before into current
	    g_string_append_printf(current,"%s",after->str); 


	    // // update cursor
	    // wmove(bufSubWin,point.y,point.x+1);
	    wmove(stdscr,cy,cx+1);

	    // // restore line in structure
	    // lines[currentLine].text = current;
	    // g_array_insert_val(lines,currentLine,&current);
	    g_array_insert_vals(lines,currentLine,current,0);

	    // clean up
	    // g_string_free(before,TRUE);
	    // g_string_free(after,TRUE);
	    // g_string_free(current,TRUE);

	    break;
	  }
	} // end switch
      displayLines(lines,startLine,finishLine);
      stats(lines,
	    currentLine,
	    startLine,
	    finishLine);

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
      ar = g_string_new(NULL); // initialize string
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
