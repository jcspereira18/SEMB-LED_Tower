# -*- Makefile -*-
all:
	g++ led_tower.cpp -lwiringPi -o led_tower

run:
	./led_tower
