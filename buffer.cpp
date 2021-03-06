/* 
   This program should define a buffer object.
   TODO: describe buffer object
   TODO: add tests
 */
#include "buffer.h"

void buffer::init()
{
  getmaxyx(stdscr,ymax,xmax);
  startLine = 0;
  currentLine = 0;
  finishLine = ymax;
  CurrentFileName = string(" ");
  editorIsRunning = true;
  point.x = 0;
  point.y = 0;
  bufSubWin = subwin(stdscr,ymax-1,xmax,1,0);  // subwin in stdscr
  wmove(bufSubWin,point.y,point.x);
  //name = "";
}

// get current screen dimensions,
// and current cursor coordinates
void buffer::update()
{
  getmaxyx(bufSubWin,ymax,xmax);
  getyx(bufSubWin,point.y,point.x);
  lineNumber = currentLine + 1;
  getStartFinish();
  displayLines();
  refresh();
  stats();
  //touchwin(stdscr);
  wrefresh(bufSubWin);
}

  
void buffer::setName(const string& thisName) 
{
  name = thisName;
}


string buffer::getName()
{
  return name;
}


// display relevent vars
/*
  Tue Dec 02, 2014 01:38:39
  Took me a long time to figure out why this single line
  kept following the cursor around.   
  
  The problem is that getmaxyx() returns 
  the max number of lines + 1.
  
  NOT THE MAX NUMBER OF LINES!
  
    This of course changes everything.
*/
void buffer::stats()
  {
    wattron(bufSubWin,A_REVERSE);
    int ty,tx;
    getyx(bufSubWin,ty,tx); // store point
    getmaxyx(bufSubWin,ymax,xmax);
    
    wmove(bufSubWin,ymax-1,0); 
    //wmove(stdscr,finishLine,0); 
    wclrtoeol(bufSubWin); // clear line
    wmove(bufSubWin,ymax-1,0);
    wattron(bufSubWin,A_BOLD);
    wprintw(bufSubWin,"%s: ",name.c_str());
    wattroff(bufSubWin,A_BOLD);
    wprintw(bufSubWin,"cx:%d,cy:%d,sl:%d,cl:%d,fl:%d,xmax:%d,ymax:%d,ln:%d,lines:%d",
	    point.x,point.y,startLine,currentLine,finishLine,xmax,ymax,lineNumber,lines.size());
    wrefresh(bufSubWin);
    wmove(bufSubWin,ty,tx); // restore point
    wattroff(bufSubWin,A_REVERSE);
  }


/*
  Mon Dec 01, 2014 23:10:15
  Can you use point.y and currentLine to find startLine and finishLine?
  
  I think so. But how?
  
  
   */
void buffer::getStartFinish()
{
  int minBoundary = currentLine - point.y; // obviously this is wrong
  // from test cases
  
  int maxBoundary = currentLine + ymax - point.y;
  
  // startLine must be >= 0 
  startLine = (minBoundary >= 0) ? minBoundary : 0;
  // finishLine must be <= lines.size()
  finishLine = (maxBoundary < lines.size()) ? maxBoundary : lines.size();
}

// this should renumber the lines appropriately
void buffer::updateLineNumbers()  {
  for (int i = 0; i < lines.size(); i++)
    lines[i].number = i+1;
}  

// this should just push text into a vector
void buffer::print2vec(const char cstring[])  {
  line thisLine;
  thisLine.text = string(cstring);
  lines.push_back(thisLine);
  updateLineNumbers();
}

/* 
   displayLines() should render ymax lines on the screen.
   Where does it start from though?
*/
void buffer::displayLines() 
{
  int ty,tx;
  getyx(bufSubWin,ty,tx);
  getmaxyx(bufSubWin,ymax,xmax);
  for (int i = startLine; i < finishLine; i++)
    {
      wmove(bufSubWin,(startLine-i), 0); // reposition cursor
      
      for (int j = 0; j < min(xmax,(int)lines[i].text.length()); j++)
	  {
	    if (lines[i].text[j] != '\t')
	      wprintw(bufSubWin,"%c", lines[i].text[j] );
	    else{
	      lines[i].text[j] = ' '; // delete tab.  Ouch.
	      //wprintw(bufSubWin,"%s", "     "); // 5 spaces.
	      for (int k = 0; k < 4; k++)
		lineGap(' ');
	    }
	  }
      
    }
  wmove(bufSubWin,ty,tx);
}



void buffer::getCwd()
{
  pathName = fs::current_path().string();
}


// void buffer::getNumberOfBuffers()
// {
// }

//============== KEY COMMANDS ========================//
// callback for up arrow keypresses
void buffer::key_up()
{
  update();
    if (point.y-1 >= 0) 
      {
	int prevLineLength = lines[currentLine-1].text.length()-1;
	currentLine--;
	if (point.x <= prevLineLength)
	  wmove(bufSubWin,point.y-1,point.x);
	else
	  wmove(bufSubWin,point.y-1,prevLineLength);
	update();
      }
    else
      {
	if (currentLine - 1 >= 0)
	  {
	    startLine--;
	    currentLine--;
	    finishLine--;
	    displayLines();
	    update();
	  }
      }
  } // end key_up

void buffer::key_down()
{
  update();
  if (point.y+1 < ymax-1)  {
    if (point.y+1 < lines.size()) {
      int nextLineLength = lines[currentLine+1].text.length()-1;
      if (point.x <= nextLineLength){
	wmove(bufSubWin,point.y+1,point.x);
      }
      else
	wmove(bufSubWin,point.y+1,nextLineLength);
      getyx(bufSubWin,point.y,point.x);
      currentLine++;
      update();
    }
  }
  else // scroll window by one line
    {
      if (currentLine + 1 < lines.size())
	{
	  startLine++;
	  currentLine++;
	  finishLine++;
	  displayLines();
	  update();
	}
    }
  
} // end key_down

void buffer::key_right()
{
  // getyx(stdscr,point.y,point.x);
  update();
  if (point.x+1 < lines[currentLine].text.length())
    wmove(bufSubWin,point.y,point.x+1);
  else
      {
	wmove(bufSubWin,point.y+1,0);
	currentLine++;
      }
  update();
}

void buffer::key_left()
  {
    //getyx(stdscr,point.y,point.x);
    update();
    if (point.x-1 >= 0)
      wmove(bufSubWin,point.y,point.x-1);
    else
      {
	if (currentLine-1 >= 0)
	  {
	    wmove(bufSubWin,point.y-1,
		 lines[currentLine-1].text.length()-1
		 );
	    currentLine--;
	  }
      }
    update();
  }

void buffer::key_home()
  {
    //getyx(stdscr,point.y,point.x);
    update();
    wmove(bufSubWin,point.y,0);
    update();
  }


void buffer::key_end()
  {
    //getyx(stdscr,point.y,point.x);
    update();
    wmove(bufSubWin,point.y,
	  lines[currentLine].text.length()-1);
    update();
  }


void buffer::key_pgUp()
{
    //getyx(stdscr,point.y,point.x);
    update();
    if (startLine - ymax - 1 <= 0){
      startLine = 0;
      currentLine = startLine;
      wmove(bufSubWin,0,0);
      update();
    }
    else{
      startLine = startLine - ymax - 1;
      currentLine = startLine;
      update();
      wmove(bufSubWin,0,0);
      update();
    }
  }


void buffer::key_pgDown()
{
    //getyx(stdscr,point.y,point.x);
    update();
    if (finishLine + ymax - 1 <= lines.size()){
      startLine = finishLine;
      currentLine = startLine;
      update();
      wmove(bufSubWin,0,0);
      update();
    }
    else{
      finishLine = lines.size();
      startLine = finishLine - ymax; 
      currentLine = startLine;
      update();
      wmove(bufSubWin,0,0);
      update();
    }
  }


void buffer::key_backspace()
{
  getyx(bufSubWin,point.y,point.x);
  string before,after,current;
  current = lines[currentLine].text;
  // split current line text into two strings
  // 1 before cursor, 1 after cursor
  before = current.substr(0,point.x); // includes current cursor
  after = current.substr(point.x,current.length());
  if (point.x-1 >= 0)
    {
      // remove last char before point.
      // can't use string.pop_back() 
      // because it's only available in c++11
      // before.pop_back(); 
      before = before.substr(0,before.length()-1);
      lines[currentLine].text = before + after;
      wmove(bufSubWin,point.y,point.x-1);
    }
  else // need to join the front of this line with the tail of the
    // previous line 
    {
      if (currentLine > 0)
	{
	  //before = current = "";
	  before = lines[currentLine-1].text;
	  before = before.substr(0,before.length()-1);
	  after = lines[currentLine].text;
	  lines[currentLine-1].text = before + after;
	  wmove(bufSubWin,point.y-1,before.length());
	  lines.erase(lines.begin()+currentLine);
	  currentLine--;
	  update();
	}
    }
  refresh();
  update();
}



void buffer::key_enter_fileMode()
{
  string fn = lines[currentLine].text;
  // need to remove '\n' newline chars from fn.  
  fn.erase(std::remove(fn.begin(),fn.end(),'\n'),fn.end());
  // clear();
  // printw("keyed to open: %s",fn.c_str());
  // refresh();
  // usleep(10000);
  openFile(fn);
}

/* 
     add a new line.
     
     This is trickier than it sounds. 
     
     case0:  there is no text before point
*/
void buffer::key_enter_textMode()
{
  getyx(bufSubWin,point.y,point.x);
  if (point.x == 0)
    {
      line lineToAdd; // make an empty string for this 
      lineToAdd.text = "\n";
      lines.insert(lines.begin()+currentLine, lineToAdd);
      if (currentLine + 1 == finishLine)
	{
	  startLine++;
	  currentLine++;
	  finishLine++;
	  
	  key_down();
	  key_up();
	}
    }
  else
    {
      // split the current line.    
      line thisLine = lines[currentLine];
      line lineToAdd;
      string before,after;
      // get currentLine.text before point
      before = thisLine.text.substr(0,point.x) + "\n";
      // get currentLine.text after point
      after = thisLine.text.substr(point.x,thisLine.text.length());
      // put text after point on the new line.
      thisLine.text = before;
      lineToAdd.text = after;
      // restore modified currentLine text
      lines[currentLine] = thisLine;
      // insert the new line into the vector of lines
      lines.insert(lines.begin()+currentLine+1, lineToAdd);
      //update();
    }
  refresh();
  update();
  
  currentLine++;     // update currentLine
  wmove(bufSubWin,point.y+1,0); // update cursor position
  
  refresh();
  update();
} // end key_enter

void buffer::lineGap(int character)
{
  char thisChar = (char)character;
  // if (thisChar == '\n')
  //   if (mode == "text")
  //     key_enter_textMode();
  //   else if (mode == "file")
  //     key_enter_fileMode();
  // else
  if (character == TAB)
    {
      // insert 5 spaces.
      // call lineGap 5 times?
      static int n = 5;
      while (n !=0){
	lineGap(' ');
	n--;
      }
      // wmove(bufSubWin,point.y,point.x+5);
    }
  else
    {
      getyx(bufSubWin,point.y,point.x);
      string before,after,current;
      current = lines[currentLine].text;
      // split current line text into two strings
      // 1 before cursor, 1 after cursor
      before = current.substr(0,point.x); // includes current cursor
      after = current.substr(point.x,current.length());
      // push character into string "before" point
      before.push_back(thisChar);
      // concat "before" and "after" strings
      current = before + after;
      // update cursor
      wmove(bufSubWin,point.y,point.x+1);
      // restore line in structure
      lines[currentLine].text = current;
      refresh();
      update();
    }
} // end lineGap function



void buffer::getUserInput()
{
  while (editorIsRunning)
    {
      keyPress = getch();
      getUserInput(keyPress);
    } // end while
} // end getUserInput() 

// TODO: make this a factory, get rid of switch
void buffer::getUserInput(int keyPress)
{
  switch (keyPress)
    {
    case KEY_UP: 
      {
	key_up();
	break;
      }
    case KEY_DOWN: 
      {
	key_down();
	break;
      }
    case KEY_LEFT: 
      {
	key_left();
	break;
      }
    case KEY_RIGHT: 
      {
	key_right();
	break;
      }
    case KEY_HOME: 
      {
	key_home();
	break;
      }
    case KEY_NPAGE: 
      {
	key_pgDown();
	break;
      }
    case KEY_PPAGE: 
      {
	key_pgUp();
	break;
      }
    case KEY_END: 
      {
	key_end();
	break;
      }

    // case TAB:
    //   {
    // 	break;
    //   }
    case ENTER: // doesnt' seem to work 
      {
	if (mode == "text")
	  key_enter_textMode();
	else if (mode == "file")
	  key_enter_fileMode();
	break;
      }
    case KEY_BACKSPACE: // needs to update string in lines
      {
	update();
	key_backspace();
	break;
      }
      // http://stackoverflow.com/questions/406933/how-to-intercept-special-alt-ctrl-key-pressed-in-python-curses
      /*
	CTRL-A: getch() returns 1
	CTRL-B: getch() returns 2
	...
	CTRL-Z: getch() returns 26
      */
    case  15:// ctrl+o: create a new fileEditor buffer
      {
	//fileEditor fe;
	//printw("Ctrl-o pressed");
	string thisArg = string(".");
	openFile(thisArg);
	break;
      }
    default:
      {
	lineGap(keyPress);
	break;
      }
    } // end switch
} // end getUserInput() 




/* --------------------   listFiles(string thisPath)
  overview:
  input:
  output:
---------------------------------------------------- */
void buffer::listFiles(string& thisPath)
{
  clear();
  lines.clear();
  init();
  update();
  stringstream textStream;
  if (fs::exists(thisPath.c_str()))
    {
      fs::path p (thisPath.c_str());
      fs::path absPath = p.string();
      fs::directory_iterator end_itr;

      print2vec(".\n");
      print2vec("..\n");
      
      // cycle through the directory
     for (fs::directory_iterator itr(p); itr != end_itr; ++itr)
	{
	  if (is_regular_file(itr->path())) {
	    // assign current file name to current_file and echo it out to the console.
	    //string current_file = itr->path().string();
	    string current_file = itr->path().filename().c_str();
	    textStream << current_file << endl;
	    print2vec(textStream.str().c_str());
	    textStream.str(std::string()); // clear textStream for future use

	  }
	  else if (is_directory(itr->path())) { // file is a directory
	    string current_file = itr->path().string();
	    attron(A_REVERSE);
	    textStream << current_file << endl;
	    print2vec(textStream.str().c_str());
	    textStream.str(std::string()); // clear textStream for future use
	    attroff(A_REVERSE);
	  }
	} // end for loop
    } // end if file exists
  else 
    {
      textStream << "That path does not exist." << endl;
      print2vec(textStream.str().c_str());
      textStream.str(std::string()); // clear textStream for future use
    }
  update();
  refresh();
}



/* --------------------   openFile(string fileName)
  overview:
  input:
  output:
---------------------------------------------------- */

void buffer::openFile(string fileName)
{
  //fileName = fileToRead;
  //printw("%s","Opening file");
  string tmp;
  std::fstream fin;
  wmove(bufSubWin,0,0);
  getyx(bufSubWin,point.y,point.x);
  init();
  startLine=0;
  currentLine=0;
  finishLine=1;
  clear();
  lines.clear();
  update();

  if (fs::exists(fileName.c_str())){
    fs::path p(fileName.c_str());
    name = p.string();
    CurrentFileName = name;
    
    if (is_directory(p))
      {
	mode = "file";
	name = fs::absolute(p.string()).string();
	//chdir(p.string().c_str());
	chdir(name.c_str());
	listFiles(fileName);
	//wmove(bufSubWin,0,0);
	key_down();
	wmove(bufSubWin,0,0);
	currentLine = 0;
	refresh();
      }
    else if (is_regular_file(p))
      {
	mode = "text";
	// need to convert fileName string to something more useful
	// already converted it to a path.
	//fin.open(fileName.c_str(),std::ios::in);
	wprintw(bufSubWin,"Trying to open: %s okay?",p.string().c_str());
	wrefresh(bufSubWin);
	usleep(100000);      
	fin.open(p.string().c_str(),std::ios::in);
	if (fin.fail())
	  {
	    wmove(bufSubWin,ymax,0);
	    wprintw(bufSubWin,"failed to open file");
	    refresh();
	    //exit (1);
	  }
	
	while (getline(fin,tmp)) // read file into lines vector
	  print2vec((tmp+"\n").c_str());
	//wmove(bufSubWin,1,0);
	key_down();
	wmove(bufSubWin,0,0);
	currentLine = 0;
	refresh();
      } // end regular file case
    //wmove(bufSubWin,1,0);
    // wmove(bufSubWin,0,0);
    fin.close();
    update();
    refresh();
  } // end file exists check
  else {
    wprintw(bufSubWin,"file %s does not exist.",fileName.c_str());
    refresh();
    usleep(5000000);      
    tmp = "./";
    listFiles(tmp);
  }
} // end openFile function

/* --------------------   openFile(string fileName)
  overview:
  input:
  output:
---------------------------------------------------- */

void buffer::writeFile(string fileName)
{
  int ty,tx;
  getyx(bufSubWin,ty,tx); // store point
      
  std::fstream fOut;
  fOut.open(fileName.c_str(),std::ios::out);
  if (fOut.fail())
    {
      wmove(bufSubWin,ymax,0);
      wprintw(bufSubWin,"failed to open file for writing");
      refresh();
      //exit (1);
    }
  for (int i = 0; 
       i < lines.size(); 
       ++i)
    //fOut << lines[i].text << '\n';
    fOut << lines[i].text ;
  fOut.close();

  wattron(bufSubWin,A_REVERSE);
  wattron(bufSubWin,A_BOLD);
  mvwprintw(bufSubWin,ymax-1,0,"Wrote %s.",fileName.c_str());
  wattroff(bufSubWin,A_REVERSE);
  wattroff(bufSubWin,A_BOLD);
  wmove(bufSubWin,ty,tx); // restore point
  wrefresh(bufSubWin);
  refresh();
  usleep(100000);      
}
