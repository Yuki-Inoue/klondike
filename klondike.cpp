#include "klondike.hh"
#include "shuffle.hh"
#include <algorithm>
#include <boost/foreach.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#define foreach BOOST_FOREACH

typedef std::pair<int,int> IntPair;
using std::max;
using std::min;
using std::list;
using std::ostream;
using std::cout;
using std::cin;
using std::endl;
using std::make_pair;
using std::string;

Field::Field() {
  for(int i = 0; i < 4; ++i)
    finished_[i] = 0;

  Shuffle s(52);
  for(int i = 0; i < 7; ++i)
    opened_[i].push_back(s.get());
  for(int i = 0; i < 7; ++i)
    for(int j = 0; j < i; ++j)
      reversed_[i].push_back(s.get());
  while(!s.empty())
    reversedDeck_.push_back(s.get());
}

// given the top(the largest) and the bottom (the smallest)
// of continuous pile, consider if it is movable right under the dst card.
bool movable_base(int dst, int src_top, int src_bottom) {
  return

    // first 2 rows check if the number is all right.
    !(getNumber(dst) > getNumber(src_top) + 1
      || getNumber(src_bottom) >= getNumber(dst)

      // and this row checks if the color is all right.
      || (getColor(dst)
	  + getColor(src_back)
	  + getNumber(dst)
	  - getNumber(src_back)) % 2 != 0);

}

void Field::move(IntPair act) {
  assert( max(act.first,act.second) < 7 && min(act.first,act.second) >= 0 );

  list<int> &srclist = o[act.first];
  if(srclist.empty())
    return;

  list<int> &dstlist = o[act.second];
  int src_back = srclist.back();
  int src_top = srclist.front();

  if(dstlist.empty()) {
    if (src_top % 13 != 12)
      return;

    dstlist.splice
      (dstlist.end(),
       srclist,
       srclist.begin(),
       srclist.end());
    return;
  }

  int dst = dstlist.back();
  int ofs = dst%13 - src_back % 13;

  if(movable_base(dst, src_top, src_back)) {
    list<int>::iterator it, itend;
    it = itend = srclist.end();
    for(int i = 0; i < ofs; ++i)
      --it;
    dstlist.splice(dstlist.end(), srclist, it, itend);
  }

}

void Field::moveup(int pile) {
  if(opened_[pile].empty())
    return;

  list<int> &openlist = opened_[pile];
  int card = openlist.back();
  if( finished_[getSuit(card)] == getNumber(card) ) {
    ++finished_[getSuit(card)];
    openlist.pop_back();
  }
}

void Field::movedown(int pile) {
  if( openedDeck_.empty() )
    return;

  int moving_card = openedDeck_.front();
  list<int> &dstlist = opened_[pile];

  if(dstlist.empty()) {
    if( getNumber(moving_card) == 12)
      goto MOVE;
  }
  else {
    int dstcard = dstlist.back();
    if( movable_base( dstcard, moving_card, moving_card) )
      goto MOVE;
  }
  return;

 MOVE:
  dstlist.splice( dstlist.end(), openedDeck_, openedDeck_.begin() );
}

void Field::moveDirect() {
  if( openedDeck_.empty() )
    return;

  int card = openedDeck_.front();
  int suit = getSuit(card);
  if( finished[suit] == getNumber(card) ) {
    ++finished[suit];
    openedDeck_.pop_front();
  }
}

void Field::flipDeck(int n) {
  for(int i = 0; reversedDeck_.empty() && i < n; ++i)
    openedDeck_.splice(openedDeck_.begin(), reversedDeck_, --reversedDeck_.end());
}

void Field::flipReversed(int n) {
  if( r[n].empty() )
    return;

  o[n].push_back(r[n].back());
  r[n].pop_back();
}



//  for test
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

ostream &operator<<(ostream &os, const Field &f) {

  os << "7: deck " << (f.deckNull() ? "<null>" : "**") << endl;
  os << "8: openedDeck ";
  if ( f.getDeckit() == f.getDeck().begin() )
    os << "<null>";
  else {
    list<int>::iterator it = f.getDeckit();
    outputcard(os, *(--it));
  }
  os << endl;

  os << "9: finished ";
  for(int i = 0; i < 4; ++i)
    os << suitArray[i] << ":" << f.getFinished()[i] << " ";
  os << endl;


  os << endl;

  for(int i = 0; i < 7; ++i) {
    os << i << ": ";
    for(int j = 0; j < f.numReversed(i); ++j)
      os << "*";
    os << " ";
    foreach(int i, f.getOpened()[i])
      outputcard(os, i) << " ";
    os << endl;
  }

  os << endl << "------------" << endl << endl;

  return os;
}


int main() {

  Field f;
  while(true) {
    cout << f;
    int i1,i2;
    cin >> i1;

    if(i1 == 7) {
      if(f.deckNull())
	f.resetDeck();
      else
	f.flipDeck(3);
      continue;
    }

    if(i1 == 8){
      cin >> i2;
      if(i2 == 9)
	f.movedirect();
      else
	f.movedown(i2);
      continue;
    }

    if(f.openPileNull(i1)) {
      f.flipReversed(i1);
      continue;
    }

    cin >> i2;
    if (i2 == 9) {
      f.moveup(i1);
      continue;
    }

    f.move(make_pair(i1,i2));
  }

  return 0;
}
