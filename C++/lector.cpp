#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include "macrobloque.cpp"
#include <string>

using namespace std;

class lector
{
  private:
    char* frame1;
    char* frame2;
    int w;
    int h;

  public:
    lector(){

    }

    lector(char* frame1, char* frame2)
    {
        this->frame1 = frame1;
        this->frame2 = frame2;
    }

    vector<vector<int> > llenarMatrizPixeles(char *filename){

        int c;
        FILE *f = fopen(filename, "rb");
        unsigned char info[54];
        fread(info, sizeof(unsigned char), 54, f); 

        int width = *(int *)&info[18];
        int height = *(int *)&info[22];

        this->w = width;
        this->h = height;

        int size = 3 * width * height;
        unsigned char *data = new unsigned char[size]; 
        fread(data, sizeof(unsigned char), size, f);   
        fclose(f);

        queue<int> pixeles;
        for (c = 0; c < size; c += 3)
        {
            stringstream ss;
            ss << int(data[c]);
            ss<< int(data[c + 1]);
            ss<< int(data[c + 2]);
            string p;
            ss >> p;
            int x= stoi(p);
            pixeles.push(x);
        }
        vector<vector<int> > matrizPixeles = vector<vector<int> >(width);
        for (int i = 0; i < width; i++)
        {
            vector<int> columna = vector<int>(height);
            matrizPixeles.push_back(columna);
            for (int j = 0; j < height; j++)
            {
                matrizPixeles[i].push_back(pixeles.front());
                pixeles.pop(); 
            }
        }
        return matrizPixeles;
    }

    vector<macrobloque> crearArregloMB16x16(vector<vector<int> > pixelMatrix, int w, int h){
        vector<macrobloque> arrMB;
        int xsatan=0;
        for (int x = 0; x < w; x += 16){
            for (int y = 0; y < h; y += 16){
                vector<int> pixelesMB;
                for (int i = x; i < x + 16; ++i){
                    for (int j = y; j < y + 16; ++j){
                        pixelesMB.push_back(pixelMatrix[i][j]);
                    }
                }
                //cout<<pixelesMB.size()<<x<<y<<endl;
                macrobloque MB = macrobloque(pixelesMB, x, y);
                arrMB.push_back(MB);
            }
        } 
        return arrMB;
    }

    vector<macrobloque> crearArregloMBpxp(vector<vector<int> > pixelMatrix, int w, int h){
        vector<macrobloque> arrMB;
        for (int x = 0; x < w-15; ++x){
            for (int y = 0; y<h-15; ++y){
                vector<int> pixelesMB;
                for (int i = x; i < x + 16; ++i){
                    for (int j = y; j < y + 16; ++j){
                        pixelesMB.push_back(pixelMatrix[i][j]);
                    }
                }
                macrobloque MB = macrobloque(pixelesMB, x, y);
                arrMB.push_back(MB);
            }
        }
        return arrMB;
    }

    vector<vector<macrobloque> > leer(){
        vector<vector<int> > matrizPixel1 = llenarMatrizPixeles(this->frame1);
        vector<vector<int> > matrizPixel2 = llenarMatrizPixeles(this->frame2);

        vector<macrobloque> ArrMB1 = crearArregloMB16x16(matrizPixel1, w, h);
        vector<macrobloque> ArrMB2 = crearArregloMBpxp(matrizPixel2, w, h);

        vector<vector<macrobloque> > 
        datos;

        datos.push_back(ArrMB1);
        datos.push_back(ArrMB2);

        return datos;
    }
};