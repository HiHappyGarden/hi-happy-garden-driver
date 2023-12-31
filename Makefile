# make -C /lib/modules/$(uname -r)/build M=$(pwd) modules

program_name = hhgd

obj-m += $(program_name).o 
happy_gardenpi_driver-objs := src/main.o \
							src/button.o \
							src/lcd.o \
							src/relay.o \
							src/gpio_config.o \
							src/error.o \
							src/led.o \
							src/parser.o 

ccflags-y := -std=gnu11 -Wno-declaration-after-statement
EXTRA_CFLAGS:= -D TEST=2

PWD := $(CURDIR)
 
all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
 
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


# Utilities for fast testing ...
insert:
	sudo insmod $(program_name).ko	 # insert $ make i
remove:
	sudo rmmod $(program_name)		# remove $ make r
print:
	sudo dmesg				# print  $ make p
c:
	sudo dmesg -c		# clear dmesg	 $ make c
info:
	sudo modinfo $(program_name).ko #userspace program prints .modinfo section

