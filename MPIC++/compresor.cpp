#pragma once
#include <iostream>
#include <vector>
#include "macrobloque.cpp"
#include "vector4.cpp"
#include <omp.h>
#include <mpi.h>
#include "mpi.h"

using namespace std;

struct CustomMB{
  int x,y;
  vector<int> arr;
};

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

            constexpr int n_structure_per_process = numMB;

            CustomData data[n_structure_per_process];

            for (int i=0; i < n_structure_per_process; ++i) {  
                data[i].x = MBArr1[i].x;
                data[i].y = MBArr1[i].y;              
                for (int j=0; j < MBArr1[i].arr.size(); ++j){                    
                    data[i].arr[j] = MBArr1[i].arr[j];
                }
            }

            int int_send_bufx[n_structure_per_process];
            int int_send_bufy[n_structure_per_process];
            vector<int> dbl_send_bufArr[n_structure_per_process * 256];

            for (int i=0; i < n_structure_per_process; ++i) {
                int_send_bufx[i] = data[i].x;
                int_send_bufy[i] = data[i].y;
                for (int j=0; j < data[i].n_values; ++j)
                    dbl_send_bufArr[i*256 + j] = data[i].arr[j];
            }
            

            // Gathering everything on process 0
            int *xre = nullptr; 
            int *yre = nullptr;
            vector<int> *dbl_arr = nullptr;

            if (pid == 0) {
                xre = new int[n_structure_per_process * npr];
                yre = new int[n_structure_per_process * npr];
                dbl_arr = new[n_structure_per_process* 256]
            }
    
            MPI_Scatter(int_send_bufx, n_structure_per_process, MPI_INT,&xre, n_structure_per_process, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Scatter(int_send_bufy, n_structure_per_process, MPI_INT,&yre, n_structure_per_process, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Scatter(dbl_send_bufArr, n_structure_per_process * 256, MPI_INT, &dbl_arr, n_structure_per_process * 256, MPI_INT, 0, MPI_COMM_WORLD);
            

            cantMBxProc = numMB / npr;
            MPI_Scatter(MBArr1, cantMBxProc, MPI_macrobloque, &MBrecv, cantMBxProc, MPI_macrobloque, 0, MPI_COMM_WORLD);

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