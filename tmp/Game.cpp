#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector> // needed???
#include "Game.h"
#include "Player.h"
using namespace std;

//Game::Game():players(),deck(), highNumVal(0){} // needed?

Game::Game(char* config): count(0),twoWinners(false),winner1(0),winner2(0),initialDeck(){
	vector<Card*> GameCards;
	ifstream filetext(config);
	if (!filetext){
		cout << "error reading conf file";
		return;
	}

	bool verbal;
	int N;
	bool verbalpass=false;
	bool highest=false;
	bool deckCheck=false;
	bool playersPass= false;

	string word;
	while (getline(filetext,word)){

		if ((!word.empty())&&(word.at(0)!='#')){

			if (!verbalpass){
				if (word.at(0)=='1')
					verbal=true;
				else
					verbal=false;
				verbalpass=true;
				cout << "verbal:" << verbal << '\n';
			}
			else if (!highest){
				cout << "N:" << word <<'\n';
				N=(int)word.at(0)-'0';
				highest=true;
			}
			else if (deckCheck==false){
				char shape;
				char fig;
				int value;
				unsigned int index=0;
				string s;
				while (word.length()>index){

					if (word.at(index)!=(char)32){ //not a space

						if (word.at(index)<='9'){ // numeric card
							s="";
							while (word.at(index)!=(char)32){ //this char isnt space
								if (word.at(index+1)==(char)32){ //next char is space
									shape=word.at(index);
									value=std::atoi(s.c_str());
									NumericCard c=NumericCard(shape,value);
									GameCards.push_back(&c);
									index++;
								}
								else{ //next one isnt space, keep going on nums
									s=s+word.at(index);
									index++;
								}
							}
						}

						else{ //figure card
							fig=word.at(index);
							switch (fig){
							case 'J':
								value=N+1;
								break;
							case 'Q':
								value=N+2;
								break;
							case 'K':
								value=N+3;
								break;
							case 'A':
								value=N+4;
								break;
							}
							index++;
							shape=word.at(index);
							FigureCard f=FigureCard(shape,fig,value);
							GameCards.push_back(&f);
						}

						cout << "next card:" << value << "," << fig << " of " << shape <<'\n';

					}
					index++;

				}
				deckCheck=true;
				deck.SetDeck(GameCards);
				//initialize the deck from the gameCards.
			}
		}
		else if (!playersPass){
			int playerNum=1;
			while (!word.empty()){
				string name="";
				int index=0;
				int strtg;
				while (word.at(index)!=(char)32){
					name+=word.at(index);
					index++;
				}
				strtg=word.at(index+1)-'0';
				Player* temp;
				switch (strtg){
				case 1:
					temp = new PlayerType1(name,playerNum,deck,strtg);
					break;
				case 2:
					temp = new PlayerType2(name,playerNum,deck,strtg);
					break;
				case 3:
					temp = new PlayerType3(name,playerNum,deck,strtg);
					break;
				case 4:
					temp = new PlayerType4(name,playerNum,deck,strtg);
					break;
				}
				//initialize player & his hand and strategy.
				players.push_back(temp);
				cout << "player name: " << name << " playing " << strtg <<'\n';
				getline(filetext,word);
				playerNum++;
			}
		}
	}
}

Game ::~Game(){
	//delete temp;
}

vector<Player *> Game::getPlayers(){ return players;};
void Game::init(){
initialDeck=deck.GetDeck();
}

void Game::play(){
	int numOfPlayer=1,askedCard,askedPlayer;
	bool isAskedWon=false;
	Player* myPlayer;


	while(winner1==0){
		count++;
		cout << "Turn" << count <<'\n';
		deck.toString();
		for(unsigned int i=0;i<players.size();++i){
			cout << players[i]->getName() << ": " << players[i]->toString()<<'\n';
		}
		numOfPlayer=count%players.size();
		myPlayer=players[numOfPlayer];
		askedCard=myPlayer->selectCard(myPlayer->getHand());

		if(myPlayer->getStrategy()<=2)
			askedPlayer=myPlayer->PlayerWithMostCards(getPlayers());
		else
			askedPlayer= myPlayer->cyclicOrder(getPlayers());

		cout << myPlayer->getName() << "Asked " << players[askedPlayer]->getName()<< "for the value "<< askedCard<<'\n';

		if(players[askedPlayer]->removeCard(askedCard))
		{

			//actual remove of cards and adding it from one player to other


			if(players[askedPlayer]->getNumberOfCards()==0){//checking if the game has ended.
				winner1=askedPlayer;
				isAskedWon=true;
			}
			if (myPlayer->getNumberOfCards()==0){
				winner1=numOfPlayer;
				if(isAskedWon){
					twoWinners=true;
					winner2=askedPlayer;
				}
			}
		}
		else if(myPlayer->getNumberOfCards()!=0)//player can fetchcard only if is not empty
			myPlayer->deck.fetchCard(); //the asked card is not at the asked player hand. so my player takes a card from deck.
	}//end of while.game over.

	printWinner();
}
void Game::printWinner(){
	if(!twoWinners){
		cout<<"***** The Winner is: "<< players[winner1]->getName()<<" *****"<<endl;
		printNumberOfTurns();
		printInitialState();
		cout<<"Final state: "<<endl;
		printState();
	}
	else{
		cout<<"***** The Winners are: "<< players[winner1]->getName()<<"and: "<<players[winner2]->getName()<<" *****"<<endl;
			printNumberOfTurns();
			printInitialState();
			cout<<"Final state: "<<endl;
			printState();
	}
}

void Game::printInitialState(){
	string ans="";
	vector<vector<Card*> > printMe;
	cout<<"Initial state: "<<endl;
			cout<<"----------"<<endl;
			cout<<"Deck: "<<initialDeck.toString()<<endl;
			for(unsigned int i=0;i<players.size();++i){
				printMe=players[i]->initialHand;
				for(unsigned j=0;j<printMe.size();j++){
					for(unsigned k=0;k<printMe[j].size();k++)
						ans+=printMe[j][k]->toString();
				}
				cout << players[i]->getName() << ": " << ans <<endl;;
			}
					cout<<"----------"<<endl;
}
void Game::printState(){ //Print the state of the game as described in the assignment.

	cout <<"Deck: "<< deck.toString()<<endl;
	for(unsigned int i=0;i<players.size();++i)
		cout << players[i]->getName() << ": " << players[i]->toString()<<endl;;

}
void Game::printNumberOfTurns(){//Print the number of played turns at any given time.
	cout << "Number of turns: " << count<< endl;
}


//int main(){



//   }

//empty constructor is needed?, different functions
