all: bff

bff: bff.c

run: bff
	./bff programs/hello.bf

clean:
	rm test bff

.PHONY: all run clean
