obj-m += lab3-1_driver.o
KDIR = /home/chang311605015/Documents/linux/
all:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean

