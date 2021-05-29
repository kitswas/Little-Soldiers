/******************************************************************************
* @author Swastik Pal
* @version 1.0
* Date 28/03/2021
* About: The main() method and the different screens
*******************************************************************************/

//macros here

//header files here
#include"header.h"
#include<ctime>
#include<cstdlib>

//namespace items here

//global variables
controller P1control, P2control;
string controllerNames[] = {"human", "randomAI", "greedyAI", "minimaxAI"};
bool enableTraits;

//functions here
void timed_delay(double t)/**makes program wait for t seconds*/
{
	//complete
	clock_t delay_st,delay_end;
	delay_st=clock();
	delay_end=clock();
	while((double)(delay_end-delay_st)/CLOCKS_PER_SEC<t)
		delay_end=clock();
}

void movexy(int col, int row)/**moves cursor by specified steps while filling up the screen*/
{
	//complete
	for(int i=1;i<=row;i++)
		cout<<'\n';
	for(int i=1;i<=col;i++)
		cout<<' ';
}

void welcomeScreen()/**title screen*/
{
	//complete
	cls;
	char text[]="Little Soldiers";
	movexy(TOTAL_COL/2 - 8, TOTAL_ROW/2);
	for(int i=0;text[i]!='\0';i++)
	{
		cout<<text[i];
		timed_delay(0.1);
	}
	timed_delay(0.15);
}

void help()/**shows help screen*/
{
	//complete
	cls;
	ifstream helptext;
	helptext.clear();
	helptext.open("helptext.txt");
	if(helptext.fail())//checks whether read operation failed
		cout<<"Unable to open help file\nError";
	else
		cout<<helptext.rdbuf();
	helptext.close();
	cout<<"\n\n\nPress X to continue...";
	char ch;
	do
	{
		cin>>ch;
	}while(ch!='X');
}

void credits()/**shows credits*/
{
	//complete
	cls;
	movexy(TOTAL_COL/2 - 10, TOTAL_ROW/2);
	cout<<"Thanks for playing.";
	movexy(TOTAL_COL/2 - 13, 1);
	cout<<"Hope you enjoyed the game.";
	timed_delay(1.5);
	cls;
	movexy(TOTAL_COL/2 - 4, TOTAL_ROW/2);
	cout<<"Made with C++";
	timed_delay(1.15);
	cls;
	movexy(TOTAL_COL/2 - 12, TOTAL_ROW/2);
	cout<<"This game is a freeware.";
	movexy(TOTAL_COL/2 - 12, 1);
	cout<<"Feel free to distribute.";
	timed_delay(1.2);
	cls;
	movexy(TOTAL_COL/2 - 5, TOTAL_ROW/2);
	cout<<"With love:";
	movexy(TOTAL_COL/2 - 8, 1);
	cout<<"From Swastik Pal";
	timed_delay(1.2);
}

void loadSettings()
{
    fstream inifile;
    inifile.clear();
    inifile.open("settings.ini", std::ios::in);
    if(inifile.fail())//checks whether read operation failed
    {
	    cout<<"Unable to open settings file\nError";
	    //load defaults
	    P1control = controller::human;
	    P2control = controller::minimaxAI;
    }
    else
    {
        char ch;
        int propertyNo = 0;
        while(!inifile.eof())
        {
            ch = inifile.get();
            if((ch == '=') && (inifile.peek()>='0' && inifile.peek()<='9'))
            {
                ch = inifile.get();
                ++propertyNo;
                switch(propertyNo)
                {
                    case 1:
                        P1control = static_cast<controller>(ch - '0');
                        break;
                    case 2:
                        P2control = static_cast<controller>(ch - '0');
                        break;
                    case 3:
                        enableTraits = static_cast<bool>(ch - '0');
                }
            }
        }
        if(propertyNo==0)
        {
            //load defaults
            P1control = controller::human;
            P2control = controller::minimaxAI;
            enableTraits = false;
        }
    }
    inifile.close();
}

void settings()
{
    cls;
    fstream inifile;
    inifile.clear();
    inifile.open("settings.ini", std::ios::out);
    if(inifile.fail())//checks whether read operation failed
		cout<<"Unable to open settings file\nError";
    else
    {
        char ch;
        while(true)
        {
            cls;
            movexy(TOTAL_COL/2 - 3, TOTAL_ROW/2 - 6);
            cout<<"Menu:";
            movexy(TOTAL_COL/2 - 15, 2);
            cout<<"1> Player 1 Controller: "<<controllerNames[P1control];
            movexy(TOTAL_COL/2 - 15, 1);
            cout<<"2> Player 2 Controller: "<<controllerNames[P2control];
            movexy(TOTAL_COL/2 - 15, 1);
            cout<<"3> Enable Traits: "<<(enableTraits?"true":"false");
            movexy(TOTAL_COL/2 - 15, 1);
            cout<<"4> Back";
            movexy(TOTAL_COL/2 - 6, 2);
            cout<<"Your choice?";
            cin>>ch;
            if(ch=='4')
                break;
            switch(ch)
            {
                case '1':
                    while(true)
                    {
                        char choice;
                        cls;
                        movexy(TOTAL_COL/2 - 10, TOTAL_ROW/2 - 6);
                        cout<<"Player 1 Controller: ";
                        for(int i=0; i<4; i++)
                        {
                            movexy(TOTAL_COL/2 - 5, 1);
                            cout<<(i+1)<<"> "<<controllerNames[i];
                        }
                        movexy(TOTAL_COL/2 - 10, 1);
                        cout<<"Enter choice:";
                        cin>>choice;
                        if(choice>='1' || choice<='4')
                        {
                            P1control = static_cast<controller>(choice - '1');
                            break;
                        }
                    }
                    break;
                case '2':
                    while(true)
                    {
                        char choice;
                        cls;
                        movexy(TOTAL_COL/2 - 10, TOTAL_ROW/2 - 6);
                        cout<<"Player 2 Controller: ";
                        for(int i=0; i<4; i++)
                        {
                            movexy(TOTAL_COL/2 - 5, 1);
                            cout<<(i+1)<<"> "<<controllerNames[i];
                        }
                        movexy(TOTAL_COL/2 - 10, 1);
                        cout<<"Enter choice:";
                        cin>>choice;
                        if(choice>='1' || choice<='4')
                        {
                            P2control = static_cast<controller>(choice - '1');
                            break;
                        }
                    }
                    break;
                case '3':
                    while(true)
                    {
                        char choice;
                        cls;
                        movexy(TOTAL_COL/2 - 7, TOTAL_ROW/2 - 6);
                        cout<<"Enable Traits: ";
                        movexy(TOTAL_COL/2 - 7, 1);
                        cout<<"Enter choice(Y/N):";
                        cin>>choice;
                        if(choice=='n' || choice=='N')
                        {
                            enableTraits = false;
                            break;
                        }
                        else if(choice=='y' || choice=='Y')
                        {
                            enableTraits = true;
                            break;
                        }
                    }
                    break;
            }
        }
    }
    inifile<<"[Settings]\n";
    inifile<<"P1control="<<(int)P1control<<"\n";
    inifile<<"P2control="<<(int)P2control<<"\n";
    inifile<<"enableTraits="<<(int)enableTraits<<"\n";
    inifile.close();
}

void showCharacterCards()
{
    //complete
	cls;
	ifstream cards;
	cards.clear();
	cards.open("character_cards");
	if(cards.fail())//checks whether read operation failed
		cout<<"Unable to open cards file\nError";
	else
		cout<<cards.rdbuf();
	cards.close();
	cout<<"\n\n\n\nPress X to continue...";
	char ch;
	do
	{
		cin>>ch;
	}while(ch!='X');
}

void menu()/**shows main menu*/
{
	char ch;
	while(true)
	{
		cls;
		movexy(TOTAL_COL/2 - 3, TOTAL_ROW/2 - 6);
		cout<<"Menu:";
		movexy(TOTAL_COL/2 - 6, 1);
		cout<<"1> Play";
		movexy(TOTAL_COL/2 - 6, 1);
		cout<<"2> Help";
		movexy(TOTAL_COL/2 - 6, 1);
		cout<<"3> Show Character Cards";
		movexy(TOTAL_COL/2 - 6, 1);
		cout<<"4> Settings";
		movexy(TOTAL_COL/2 - 6, 1);
		cout<<"5> Exit";
		movexy(TOTAL_COL/2 - 6, 2);
		cout<<"Your choice?";
		cin>>ch;
		switch(ch)
		{
			case '1':
				play();
				break;
			case '2':
				help();
				break;
			case '3':
				showCharacterCards();
				break;
				break;
			case '4':
				settings();
				break;
			case '5':
				credits();
				exit(0);
		}
	}
}

int sgn(int x)/**signum function*/
{
    //complete
    return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

int main()
{
	/**Windows only*/
	system("@ECHO OFF ");
	// system("mode 200, 60 ");//Large screen
	system("mode con: cols=120 lines=50 ");//Small screen
	system("color 3F ");//Console Colour
	system("title Little Soldiers ");//Console Title
	/**Windows end*/
	/**Linux only*/
	//system("PS1=$");//preparations for the next command on Ubuntu 16.04 and above
	//system("PROMPT_COMMAND=\'echo -ne \"\033]0;Little Soldiers\007\"\'");//Console Title
	/**Linux end*/
	cls;
	welcomeScreen();
    srand((unsigned)time(nullptr));
    loadSettings();
	menu();
	return 0;
}
