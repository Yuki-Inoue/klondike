#ifndef SHUFFLE_HH_
#define SHUFFLE_HH_

#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>


class ShuffleCore {
  unsigned n_;
  unsigned *cards_;
public:
  explicit ShuffleCore(unsigned n);
  ~ShuffleCore(){ delete [] cards_; }

  unsigned size() const { return n_; }

protected:
  unsigned get_base(unsigned rand_ind);
};


template <class PseudoRand = boost::random::mt19937>
class Shuffle : public ShuffleCore {
  PseudoRand rng_;
public:
  Shuffle(unsigned n) : ShuffleCore(n) {}
  template <class Seed>
  Shuffle(unsigned n, Seed s) : ShuffleCore(n), rng_(s) {}
  unsigned get() {
    boost::random::uniform_int_distribution<unsigned> dist(0,this->size()-1);
    return get_base(dist(rng_));
  }
};




#endif
