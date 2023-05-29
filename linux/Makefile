PROJECT: client

$(PROJECT): $(PROJECT).c
	gcc -Wall -o $(PROJECT) $(PROJECT).c

run: client
	./client

edit:
	vim client.c

clean:
	rm -rf client *.o *.sock
