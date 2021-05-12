#include <iostream>
#include <ncurses.h>
#include <cdk.h>
#include <string>

using std::string;
using std::endl;
using std::cout;
using std::cin;

#include "../inc/hex_editor.h"
#include "../inc/window.h"

int hex_editor(string);

int hex_editor(string arquivo) {
  CDKSCREEN *cdkscreen;
  CDKLABEL *demo;
  WINDOW *screen;
  const char *mesg[4];
  screen=initscr();
  cdkscreen = initCDKScreen(screen);
  initCDKColor();
  mesg[0] = "</31>This line should have a yellow foreground and a cyan background.<!31>";
  mesg[1] = "</32>This line should have a white  foreground and a blue background.<!32>";
  mesg[2] = "</33>This line should have a yellow foreground and a red  background.<!33>";
  mesg[3] = "<C>This line should be set to whatever the screen default is.";


  demo = newCDKLabel (cdkscreen, CENTER, CENTER, mesg, 4, TRUE, TRUE);

  drawCDKLabel (demo, TRUE);
  waitCDKLabel (demo, ' ');

  destroyCDKLabel (demo);

  destroyCDKScreen (cdkscreen);
  endCDK();


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

