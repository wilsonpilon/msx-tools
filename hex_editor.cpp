#include <iostream>
#include <ncurses.h>
#include <string>

using std::string;
using std::endl;
using std::cout;
using std::cin;

#include "hex_editor.h"
#include "window.h"

int hex_editor(string);

int hex_editor(string arquivo) {
  int tecla = 0;
  int cy = 0, cx = 0;

  string bars = std::string(80, '=');
  string title = "MSX Hex-Editor";
  tecla =getch();

  WINDOW *my_win;
  int startx, starty, width, height;
  int ch;

  initscr();			/* Start curses mode 		*/
  cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
  keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

  height = 24;
  width = 80;
  starty = (LINES - height) / 2;	/* Calculating for a center placement */
  startx = (COLS - width) / 2;	/* of the window		*/
  printw("Press F1 to exit");
  refresh();
  my_win = create_newwin(height, width, starty, startx);

  while((ch = getch()) != KEY_F(1))
  {	switch(ch)
    {	case KEY_LEFT:
        destroy_win(my_win);
        my_win = create_newwin(height, width, starty,--startx);
        break;
      case KEY_RIGHT:
        destroy_win(my_win);
        my_win = create_newwin(height, width, starty,++startx);
        break;
      case KEY_UP:
        destroy_win(my_win);
        my_win = create_newwin(height, width, --starty,startx);
        break;
      case KEY_DOWN:
        destroy_win(my_win);
        my_win = create_newwin(height, width, ++starty,startx);
        break;
    }
  }

  endwin();			/* End curses mode		  */


  tecla = getch();

  noecho();
  move(0,0);
  printw("%s", bars.c_str());
  move(1,2);
  printw("%s", title.c_str());
  move(2,0);
  printw("%s", bars.c_str());
  move(21,0);
  printw("%s", bars.c_str());
  move(22,0);
  printw("Status: Editando: %s\n", arquivo.c_str());

  while((tecla = getch()) != 27) {
    attron(A_BOLD);
    printw("%c", tecla);
    attroff(A_BOLD);
  }

  refresh();
  endwin();

  return 0;
}

