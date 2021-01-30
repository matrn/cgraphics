#include "utils.h"



int min(int a, int b){
	return a<b?a:b;
}

int max(int a, int b){
	return a>b?a:b;
}


term_info_t get_terminal_info(){
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	term_info_t terminal = {
		.width = w.ws_col,
		.height = w.ws_row
	};

	/*
    printf ("lines %d\n", w.ws_row);
    printf ("columns %d\n", w.ws_col);
	*/

    return terminal;
}


void clear_terminal(){
	term_info_t term = get_terminal_info();

	printf("\x1b[H");   //home cursor
	for(int i = 0; i < term.height; i ++){
		printf("\033[K\n");
	}
}

void reset_colors(){
	printf("\033[0m");
}



/* msleep(): sleep for the requested number of milliseconds. */
//https://stackoverflow.com/a/1157217/7351855
int msleep(long msec){
    struct timespec ts;
    int res;

    if (msec < 0){
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}