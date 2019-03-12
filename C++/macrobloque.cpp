#include <vector>

using namespace std;

class macrobloque
{
  public:
  int x,y;
  vector<int>arr;
  macrobloque(){}
  macrobloque(vector<int>arr,int x, int y){
    this->arr=arr;
    this->x=x;
    this->y=y;
  }

  int comparar(macrobloque MB){
      int contadorCoincidencias=0;
      for(int i=0; i<this->arr.size();i++){
          if(this->arr!=MB.arr){
              contadorCoincidencias++;
          }
      }    
      return contadorCoincidencias;  
  }

};