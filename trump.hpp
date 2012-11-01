#ifndef YUKI_INOUE_TRUMP_HPP
#define YUKI_INOUE_TRUMP_HPP

class Suit{
  int v_;
public:
  Suit(int i=0) : v_(i) {
    if(i<0 || i>=4) throw "suit range error";
  }
  operator int() { return v_; }
};

inline int getNumber(int card) {
     return card % 13;
}

inline int getSuit(int card) {
     return card / 13;
}

inline int getColor(int card) {
     return card / 26;
}

#endif
