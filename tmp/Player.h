#ifndef PLAYER_H_
#define PLAYER_H_

#include <iostream>
#include "Hand.h"

using namespace std;

class Player : public Hand {
private:
	const string name;
	int position;
	int strategy;
	//Game& game;

public:
	int posIterator;
	Player();
	Player(string name, int position , Deck d,int strtg);
	Player& operator =(Player &other);
	Player(const Player &other);
	virtual ~Player();


	string getName();   //Returns the name of the player
	int getPosition();
	int getStrategy();
	virtual vector<Card*> selectCard(vector<vector<Card*> > myCards);
	int PlayerWithMostCards(vector<Player*> player);
	int cyclicOrder(vector<Player*> players); //need to check if Player is required
};

class PlayerType1 : public Player {  //For strategy 1
public:
	PlayerType1(string name, int position , Deck d, int strtg);
	virtual vector<Card*> selectCard(vector<vector<Card*> > myCards);
};

class PlayerType2 : public Player {  //For strategy 2
public:
	PlayerType2(string name, int position , Deck d,int strtg);
	virtual vector<Card*> selectCard(vector<vector<Card*> > myCards);
};

class PlayerType3 : public Player {  //For strategy 3
public:
	PlayerType3(string name, int position , Deck d,int strtg);
	virtual vector<Card*> selectCard(vector<vector<Card*> > myCards);
};

class PlayerType4 : public Player {  //For strategy 4
public:
	PlayerType4(string name, int position , Deck d,int strtg);
	virtual vector<Card*> selectCard(vector<vector<Card*> > myCards);
};

#endif
