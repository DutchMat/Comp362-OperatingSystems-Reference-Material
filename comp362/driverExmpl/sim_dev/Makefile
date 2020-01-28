UNAME	:= $(shell uname -r)
KDIR	:= /lib/modules/$(UNAME)/build
PWD	:= $(shell pwd)

MOD_TARGET = sim_dev
simdev-objs := sim_dev.o disk.o

obj-m	+= $(MOD_TARGET).o

default::
	$(MAKE) -C "$(KDIR)" M="$(PWD)" modules
	
clean::
	$(MAKE) -C "$(KDIR)" M="$(PWD)" clean
