all: bff

bff: bff.c

run: bff
	./bff programs/file.bf

clean:
	rm test bff

.PHONY: all run clean
