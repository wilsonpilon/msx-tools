#include <iostream>
#include <ncurses.h>
#include <string>

using std::string;
using std::endl;
using std::cout;
using std::cin;

#include "../inc/hex_editor.h"

int hex_editor(string arquivo) {
  int tecla = 0;
  int cy = 0, cx = 0;
  string bars = std::string(80, '=');
  string title = "MSX Hex-Editor";

  initscr();
  raw();
  keypad(stdscr, true);
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

