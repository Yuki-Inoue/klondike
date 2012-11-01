#include "shuffle.hh"

ShuffleCore::ShuffleCore(unsigned n) : n_(n), cards_(new unsigned[n]) {
  unsigned i;
  for(i=0; i<n_; ++i)
    cards_[i] = i;
  --n_;
}

unsigned ShuffleCore::get_base(unsigned i){
  unsigned ret = cards_[i];
  --n_;
  for(; i<n_; ++i)
    cards_[i] = cards_[i+1];

  return ret;
}
