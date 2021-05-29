/******************************************************************************
* @author Swastik Pal
* @version 1.0
* Date 28/03/2021
* About: This header file contains everything needed for this project
*******************************************************************************/

#ifndef HEADER_H
#define HEADER_H

//macros here
#define TOTAL_COL 120
#define TOTAL_ROW 50
// #define TOTAL_COL 200
// #define TOTAL_ROW 60
#define PLAYER_1 0
#define PLAYER_2 1
#define COMPUTER 1
/**Windows only*/
#define cls system("cls") //clear screen on Windows
/**Windows end*/
/**Linux only**/
//#define cls system("clear") //clear screen on Linux
/**Linux end*/
//#define cls cout << "\033[2J\033[1;1H" //code to clear screen on both Windows 10 and Linux. Not used to enable execution on old Windows OSes

//header files here
#include<iostream>
#include<fstream>
#include<string>

//namespace items here
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::string;

//global variables here
enum controller{human, randomAI, greedyAI, minimaxAI};
extern controller P1control, P2control;
extern string controllerNames[];
extern char unitLabels[2][4];
extern string unitNames[];
extern bool enableTraits;

//classes here
struct Pos//Unit locations in alphabetical order K, P, R, S//Unit locations as HV; like A1, B6
{
    char col;
    int row;
};

class Unit
{
    private:
    int attack;
    int defence;
    int movement;
    int health;
    int contribution;//for debugging only
    int index;

    public:
    bool isDead;
    Pos position;
    Unit(int);
    Unit* getCurrentInstance(){return this;}
    string getName(){return unitNames[index];}
    int getAttack(){return attack;}
    int getDefence(){return defence;}
    int getMovement(){return movement;}
    int getHealth(){return health;}
    void setHealth(int hp){health = hp;}
    int getIndex(){return index;}
    int getContribution(){return contribution;}
    void setContribution(int c){contribution = c;}
};

class Board
{
    private:
    static const char HLabels[10];//={'X','A','B','C','D','E','F','G','H','I'};
    static const int VLabels[9];//={1,2,3,4,5,6,7,8,9};
    char playArea[9][9];
    double state;//calculated as (Player_2-Player_1) (total_health - units_dead*WEIGHTAGE_OF_DEATH)
    class Player
    {
        public:
        Unit unitSet[4]={Unit(0), Unit(1), Unit(2), Unit(3)};

    } player[2];

    public:

    Board();
    void clearPos(char&, int&);
    void clone(Board board);
    void display();
    Board* getCurrentInstance(){return this;}
    double getState();
    Unit* getUnit(int, int);
    bool hasRemainingUnits(int);
    bool isLegalMove(int, Unit*, char, int);
    int isOccupied(char, int);
    int makeMove(int, Unit*, char, int);
    void reset();
    void setPos(int, char, int, int);
    void showDetails();
};

//functions here
void timed_delay(double);
void movexy(int, int);
int sgn(int);
void welcomeScreen();
void play();
void help();
void credits();
void settings();
void menu();
bool hasEnded();
void showUnit(Unit*);
void manual(int=PLAYER_1);
void AIrandom(int=COMPUTER);
void AIgreedy(int=COMPUTER);
void AIminimax(int=COMPUTER);
void loadState(string, Board*);
void saveState(string, Board*);

#endif
