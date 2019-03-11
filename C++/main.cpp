#include <iostream>
#include <vector>
#include "compresor.cpp"

using namespace std;

int main(void){
    vector<int>arr1={1,2,3,4,5};
    vector<int>arr2={1,2,3,4,5};
    vector<int>arr3={6,8,7,9,10};
    vector<int>arr4={6,7,8,9,10};

    macrobloque mb1= macrobloque(arr1,1,2);
    macrobloque mb2= macrobloque(arr2,1,2);
    macrobloque mb4= macrobloque(arr3,3,4);
    macrobloque mb5= macrobloque(arr4,3,4);

    vector<macrobloque> MBarr1= {mb1,mb2};
    vector<macrobloque> MBarr2= {mb4,mb5};

    compresor c= compresor(MBarr1,MBarr2,2,2);
    c.compresionMPEG2();


    return 0;
}