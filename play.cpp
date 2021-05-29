/******************************************************************************
* @author Swastik Pal
* @version 1.0
* Date 28/03/2021
* About: The AIengine and Game Mechanism
*******************************************************************************/

//macros here
#define STEPS 4 //must be even
#define PAUSE_BEFORE_NEXT 3 //number of seconds to wait before refreshing screen

//header files here
#include"header.h"
#include<climits>

//namespace items here

//global variables
Board board;
struct Move
{
	int player;
	Unit* unit;
	char col;
	int row;
} moveSequence[STEPS];

//functions
bool hasEnded()/**Checks if the game has ended and displays messages accordingly*/
{
	if(!board.hasRemainingUnits(PLAYER_2))
	{
		cls;
		movexy(TOTAL_COL/2 - 7, TOTAL_ROW/2);
		cout<<"Player 1 Wins\n";
		timed_delay(6);//Let the winner celebrate
		return true;
	}
	else if(!board.hasRemainingUnits(PLAYER_1))
	{
		cls;
		movexy(TOTAL_COL/2 - 7, TOTAL_ROW/2);
		cout<<"Player 2 Wins\n";
		timed_delay(6);//Let the winner celebrate
		return true;
	}
	return false;
}

void initialize()
{
	//complete
    board.reset();
	char col;
	int row;
	for(int i=0; i<4; i++)
	{
		cls;
		cout<<"Place your units on the board:\n(Unit locations as HV; like A1, B6)\n\n";
		board.display();
		cout<<'\n'<<unitNames[i]<<'\n';
		cout<<"Column(A~I):";
		cin>>col;
		if(col<'A' || col>'I')
		{
			--i;
			continue;
		}
		cout<<"Row(7~9):";
		cin>>row;
		if(row<7 || row>9)
		{
			--i;
			continue;
		}
		if(board.isOccupied(col, row))
		{
			cout<<"Occupied\n";
			--i;
			timed_delay(0.3);
			continue;
		}
		board.setPos(PLAYER_1, col, row, i);
		board.setPos(PLAYER_2, col, 10 - row, i);
	}
}

void play()
{
	initialize();
	// int turn = 0;//variable turn for debugging only
	while(true)
	{
		if(hasEnded())
			return;
		cls;
		// ++turn;
        // cout<<"Turn:"<<turn<<'\n';
		board.display();
		board.showDetails();
		switch(P1control)
		{
			case controller::human:
				manual();
				break;
			case controller::randomAI:
				AIrandom(PLAYER_1);
				break;
			case controller::greedyAI:
				AIgreedy(PLAYER_1);
				break;
			case controller::minimaxAI:
				AIminimax(PLAYER_1);
				break;
		}
		if(hasEnded())
			return;
		cout<<"Waiting for opponent...\n";
		switch(P2control)
		{
			case controller::human:
				manual(PLAYER_2);
				break;
			case controller::randomAI:
				AIrandom();
				break;
			case controller::greedyAI:
				AIgreedy();
				break;
			case controller::minimaxAI:
				AIminimax();
				break;
		}
	}
}

void manual(int player)
{
	//complete
	char col;
	int row;
	while(true)
	{
		cls;
		board.display();
		board.showDetails();
		int ch;
		cout<<"Choose unit:(1, 2, 3, 4)\n";
		cin>>ch;
		if(ch<1 || ch>4)
			continue;
		Unit* unit=board.getUnit(ch-1, player);
		if(unit->isDead)
		{
			cout<<"Cannot choose a dead unit\n";
			timed_delay(0.3);
			continue;
		}
		cout<<"Move to - \n";
		cout<<"Column(A~I):";
		cin>>col;
		cout<<"Row(1~9):";
		cin>>row;
		if(!board.isLegalMove(player, unit, col, row))
		{
			cout<<"Illegal move";
			timed_delay(0.3);
			continue;
		}
		board.makeMove(player, unit, col, row);
		return;
	}
}

void AIrandom(int player)
{
	//complete
	Unit* unit = nullptr;
	while(true)
	{
		int idx = rand()%4;
		unit = board.getUnit(idx, player);
		if((unit->isDead))
			continue;
		int i;
		char desCol = -1;
		int desRow = -1;
		for(i=unit->getMovement(); i>0; i--)
		{
			if(board.isLegalMove(player, unit, (desCol=unit->position.col), (desRow=unit->position.row+i))) //Move down
			{
				break;
			}
			else if(board.isLegalMove(player, unit, (desCol=unit->position.col), (desRow=unit->position.row-i))) //Move up
			{
				break;
			}
			else if(board.isLegalMove(player, unit, (desCol=unit->position.col+i), (desRow=unit->position.row))) //Move right
			{
				break;
			}
			else if(board.isLegalMove(player, unit, (desCol=unit->position.col-i), (desRow=unit->position.row))) //Move left
			{
				break;
			}
		}
		if(i>0)
		{//Movement possible in atleast one direction
			while(true)
            {
                int temp = rand()%4;
                switch(temp)//Scramble direction
                {
                    case 0:
                        desCol = unit->position.col;
                        desRow = unit->position.row + i;
                        break;
                    case 1:
                        desCol = unit->position.col;
                        desRow = unit->position.row - i;
                        break;
                    case 2:
                        desCol = unit->position.col + i;
                        desRow = unit->position.row;
                        break;
                    case 3:
                        desCol = unit->position.col - i;
                        desRow = unit->position.row;
                        break;
                }
                if(board.isLegalMove(player, unit, desCol, desRow))//checks if movement is possible
                    break;
            }
		}
		cout<<"Computer moved "<<unit->getName()<<" from "<<unit->position.col<<unit->position.row;
		board.makeMove(player, unit, desCol, desRow);
		cout<<" to "<<desCol<<desRow<<endl;
		timed_delay(PAUSE_BEFORE_NEXT);
		return;
	}
}

void AIgreedy(int player)
{
	Unit* unit = nullptr;
	int idx;
	Board savestate;
	savestate.clone(board);
	int saveindex = -1;
	char col = -1;
	int row = -1;
	int maxScore = INT_MIN;
	for(idx=0; idx<4; idx++)
	{
		unit = board.getUnit(idx, player);
		if((unit->isDead))
			continue;
		int score = 0;
		int i;
		for(i=1; i<=unit->getMovement(); i++)
		{
			if(board.isLegalMove(player, unit, unit->position.col, unit->position.row+i)) //Move down
			{
				score = board.makeMove(player, unit, unit->position.col, unit->position.row+i);
				board.clone(savestate);
				if(score>maxScore)
				{
					maxScore = score;
					col = unit->position.col;
					row = unit->position.row + i;
					saveindex = idx;
				}
			}
			if(board.isLegalMove(player, unit, unit->position.col, unit->position.row-i)) //Move up
			{
				score = board.makeMove(player, unit, unit->position.col, unit->position.row-i);
				board.clone(savestate);
				if(score>maxScore)
				{
					maxScore = score;
					col = unit->position.col;
					row = unit->position.row - i;
					saveindex = idx;
				}
			}
			if(board.isLegalMove(player, unit, unit->position.col+i, unit->position.row)) //Move right
			{
				score = board.makeMove(player, unit, unit->position.col+i, unit->position.row);
				board.clone(savestate);
				if(score>maxScore)
				{
					maxScore = score;
					col = unit->position.col + i;
					row = unit->position.row;
					saveindex = idx;
				}
			}
			if(board.isLegalMove(player, unit, unit->position.col-i, unit->position.row)) //Move left
			{
				score = board.makeMove(player, unit, unit->position.col-i, unit->position.row);
				board.clone(savestate);
				if(score>maxScore)
				{
					maxScore = score;
					col = unit->position.col - i;
					row = unit->position.row;
					saveindex = idx;
				}
			}
		}
	}
	unit = board.getUnit(saveindex, player);
	cout<<"Computer moved "<<unit->getName()<<" from "<<unit->position.col<<unit->position.row;
	board.makeMove(player, unit, col, row);
	cout<<" to "<<col<<row<<endl;
    timed_delay(PAUSE_BEFORE_NEXT);
	return;
}

double minimax(int step, int player)
{
	/**
	In the Minimax algorithm the two players are called maximizer and minimizer.
	The maximizer tries to get the highest score possible while the minimizer tries to do the opposite and get the lowest score possible.
	Every board state has a value associated with it.
	In a given state if the maximizer has upper hand then, the score of the board will tend to be some positive value.
	If the minimizer has the upper hand in that board state then it will tend to be some negative value.
	The values of the board are calculated by some heuristics which are unique for every type of game.
	**/
	//complete
	if(board.getUnit(0, player)->isDead && board.getUnit(1, player)->isDead && board.getUnit(2, player)->isDead && board.getUnit(3, player)->isDead)
	{
		return 0;
	}
	Unit* unit = nullptr;
	int opponent = (player==PLAYER_1)?PLAYER_2:PLAYER_1;
	int idx;
	Board savestate;
	savestate.clone(board);
	int saveindex = -1;
	char col = -1;
	int row = -1;
	if(step%2==1)/**Maximizer*/
	{
		double maxState = INT_MIN;
		for(idx=0; idx<4; idx++)
		{
			unit = board.getUnit(idx, player);
			if((unit->isDead))
				continue;
			double state = 0;
			int i;
			for(i=1; i<=unit->getMovement(); i++)
			{
				if(board.isLegalMove(player, unit, unit->position.col, unit->position.row+i)) //Move down
				{

					board.makeMove(player, unit, unit->position.col, unit->position.row+i);
					state = board.getState();
					if(step<STEPS && board.hasRemainingUnits(opponent))
						state = minimax(step+1, opponent);
					board.clone(savestate);
					if(state>maxState)
					{
						maxState = state;
						col = unit->position.col;
						row = unit->position.row + i;
						saveindex = idx;
					}
				}
				if(board.isLegalMove(player, unit, unit->position.col, unit->position.row-i)) //Move up
				{

					board.makeMove(player, unit, unit->position.col, unit->position.row-i);
					state = board.getState();
					if(step<STEPS && board.hasRemainingUnits(opponent))
						state = minimax(step+1, opponent);
					board.clone(savestate);
					if(state>maxState)
					{
						maxState = state;
						col = unit->position.col;
						row = unit->position.row - i;
						saveindex = idx;
					}
				}
				if(board.isLegalMove(player, unit, unit->position.col+i, unit->position.row)) //Move right
				{
					board.makeMove(player, unit, unit->position.col+i, unit->position.row);
					state = board.getState();
					if(step<STEPS && board.hasRemainingUnits(opponent))
						state = minimax(step+1, opponent);
					board.clone(savestate);
					if(state>maxState)
					{
						maxState = state;
						col = unit->position.col + i;
						row = unit->position.row;
						saveindex = idx;
					}
				}
				if(board.isLegalMove(player, unit, unit->position.col-i, unit->position.row)) //Move left
				{
					board.makeMove(player, unit, unit->position.col-i, unit->position.row);
					state = board.getState();
					if(step<STEPS && board.hasRemainingUnits(opponent))
						state = minimax(step+1, opponent);
					board.clone(savestate);
					if(state>maxState)
					{
						maxState = state;
						col = unit->position.col - i;
						row = unit->position.row;
						saveindex = idx;
					}
				}
			}
		}
		unit = board.getUnit(saveindex, player);
		moveSequence[step-1].player = player;
		moveSequence[step-1].unit = unit;
		moveSequence[step-1].col = col;
		moveSequence[step-1].row = row;
		return maxState;
	}
	else/**Minimizer*/
	{
		double minState = INT_MAX;
		for(idx=0; idx<4; idx++)
		{
			unit = board.getUnit(idx, player);
			if((unit->isDead))
				continue;
			double state = 0;
			int i;
			for(i=1; i<=unit->getMovement(); i++)
			{
				if(board.isLegalMove(player, unit, unit->position.col, unit->position.row+i)) //Move down
				{
					board.makeMove(player, unit, unit->position.col, unit->position.row+i);
					state = board.getState();
					if(step<STEPS && board.hasRemainingUnits(opponent))
						state = minimax(step+1, opponent);
					board.clone(savestate);
					if(state<minState)
					{
						minState = state;
						col = unit->position.col;
						row = unit->position.row + i;
						saveindex = idx;
					}
				}
				if(board.isLegalMove(player, unit, unit->position.col, unit->position.row-i)) //Move up
				{
					board.makeMove(player, unit, unit->position.col, unit->position.row-i);
					state = board.getState();
					if(step<STEPS && board.hasRemainingUnits(opponent))
						state = minimax(step+1, opponent);
					board.clone(savestate);
					if(state<minState)
					{
						minState = state;
						col = unit->position.col;
						row = unit->position.row - i;
						saveindex = idx;
					}
				}
				if(board.isLegalMove(player, unit, unit->position.col+i, unit->position.row)) //Move right
				{
					board.makeMove(player, unit, unit->position.col+i, unit->position.row);
					state = board.getState();
					if(step<STEPS && board.hasRemainingUnits(opponent))
						state = minimax(step+1, opponent);
					board.clone(savestate);
					if(state<minState)
					{
						minState = state;
						col = unit->position.col + i;
						row = unit->position.row;
						saveindex = idx;
					}
				}
				if(board.isLegalMove(player, unit, unit->position.col-i, unit->position.row)) //Move left
				{
					board.makeMove(player, unit, unit->position.col-i, unit->position.row);
					state = board.getState();
					if(step<STEPS && board.hasRemainingUnits(opponent))
						state = minimax(step+1, opponent);
					board.clone(savestate);
					if(state<minState)
					{
						minState = state;
						col = unit->position.col - i;
						row = unit->position.row;
						saveindex = idx;
					}
				}
			}
		}
		unit = board.getUnit(saveindex, player);
		moveSequence[step-1].player = player;
		moveSequence[step-1].unit = unit;
		moveSequence[step-1].col = col;
		moveSequence[step-1].row = row;
		return minState;
	}
}

void AIminimax(int player)
{
	//complete
	// double endState = minimax(1, player);//variable endState for debugging only
	// cout<<endState<<endl;
	minimax(1, player);
	cout<<"Computer moved "<<moveSequence[0].unit->getName()<<" from "<<moveSequence[0].unit->position.col<<moveSequence[0].unit->position.row;
	board.makeMove(moveSequence[0].player, moveSequence[0].unit, moveSequence[0].col, moveSequence[0].row);
	cout<<" to "<<moveSequence[0].col<<moveSequence[0].row<<endl;
    timed_delay(PAUSE_BEFORE_NEXT);
	return;
}

void saveState(string path, Board* state)
{
	ofstream saveFile;
	saveFile.clear();
	saveFile.open(path, std::ios::trunc | std::ios::binary);
	saveFile.write((char*)state, sizeof(*state));
	saveFile.close();
}

void loadState(string path, Board* state)
{
	ifstream saveFile;
	saveFile.clear();
	saveFile.open(path, std::ios::binary);
	saveFile.read((char*)state, sizeof(*state));
	saveFile.close();
}
