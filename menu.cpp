/*

  Sat Dec 06, 2014 21:32:57 fixed segfaults, fixed menu wrapping

  Sat Dec 06, 2014 15:59:32   packed everything into a class.  Doesn't run. 
  19:34:26 Now it runs.

  Sat Dec 06, 2014 12:30:42
  14:00:05 : segfaults on scroll_menu(menuBar).   For some reason,
  this isn't getting the correct size of the vector<string> subItem.

  I suspect this is due to pointer errors.

  Fri Dec 05, 2014 14:00:12
  (browse-url "http://www.linuxfocus.org/English/March2002/article233.shtml")
  
  16:03:00 example-02.cpp
  Two hours later. This works using vectors instead of c_arrays.

  Could still clean it up quite a bit.  Maybe put everything into
  classes instead of having a main() driver.

 * todo [3/5]
   - [X] wrap everything into a generic class somehow
   - [X] fix key_left key_right arrow problems
   - [X] fix delete window problem (doesn't delete)
   - [ ] functions in menu don't actually execute yet
   - [ ] merge with buffer.cpp somehow to execute functions



 */

#include "menu.h"

menuHeader::menuHeader(){
    printf("starting menuHeader");

    dropDownMenu fileMenu;
    dropDownMenu editMenu;
    dropDownMenu bufferMenu;
    dropDownMenu gameMenu;


    // pack subitems into file menu
    fileMenu.name =  string("(F1): File");
    fileMenu.subItem.push_back(string(""));
    fileMenu.subItem.push_back(string("New"));
    fileMenu.subItem.push_back(string("Open"));
    fileMenu.subItem.push_back(string("Save"));
    fileMenu.subItem.push_back(string("Save As"));
    fileMenu.subItem.push_back(string("List All Buffers"));
    fileMenu.subItem.push_back(string("Close"));
    fileMenu.subItem.push_back(string("Quit"));

    // pack subitems into edit menu    
    editMenu.name =  string("(F2): Edit");
    editMenu.subItem.push_back(string(""));
    editMenu.subItem.push_back(string("Cut"));
    editMenu.subItem.push_back(string("Copy"));
    editMenu.subItem.push_back(string("Paste"));
    editMenu.subItem.push_back(string("Select All"));
    editMenu.subItem.push_back(string("Find"));
    editMenu.subItem.push_back(string("Preferences"));

    // pack subitems into buffer menu    
    bufferMenu.name =  string("(F3): Buffer");
    bufferMenu.subItem.push_back(string(""));
    bufferMenu.subItem.push_back(string("List All"));
    bufferMenu.subItem.push_back(string("Next Buffer"));
    bufferMenu.subItem.push_back(string("Prev Buffer"));

    // pack subitems into game menu    
    gameMenu.name =  string("(F4): Game");
    gameMenu.subItem.push_back(string(""));
    gameMenu.subItem.push_back(string("Life"));
    gameMenu.subItem.push_back(string("Pong"));
    gameMenu.subItem.push_back(string("SpaceInvaders"));

    // pack submenus into header menu
    subMenu.push_back(fileMenu);
    subMenu.push_back(editMenu);
    subMenu.push_back(bufferMenu);
    subMenu.push_back(gameMenu);
    
    setStartCoordinates();

    bkgd(COLOR_PAIR(1));
    menubar=subwin(stdscr,1,80,0,0);
    //header.win=subwin(stdscr,1,80,0,0);
    messagebar=subwin(stdscr,1,79,23,1);
    // draw_menubar(menubar);
    drawMenuBar();
    // move(2,1);
    // printw("Press F1 or F2 to open the menus. ");
    // printw("ESC quits.");
    refresh();

    //getKeys();
    // do {
    //   //WINDOW **menu_items;
    //   //vector<WINDOW*> menu_items;
    //   key=getch();
    //   werase(messagebar);
    //   wrefresh(messagebar);
    //   if (key==KEY_F(1)) { handleSubMenu(0);  } 
    //   else if (key==KEY_F(2)) { handleSubMenu(1);     }
    //   else if (key==KEY_F(3)) { handleSubMenu(2);     }
    //   else if (key==KEY_F(4)) { handleSubMenu(3);     }

    // } while (key!=ESCAPE);
    
  } // end constructor function
 
menuHeader::~menuHeader() {
    delwin(menubar);
    delwin(messagebar);
    printf("ending menuHeader");
  } // end destructor function
  
string menuHeader::getKeys(){
  do {
    key=getch();
    // werase(messagebar);
    // wrefresh(messagebar);
    // if (key==KEY_F(1)) { handleSubMenu(0);  } 
    // else if (key==KEY_F(2)) { handleSubMenu(1);     }
    // else if (key==KEY_F(3)) { handleSubMenu(2);     }
    // else if (key==KEY_F(4)) { handleSubMenu(3);     }
    getKeys(key);
  } while (key!=ESCAPE);
} // end getKeys()


string menuHeader::getKeys(int key){
  //  do {
  //key=getch();
    werase(messagebar);
    wrefresh(messagebar);
    if (key==KEY_F(1)) { handleSubMenu(0);  } 
    else if (key==KEY_F(2)) { handleSubMenu(1);     }
    else if (key==KEY_F(3)) { handleSubMenu(2);     }
    else if (key==KEY_F(4)) { handleSubMenu(3);     }
    
    //  } while (key!=ESCAPE);
} // end getKeys()


string menuHeader::handleSubMenu(int num){
  int ty,tx;
  getyx(stdscr,ty,tx);
  // int selected_item;
  string selected_item;
  drawSubMenu(num);
  // selected_item=scroll_menu(fileMenu,8,0);
  selected_item=scrollSubMenu(num);
  //delete_menu(fileMenu,9);
  deleteSubMenu(num);
  // if (selected_item<0)
  //   wprintw(messagebar,"You haven't selected any item.");
  // else
  
  /*
    wprintw(messagebar,
    //"You have selected menu item %d.",selected_item+1);
    //"You have selected %s.",fileMenu.name[selected_item+1].c_str());
    //"You have selected %s.",subMenu[num].subItem[selected_item+1].c_str());
    selected_item.c_str());
  */
  
  touchwin(stdscr);
  refresh();
  move(ty,tx);
  return selected_item;
} // end handleSubMenu()

  void menuHeader::setStartCoordinates(){
    if (subMenu.size() >= 0){
      subMenu[0].xStart = 0; // or padded number
      if (subMenu.size() >= 1){
	for (int i = 1; i < subMenu.size(); i++)
	  // subMenu[i].xStart = subMenu[i-1].name.length()+1;
	  // NO!  it's the sum of lengths > 0!
	  subMenu[i].xStart = getSubMenuNameLengths(i,0);
      }
    }
  } // end setStartCoodinates()

  // draws all submenu names in class horizontally across screen
  void menuHeader::drawMenuBar(){
    string itemName;
    int tx,ty;
    getyx(stdscr,ty,tx);
    wbkgd(menubar,COLOR_PAIR(2));
    wmove(menubar,0,0);
    for (int i = 0; i < subMenu.size(); i++){

      // itemName = subMenu[i].name;
      // //itemName = "(F1): File";
      // waddstr(menubar,itemName.c_str());
      //wmove(menubar,0,itemName.length());
      wprintw(menubar,"%s ",subMenu[i].name.c_str());
      // if (i+1 < subMenu.size()){
      // 	// 	subMenu[i+1].xStart = getSubMenuNameLengths();
      // 	getyx(menubar,NULL,subMenu[i+1].xStart);
      // 	subMenu[i+1].xStart++;
      // }
    }
    wrefresh(menubar);
    move(ty,tx);
    curs_set(2);
  } // end drawMenuBar()

  // recurse over vector of names
  int menuHeader::getSubMenuNameLengths(int n, int sum){
    //static int sum = 0;
    // int nameLength;
    if (n < 0)
      return sum;
    else 
      sum += subMenu[n-1].name.length();
    // nameLength = subMenu[number-1].name.length();
    return getSubMenuNameLengths(--n,sum);
  }

  void menuHeader::drawSubMenu(int number) {
    int ty,tx;
    getyx(stdscr,ty,tx);
    dropDownMenu& thisMenu = subMenu[number];
    if (thisMenu.window.size() > 0)
      thisMenu.window.clear(); 
    thisMenu.window.push_back(newwin(thisMenu.name.size()+1,
				     19,
				     1,
				     thisMenu.xStart));
    wbkgd(thisMenu.window[0],COLOR_PAIR(2));
    box(thisMenu.window[0],ACS_VLINE,ACS_HLINE);
    // WINDOW *subwin(WINDOW *orig, int nlines, int ncols, int begin_y, int begin_x);
    //  for (int i=1; i<9; i++)
    for (int i=1; i<thisMenu.subItem.size(); i++)
      //thisMenu.window.push_back(subwin(thisMenu.window[0],1,17,i+1,thisMenu.xStart)); 
      thisMenu.window.push_back(subwin(thisMenu.window[0],
				       1,
				       17,
				       i+1,
				       thisMenu.xStart+1)); 
    
    // print strings to windows
    for (int i = 1; i < thisMenu.subItem.size(); i++)
      wprintw(thisMenu.window[i],thisMenu.subItem[i].c_str());
      //wprintw(subMenu[number].window[i],thisMenu.subItem[i].c_str());
      //wprintw(subMenu[number].window[i],"hello");
    
    wbkgd(thisMenu.window[1],COLOR_PAIR(1));
    wrefresh(thisMenu.window[0]);
    move(ty,tx);

    
  } // end drawSubMenu()
  
  void menuHeader::deleteSubMenu(int number){
    for (int i=0;i<subMenu[number].window.size();i++)
      delwin(subMenu[number].window[i]);

    subMenu[number].window.clear(); // remove all elements from vec of wins
  } // end deleteSubMenu()
  
  string menuHeader::scrollSubMenu(int number){
    int key = 0;
    int count = 0;
    int selected = 0;
    string selection = "";
    int nextItem = 0;
    // int selected = 0;
    // int nextItem;
    dropDownMenu& thisMenu = subMenu[number];
    vector<WINDOW*>& items = thisMenu.window;
    count = thisMenu.subItem.size()-1;
    //dropDownMenu*& thisMenu = menu.subMenu[subMenuItem];
    //dropDownMenu* thisMenu = &menu.subMenu[subMenuItem];
    //dropDownMenu& thisMenu = *menu.subMenu[subMenuItem];

    //vector<WINDOW*>& items = thisMenu->window;

    //printf("%d",count);
    while (1) {
      key=getch();
      if (key==KEY_DOWN || key==KEY_UP) {
	wbkgd(items[selected+1],COLOR_PAIR(2));
	wnoutrefresh(items[selected+1]);
	if (key==KEY_DOWN) {
	  // selected=(selected+1) % count;
	  //selected=mod((selected+1), count);
	  selected=mod((selected+1), count);
	  //selected = (selected+1) % thisMenu.subItem.size();
	  //selected = mod((selected+1), thisMenu->subItem.size());
	  // selected = mod((selected+1), thisMenu.subItem.size());
	  //selected++;
	} 
	else {
	  selected=mod((selected-1), count);
	  // selected=(selected+count-1) % count;
	  //selected = mod((selected-1), thisMenu.subItem.size());
	  //selected = mod((selected-1), thisMenu->subItem.size());
	  //selected--;
	}
	wbkgd(items[selected+1],COLOR_PAIR(1));
	wnoutrefresh(items[selected+1]);
	doupdate();
      } 
      else if ((key==KEY_LEFT) || (key==KEY_RIGHT)) {
	//delete_menu(thisMenu,count+1);
	deleteSubMenu(number);
	touchwin(stdscr);
	refresh();
	if (key==KEY_RIGHT){
	  nextItem = mod(number + 1 , subMenu.size());
	  //number = nextItem;
	}
	else{
	  nextItem = mod(number - 1 , subMenu.size());
	  //number = nextItem;
	}
	//thisMenu = subMenu[number];
	//items = thisMenu.window;
	//count = thisMenu.subItem.size()-1;
	drawSubMenu(nextItem);
	// return scroll_menu(thisMenu,8,20-menu_start_col);
	return scrollSubMenu(nextItem);
      } 
      else if (key==ESCAPE) {
	// return -1;
	return "Escape";
      } 
      else if (key==ENTER) {
	// return selected;
	// search subMenus for currentMenu name?
	return subMenu[number].subItem[selected+1];
      }
    }  // end key check
  } // end scrollSubMenu()
  
//}; // end menuHeader class


  // void init_curses(){
  //   initscr();
  //   start_color();
  //   /*
  //     init_pair(1,COLOR_WHITE,COLOR_BLUE);
  //     init_pair(2,COLOR_BLUE,COLOR_WHITE);
  //     init_pair(3,COLOR_RED,COLOR_WHITE);
  //   */

  //   init_pair(1,COLOR_GREEN,COLOR_BLACK);
  //   init_pair(2,COLOR_BLACK,COLOR_GREEN);
  //   init_pair(3,COLOR_BLUE,COLOR_GREEN);
    
  //   curs_set(0);
  //   noecho();
  //   keypad(stdscr,TRUE);
    
  // } // end init_curses







// int main()
// {
//   init_curses();
//   menuHeader startItUp;
//   startItUp.getKeys();
//   endwin();
//   return 0;
// }
