# Conway_GameOfLife
A 'cellular automaton' game, invented by Cambridge mathematician John Conway.
Made using c++ and FLTK 1.3.4 library.

### Here The Rules:

#### For a living cell:
  - Each living cell with one or no neighbors dies by solitude. 
  - Each living cell with four or more neighbors dies by overpopulation. 
  - Each living cell with two or three neighbors survives. 
#### For an empty space:
  - Each cell with exactly three neighbors becomes populated.
    

### Compilation
> Requirements: gcc >= 4 && FLTK lib >= 1.3.4 

g++ main.cpp -o game `fltk-config --ldflags` -std=c++11


*Project done for learning C / FLTK purpose, if you looking for "quality code" pls look away*