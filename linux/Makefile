PROJECT: client

$(PROJECT): $(PROJECT).c
	gcc -o $(PROJECT) $(PROJECT).c

run: client
	./client

edit:
	vim $(PROJECT).c

clean:
	rm -rf $(PROJECT) *.o
