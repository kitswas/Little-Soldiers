/******************************************************************************
* @author Swastik Pal
* @version 1.0
* Date 28/03/2021
* About: Board class definition
*******************************************************************************/

//macros here
#define WEIGHT_OF_DEATH 1.5

//header files here
#include"header.h"
#include<cmath>

//namespace items here
using std::abs;
using std::ceil;

//functions here
void showUnit(Unit* u)
{
    //complete
	cout<<'\t'<<u->getAttack();
	cout<<'\t'<<u->getDefence();
	cout<<'\t'<<u->getMovement();
	cout<<'\t'<<u->getHealth();
	//cout<<'\t'<<u->getContribution();
}

//class members here
const char Board::HLabels[10]={'X','A','B','C','D','E','F','G','H','I'};
const int Board::VLabels[9]={1,2,3,4,5,6,7,8,9};

Board::Board()/**constructor*/
{
    //complete
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			playArea[i][j]='.';
		}
	}
	state = 0;
}

void Board::clearPos(char &col, int &row)
{
    //complete
	if(!((col<'A' || col>'I') || (row<1 || row>9)))
		playArea[row-1][col-'A']='.';
}

void Board::clone(Board source)
{
    //complete
    //playArea[9][9];
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            playArea[i][j] = source.playArea[i][j];
        }
    }
    //player[2];
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            player[i].unitSet[j].isDead = source.player[i].unitSet[j].isDead;
            player[i].unitSet[j].position.col = source.player[i].unitSet[j].position.col;
            player[i].unitSet[j].position.row = source.player[i].unitSet[j].position.row;
            player[i].unitSet[j].setContribution(source.player[i].unitSet[j].getContribution());
            player[i].unitSet[j].setHealth(source.player[i].unitSet[j].getHealth());
        }
    }
    //state;
    state = source.state;
}

void Board::display()/**shows the entire board*/
{
	//complete
	for(int i=0;i<10;i++)
	{
		cout<<HLabels[i]<<'\t';
	}
	for(int i=0;i<9;i++)
	{
		cout<<"\n\n\n";
		cout<<VLabels[i];
		for(int j=0;j<9;j++)
		{
			cout<<'\t'<<playArea[i][j];
		}
	}

	cout<<endl;
}

double Board::getState()/**positive if PLAYER_2 is in the lead, negative for PLAYER_1*/
{
    //complete
    state = 0;
    if(!hasRemainingUnits(PLAYER_1))
    {
        state = 100;//very high value
    }
    else if(!hasRemainingUnits(PLAYER_2))
    {
        state = -100;//very low value
    }
    else
    {
        for(int i=0; i<4; i++)
        {
            state += player[PLAYER_2].unitSet[i].getHealth() - WEIGHT_OF_DEATH*player[PLAYER_2].unitSet[i].isDead;
        }
        for(int i=0; i<4; i++)
        {
            state -= player[PLAYER_1].unitSet[i].getHealth() - WEIGHT_OF_DEATH*player[PLAYER_1].unitSet[i].isDead;
        }
    }
	return state;
}

Unit* Board::getUnit(int unitIndex, int player)
{
	//complete
    return Board::player[player].unitSet[unitIndex].getCurrentInstance();
}

bool Board::hasRemainingUnits(int pNo)
{
    if(player[pNo].unitSet[0].isDead && player[pNo].unitSet[1].isDead && player[pNo].unitSet[2].isDead && player[pNo].unitSet[3].isDead)
    {
        return false;
    }
    return true;
}

int Board::isOccupied(char col, int row)/**returns positive codes for PLAYER_1 and negative for PLAYER_2*/
{
	//complete
	for(int i=0; i<4; i++)
	{
		if(player[PLAYER_1].unitSet[i].position.col==col && player[PLAYER_1].unitSet[i].position.row==row)
			return (i+1);
	}
	for(int i=0; i<4; i++)
	{
		if(player[PLAYER_2].unitSet[i].position.col==col && player[PLAYER_2].unitSet[i].position.row==row)
			return -(i+1);
	}
	return 0;
}

bool Board::isLegalMove(int player, Unit *unit, char desCol, int desRow)/**checks legality of move*/
{
    //complete
	int occupancy = isOccupied(desCol, desRow);
	int moveH = desCol - unit->position.col;
	int moveV = desRow - unit->position.row;
	if((occupancy>0 && player==PLAYER_1) || (occupancy<0 && player==PLAYER_2))
		return false;
	if(desCol<'A' || desCol>'I' || desRow<1 || desRow>9)
		return false;
	if((moveH != 0 && moveV != 0) || (moveH == 0 && moveV == 0))
		return false;
	else if(abs(moveH)>unit->getMovement() || abs(moveV)>unit->getMovement())
		return false;
	else//checks if road is clear
	{
		int i;
		for(i=sgn(moveH); abs(i)<abs(moveH); i+=sgn(moveH))
		{
			if(isOccupied(unit->position.col+i, unit->position.row))
				break;
		}
		if(abs(i)<abs(moveH))
			return false;
		for(i=sgn(moveV); abs(i)<abs(moveV); i+=sgn(moveV))
		{
			if(isOccupied(unit->position.col, unit->position.row+i))
				break;
		}
		if(abs(i)<abs(moveV))
			return false;
	}
	return true;
}

int Board::makeMove(int player, Unit *unit, char desCol, int desRow)
{
	int score=0;
	int opponent = (player==PLAYER_1)?PLAYER_2:PLAYER_1;
	int occupancy = isOccupied(desCol, desRow);
	if(occupancy == 0)
	{
		setPos(player, desCol, desRow, unit->getIndex());
	}
	else
	{
        int moveH = desCol - unit->position.col;
        int moveV = desRow - unit->position.row;
        Unit* occupant = getUnit(abs(occupancy)-1, opponent);
        int damage = unit->getAttack() - occupant->getDefence(); /**damage calculation formula here*/
        if(damage<0)
			damage = 0;

        if(enableTraits)/**traits implemented here*/
        {
            int occCur = isOccupied(unit->position.col, unit->position.row);
            switch(unit->getIndex())
            {
                case 0://Knight
                    damage += abs(moveH + moveV);//charge
                    break;
                case 2://Rogue
                    if((isOccupied(desCol+1, desRow)==0 || isOccupied(desCol+1, desRow)==occCur) && (isOccupied(desCol-1, desRow)==0 || isOccupied(desCol-1, desRow)==occCur) && (isOccupied(desCol, desRow+1)==0 || isOccupied(desCol, desRow+1)==occCur) && (isOccupied(desCol, desRow-1)==0 || isOccupied(desCol, desRow-1)==occCur))
                        damage *= 2;
                    break;
                case 3://Swordsman
                    damage = unit->getAttack();//penetrate
                    break;
            }
        }
        occupant->setHealth(occupant->getHealth() - damage);
        occupant->setContribution(occupant->getContribution() - damage);
        unit->setContribution(unit->getContribution() + damage);
        score+=damage;

        if(enableTraits)/**traits implemented here*/
        {
            switch(occupant->getIndex())
            {
                case 1://Pikeman
                    if(!occupant->isDead)
                        unit->setHealth(unit->getHealth() - 1);//counter
                    break;
            }
        }

        if(occupant->getHealth()<=0)
        {
            occupant->setHealth(0);
            occupant->isDead = true;
            setPos(opponent, -1, -1, occupant->getIndex());
            unit->setContribution(unit->getContribution() + damage);
            score+=damage;
        }

        setPos(player, desCol - sgn(moveH), desRow - sgn(moveV), unit->getIndex());
	}
    return score;
}

void Board::reset()
{
    //complete
    //playArea[9][9];
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            playArea[i][j] = '.';
        }
    }
    //player[2];
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            player[i].unitSet[j].isDead = false;
            player[i].unitSet[j].position.col = 0;
            player[i].unitSet[j].position.row = 0;
            player[i].unitSet[j].setContribution(0);
            player[i].unitSet[j].setHealth(5);
        }
    }
    //state;
    state = 0;
}

void Board::setPos(int pNo, char col, int row, int index)
{
	clearPos(player[pNo].unitSet[index].position.col, player[pNo].unitSet[index].position.row);
	player[pNo].unitSet[index].position.col=col;
	player[pNo].unitSet[index].position.row=row;
	if(col>='A' && col<='I' && row>=1 && row<=9)
        playArea[row-1][col-'A']=unitLabels[pNo][index];
}

void Board::showDetails()/**shows details of units*/
{
	//complete
	cout<<"\n\t\tAtk\tDef\tMov\tHP";
	for(int i=0; i<2; i++)
	{
		cout<<"\nPlayer "<<i+1<<":";
		for(int j=0; j<4; j++)
        {
            cout<<'\n'<<(j+1)<<"> "<<unitNames[j];
            if(player[i].unitSet[j].isDead)
                cout<<"\t\t\t\t\tDEAD";
            else
                showUnit(player[i].unitSet[j].getCurrentInstance());
        }
        cout<<'\n';
	}
	cout<<endl;
}
