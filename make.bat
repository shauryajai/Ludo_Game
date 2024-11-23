@echo off
g++ -std=c++11 -IC:/SFML/include -LC:/SFML/lib *.cpp -o executable/ludo.exe -lsfml-graphics -lsfml-window -lsfml-system
