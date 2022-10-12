#ifndef _RIT_H_
#define _RIT_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define CTRL_KEY(k) ((k)&0x1f)

static inline void _ExecOrDie(bool funcReturn, const char *err)
{
    if (!funcReturn)
    {
        perror(err);
        exit(1);
    }
}

#endif
