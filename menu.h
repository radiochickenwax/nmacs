/*

 */

#ifndef MENU_H
#define MENU_H

#include <ncurses.h>
//#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include <string>

#define ENTER 10
#define ESCAPE 27

using std::vector;
using std::string;

struct dropDownMenu{
  string name;
  int xStart; // x-coordinate to draw from
  vector<string> subItem; // subitems
  vector<WINDOW*> window;
  // vector of function pointers? TODO
};


class menuHeader{
public:
  // constructor
  menuHeader();  // end constructor function
  ~menuHeader();
  // member variables
  int key;
  WINDOW *menubar; // menubar contains subwins?
  WINDOW *messagebar; 
  vector<dropDownMenu> subMenu; // container for dropDownMenus

  // member functions
  
  int mod(int a, int b) { 
    int r = a % b;
    return r < 0 ? r + b : r;
  }
  
  string getKeys();
  string getKeys(int key);
  string handleSubMenu(int num);
  void setStartCoordinates();
  // draws all submenu names in class horizontally across screen
  void drawMenuBar();
  // recurse over vector of names
  int getSubMenuNameLengths(int n, int sum);
  void drawSubMenu(int number);  
  void deleteSubMenu(int number);  
  string scrollSubMenu(int number);  
}; // end menuHeader class

#endif //MENU_H
