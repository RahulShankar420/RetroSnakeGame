# RetroSnakeGame

## A retro style snake game with C++ and Raylib

## About Project
This is the second game made with raylib and C++, paying homage to the original snake game. The game is a timed puzzle rusher, where you have to keep eating food to make sure the time doesnt run out. The rules of the classic game still apply
* The game is rendered in terms of cells with calculations being done on the cell position, calculated by multiplying the position with the cell size
* THe snake is rendered using a deque so that it moves with the familiar snake like movement
* The game is created in one file for ease of use
* The game stores the last highscore as a readable file so that it can be loaded the next time you open the game.

## Cyber security implementation
* The main file is actually a integrity checker using md5, which verifies if the mainfile has been modified to any extent. If it is not modified, it runs the game. Else, it stops and says invalid file.
* The program has a hardcoded string that contains the hash value, which it compares against the calculated md5 of the game executable
* Though not the most secure or reliable, it gets the job done.

![image](https://github.com/user-attachments/assets/d53bd7b9-ef61-4733-94c4-1b2739b7cb3e)

## Installation Instructions
This build is exclusive to the linux environment. For other OSes, ensure the make file is updated or run the required command
* Download the repository
*  Run the make file
*  Run the generated executable `main`

## Known Issues
* One File is not great for code organization
* HighScore text is not encrypted
* The game file can be directly executed without main
* Integrity of other assets are not checked.
* Integrity file checker prints hashvalue for debug reason

