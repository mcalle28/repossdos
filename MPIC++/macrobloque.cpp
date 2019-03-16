#pragma once
#include <vector>
#include <omp.h>
using namespace std;

class macrobloque
{
  public:
  int x,y;
  vector<int> arr;
  macrobloque(){}
  macrobloque(vector<int>arr,int x, int y){
    this->arr=arr;
    this->x=x;
    this->y=y;
  }

  int comparar(macrobloque MB){
      int contadorCoincidencias=0;
      #pragma omp for
      for(int i=0; i<arr.size();i++){
          if(arr[i]!=MB.arr[i]){
              contadorCoincidencias++;
          }
      }
      return contadorCoincidencias;
  }
};