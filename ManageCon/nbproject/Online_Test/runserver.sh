#!/bin/bash
g++ Server.o Input.o -o server -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
./server
