#ifndef SHUFFLE_HH_
#define SHUFFLE_HH_

class Shuffle {
  int n;
  int *cards;
public:
  explicit Shuffle(int n_);
  ~Shuffle();
  int get();
  bool empty() const { return n < 0; }
};

#endif
