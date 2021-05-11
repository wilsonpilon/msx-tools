//
// Created by wilso on 5/11/2021.
//

#ifndef MSX_TOOLS_WINDOW_H
#define MSX_TOOLS_WINDOW_H

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

#endif //MSX_TOOLS_WINDOW_H
