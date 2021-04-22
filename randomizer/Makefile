obj-m = randomizer.o

kdir = /lib/modules/`uname -r`/build

build:
	make -C $(kdir) M=`pwd`
	make testmodule
clean:
	make -C $(kdir) M=`pwd` clean
	rm testmodule
insert:
	sudo insmod randomizer.ko
remove:
	sudo rmmod randomizer.ko
logger:
	sudo dmesg
random:
	sudo ./testmodule