#!/bin/bash
g++ Client.o User.o Author.o Reviewer.o Admin.o Date.o -o client -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
./client
