#include "Hand.h"
#include  <vector>
#include <iostream>
//#include "Deck.h"

using namespace std;
Hand::Hand():HandCards(),count(0),deck() {}


Hand:: Hand(Deck deck2) :HandCards(), count(7),deck(deck2){//empty constructor
	for(unsigned int i=0;i<7;i++)
		Hand::addCard(*deck.fetchCard());

}

Hand::Hand(vector<vector<Card*> > other): HandCards(other) , count (other.size()) {};
//copy constructor

//Hand::Hand operator =(const Hand &other):count(other.count) ,deck(other.deck){};

Hand::~Hand(){}

int Hand:: whereToInsert(Card &card){   // made for the addCard method.
	unsigned int mid, left = 0 ;
	unsigned int right = HandCards.size();
	while (left < right) {
		mid = left + (right - left)/2;
		if (card.getValue() > HandCards[mid][0]->getValue()){
			left = mid+1;
		}
		else if (card.getValue() < HandCards[mid][0]->getValue()){
			right = mid;
		}
		else {
			return mid;
		}
	}
	if(mid!=0)
		return mid-1; //for making sure its inserted before the closer bigger value. need to check it.
	else
		return 0; //if the vec is empty , it will insert at first position.
}

bool Hand:: addCard(Card &card){
	int pos=whereToInsert(card);
	HandCards[pos].push_back(&card);
	if(HandCards[pos].size()==4)   //if player got revia.
			Hand::removeCard(*HandCards[pos][0]);

	else if(HandCards[pos].size()>1)
		HandCards[pos]=sortByShape(HandCards[pos]);

	count++;
	return true;
}

vector<Card*> Hand:: sortByShape(vector<Card*> sortMe) {
	//insertion sort.
	Card* tmp;
	for (unsigned int i = 1; i < sortMe.size(); i++)  {
		for (unsigned int j = i; j >= 1; j--) {
			if (sortMe[j]->getShape() < sortMe[j-1]->getShape()) {
				tmp = sortMe[j];
				sortMe[j] = sortMe[j-1];
				sortMe[j-1] = tmp;
			}
		}
	}
	return sortMe;
}

bool  Hand:: removeCard(Card &card) {
	vector<Card*> insideVec;
	int pos = searchCard(card);
	if (pos < 0)
		return false; //no such card at the hand.
	else {
		insideVec=HandCards[pos];
		int K=insideVec.size();
		insideVec.erase(insideVec.begin(),insideVec.end());
		count-=K;
		int j=0;
		while(j<K){
			Hand::addCard(*deck.fetchCard());
			count++;
			j++;
		}
		return true;
	}
}


int Hand:: searchCard(Card &card){  //made for remove card method. (had to make 2 diffrenet ones).
	unsigned int mid, left = 0 ;
	unsigned int right = HandCards.size();
	while (left < right) {
		mid = left + (right - left)/2;
		if (card.getValue() > HandCards[mid][0]->getValue()){
			left = mid+1;
		}
		else if (card.getValue() < HandCards[mid][0]->getValue()){
			right = mid;
		}
		else {
			return mid;
		}
	}
	return -1; // if the exact value is not found at the vec.
}

int Hand:: getNumberOfCards(){
	return count;
}

string Hand:: toString(){
	string ans="";
	vector<Card*> insideVec;
	for(unsigned int i=0; i<HandCards.size();i++){
		insideVec=HandCards[i];
		for(unsigned int j=0; j<insideVec.size();j++)
			ans+=insideVec[j]->toString() + " ";
	}
	return ans;
}

//opertor = , check gameplay
