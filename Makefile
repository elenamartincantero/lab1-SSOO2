DIREXE := exec/
DIRSRC := src/
DIRSTD := estudiantes/


CC := gcc

all : dirs manager PA PB PC PD daemon exe

dirs:
	mkdir -p $(DIREXE)

manager: 
	$(CC) -o $(DIREXE)$@ $(DIRSRC)$@.c -g

PA: 
	$(CC) -o $(DIREXE)$@ $(DIRSRC)$@.c -g

PB:
	$(CC) -o $(DIREXE)$@ $(DIRSRC)$@.c -g

PC: 
	$(CC) -o $(DIREXE)$@ $(DIRSRC)$@.c -g

PD: 
	$(CC) -o $(DIREXE)$@ $(DIRSRC)$@.c -g

daemon:
	$(CC) -o $(DIREXE)$@ $(DIRSRC)$@.c -g

exe:
	./$(DIREXE)daemon
	./$(DIREXE)manager

clean:
	rm -rf *~ core $(DIREXE) $(DIRSTD)

