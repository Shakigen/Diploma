#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QLabel>
#include <QDebug>

#include<iostream>
using namespace std;

//Результат работы алгоритмов можно посмотреть в Презентации

//Блочное скремблирование
void sudoku_change(int **image_matrix, int n, int H_full, int W_full, QImage *I, int W, int H);
//rc_bg скремблирование
void rc_bg_change(int **image_matrix, QImage *I, int W, int H);
//Битовое скремблирование
void sudoku_bit_change(int **image_matrix, int n, QImage *I, int W, int H);
//Блочное дескремблирование
void back_sudoku_change(int **image_matrix, int n, int H_full, int W_full, QImage *I, int W, int H);
//rc_bg дескремблирование
void back_rc_bg_change(int **image_matrix, QImage *I, int W, int H);
//Битовое дескремблирование
void back_sudoku_bit_change(int **image_matrix, int n, QImage *I, int W, int H);


int sudoku_puzzle_1[25][25] = { {2,25,18,11,9,3,21,19,12,10,4,22,20,13,6,5,23,16,14,7,1,24,17,15,8},
                                {14,7,5,23,16,15,8,1,24,17,11,9,2,25,18,12,10,3,21,19,13,6,4,22,20},
                                {21,19,12,10,3,22,20,13,6,4,23,16,14,7,5,24,17,15,8,1,25,18,11,9,2},
                                {8,1,24,17,15,9,2,25,18,11,10,3,21,19,12,6,4,22,20,13,7,5,23,16,14},
                                {20,13,6,4,22,16,14,7,5,23,17,15,8,1,24,18,11,9,2,25,19,12,10,3,21},
                                {4,22,20,13,6,5,23,16,14,7,1,24,17,15,8,2,25,18,11,9,3,21,19,12,10},
                                {11,9,2,25,18,12,10,3,21,19,13,6,4,22,20,14,7,5,23,16,15,8,1,24,17},
                                {23,16,14,7,5,24,17,15,8,1,25,18,11,9,2,21,19,12,10,3,22,20,13,6,4},
                                {10,3,21,19,12,6,4,22,20,13,7,5,23,16,14,8,1,24,17,15,9,2,25,18,11},
                                {17,15,8,1,24,18,11,9,2,25,19,12,10,3,21,20,13,6,4,22,16,14,7,5,23},
                                {1,24,17,15,8,2,25,18,11,9,3,21,19,12,10,4,22,20,13,6,5,23,16,14,7},
                                {13,6,4,22,20,14,7,5,23,16,15,8,1,24,17,11,9,2,25,18,12,10,3,21,19},
                                {25,18,11,9,2,21,19,12,10,3,22,20,13,6,4,23,16,14,7,5,24,17,15,8,1},
                                {7,5,23,16,14,8,1,24,17,15,9,2,25,18,11,10,3,21,19,12,6,4,22,20,13},
                                {19,12,10,3,21,20,13,6,4,22,16,14,7,5,23,17,15,8,1,24,18,11,9,2,25},
                                {3,21,19,12,10,4,22,20,13,6,5,23,16,14,7,1,24,17,15,8,2,25,18,11,9},
                                {15,8,1,24,17,11,9,2,25,18,12,10,3,21,19,13,6,4,22,20,14,7,5,23,16},
                                {22,20,13,6,4,23,16,14,7,5,24,17,15,8,1,25,18,11,9,2,21,19,12,10,3},
                                {9,2,25,18,11,10,3,21,19,12,6,4,22,20,13,7,5,23,16,14,8,1,24,17,15},
                                {16,14,7,5,23,17,15,8,1,24,18,11,9,2,25,19,12,10,3,21,20,13,6,4,22},
                                {5,23,16,14,7,1,24,17,15,8,2,25,18,11,9,3,21,19,12,10,4,22,20,13,6},
                                {12,10,3,21,19,13,6,4,22,20,14,7,5,23,16,15,8,1,24,17,11,9,2,25,18},
                                {24,17,15,8,1,25,18,11,9,2,21,19,12,10,3,22,20,13,6,4,23,16,14,7,5},
                                {6,4,22,20,13,7,5,23,16,14,8,1,24,17,15,9,2,25,18,11,10,3,21,19,12},
                                {18,11,9,2,25,19,12,10,3,21,20,13,6,4,22,16,14,7,5,23,17,15,8,1,24},
                            };

int sudoku_puzzle_2[25][25] = { {4,22,20,13,6,5,23,16,14,7,1,24,17,15,8,2,25,18,11,9,3,21,19,12,10},
                                {17,15,8,1,24,18,11,9,2,25,19,12,10,3,21,20,13,6,4,22,16,14,7,5,23},
                                {11,9,2,25,18,12,10,3,21,19,13,6,4,22,20,14,7,5,23,16,15,8,1,24,17},
                                {23,16,14,7,5,24,17,15,8,1,25,18,11,9,2,21,19,12,10,3,22,20,13,6,4},
                                {10,3,21,19,12,6,4,22,20,13,7,5,23,16,14,8,1,24,17,15,9,2,25,18,11},


                                {1,24,17,15,8,2,25,18,11,9,3,21,19,12,10,4,22,20,13,6,5,23,16,14,7},
                                {25,18,11,9,2,21,19,12,10,3,22,20,13,6,4,23,16,14,7,5,24,17,15,8,1},
                                {7,5,23,16,14,8,1,24,17,15,9,2,25,18,11,10,3,21,19,12,6,4,22,20,13},
                                 {13,6,4,22,20,14,7,5,23,16,15,8,1,24,17,11,9,2,25,18,12,10,3,21,19},
                                {19,12,10,3,21,20,13,6,4,22,16,14,7,5,23,17,15,8,1,24,18,11,9,2,25},

                                {5,23,16,14,7,1,24,17,15,8,2,25,18,11,9,3,21,19,12,10,4,22,20,13,6},
                                {12,10,3,21,19,13,6,4,22,20,14,7,5,23,16,15,8,1,24,17,11,9,2,25,18},
                                {24,17,15,8,1,25,18,11,9,2,21,19,12,10,3,22,20,13,6,4,23,16,14,7,5},
                                {6,4,22,20,13,7,5,23,16,14,8,1,24,17,15,9,2,25,18,11,10,3,21,19,12},
                                {18,11,9,2,25,19,12,10,3,21,20,13,6,4,22,16,14,7,5,23,17,15,8,1,24},

                                {3,21,19,12,10,4,22,20,13,6,5,23,16,14,7,1,24,17,15,8,2,25,18,11,9},
                                {15,8,1,24,17,11,9,2,25,18,12,10,3,21,19,13,6,4,22,20,14,7,5,23,16},
                                {22,20,13,6,4,23,16,14,7,5,24,17,15,8,1,25,18,11,9,2,21,19,12,10,3},
                                {16,14,7,5,23,17,15,8,1,24,18,11,9,2,25,19,12,10,3,21,20,13,6,4,22},
                                {9,2,25,18,11,10,3,21,19,12,6,4,22,20,13,7,5,23,16,14,8,1,24,17,15},

                                {21,19,12,10,3,22,20,13,6,4,23,16,14,7,5,24,17,15,8,1,25,18,11,9,2},
                                {8,1,24,17,15,9,2,25,18,11,10,3,21,19,12,6,4,22,20,13,7,5,23,16,14},
                                {2,25,18,11,9,3,21,19,12,10,4,22,20,13,6,5,23,16,14,7,1,24,17,15,8},
                                {14,7,5,23,16,15,8,1,24,17,11,9,2,25,18,12,10,3,21,19,13,6,4,22,20},
                                {20,13,6,4,22,16,14,7,5,23,17,15,8,1,24,18,11,9,2,25,19,12,10,3,21},
                            };

struct block_grid {
    int b;
    int g;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QImage *I;
   // I=new QImage ("/home/denis/Pictures/Wallpapers/moon_or.jpg");
    I=new QImage ("/home/denis/Pictures/Wallpapers/moon_sud.png");
    //размерности шифруемого изображения
    int H=I->height();
    int W=I->width();

    QLabel *image;
    this->resize(W,H);
        image=new QLabel(this);
        image->resize(W,H);
    //размер шифруемого блока
    int n=25;
    //проверка не меньше ли исходное изображение установленного размера шифруемого блока
    if (H<n)
        n=H;
    if (W<n)
        n=W;

    //число частей исходного изображения шифруемых без наложения
    int H_full, W_full;
    H_full = H/n;
    W_full = W/n;

    //наличие наложения блоков при шифровании
    bool H_overlay, W_overlay;
    if(H%n) H_overlay = true;
    else H_overlay = false;
    if(W%n) W_overlay = true;
    else W_overlay = false;

    int x,y,R,G,Bl,Grey,j;
    //матричное представление исходного изображения
    int **image_matrix = new int *[H];
    for(int i=0;i<H;i++)
        image_matrix[i]=new int[W];

    int valueX, valueY;

    //Подготовка матриц Судоку
    for(int j=0;j<n;j++)
        for(int i=0;i<n;i++)
            sudoku_puzzle_1[i][j]+=100*(j+1);

    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            sudoku_puzzle_2[i][j]+=100*(i+1);

    valueX =0;
    valueY =0;
    //перевод изображения в оттенки серого
    for (x=0;x<H;x++)
               for (y=0;y<W;y++)
               {   QColor color(I->pixel(y,x));
                   G=color.green();
                   Bl=color.blue();
                   R=color.red();
                   Grey=0.299*R+0.587*G+0.114*Bl;
                   image_matrix[x][y]=Grey;
               }
    //Блочное скремблирование
    sudoku_change(image_matrix, n, H_full, W_full, I, H, W);

    //RC into BG скремблирование
    rc_bg_change(image_matrix, I, W, H);

    //Битовое скремблирование
    sudoku_bit_change(image_matrix, n, I, W, H);


    //Битовое дескремблирование
    back_sudoku_bit_change(image_matrix, n, I, W, H);

    //RC into BG дескремблирование
    back_rc_bg_change(image_matrix, I, W, H);

    //Блочное дескремблирование
    back_sudoku_change(image_matrix, n, H_full, W_full, I, H, W);


    I->save("/home/denis/Pictures/Wallpapers/s3.png","png");

    QImage field = I->scaled(W,H,Qt::IgnoreAspectRatio);
    image->setPixmap(QPixmap::fromImage(field));

}

//Блочное скремблирование
void sudoku_change(int **image_matrix, int n, int H_full, int W_full, QImage *I, int W, int H)
{
    //матричное представление шифруемого блока
    int **matrix = new int *[n];
    for(int i=0;i<n;i++)
        matrix[i]=new int[n];

    //Матрица заскремблированного блока
    int **matrix_changed = new int *[n];
    for(int i=0;i<n;i++)
        matrix_changed[i]=new int[n];

    bool found = false;
    for(int i=0; i<H_full; i++){
        for(int j=0; j<W_full; j++){
            for (int x=0;x<n;x++)
               for (int y=0;y<n;y++)
               {
                   matrix[x][y]=image_matrix[y+i*n][x+j*n];
               }

           //поблочное скремблирование
           found = false;
           for(int x=0;x<n;x++)
               for(int y=0;y<n;y++) {
                   found = false;
                   for(int k=0;k<n;k++){
                       if(!found)
                           for(int l=0;l<n;l++)
                               if(!found)
                                     if(sudoku_puzzle_1[x][y]==sudoku_puzzle_2[k][l]){
                                            matrix_changed[k][l]=matrix[x][y];
                                            found = true;
                                      }
                    }

                 }
            for (int x=0;x<n;x++)
                    for (int y=0;y<n;y++)
                    {
                        I->setPixel(x+j*n,y+i*n,qRgb(matrix_changed[x][y],matrix_changed[x][y],matrix_changed[x][y]));
                    }
        }
    }


}

//Битовое скремблирование
void sudoku_bit_change(int **image_matrix, int n,  QImage *I, int W, int H)
{
    for (int x=0;x<W;x++)
               for (int y=0;y<H;y++)
               {   QColor color(I->pixel(x,y));

                   image_matrix[y][x]=color.green();
               }
    //kolvo pixeley v izobrazenii
    int P = W*H;
    //massiv isobrazeniya
    int *mass1 = new int[P];
    //матрица битового представления изображения
    int **mass2 = new int *[8];
    for(int i=0;i<8;i++)
        mass2[i]=new int[P];
    for (int i=0;i<H;i++)
         for (int j=0;j<W;j++)
         {
              mass1[i*H+j] = image_matrix[i][j];
         }
    for(int j=0; j<P;j++)
        for(int i=7;i>=0;i--) {
            mass2[i][j]=mass1[j]%2;
            mass1[j]/=2;
        }
    int L, m=8*P;
    //kolvo blokov
    L=m/625;
    L*=n;
    int f =L*n;
    int *med = new int[f];
    for(int i=0; i<8;i++)
        for(int j=0;j<P;j++) {
            med[i*P+j]=mass2[i][j];
        }
    //матрица битового представления подвергаемая скремблированию
    int **mass3 = new int *[L];
    for(int i=0;i<L;i++)
        mass3[i]=new int[n];

    for (int i=0;i<L;i++)
        for (int j=0;j<n;j++)
                mass3[i][j]=med[i*n+j];

    int H_full = L/n;
    int W_full = 1;

    //матричное представление шифруемого блока
    int **matrix = new int *[n];
    for(int i=0;i<n;i++)
        matrix[i]=new int[n];

    //Матрица заскремблированного блока
    int **matrix_changed = new int *[n];
    for(int i=0;i<n;i++)
        matrix_changed[i]=new int[n];

    bool found = false;
    for(int i=0; i<H_full; i++){
        for(int j=0; j<W_full; j++){
            for (int x=0;x<n;x++)
               for (int y=0;y<n;y++)
               {
                   matrix[x][y]=mass3[y+i*n][x+j*n];
               }

           //поблочное скремблирование
           found = false;
           for(int x=0;x<n;x++)
               for(int y=0;y<n;y++) {
                   found = false;
                   for(int k=0;k<n;k++){
                       if(!found)
                           for(int l=0;l<n;l++)
                               if(!found)
                                     if(sudoku_puzzle_1[x][y]==sudoku_puzzle_2[k][l]){
                                            matrix_changed[k][l]=matrix[x][y];
                                            found = true;
                                      }
                    }
                 }
            for (int x=0;x<n;x++)
                    for (int y=0;y<n;y++)
                    {
                        mass3[y+i*n][x+j*n] = matrix_changed[x][y];
                    }
        }
    }
    for (int i=0;i<L;i++)
        for (int j=0;j<25;j++)
                med[i*25+j] = mass3[i][j];
    for(int i=0; i<8;i++)
        for(int j=0;j<P;j++) {
            mass2[i][j] = med[i*P+j];
        }

    for(int j=0; j<P;j++){
        mass1[j]=0;
        for(int i=7;i>=0;i--)
            switch(i){
            case 7: mass1[j]+=mass2[i][j]*1;
            case 6: mass1[j]+=mass2[i][j]*2;
            case 5: mass1[j]+=mass2[i][j]*4;
            case 4: mass1[j]+=mass2[i][j]*8;
            case 3: mass1[j]+=mass2[i][j]*16;
            case 2: mass1[j]+=mass2[i][j]*32;
            case 1: mass1[j]+=mass2[i][j]*64;
            case 0: mass1[j]+=mass2[i][j]*128;
            };
    }
    for (int i=0;i<H;i++)
         for (int j=0;j<W;j++)
         {
              image_matrix[i][j] = mass1[i*H+j];
         }
    for (int i=0;i<H;i++)
         for (int j=0;j<W;j++)
         {
                I->setPixel(j,i,qRgb(mass1[i*H+j],mass1[i*H+j],mass1[i*H+j]));
         }
}





//rc_bg скремблирование
void rc_bg_change(int **image_matrix, QImage *I, int W, int H)
{
    int nn, n;
    W <= H ? nn = sqrt(W) : nn = sqrt(H);
    n = nn*nn;
    int H_full = H/n;
    int W_full = W/n;
    bool H_overlay, W_overlay;
    if(H%n) H_overlay = true;
    else H_overlay = false;
    if(W%n) W_overlay = true;
    else W_overlay = false;

    for (int x=0;x<W;x++)
               for (int y=0;y<H;y++)
               {   QColor color(I->pixel(x,y));

                   image_matrix[y][x]=color.green();
               }
    //матричное представление шифруемого блока
    int **matrix = new int *[n];
    for(int i=0;i<n;i++)
        matrix[i]=new int[n];
    //Матрица заскремблированного блока
    int **matrix_changed = new int *[n];
    for(int i=0;i<n;i++)
        matrix_changed[i]=new int[n];

    for (int x=0;x<W;x++)
               for (int y=0;y<H;y++)
               {   QColor color(I->pixel(x,y));

                   image_matrix[y][x]=color.green();
               }
    for(int i=0; i<H_full; i++){
        for(int j=0; j<W_full; j++){
            for (int x=0;x<n;x++)
               for (int y=0;y<n;y++)
               {
                   matrix[x][y]=image_matrix[y+i*n][x+j*n];
               }

               for(int x=0;x<n;x++)
                    for(int y=0;y<n;y++) {
                        int block = ((x)/nn) * nn + (y)/nn;
                        int grid =  (x  - ((x)/nn)*nn)*nn + y  -((y)/nn)*nn ;
                        matrix_changed[block][grid]=matrix[x][y];
               }

               for (int x=0;x<n;x++)
                        for (int y=0;y<n;y++)
                        {
                            I->setPixel(x+j*n,y+i*n,qRgb(matrix_changed[x][y],matrix_changed[x][y],matrix_changed[x][y]));
                        }
        }
    }
}

//rc_bg дескремблирование
void back_rc_bg_change(int **image_matrix, QImage *I, int W, int H)
{
    int nn, n;
    W <= H ? nn = sqrt(W) : nn = sqrt(H);
    n = nn*nn;
    int H_full = H/n;
    int W_full = W/n;
    bool H_overlay, W_overlay;
    if(H%n) H_overlay = true;
    else H_overlay = false;
    if(W%n) W_overlay = true;
    else W_overlay = false;

    for (int x=0;x<W;x++)
               for (int y=0;y<H;y++)
               {   QColor color(I->pixel(x,y));

                   image_matrix[y][x]=color.green();
               }
    //матричное представление шифруемого блока
    int **matrix = new int *[n];
    for(int i=0;i<n;i++)
        matrix[i]=new int[n];
    //Матрица заскремблированного блока
    int **matrix_changed = new int *[n];
    for(int i=0;i<n;i++)
        matrix_changed[i]=new int[n];

    for (int x=0;x<W;x++)
               for (int y=0;y<H;y++)
               {   QColor color(I->pixel(x,y));

                   image_matrix[y][x]=color.green();
               }
    for(int i=0; i<H_full; i++){
        for(int j=0; j<W_full; j++){
            for (int x=0;x<n;x++)
               for (int y=0;y<n;y++)
               {
                   matrix[x][y]=image_matrix[y+i*n][x+j*n];
               }

               for(int x=0;x<n;x++)
                    for(int y=0;y<n;y++) {
                        int row = ((x)/nn) * nn + (y)/nn;
                        int column =  (x  - ((x)/nn)*nn)*nn + y  -((y)/nn)*nn ;
                        matrix_changed[row][column]=matrix[x][y];
               }

               for (int x=0;x<n;x++)
                        for (int y=0;y<n;y++)
                        {
                            I->setPixel(x+j*n,y+i*n,qRgb(matrix_changed[x][y],matrix_changed[x][y],matrix_changed[x][y]));
                        }
        }
    }
}

//Блочное дескремблирование
void back_sudoku_change(int **image_matrix, int n, int H_full, int W_full, QImage *I, int W, int H)
{
    for (int x=0;x<W;x++)
               for (int y=0;y<H;y++)
               {   QColor color(I->pixel(x,y));

                   image_matrix[y][x]=color.green();
               }
    //матричное представление шифруемого блока
    int **matrix = new int *[n];
    for(int i=0;i<n;i++)
        matrix[i]=new int[n];

    //Матрица заскремблированного блока
    int **matrix_changed = new int *[n];
    for(int i=0;i<n;i++)
        matrix_changed[i]=new int[n];

    bool found = false;
    //Шифруем главную часть
    for(int i=0; i<H_full; i++){
        for(int j=0; j<W_full; j++){
            for (int x=0;x<n;x++)
               for (int y=0;y<n;y++)
               {
                   matrix[x][y]=image_matrix[y+i*n][x+j*n];
               }

           //поблочное скремблирование
           found = false;
           for(int x=0;x<n;x++)
                                 for(int y=0;y<n;y++) {
                                     found = false;
                                     for(int k=0;k<n;k++){
                                         if(!found)
                                            for(int l=0;l<n;l++)
                                                if(!found)
                                                      if(sudoku_puzzle_2[x][y]==sudoku_puzzle_1[k][l]){
                                                         matrix_changed[k][l]=matrix[x][y];
                                                         found = true;
                                                      }
                                     }

                                  }


            for (int x=0;x<n;x++)
                    for (int y=0;y<n;y++)
                    {
                        I->setPixel(x+j*n,y+i*n,qRgb(matrix_changed[x][y],matrix_changed[x][y],matrix_changed[x][y]));
                    }
        }
    }
}


//Битовое дескремблирование
void back_sudoku_bit_change(int **image_matrix, int n,  QImage *I, int W, int H)
{
    //kolvo pixeley v izobrazenii
    int P = W*H;
    //massiv isobrazeniya
    int *mass1 = new int[P];
    //матрица битового представления изображения
    int **mass2 = new int *[8];
    for(int i=0;i<8;i++)
        mass2[i]=new int[P];
    for (int i=0;i<H;i++)
         for (int j=0;j<W;j++)
         {
              mass1[i*H+j] = image_matrix[i][j];
         }
    for(int j=0; j<P;j++)
        for(int i=7;i>=0;i--) {
            mass2[i][j]=mass1[j]%2;
            mass1[j]/=2;
        }
    int L, m=8*P;
    //kolvo blokov
    L=m/625;
    L*=n;
    int f =L*n;
    int *med = new int[f];
    for(int i=0; i<8;i++)
        for(int j=0;j<P;j++) {
            med[i*P+j]=mass2[i][j];
        }
    //матрица битового представления подвергаемая скремблированию
    int **mass3 = new int *[L];
    for(int i=0;i<L;i++)
        mass3[i]=new int[n];

    for (int i=0;i<L;i++)
        for (int j=0;j<n;j++)
                mass3[i][j]=med[i*n+j];

    int H_full = L/n;
    int W_full = 1;

    //матричное представление шифруемого блока
    int **matrix = new int *[n];
    for(int i=0;i<n;i++)
        matrix[i]=new int[n];

    //Матрица заскремблированного блока
    int **matrix_changed = new int *[n];
    for(int i=0;i<n;i++)
        matrix_changed[i]=new int[n];

    bool found = false;
    for(int i=0; i<H_full; i++){
        for(int j=0; j<W_full; j++){
            for (int x=0;x<n;x++)
               for (int y=0;y<n;y++)
               {
                   matrix[x][y]=mass3[y+i*n][x+j*n];
               }

           //поблочное скремблирование
           found = false;
           for(int x=0;x<n;x++)
               for(int y=0;y<n;y++) {
                   found = false;
                   for(int k=0;k<n;k++){
                       if(!found)
                           for(int l=0;l<n;l++)
                               if(!found)
                                     if(sudoku_puzzle_2[x][y]==sudoku_puzzle_1[k][l]){
                                            matrix_changed[k][l]=matrix[x][y];
                                            found = true;
                                      }
                    }
                 }
            for (int x=0;x<n;x++)
                    for (int y=0;y<n;y++)
                    {
                        mass3[y+i*n][x+j*n] = matrix_changed[x][y];
                    }
        }
    }
    for (int i=0;i<L;i++)
        for (int j=0;j<25;j++)
                med[i*25+j] = mass3[i][j];
    for(int i=0; i<8;i++)
        for(int j=0;j<P;j++) {
            mass2[i][j] = med[i*P+j];
        }

    for(int j=0; j<P;j++){
        mass1[j]=0;
        for(int i=7;i>=0;i--)
            switch(i){
                case 7: mass1[j]+=mass2[i][j]*1;
                case 6: mass1[j]+=mass2[i][j]*2;
                case 5: mass1[j]+=mass2[i][j]*4;
                case 4: mass1[j]+=mass2[i][j]*8;
                case 3: mass1[j]+=mass2[i][j]*16;
                case 2: mass1[j]+=mass2[i][j]*32;
                case 1: mass1[j]+=mass2[i][j]*64;
                case 0: mass1[j]+=mass2[i][j]*128;
            };
    }
    for (int i=0;i<H;i++)
         for (int j=0;j<W;j++)
         {
              image_matrix[i][j] = mass1[i*H+j];
         }
    for (int i=0;i<H;i++)
         for (int j=0;j<W;j++)
         {
                I->setPixel(j,i,qRgb(mass1[i*H+j],mass1[i*H+j],mass1[i*H+j]));
         }
}

void SaltAndPepper(float** cordata, int H, int W)
{
    double temp;
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 1);
    for (int i=0; i<H; i++)
        for (int j=0; j<W; j++)
        {
            temp = urd(gen);
          if (temp <= white && white != 0)
            cordata[i][j] = 255;
          else if (temp >= black && black != 1)
              cordata[i][j] = 0;
        }
}

void GausNoise(float** cordata, int H, int W)
{
    Mat imGray = Mat(H,W,CV_64F);
    for (int i=0;i<H;i++)
        for (int j=0;j<W;j++)
            imGray.at<double>(H*i+j)=cordata[i][j];
    Mat noise = Mat(imGray.size(),CV_64F);
    Mat result;
    normalize(imGray, result, 0.0, 1.0, CV_MINMAX, CV_64F);
    randn(noise, mat, Otk);
    result = result + noise;
    normalize(result, result, 0.0, 1.0, CV_MINMAX, CV_64F);
    result.convertTo(result, CV_64F, 255, 0);
    for (int i=0;i<H;++i)
        for (int j=0;j<W;++j)
            cordata[i][j]=result.at<double>(H*i+j);
    imGray.release();
    noise.release();
    result.release();
}

MainWindow::~MainWindow()
{
    delete ui;
}
