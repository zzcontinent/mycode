CFLAGS  = -g -D_REENTRANT -I/usr/local/include/SDL2 -DHAVE_OPENGL -g
LIBS	=  -L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -lSDL2

c_src := $(shell ls *.c) $(shell ls utils/*.c) 
objs := $(patsubst %.c,%.o,$(c_src))
test: $(objs)
	gcc -o $@ $(objs) $(CFLAGS) $(LIBS)
%.o: %.c 
	gcc -c -o $@ $< $(CFLAGS) $(LIBS)

clean: 
	rm -rf test $(objs)

FORCE:

PHONY += FORCE

.PHONY: $(PYONY)
