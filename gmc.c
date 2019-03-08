/*
 * simple program to get CPM from my GMC Geiger Counter
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
        int cpm = gc_cpm(fd);

        if (cpm != -1)
                printf("%d\n", gc_cpm(fd));
}
