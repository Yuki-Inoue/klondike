
Pile::Pile(int i) : v_(i) {
  if(i<0 || i>= 7)
    throw "pile out of bounds"
}
