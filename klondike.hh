#ifndef KLONDIKE_HH_
#define KLONDIKE_HH_

#include <vector>
#include <list>
#include <utility>


typedef std::list<int> Deck;
typedef std::vector<std::vector<int> > Reversed;
typedef std::vector<std::list <int> > Opened;

class Field {
  Deck deck;
  Deck::iterator deckit;
  Reversed r;
  Opened o;
  int finished[4];
public:

  // constructor
  Field();

  // operator
  void move (std::pair<int,int> act);
  void moveup (int pile);
  void movedown (int pile);
  void movedirect ();
  void flipDeck (int n);
  void flipReversed (int n);
  void resetDeck ();
  // attributes
  bool openedDeckNull () const;
  bool openPileNull (int n) const;
  bool deckNull() const;
  int numReversed(int pile) const { return r[pile].size(); }
  // accessor
  const Deck &getDeck() const { return deck; }
  Deck::iterator getDeckit() const { return deckit; }
  const Opened &getOpened() const { return o; }
  const int *getFinished() const { return finished; }
};

#endif
