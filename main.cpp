#include "buffer.h"
#include "menu.h"

// TODO:  make this into a factory pattern - get rid of if/else
class bufferMenu{
public:
  int key;
  buffer editor;
  menuHeader header;
  void handleCallBack(string callBack){
    if (callBack == "New"){
      ;
    }
    if (callBack == "Open"){
      editor.openFile(string("./"));
    }
    if (callBack == "Save"){
      if (editor.CurrentFileName != " ")
	editor.writeFile(editor.CurrentFileName);
      else
	// prompt for filename
	;
    }
    if (callBack == "Save As"){
      // open new window to prompt for fileName
      ;
    }
    if (callBack == "List All Buffers"){
      ;
    }
    if (callBack == "Cut"){
      ;
    }
    if (callBack == "Copy"){
      ; 
    }
    if (callBack == "Paste"){
      ;
    }
    if (callBack == "Select All"){
      ;
    }
    if (callBack == "Find"){
      ;
    }
    if (callBack == "Preferences"){
      ;
    }
    if (callBack == "Next Buffer"){
      ;
    }
    if (callBack == "Prev Buffer"){
      ;
    }
    if (callBack == "Life"){
      // system("./gameOfLife");
    }
    if (callBack == "Pong"){
      ;
    }
    if (callBack == "Space Invaders"){
      ;
    }

  }
  void getUserInput(){
    while (1)
      {
	key = getch();
	//string callBack;
	int ty,tx;
	getyx(editor.bufSubWin,ty,tx);
	
	header.drawMenuBar();
	wmove(editor.bufSubWin,ty,tx);
	
	//werase(nmacs.header.messagebar);
	//wrefresh(nmacs.header.messagebar);
	if (key==KEY_F(1)) { handleCallBack( header.handleSubMenu(0) );   } 
	else if (key==KEY_F(2)) { handleCallBack( header.handleSubMenu(1) );     }
	else if (key==KEY_F(3)) { handleCallBack( header.handleSubMenu(2) );     }
	else if (key==KEY_F(4)) { handleCallBack( header.handleSubMenu(3) );     }
	else { editor.getUserInput(key); }
	//
	curs_set(2);
      }
  
  }
}; // end bufferMenu class

int main()
{

  initscr();
  cbreak();
  curs_set(2);
  noecho();

  // this section is iffy
  start_color();
  init_pair(1,COLOR_GREEN,COLOR_BLACK);
  init_pair(2,COLOR_BLACK,COLOR_GREEN);
  init_pair(3,COLOR_BLUE,COLOR_GREEN);


  keypad(stdscr,true);

  bufferMenu nmacs;
  //menuHeader nmacs;
  int key = 0;

  nmacs.editor.init();
  nmacs.editor.update();

  nmacs.editor.mode = "file";
  string filename = string("Makefile");
  nmacs.editor.openFile(filename);
  nmacs.header.drawMenuBar();
  nmacs.getUserInput();



  

  
  // getch();
  endwin();
  return 0;
}
