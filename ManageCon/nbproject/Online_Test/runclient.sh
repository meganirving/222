#!/bin/bash
g++ Client.o Classes.o -o client -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
./client
