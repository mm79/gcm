/*
 * simple program to get CPM from my GMC Geiger Counter
 *
 * output example:
 * GMC-300Re 4.22
 * 11 CPM
 */


#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

int
gc_open(const char *device)
{
        struct termios ts;
        int fd;

        memset(&ts, 0, sizeof(ts));
        ts.c_cflag = CS8 | CREAD | CLOCAL;
        ts.c_cc[VMIN] = 0;
        ts.c_cc[VTIME] = 5;

        if ((fd = open(device, O_RDWR)) < 0)
                return -1;

        if (cfsetspeed(&ts, B57600) < 0)
                return -1;

        if (tcsetattr(fd, TCSANOW, &ts) < 0)
                return -1;

        return fd;
}

char *
gc_getver(int fd)
{
        static char buf[14+1];

        if (write(fd, "<GETVER>>", 9) < 9)
                return NULL;
        if (read(fd, buf, sizeof buf) != 14)
                return NULL;

        buf[14] = '\0';

        return buf;
}

int
gc_cpm(int fd)
{
        char buf[2];

        if (write(fd, "<GETCPM>>", 9) < 9)
                return -1;
        if (read(fd, buf, 2) < 2)
                return -1;

        return buf[0] * 256 + buf[1];
}

int
main()
{
        int fd = gc_open("/dev/cuaU0");
        char *ver = gc_getver(fd);
        int cpm = gc_cpm(fd);

        if (ver != NULL)
                printf("%s\n", ver);

        if (cpm != -1)
                printf("%d CPM\n", gc_cpm(fd));

        gc_getver(fd);
}

