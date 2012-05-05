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

Field::Field() : o(7), r(7) {
  Shuffle s(52);
  for(int i = 0; i < 4; ++i)
    finished[i] = 0;
  for(int i = 0; i < 7; ++i)
    o[i].push_back(s.get());
  for(int i = 0; i < 7; ++i)
    for(int j = 0; j < i; ++j)
      r[i].push_back(s.get());
  while(!s.empty())
    deck.push_back(s.get());
  deckit = deck.begin();
}

bool movable_base(int dst, int src_top, int src_back) {
  return
    !(dst%13 > src_top%13 + 1
      || src_back%13 >= dst%13
      || (dst/26 + src_back/26 + dst%13 - src_back%13) % 2 != 0);
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
  if(o[pile].empty())
    return;

  list<int> &openlist = o[pile];
  int card = openlist.back();
  if( finished[card/13] == card % 13 ) {
    ++finished[card/13];
    openlist.pop_back();
  }
}

void Field::movedown(int pile) {
  if(deckit == deck.begin())
    return;

  list<int> &dstlist = o[pile];
  --deckit;
  int movablecard = *deckit;

  if(dstlist.empty()) {
    if( movablecard % 13 == 12) {
      dstlist.splice(dstlist.begin(), deck, deckit++);
    }
    return;
  }

  int dstcard = dstlist.back();
  if( movable_base( dstcard, movablecard, movablecard) ) {
    dstlist.splice( dstlist.end(), deck, deckit++ );
  }
  else
    ++deckit;
}

void Field::movedirect() {
  if(deckit == deck.begin())
    return;

  --deckit;
  int card = *deckit;
  if( finished[card/13] == card % 13 ) {
    ++finished[card/13];
    deck.erase(deckit++);
  }

}

void Field::flipDeck(int n) {
  if(deckit == deck.end() )
    deckit = deck.begin();
  for(int i = 0; deckit != deck.end() && i < n; ++i)
    ++deckit;
}

void Field::flipReversed(int n) {
  if( !o[n].empty() || r[n].empty() )
    return;

  o[n].push_back(r[n].back());
  r[n].pop_back();
}

void Field::resetDeck() {
  deckit = deck.begin();
}

bool Field::openedDeckNull () const {
  return deckit == deck.begin();
}

bool Field::openPileNull (int i) const {
  return o[i].empty();
}

bool Field::deckNull () const {
  return deckit == deck.end();
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
  if ( f.deckit == f.deck.begin() )
    os << "<null>";
  else {
    list<int>::iterator it = f.deckit;
    outputcard(os, *(--it));
  }
  os << endl;

  os << "9: finished ";
  for(int i = 0; i < 4; ++i)
    os << suitArray[i] << ":" << f.finished[i] << " ";
  os << endl;


  os << endl;

  for(int i = 0; i < 7; ++i) {
    os << i << ": ";
    foreach(int i, f.r[i])
      os << "*";
    os << " ";
    foreach(int i, f.o[i])
      outputcard(os, i) << " ";
    os << endl;
  }

  os << endl << "------------" << endl << endl;

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
