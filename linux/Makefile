PROJECT: client

$(PROJECT): $(PROJECT).cpp
	g++ -Wall -o $(PROJECT) $(PROJECT).c

run: client
	./client

edit:
	vim client.cpp

clean:
	rm -rf client *.o *.sock
