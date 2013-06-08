KERNEL_VERSION  := `uname -r`
KERNEL_DIR      := /lib/modules/$(KERNEL_VERSION)/build
INSTALL_MOD_DIR := .

PWD             := $(shell pwd)

obj-m           := dfs.o
sarlkm-objs     := dfs.o

all: build


build:
	@(cd $(KERNEL_DIR) && make -C $(KERNEL_DIR) SUBDIRS=$(PWD) CROSS_COMPILE=$(CROSS_COMPILE) modules)


install:
	cp ./dfs.ko /dev/shm


clean:
	-rm -f *.o *.ko .*.cmd .*.flags *.mod.c Module.symvers version.h modules.order output
	-rm -rf .tmp_versions
