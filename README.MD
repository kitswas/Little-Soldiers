# Little Soldiers

A game with AI.

## Motivation

It was for one of my undergraduate courses.  
See the [Problem Statement](./Problem_Statement.pdf).

## How to Build

You need GCC on your system. [Download and install MinGW](https://www.mingw-w64.org/downloads/) on Windows.  
Open the file [Build commands.txt](./Build_commands.txt). Copy and paste everything on your console.  
Then execute the generated file `game.exe`.

## How to Play

See the in-game help.
If you need a sample run, see the file [Game_demo_run_commands.txt](./Game_demo_run_commands.txt).  
Copy and paste all commands in any set.

## About the Project

**I recommend going through the section with the same name in the file [Project_Details.pdf](./Project_Details.pdf).**  
In case you are too lazy to do so, here's an excerpt:

The greatest feature of this project's sourcecode is its portability.
It is entirely written in standard C++.  
No `#include<windows.h>`, no use of external libraries like ncurses.  
Hence, though parts of the code have to be commented out or uncommented, it is possible to compile and run the program on Windows, Linux and even on online compilers.  
Code in `/**Windows only*/  /**Windows end*/` needs to be enabled on Windows.  
Code in `/**Linux only*/  /**Linux end*/` should be enabled on Linux.  

Since this project aims to create a game, it is essential to clear the screen every-so-often.  
The cls macro was defined to achieve this.

```c++
#define cls system("cls") //clear screen on Windows
#define cls system("clear") //clear screen on Linux
#define cls cout << "\033[2J\033[1;1H" //ANSI code to clear screen on both Windows 10 and Linux.
//Not used to enable execution on old Windows OSes. 
```

Note that although the ANSI code is more versatile, Windows does not support ANSI codes.  
Windows 10 does, but it is not default behaviour.  

_See: <https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences> and <https://en.wikipedia.org/wiki/ANSI_escape_code#DOS,_OS/2,_and_Windows>_

The settings are stored in the settings.ini file (generated on first-run).  
I advise against directly modifying the configuration file.  
In case of unintended modifications or program errors, delete the file and you are good to go.  
_Default values will be used in such a case._

The macro STEPS in `play.cpp` defines the number of moves the minimax algorithm will calculate in advance to find the optimal path.  
Increase it for a more challenging game.  
Higher values lead to more resource consumption and lower execution speed.  

AI vs AI matches are possible, but not recommended.  
This is because some of the assumptions made about the player's behaviour are invalid for an AI.  
