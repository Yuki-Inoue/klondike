#include "shuffle.hh"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>



Shuffle::Shuffle(int n_) : n(n_-1), cards(new int[n_]) {
  int i;
  for(i=0; i<n_; ++i)
    cards[i] = i;
}

Shuffle::~Shuffle() {
  delete [] cards;
}

int Shuffle::get() {
  static boost::random::mt19937 rng(std::time(0));
  boost::random::uniform_int_distribution<> dist(0,n);

  int i = dist(rng);
  int ret = cards[i];
  for(; i < n;++i)
    cards[i] = cards[i+1];

  --n;

  return ret;
}
