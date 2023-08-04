#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>

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


static void signal_handler(int sig) {
	printf("Button was pressed type %d!\n", sig);
}

static void signal_handler_info(int sig, siginfo_t *info, void *context)
{
	printf("Button was pressed type %d ", sig);
	if(info)
	{
		printf("info %d", info->si_value);
	}
	printf("!");

}

int main() {
	int fd;
	//signal(SIGETX, signal_handler);

	struct sigaction sa = {
		.sa_sigaction = signal_handler_info,
		.sa_flags = SA_SIGINFO
	};
	sigset_t mask = {0};

	// memset(&sa, 0x00, sizeof(sa));
	// sa.sa_sigaction = signal_handler_info;
	// sa.sa_flags = SA_SIGINFO;

	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("Could not register signal");
		exit(1);
	}
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

	printf("PID: %d\n", getpid());

	/* Open the device file */
	fd = open("/dev/hhgd", O_RDWR | O_NDELAY);
	if(fd < 0) {
		perror("Could not open device file");
		return -1;
	}

// 	int status = 1;
// 	if( ioctl( fd, HHGD_RELAY_IN3, &status) < 0)
// 	{
		
// 	}
		
// 	char msg[34] = "ciao";
// 	if( ioctl( fd, HHGD_LCD, &msg) < 0)
// 	{
		
// 	}

	/* Register app to KM */
	uint32_t foo;
	if(ioctl(fd, REGISTER_APP, &foo)) 
	{
		printf("Error registering app %d\n", errno);
		close(fd);
		return -1;
	}


	/* Wait for Signal */
	printf("Wait for signal...\n");

	while(1)
	{
		sigprocmask(SIG_BLOCK, &mask, NULL);
        sleep(3);
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
	}

	close(fd);

	return 0;
}