
main: main.c libtest_so.so
	gcc -o main main.c -ltest_so -L.

libtest_so.so: test_so.c
	gcc -c -fPIC -o libtest_so.o test_so.c
	gcc -shared -o libtest_so.so libtest_so.o

clean:
	@rm libtest_so.so libtest_so.o main
