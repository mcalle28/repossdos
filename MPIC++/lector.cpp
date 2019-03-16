#pragma once
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <queue>
#include "macrobloque.cpp"
#define HEADER_SIZE 54

using namespace std;

typedef struct
{
    unsigned char signature[2];
    unsigned int width;
    unsigned int height;
    unsigned short int bpp;
    unsigned int padding;
    unsigned char **pixels;
} BMP;

BMP leerBMP(char *file_name);

class lector
{
  private:
    char *frame1;
    char *frame2;
    int w;
    int h;

  public:
    lector()
    {
    }

    lector(char *frame1, char *frame2)
    {
        this->frame1 = frame1;
        this->frame2 = frame2;
    }

    BMP leerBMP(char *file_name)
    {
        BMP bmp_frame;
        FILE *bmp_file;
        unsigned char bmp_header[HEADER_SIZE];
        unsigned int matrix_addr;

        bmp_file = fopen(file_name, "r");

        if (bmp_file != NULL)
        {

            if (fread((void *)bmp_header, sizeof(unsigned char),
                      HEADER_SIZE, bmp_file) != HEADER_SIZE)
            {
                fprintf(stderr, "Error leyendo el archivo file %s", file_name);
                exit(EXIT_FAILURE);
            }

            bmp_frame.width = *(int *)&bmp_header[18];
            bmp_frame.height = fabs(*(int *)&bmp_header[22]);

            bmp_frame.bpp = *(short *)&bmp_header[28];

            bmp_frame.padding = 0;
            bmp_frame.padding = (bmp_frame.width * 3) % 4;
            bmp_frame.width += bmp_frame.padding;

            matrix_addr = *(int *)&bmp_header[10];

            bmp_frame.pixels = (unsigned char **)malloc((size_t)bmp_frame.height * sizeof(unsigned char **));

            for (int row = 0; row < bmp_frame.height; ++row)
            {
                bmp_frame.pixels[row] =
                    (unsigned char *)malloc((size_t)bmp_frame.width);
            }

            fseek(bmp_file, matrix_addr - HEADER_SIZE, SEEK_CUR);

            for (int row = 0; row < bmp_frame.height; ++row)
            {
                if (fread((void *)bmp_frame.pixels[row], sizeof(unsigned char),
                          (size_t)bmp_frame.width, bmp_file) != bmp_frame.width)
                    ;
            }
        }

        fclose(bmp_file);
        return bmp_frame;
    }

    vector<vector<int> > llenarMatrizPixeles(char *filename)
    {
        BMP frame1bmp = leerBMP(filename);
        this->w = frame1bmp.width;
        this->h = frame1bmp.height;

        vector<vector<int> > matrizPixeles = vector<vector<int> >();

        queue<int> matrizDoble;

        for (int i = w - 1; i >= 0; --i)
        {
            for (int j = 0; j < h; ++j)
            {
                matrizDoble.push((int)frame1bmp.pixels[i][j]);
            }
        }

        for (int i = 0; i < w; i++)
        {
            vector<int> columna = vector<int>();
            matrizPixeles.push_back(columna);
            for (int j = 0; j < h; j++)
            {
                matrizPixeles[i].push_back(matrizDoble.front());
                matrizDoble.pop();
            }
        }

        free(frame1bmp.pixels);
        return matrizPixeles;
    }

    vector<macrobloque> crearArregloMB16x16(vector<vector<int> > pixelMatrix, int w, int h)
    {
        vector<macrobloque> arrMB;
        for (int x = 0; x < w; x += 16)
        {
            for (int y = 0; y < h; y += 16)
            {
                vector<int> pixelesMB;
                for (int i = x; i < x + 15; ++i)
                {
                    for (int j = y; j < y + 15; ++j)
                    {
                        pixelesMB.push_back(pixelMatrix[i][j]);
                    }
                }
                macrobloque MB = macrobloque(pixelesMB, x, y);
                arrMB.push_back(MB);
            }
        }
        return arrMB;
    }

    vector<macrobloque> crearArregloMBpxp(vector<vector<int> > pixelMatrix, int w, int h)
    {
        vector<macrobloque> arrMB;
        for (int x = 0; x < w - 15; ++x)
        {
            for (int y = 0; y < h - 15; ++y)
            {
                vector<int> pixelesMB;
                for (int i = x; i < x + 15; ++i)
                {
                    for (int j = y; j < y + 15; ++j)
                    {
                        pixelesMB.push_back(pixelMatrix[i][j]);
                    }
                }
                macrobloque MB = macrobloque(pixelesMB, y, x);
                arrMB.push_back(MB);
            }
        }
        return arrMB;
    }

    vector<vector<macrobloque> > leer()
    {
        vector<vector<int> > matrizPixel1 = llenarMatrizPixeles(this->frame1);
        vector<vector<int> > matrizPixel2 = llenarMatrizPixeles(this->frame2);

        vector<macrobloque> ArrMB1 = crearArregloMB16x16(matrizPixel1, w, h);

        vector<macrobloque> ArrMB2 = crearArregloMBpxp(matrizPixel2, w, h);
        vector<vector<macrobloque> > datos;

        datos.push_back(ArrMB1);
        datos.push_back(ArrMB2);

        return datos;
    }
};