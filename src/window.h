#ifndef WIN_LOCAL_H
#define WIN_LOCAL_H

void			win_init(int width, int height, const char* title);

void			win_exit(void);

float			win_time(void);

int				win_loop(void);

void			win_poll(void);

void			win_cursor_lock(void);

void			win_input_init(void);

#endif
