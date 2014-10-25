#!/bin/bash
g++ Server.o Date.o -o server -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
./server
