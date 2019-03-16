#pragma once
#include <iostream>
#include <vector>
#include "macrobloque.cpp"
#include "vector4.cpp"
#include <omp.h>
#include <mpi.h>
#include "mpi.h"

using namespace std;

class compresor
{
  private:
    vector<macrobloque>MBArr1;
    vector<macrobloque> MBArr2;

  public:
  compresor(){}
  compresor(vector<macrobloque>MBArr1, vector<macrobloque>MBArr2){
      this->MBArr1=MBArr1;
      this->MBArr2=MBArr2;
  }

    void compresionMPEG2(int argc, char* argv[]){

        
        int numMB = MBArr1.size();
        int pid, npr;
        int cantMBxProc;
        vector<macrobloque>MBrecv;


        MPI_Init(&argc, &argv); 
            MPI_Comm_rank(MPI_COMM_WORLD,&pid); 
            MPI_Comm_size(MPI_COMM_WORLD,&npr);
            cantMBxProc = numMB / npr;
            MPI_Scatter(MBArr1, cantMBxProc, MPI_INT, &MBrecv, cantMBxProc, MPI_INT, 0, MPI_COMM_WORLD);


            for(int i=0;i<MBrecv.size();i++){
            int valMin=256;
            vector4 vector;
            bool compZero = false;
            #pragma omp for 
                for(int j=0;j<MBArr2.size();j++){
                    if (compZero) {
                        compZero = true;
                    } else {
                        int comparacion = MBrecv[i].comparar(MBArr2[j]);
                        if(comparacion==0){
                            vector=vector4(MBrecv[i].x, MBrecv[i].y, MBArr2[j].x, MBArr2[j].y);
                            compZero = true;
                        }
                        else if(comparacion<=valMin){
                            valMin=comparacion;
                            vector=vector4(MBrecv[i].x, MBrecv[i].y, MBArr2[j].x, MBArr2[j].y);
                        }
                    }

                }
                vector.imprimir();      
            }


        MPI_Finalize();

        
    }
};