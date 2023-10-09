#ifndef INTERFACE_H
#define INTERFACE_H

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

#define SERIAL "/dev/ttyACM0"
#define BAUD_RATE 96000

FILE* serial_init()
{
    FILE *file;
    int fd,retval;
    struct termios serial_settings;
    
    // file = fopen(SERIAL,"w+");
    // if (file == NULL)
    // {
    //     perror("Couldn't do fopen");
    //     exit(EXIT_FAILURE);
    // }
    fd = open(SERIAL, O_RDWR | O_NOCTTY);
    // fd = fileno(file);
    if (fd < 0)
    {
        perror("Couldn't open SERIAL");
        exit(EXIT_FAILURE);
    }
    if (tcgetattr(fd, &serial_settings) < 0)
    {
        perror("Couldn't get termios struct");
        exit(EXIT_FAILURE);
    }
    if (cfsetospeed(&serial_settings, B9600) < 0)
    {
        perror("Couldn't set out baud rate");
        exit(EXIT_FAILURE);
    }
    if (cfsetispeed(&serial_settings, B9600) < 0)
    {
        perror("Couldnt't set in baud rate");
        exit(EXIT_FAILURE);
    }
    serial_settings.c_cflag &= ~(PARENB | CSTOPB | CSIZE);
    serial_settings.c_cflag |= (CS8 | CREAD | CLOCAL);
    serial_settings.c_iflag &= ~(IXON |IXOFF | IXANY);
    serial_settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    serial_settings.c_oflag &= ~OPOST;
    // serial_settings.c_oflag & =~(ONLCR|OCRNL);
    serial_settings.c_iflag &= ~(INLCR|ICRNL);


    serial_settings.c_cc[VMIN]=1;
    serial_settings.c_cc[VTIME]=5;

    // serial_settings.c_cflag &= ~(PARENB | CSTOPB | CSIZE);
    // serial_settings.c_cflag |= (CS8 | CLOCAL);
    // serial_settings.c_iflag &= ~(IXON |IXOFF | IXANY);
    // serial_settings.c_lflag &= ICANON;
    // serial_settings.c_oflag &= ~OPOST;

    if (tcsetattr(fd,TCSANOW,&serial_settings) < 0)
    {
        perror("Couldn't set serial");
        exit(EXIT_FAILURE);
    }
    
    tcflush(fd, TCOFLUSH);
    //close(fd);
    //file = fdopen(fd,"w+");
    file = fopen(SERIAL,"w+");
    if (file == NULL)
    {
        perror("Couldn't open FILE");
        exit(EXIT_FAILURE);
    }
    //close(fd);
    sleep(1);
    
    {//Test connection
        printf("TEST CONNECTION PART 1..");
        uint8_t test = 134;
        putc(test, file);
        uint8_t test_back = 0;
        test_back = getc(file);
        assert(test_back == test);
        printf(" 2..");
        test = getc(file);
        putc(test+1, file);
        printf(" 3\n");
        test_back = getc(file);
        assert(test + 2 == test_back);
        printf("CONNECTION ESTABLISHED\n");
    }
    
    return file;
}

#endif
