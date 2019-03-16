#include <iostream>

using namespace std;

class vector4
{
  private:
  int i,j,x,y;

  public:
  vector4(){}
  vector4(int i, int j, int x,int y){
    this->i=i;
    this->j=j;
    this->x=x;
    this->y=y;
  }

  void imprimir(){
      cout<<"MB("<<this->i<<","<<this->j<<")=(x"<<this->x<<",y"<<this->y<<") "<<endl;
  }
};