#!/bin/sh
g++ -o querykeymap-spike querykeymap-spike.cpp -lX11
echo "type something"
./querykeymap-spike | xxd
