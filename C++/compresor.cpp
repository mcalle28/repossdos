#include <iostream>
#include <vector>
#include <iostream>
#include "macrobloque.cpp"
#include "vector4.cpp"

using namespace std;

class compresor
{
  private:
    vector<macrobloque>MBArr1;
    vector<macrobloque> MBArr2;

  public:
  compresor(vector<macrobloque>MBArr1, vector<macrobloque>MBArr2, int w, int h){
      this->MBArr1=MBArr1;
      this->MBArr2=MBArr2;
  }

    void compresionMPEG2(){
        vector4 v4;
        for(int i=0;i<this->MBArr1.size();i++){
            int valMin=256;
            for(int j=0;j<this->MBArr2.size();j++){
                int comparacion = this->MBArr1[i].comparar(this->MBArr2[j]);
                if(comparacion==0){
                    v4=vector4(this->MBArr1[i].x, this->MBArr1[i].y, this->MBArr2[j].x, this->MBArr2[j].y);
                    break;
                }
                else if(comparacion<=valMin){
                    valMin=comparacion;
                    v4=vector4(this->MBArr1[i].x, this->MBArr1[i].y, this->MBArr2[j].x, this->MBArr2[j].y);
                }

            }
            v4.imprimir();      
        }
    }
};