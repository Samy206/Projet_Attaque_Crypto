S := src/
H := headers/
C := Chiffrement/
D := Dechiffrement/
T := target/

O_DIR=BOUMALI_LYONNET

run: compile
	@./$(T)main

compile: $(S)main.c $(T)keys.o $(H)$(C)keys.h $(H)$(C)Encryption.h $(T)Decryption.o $(T)Encryption.o
	@gcc -Wall -Wextra -g $(S)main.c $(T)keys.o $(T)Encryption.o $(T)Decryption.o -o $(T)main -lm

$(T)keys.o : $(H)$(C)keys.h $(H)$(C)Encryption.h $(T)Encryption.o
	@gcc -Wall -Wextra -g $(S)$(C)keys.c -o $(T)keys.o -c

$(T)Encryption.o : $(H)$(C)Encryption.h  $(H)$(C)keys.h
	@gcc -Wall -Wextra -g  $(S)$(C)Encryption.c -o $(T)Encryption.o -c

$(T)Decryption.o: $(H)$(D)Decryption.h $(S)$(D)Decryption.c
	@gcc -Wall -Wextra -g $(S)$(D)Decryption.c -o $(T)Decryption.o -c

clean:
	@rm -rf ./$(T)

tar:
	@rsync -av --exclude='$(O_DIR)' * $(O_DIR)
	@tar zcvf $(O_DIR).tar.gz $(O_DIR)
	@rm -r $(O_DIR)
