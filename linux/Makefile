ARDUINO_DIR = /mnt/d/Users/levys/Documents/Arduino/libraries

ALL: client server hackflight newserver

HF_DIR = $(ARDUINO_DIR)/Hackflight

HF_SRC = $(HF_DIR)/src/

ETL_SRC = $(ARDUINO_DIR)/etl-arduino/src/

hackflight: hackflight.o 
	g++ -o hackflight hackflight.o 

hackflight.o: hackflight.cpp \
	$(HF_SRC)/core/*.h \
	$(HF_SRC)/core/*/*.h \
	$(HF_SRC)/core/mixers/*.h \
	$(HF_SRC)/core/mixers/fixedpitch/*.h
	g++ $(CFLAGS) -I$(HF_SRC) -I$(ETL_SRC) -c hackflight.cpp

client: client.cpp
	g++ -Wall -o client client.cpp

server: server.cpp
	g++ -Wall -o server server.cpp

newserver: newserver.cpp
	g++ -Wall -o newserver newserver.cpp

run: client
	./client

edit:
	vim client.cpp

clean:
	rm -rf client server newserver hackflight *.o *.sock
