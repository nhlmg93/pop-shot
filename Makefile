.PHONY: all

all:
	gcc main.c -o pop-shot -lraylib -lm

run: 
	make && ./pop-shot

clean:
	rm -f rt
