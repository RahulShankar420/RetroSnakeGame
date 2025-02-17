#remove the @ to see the command being printed
all:
	@g++ hashcheck.cpp -o main -lssl -lcrypto
	@g++ main.cpp -L./lib -I./include -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o game

