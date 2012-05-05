#ifndef KLONDIKE_HH_
#define KLONDIKE_HH_

#include <vector>
#include <list>
#include <utility>


typedef std::list<int> Deck;
typedef std::vector<std::vector<int> > Reversed;
typedef std::vector<std::list <int> > Opened;

struct Field {
  Deck deck;
  Deck::iterator deckit;
  Reversed r;
  Opened o;
  int finished[4];
public:
  Field();
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
};

#endif
