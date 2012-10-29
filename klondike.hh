#ifndef KLONDIKE_HH_
#define KLONDIKE_HH_

#include <vector>
#include <list>
#include <utility>



// when not crushing, the field satisfies following consistency:
// * there exists 52 cards in field
// * the opened are in alternate coloring descending order.
class Field {

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

  // constructor
public:
  Field();

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
  void flipDeck (Pile n);
  // flip the reversed[pile] card.
  void flipReversed (Pile pile);
  // make deck all reversed.
  void resetDeck ()
  { reversedDeck_.splice(reversedDeck_.end(), openDeck_); }

  // attributes
public:
  bool revDeckEmpty() const { return reversedDeck_.empty(); }
  int revDeckSize() const { return reversedDeck_.size(); }
  bool reversedEmpty(Pile pile) const { return reversed_[pile].empty(); }
  int reversedSize(Pile pile) const { return reversed_[pile].size(); }

  // accessor
public:
  const std::list<int> &getOpenedDeck() const { return openedDeck_; }
  const std::list<int> (&getOpened())[7] const { return opened_; }
  const int (&getFinished())[4] const { return finished_; }
};

#endif
