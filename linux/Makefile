ALL: server

server: server.cpp
	g++ -Wall -o server server.cpp

run: server
	./server

edit:
	vim server.cpp

clean:
	rm -rf server *.o ../WslLinuxSocket/server.sock
