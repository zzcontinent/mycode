MODULE_NAME = hello_mod
$(MODULE_NAME)-objs := hello.o
obj-m := $(MODULE_NAME).o
KERNEL_DIR = /usr/src/linux-headers-`uname -r`
MODULEDIR = $(shell pwd)

.PHONY: modules
default: modules 

modules: 
	make -C $(KERNEL_DIR) M=$(MODULEDIR)
print:
	echo $(KERNELRELEASE)
	echo $(MODULE_NAME)
	echo $(MODULEDIR)
	echo $(KERNEL_DIR)
clean distclean:
	# $(KERNEL_DIR)
	# $(MODULEDIR) 
	# $(KERNELRELEASE) 
	# $(MODULE_NAME)
	rm -f *.o *.mod *.ko *.order *.symvers *.mod.c
	rm -rf .tmp_versions
	rm -rf .*.cmd
