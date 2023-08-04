#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

//cc main.c -o main

#define SIGETX 10
#define REGISTER_APP _IOW('a','a',uint32_t*)	


enum hhgd_type
{
    HHGD_LED_GREEN,
    HHGD_LED_RED,
    HHGD_RELAY_IN1,
    HHGD_RELAY_IN2,
    HHGD_RELAY_IN3,
    HHGD_RELAY_IN4,
    HHGD_BUTTON_NEXT,
    HHGD_BUTTON_BEFORE,
    HHGD_LCD,
    HHGD_NONE,
};

enum hhgd_button_status
{
    HHGD_BUTTON_NEXT_ON = 0x01,
    HHGD_BUTTON_BEFORE_ON = 0x02
};


static bool done = false;
static int32_t check = 0;
static int32_t fd;
 
void ctrl_c_handler(int n, siginfo_t *info, void *unused)
{
    if (n == SIGINT) 
	{
        printf("\nrecieved ctrl-c\n");
        done = true;
    }
}
 
void sig_event_handler(int n, siginfo_t *info, void *unused)
{
    if (n == SIGETX) 
	{
        check = info->si_int;
        printf ("Received signal from kernel : Value =  %u\n", check);
		switch (info->si_int)
		{
		case HHGD_BUTTON_NEXT_ON:
			printf ("Handled HHGD_BUTTON_NEXT on LCD\n");
		
			char msg[34] = "Handled HHGD_BUTTON_NEXT";
			if( ioctl( fd, HHGD_LCD, msg) < 0)
			{
				perror("Fail handle HHGD_BUTTON_NEXT");
			}

			break;
		case HHGD_BUTTON_BEFORE_ON:
			printf ("Handled HHGD_BUTTON_BEFORE turn on HHGD_LED_GREEN\n");
			uint32_t value = 1;
			if( ioctl( fd, HHGD_LED_GREEN, &value) < 0)
			{
				perror("Fail handle HHGD_BUTTON_BEFORE");
			}
			break;
		default:
            printf ("No handled signal value =  %u\n", check);
			break;
		}
    }
}

int main()
{
    int32_t value, number;
    struct sigaction act;


    sigemptyset (&act.sa_mask);
    act.sa_flags = (SA_SIGINFO | SA_RESETHAND);
    act.sa_sigaction = ctrl_c_handler;
    sigaction (SIGINT, &act, NULL);
 
    /* install custom signal handler */
    sigemptyset(&act.sa_mask);
    act.sa_flags = (SA_SIGINFO | SA_RESTART);
    act.sa_sigaction = sig_event_handler;
    sigaction(SIGETX, &act, NULL);
 
 
    fd = open("/dev/hhgd", O_RDWR);
    if(fd < 0) 
	{
		perror("Cannot open device file");
		return 0;
    }
 
    if (ioctl(fd, REGISTER_APP,(int32_t*) &number)) 
	{
        perror("Failed");
        close(fd);
        exit(1);
    }
   
    while(!done) {
        printf("Waiting for signal...\n");
 
        //blocking check
        while (!done && !check)
        {
            sleep(1);
        }
        check = 0;
    }

    close(fd);
}