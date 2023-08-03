#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <errno.h>
#include <stdint.h>

//cc main.c -o main

#define SIGETX 44
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

static 	uint32_t value;

void signalhandler(int sig) {
	printf("Button was pressed type %u!\n", value);
}

int main() {
	int fd;
	signal(SIGETX, signalhandler);

	printf("PID: %d\n", getpid());

	/* Open the device file */
	fd = open("/dev/hhgd", O_RDWR | O_NDELAY);
	if(fd < 0) {
		perror("Could not open device file");
		return -1;
	}

printf("---1 \n");	

	int status = 1;
	if( ioctl( fd, HHGD_RELAY_IN3, &status) < 0)
	{
		
	}
		
	char msg[34] = "ciao";
	if( ioctl( fd, HHGD_LCD, &msg) < 0)
	{
		
	}

	/* Register app to KM */
	if(ioctl(fd, REGISTER_APP, &value)) 
	{
		printf("Error registering app %d\n", errno);
		close(fd);
		return -1;
	}


	/* Wait for Signal */
	printf("Wait for signal...\n");

	while(1)
		sleep(1);
	close(fd);

	return 0;
}