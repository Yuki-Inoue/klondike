#include "klondike.hh"

#include <iostream>
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH

using namespace std;

string suitArray[] = {"S","C","H","D"};


ostream &outputcard(ostream &os, int card) {

  os << suitArray[card/13];

  switch(card % 13) {
  case 9:
    os << "D";
    break;
  case 10:
    os << "J";
    break;
  case 11:
    os << "Q";
    break;
  case 12:
    os << "K";
    break;
  default:
    os << card % 13 + 1;
    break;
  }
  return os;
}

ostream &operator<<(ostream &os, const Klondike &f) {

  os << "7: deck " << (f.revDeckEmpty() ? "<null>" : "**") << endl;
  os << "8: openedDeck ";
  if ( f.openedDeckEmpty() )
    os << "<null>";
  else
    outputcard(os, *(f.getOpenedDeck().first));

  os << endl;

  os << "9: finished ";
  for(int i = 0; i < 4; ++i)
    os << suitArray[i] << ":" << f.getFinished(i) << " ";
  os << endl;


  os << endl;

  for(int i = 0; i < 7; ++i) {
    os << i << ": ";
    for(int j = 0; j < f.reversedSize(i); ++j)
      os << "*";
    os << " ";
    foreach(int i, f.getOpened(i))
      outputcard(os, i) << " ";
    os << endl;
  }

  os << endl << "------------" << endl << endl;

  return os;
}


int main() {

  Klondike f;
  while(true) {
    cout << f;
    int i1,i2;
    cin >> i1;

    if(i1 == 7) {
      if(f.revDeckEmpty())
	f.resetDeck();
      else
	f.flipDeck(3);
      continue;
    }

    if(i1 == 8){
      cin >> i2;
      if(i2 == 9)
	f.moveDirect();
      else
	f.moveDown(i2);
      continue;
    }

    if(f.openedEmpty(i1)) {
      f.flipReversed(i1);
      continue;
    }

    cin >> i2;
    if (i2 == 9) {
      f.moveUp(i1);
      continue;
    }

    f.move(i1,i2);
  }

  return 0;
}
