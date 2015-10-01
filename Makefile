FILES=main
BIN=hello
ADD-CFLAGS=-O3

HDD=$(ALMOS_TEST)/pf1/hdd-img.bin

include $(ALMOS_TOP)/include/appli.mk


install:
	mcopy -i $(HDD) $(BIN) ::bin/.