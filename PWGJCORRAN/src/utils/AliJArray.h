template < typename T> class AliJArray{
public:
  int Dimension();
  int SizeOf( int i );
  int Index( int i );

  int BuildArray();
  T GetItem();

  Iter Begin(){ return Iter(0); }
  Iter End(){ return Iter(-1); } // TODO
  Iter Current(){ return Iter(fPos); }

  class Iter{
  public:
    Iter & Next(){ fPos++; return this; }
  private:
    fPos;
  };
private:
  ArrayInt  fDimFactor; // Dim factor cahche to calculate Global Index
  vector<T> fArray;
  int       fPos;


}
