FILES=main
BIN=hello
TXT=text
ADD-CFLAGS=-O3

HDD=$(ALMOS_TEST)/pf1/hdd-img.bin

include $(ALMOS_TOP)/include/appli.mk


install:
	mcopy -oi $(HDD) $(BIN) ::bin/.
	mcopy -oi $(HDD) $(TXT) ::bin/.
