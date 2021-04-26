S := src/
H := headers/
C := Chiffrement/
D := Dechiffrement/

O_DIR=BOUMALI_LYONNET

run: compile
	./main

compile: $(S)main.c keys.o $(H)$(C)keys.h $(H)$(C)Encryption.h Decryption.o Encryption.o
	gcc -Wall -Wextra -g $(S)main.c keys.o Encryption.o Decryption.o -o main -lm

keys.o : $(H)$(C)keys.h $(H)$(C)Encryption.h Encryption.o
	gcc -Wall -Wextra -g $(S)$(C)keys.c -o keys.o -c

Encryption.o : $(H)$(C)Encryption.h  $(H)$(C)keys.h
	gcc -Wall -Wextra -g  $(S)$(C)Encryption.c -o Encryption.o -c

Decryption.o: $(H)$(D)Decryption.h $(S)$(D)Decryption.c
	gcc -Wall -Wextra -g $(S)$(D)Decryption.c -o Decryption.o -c

clean:
	rm -rf main keys.o Encryption.o

tar:
	rsync -av --exclude='$(O_DIR)' * $(O_DIR)
	tar zcvf $(O_DIR).tar.gz $(O_DIR)
	rm -r $(O_DIR)
