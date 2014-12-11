/*
  Wed Dec 10, 2014 11:16:36
  This is a first attempt to rewrite in C with glib

  Fri Dec 05, 2014 01:47:09
  This is a simplification.  

  There should only be one class here that handles everything. 
  
  Likewise, there should only be one editing mode.  
 */
#ifndef BUFFER_H
#define BUFFER_H

#include <ncurses.h>
//#include <vector>
//#include <string>
//#include <cstring>
#include <stdio.h>
//#include <algorithm> // min()
//#include <fstream>
//#include "boost/filesystem.hpp"
//#include "boost/filesystem/path.hpp"
//#include "boost/filesystem/operations.hpp"
#include <stdlib.h> // getline
//#include <sstream>
#include <unistd.h>  // usleep
#include <glib.h>  

// using std::vector;
// using std::string;
// using std::min;
// using std::stringstream;
// using std::endl;

// namespace fs = boost::filesystem;

#define ENTER 10
#define TAB 9
#define ESCAPE 27


// a line has a number and text
typedef struct Lines {  
  int number;
  // string text;
  GArray* text;
} line;

typedef struct Locations {
  int x;
  int y;
} location;

// a buffer has lines, but it also has a display 
// class buffer{
struct Buffers{
// public:
// variabl'es
WINDOW* bufSubWin;
//  vector<line> lines;
  
//string mode;
GString* mode;
int xmax; // maximum x position determined by screen size
int ymax; // maximum y position determined by screen size
location point; // current x,y location on screen
location mark; // location to cut/paste from
//int cx; // current x position on screen
//int cy; // current y position on screen
int startLine; // line at top of screen
int finishLine; // line at bottom of screen
int currentLine; // line that cursor is on
int lineNumber; // always should be currentLine + 1
//string name; // name of buffer
GString* name; // name of buffer
//string CurrentFileName; // filename for buffer
GString* CurrentFileName; // filename for buffer
bool editorIsRunning;
int keyPress;
//string pathName;
GString* pathName;
} buffer ;

// functions
void init(); // set necessary variables in object
void stats(); // show lines in buffer, current position
void update(); // get screen dimensions and line values
void getStartFinish(); // find startLine and finishLine for screen rendering
void updateLineNumbers(); // renumber lines appropriately
void print2vec(const char cstring[]); // pushes text into vector of lines
void displayLines(); // renders lines to fit on screen
//void setName(const string&); // set name of buffer
void setName(GString); // set name of buffer (could be GString instead of GArray)
GString getName(); // return name of buffer

// void listFiles();
void listFiles(GString);
void openFile(GString);
void writeFile(GString);
void getCwd();

void getUserInput();
void getUserInput(int keyPress);

void key_up();
void key_down();
void key_left();
void key_right();
void key_home();
void key_end();
void key_pgUp();
void key_pgDown();
void key_backspace();
void key_enter_textMode();
void key_enter_fileMode();
void lineGap(int);

//void openFile(string&);
//void writeFile(string&);

//};

#endif //BUFFER_H
