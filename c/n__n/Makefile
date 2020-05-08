#CROSS = aarch64-himix100-linux-
# CROSS = ""
CC = ${CROSS}gcc

c_src := $(shell ls *.c)
objs := $(patsubst %.c,%.o,$(c_src))

test: $(objs) FORCE
	$(CC) -o $@ $(objs)
%.o: %.c 
	$(CC) -c -g -o $@ $<

clean: 
	rm -rf *.o

FORCE:

PHONY += FORCE

.PHONY: $(PYONY)
