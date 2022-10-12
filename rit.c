#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rit.h"

struct termios _DEFAULT_TERMIOS;

/* Restore terminal to original state */
void disableRawMode()
{
    _ExecOrDie(tcsetattr(STDIN_FILENO, TCSAFLUSH, &_DEFAULT_TERMIOS), "tcsetatte");
}

/* Turns off canonical mode to take input byte by byte instead of
 * line by line and disable echoing.
 */
void enableRawMode()
{
    _ExecOrDie(tcgetattr(STDERR_FILENO, &_DEFAULT_TERMIOS), "tchetattr");

    // Exit Callback - Register terminal to default state
    atexit(disableRawMode);

    struct termios rawMode =
        {
            .c_cc[VMIN] = 0,
            .c_cc[VTIME] = 1,
        };

    // Disbale Ctrl-S, Ctrl-Q software flow control
    rawMode.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    // Disable output processing
    rawMode.c_oflag &= ~(OPOST);
    rawMode.c_cflag &= ~(CS8);

    // Disable Ctrl-C `SIGINT` and Ctrl-Z `SIGTSTP`
    rawMode.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    tcsetattr(STDERR_FILENO, TCSAFLUSH, &rawMode);
}

char edReadKey()
{
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
    {
        _ExecOrDie((nread == -1 && errno != EAGAIN), "read");
    }
    return c;
}

void edProcessKeyPress()
{
    char c = edReadKey();

    switch (c)
    {
    case CTRL_KEY('q'):
        exit(0);
        break;
    }
}

int main()
{

    enableRawMode();

    while (1)
    {
        edProcessKeyPress();
    }

    return 0;
}
