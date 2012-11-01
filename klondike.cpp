#include "klondike.hh"
#include "shuffle.hh"

#include <vector>
#include <algorithm>
#include <boost/foreach.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#define foreach BOOST_FOREACH

using std::max;
using std::min;
using std::list;
using std::ostream;
using std::cout;
using std::cin;
using std::endl;
using std::make_pair;
using std::string;

struct KlondikeImpl {
  std::vector<int> reversed_[7];
  // the smallest number the bottom
  std::list<int> opened_[7];
  int finished_[4];

  // openedDeck: visible from user.
  //   the top is the operable.
  std::list<int> openedDeck_;
  // unvisible from user.
  //   the top is the last one to be visible.
  std::list<int> reversedDeck_;
};


Klondike::Klondike() {
  pImpl_ = new KlondikeImpl;

  for(int i = 0; i < 4; ++i)
    pImpl_->finished_[i] = 0;

  Shuffle<> s(52);
  for(int i = 0; i < 7; ++i)
    pImpl_->opened_[i].push_back(s.get());
  for(int i = 0; i < 7; ++i)
    for(int j = 0; j < i; ++j)
      pImpl_->reversed_[i].push_back(s.get());
  while(s.size() > 0)
    pImpl_->reversedDeck_.push_back(s.get());
}

Klondike::~Klondike(){ delete pImpl_; }


// given the top(the largest) and the bottom (the smallest)
// of continuous pile, consider if it is movable right under the dst card.
bool movable_base(int dst, int src_top, int src_bottom) {
  return

    // first 2 rows check if the number is all right.
    !(getNumber(dst) > getNumber(src_top) + 1
      || getNumber(src_bottom) >= getNumber(dst)

      // and this row checks if the color is all right.
      || (getColor(dst)
	  + getColor(src_bottom)
	  + getNumber(dst)
	  - getNumber(src_bottom)) % 2 != 0);

}

void Klondike::move(Pile src_pile, Pile dst_pile) {

  list<int> &srclist = pImpl_->opened_[src_pile];
  if(srclist.empty())
    return;

  list<int> &dstlist = pImpl_->opened_[dst_pile];
  int src_back = srclist.back();
  int src_top = srclist.front();

  if(dstlist.empty()) {
    if (getNumber(src_top) != 12)
      return;
    dstlist.splice(dstlist.end(), srclist);
    return;
  }

  int dst = dstlist.back();
  int ofs = getNumber(dst) - getNumber(src_back);

  if(movable_base(dst, src_top, src_back)) {
    list<int>::iterator it, itend;
    it = itend = srclist.end();
    std::advance(it, -ofs);
    dstlist.splice(dstlist.end(), srclist, it, itend);
  }
}

void Klondike::moveUp(Pile pile) {
  if(pImpl_->opened_[pile].empty())
    return;

  list<int> &openlist = pImpl_->opened_[pile];
  int card = openlist.back();
  if( pImpl_->finished_[getSuit(card)] == getNumber(card) ) {
    ++pImpl_->finished_[getSuit(card)];
    openlist.pop_back();
  }
}

void Klondike::moveDown(Pile pile) {
  if( pImpl_->openedDeck_.empty() )
    return;

  int moving_card = pImpl_->openedDeck_.front();
  list<int> &dstlist = pImpl_->opened_[pile];

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
  dstlist.splice( dstlist.end(), pImpl_->openedDeck_, pImpl_->openedDeck_.begin() );
}

void Klondike::moveDirect() {
  if( pImpl_->openedDeck_.empty() )
    return;

  int card = pImpl_->openedDeck_.front();
  int suit = getSuit(card);
  if( pImpl_->finished_[suit] == getNumber(card) ) {
    ++pImpl_->finished_[suit];
    pImpl_->openedDeck_.pop_front();
  }
}

void Klondike::flipDeck(unsigned n) {
  for(int i = 0; !pImpl_->reversedDeck_.empty() && i < n; ++i)
    pImpl_->openedDeck_.splice
      (pImpl_->openedDeck_.begin(), pImpl_->reversedDeck_, --pImpl_->reversedDeck_.end());
}

void Klondike::flipReversed(Pile n) {
  if( pImpl_->reversed_[n].empty() )
    return;
  pImpl_->opened_[n].push_back(pImpl_->reversed_[n].back());
  pImpl_->reversed_[n].pop_back();
}

void Klondike::resetDeck ()
{ pImpl_->reversedDeck_.splice(pImpl_->reversedDeck_.end(), pImpl_->openedDeck_); }



bool Klondike::revDeckEmpty() const { return pImpl_->reversedDeck_.empty(); }
unsigned Klondike::revDeckSize() const { return pImpl_->reversedDeck_.size(); }

bool Klondike::openedDeckEmpty() const { return pImpl_->openedDeck_.empty(); }
unsigned Klondike::openedDeckSize() const { return pImpl_->openedDeck_.size(); }

bool Klondike::reversedEmpty(Pile pile) const { return pImpl_->reversed_[pile].empty(); }
unsigned Klondike::reversedSize(Pile pile) const { return pImpl_->reversed_[pile].size(); }

bool Klondike::openedEmpty(Pile pile) const { return pImpl_->opened_[pile].empty(); }
unsigned Klondike::openedSize(Pile pile) const { return pImpl_->opened_[pile].size(); }



Klondike::const_card_range Klondike::getOpenedDeck() const {
  const list<int> &openedDeck = pImpl_->openedDeck_;
  return make_pair(openedDeck.begin(), openedDeck.end());
}


Klondike::const_card_range Klondike::getOpened(Pile p) const {
  const list<int> &opened = pImpl_->opened_[p];
  return make_pair(opened.begin(), opened.end());
}


int Klondike::getFinished(Suit s) const {
  return pImpl_->finished_[s];
}
