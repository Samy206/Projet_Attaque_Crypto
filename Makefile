S := src/
H := headers/
C := Chiffrement/
D := Dechiffrement/
A := Attaque/
T := target/

O_DIR=BOUMALI_LYONNET

run: compile
	@./$(T)main

compile: $(S)main.c $(T)keys.o $(H)$(C)keys.h $(H)$(C)Encryption.h $(T)Encryption.o $(H)Usefull_tables.h
	@gcc -Wall -Wextra -g  -I$(H) $(S)main.c $(T)keys.o $(T)Encryption.o -o $(T)main -pthread

$(T)keys.o : $(H)$(C)keys.h $(H)$(C)Encryption.h $(T)Encryption.o $(H)Usefull_tables.h
	@gcc -Wall -Wextra -g -I$(H) $(S)$(C)keys.c -o $(T)keys.o -c  -pthread

$(T)Encryption.o : $(H)$(C)Encryption.h  $(H)$(C)keys.h $(H)Usefull_tables.h
	@gcc -Wall -Wextra -g -I$(H) $(S)$(C)Encryption.c -o $(T)Encryption.o -c -pthread

clean:
	@rm $(T)*.o
	@rm $(T)main


tar:
	@rsync -av --exclude='$(O_DIR)' * $(O_DIR)
	@tar zcvf $(O_DIR).tar.gz $(O_DIR)
	@rm -r $(O_DIR)
