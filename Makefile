T := target/
S := src/
H := headers/
C := Chiffrement/
D := Dechiffrement/

O_DIR=BOUMALI_LYONNET

run: $(T)compile
	@./$(T)main

$(T)compile: $(S)main.c $(T)keys.o $(H)$(C)keys.h $(H)$(C)Encryption.h $(T)Encryption.o
	@gcc -Wall -Wextra -g $(S)main.c $(T)keys.o $(T)Encryption.o -o $(T)main


$(T)keys.o : $(H)$(C)keys.h $(H)$(C)Encryption.h
	@gcc -Wall -Wextra -g $(S)$(C)keys.c -o $(T)keys.o -c

$(T)Encryption.o : $(H)$(C)Encryption.h  $(H)$(C)keys.h
	@gcc -Wall -Wextra -g $(T)keys.o $(S)$(C)Encryption.c -o $(T)Encryption.o -c

clean:
	@rm -rf $(T)main.dSYM
	@rm $(T)*

tar:
	@rsync -av --exclude='$(O_DIR)' * $(O_DIR)
	@tar zcvf $(O_DIR).tar.gz $(O_DIR)
	@rm -r $(O_DIR)