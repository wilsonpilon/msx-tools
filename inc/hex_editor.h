#ifndef HEX_EDITOR_H_INCLUDED
#define HEX_EDITOR_H_INCLUDED

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int hex_editor(string);

#endif // HEX-EDITOR_H_INCLUDED
