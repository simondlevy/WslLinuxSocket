ALL: client server

client: client.cpp
	g++ -Wall -o client client.cpp

server: server.cpp
	g++ -Wall -o server server.cpp

run: client
	./client

edit:
	vim client.cpp

clean:
	rm -rf client server *.o *.sock
