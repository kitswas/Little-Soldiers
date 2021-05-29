/******************************************************************************
* @author Swastik Pal
* @version 1.0
* Date 28/03/2021
* About: The Units
*******************************************************************************/

//macros here

//header files here
#include"header.h"

//functions here

//class members
Unit::Unit(int i)
{
	health = 5;
	contribution = 0;
	index = i;
	isDead = false;
	switch(i)
	{
		case 0:
			attack = 2;
			defence = 2;
			movement = 3;
		break;
		case 1:
			attack = 3;
			defence = 3;
			movement = 1;
		break;
		case 2:
			attack = 4;
			defence = 1;
			movement = 2;
		break;
		case 3:
			attack = 3;
			defence = 2;
			movement = 2;
		break;
	}
}

string unitNames[]={"Knight   ", "Pikeman  ", "Rogue    ", "Swordsman"};
char unitLabels[2][4]={	{'K', 'P', 'R', 'S'},
						{'k', 'p', 'r', 's'}};
