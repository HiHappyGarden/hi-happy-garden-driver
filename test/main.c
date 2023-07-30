#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>

//cc main.c -o main

enum hhgd_type
{
    HHGD_LED,
    HHGD_RELAY_1,
    HHGD_RELAY_2,
    HHGD_RELAY_3,
    HHGD_RELAY_4,
    HHGD_BUTTON,
    HHGD_LCD,
};

#define SIGETX 44
#define REGISTER_UAPP _IO('R', 'g')

void signalhandler(int sig) {
	printf("Button was pressed!\n");
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
	if( ioctl( fd, HHGD_RELAY_1, &status) < 0)
	{
		
	}
		
	char msg[34] = "ciao";
	if( ioctl( fd, HHGD_LCD, &msg) < 0)
	{
		
	}

	// /* Register app to KM */
	// if(ioctl(fd, REGISTER_UAPP, NULL)) {
	// 	perror("Error registering app");
	// 	close(fd);
	// 	return -1;
	// }


	/* Wait for Signal */
	// printf("Wait for signal...\n");
	// while(1)
	// 	sleep(1);

	return 0;
}