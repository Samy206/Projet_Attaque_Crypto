run: compile
	./main

compile: main.c keys.c keys.h
	gcc -Wall -Wextra -g main.c keys.c -o main

clean: 
	rm main

tar: 
	cd .. && tar zcvf LYONNET.tar.gz LYONNET/
