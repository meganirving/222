#!/bin/bash
g++ Client.o Input.o -o client -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
./client
