S := src/
H := headers/
C := Chiffrement/
D := Dechiffrement/
A := Attaque/
T := target/

O_DIR=BOUMALI_LYONNET

run: compile
	@./$(T)main

compile: $(S)main.c $(T)keys.o $(H)$(C)keys.h $(H)$(C)Encryption.h $(H)$(D)Decryption.h  $(T)Encryption.o $(T)Attack.o $(H)Usefull_tables.h $(T)Decryption.o
	@gcc -Wall -Wextra -g -I$(H) $(S)main.c $(T)keys.o $(T)Attack.o $(T)Encryption.o $(T)Decryption.o -o $(T)main -pthread

$(T)keys.o : $(H)$(C)keys.h  $(H)Usefull_tables.h
	@gcc -Wall -Wextra -g -I$(H) $(S)$(C)keys.c -o $(T)keys.o -c  -pthread

$(T)Encryption.o : $(H)$(C)Encryption.h  $(H)$(C)keys.h $(H)Usefull_tables.h
	@gcc -Wall -Wextra -g -I$(H) $(S)$(C)Encryption.c -o $(T)Encryption.o -c -pthread

$(T)Decryption.o : $(H)$(C)Encryption.h $(H)$(D)Decryption.h  $(H)$(C)keys.h $(H)Usefull_tables.h
	@gcc -Wall -Wextra -g -I$(H) $(S)$(D)Decryption.c -o $(T)Decryption.o -c -pthread

$(T)Attack.o : $(H)$(C)Encryption.h  $(H)$(C)keys.h $(H)$(A)Attack.h $(H)Usefull_tables.h
	@gcc -Wall -Wextra -g -I$(H) $(S)$(A)Attack.c -o $(T)Attack.o -c -pthread

clean:
	@rm $(T)*.o
	@rm $(T)main


tar:
	@rsync -av --exclude='$(O_DIR)' * $(O_DIR)
	@tar zcvf $(O_DIR).tar.gz $(O_DIR)
	@rm -r $(O_DIR)
