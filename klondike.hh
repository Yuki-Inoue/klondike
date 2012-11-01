#ifndef KLONDIKE_HH_
#define KLONDIKE_HH_

#include <list>
#include <utility>

#include "trump.hpp"


class Pile {
  int v_;
public:
  Pile(int i = 0) : v_(i) {
    if( i<0 || i>=7) throw "Pile range error";
  }
  operator int(){ return v_; }
};


class KlondikeImpl;


// when not crushing, the field satisfies following consistency:
// * there exists 52 cards in field
// * the opened are in alternate coloring descending order.
class Klondike {

public:
  typedef std::list<int>::const_iterator const_card_iterator;
  typedef std::pair<const_card_iterator,const_card_iterator> const_card_range;


  // constructor
public:
  Klondike();
  ~Klondike();

  // operations
public:
  // move the opened cards
  void move (Pile src, Pile dst);
  // from the top of openDeck_ move it to the finished pile.
  void moveUp (Pile pile);
  // from the top of openDeck_ move it to the pile.
  void moveDown (Pile pile);
  // from openDeck_ to finished_
  void moveDirect ();
  // flip deck by n cards
  void flipDeck (unsigned n);
  // flip the reversed[pile] card.
  void flipReversed (Pile pile);
  // make deck all reversed.
  void resetDeck ();

  // attributes
public:
  bool revDeckEmpty() const;
  unsigned revDeckSize() const;

  bool openedDeckEmpty() const;
  unsigned openedDeckSize() const;

  bool reversedEmpty(Pile pile) const;
  unsigned reversedSize(Pile pile) const;

  bool openedEmpty(Pile pile) const;
  unsigned openedSize(Pile pile) const;

  // accessor
public:
  const_card_range getOpenedDeck() const;
  const_card_range getOpened(Pile p) const;
  int getFinished(Suit s) const;

private:
  KlondikeImpl *pImpl_;
};

#endif
