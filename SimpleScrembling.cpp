#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QLabel>

#include<iostream>
using namespace std;

//Результат работы алгоритмов можно посмотреть в Презентации

//Блочное скремблирование
int** arnold_change(int **matrix, int n);
//Блочное дескремблирование
int** back_arnold_change(int **matrix, int n);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QImage *I;
    I=new QImage ("/home/denis/Pictures/Wallpapers/moon_ar.png");
    //размерности шифруемого изображения
    int H=I->height();
    int W=I->width();

    QLabel *image;
    this->resize(W,H);
        image=new QLabel(this);
        image->resize(W,H);
    //размер шифруемого блока
    int n=400;
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

    //матричное представление шифруемого блока
    int **matrix = new int *[n];
    for(int i=0;i<n;i++)
        matrix[i]=new int[n];
    int valueX, valueY;

    //количество циклов Алгоритма арнольда
    int cycleA = 10;

    //преобразование 1
    valueX =0;
    valueY =0;
    //перевод изображения в оттенки серого
    for (x=0;x<W;x++)
               for (y=0;y<H;y++)
               {   QColor color(I->pixel(x,y));
                   G=color.green();
                   Bl=color.blue();
                   R=color.red();
                   Grey=0.299*R+0.587*G+0.114*Bl;
                   image_matrix[y][x]=G;
               }

    //Поскольку необхлдимо шифровать как квадратные, так и прямоугольные изображения разработан следующий порядок применения алгоритма
    //Шифруем главную часть
    for(int i=0; i<H_full; i++){
        for(int j=0; j<W_full; j++){
            for (x=0;x<n;x++)
               for (y=0;y<n;y++)
               {
                   matrix[x][y]=image_matrix[y+i*n][x+j*n];
               }

               for(int cycle = 0; cycle < cycleA; cycle++)
                matrix = arnold_change(matrix, n);

                for (x=0;x<n;x++)
                        for (y=0;y<n;y++)
                        {
                            I->setPixel(y+j*n,x+i*n,qRgb(matrix[y][x],matrix[y][x],matrix[y][x]));
                        }
        }
    }

    //Шифруем крайний столбец с наложением
      for(int i=0; i<H_full; i++){

        if(W_overlay){
            for (x=0;x<W;x++)
                       for (y=0;y<H;y++)
                       {   QColor color(I->pixel(x,y));

                           image_matrix[y][x]=color.green();
                       }
            int start_W_overlay = W - n  ;
            for (x=0;x<n;x++)
               for (y=0;y<n;y++)
               {
                   matrix[x][y]=image_matrix[y+i*n][x+start_W_overlay];
               }

               for(int cycle = 0; cycle < cycleA; cycle++)
                    matrix = arnold_change(matrix, n);

                for (x=0;x<n;x++)
                    for (y=0;y<n;y++)
                    {
                        I->setPixel(x+start_W_overlay,y+i*n,qRgb(matrix[x][y],matrix[x][y],matrix[x][y]));
                    }
         }
    }

    //Шифруем нижнюю строку с наложением
    if(H_overlay){
        for (x=0;x<W;x++)
                   for (y=0;y<H;y++)
                   {   QColor color(I->pixel(x,y));

                       image_matrix[y][x]=color.green();
                   }
        for(int j=0; j<W_full; j++){
            int start_H_overlay = H - n  ;
            for (x=0;x<n;x++)
               for (y=0;y<n;y++)
               {
                   matrix[x][y]=image_matrix[y+start_H_overlay][x+j*n];
               }

               for(int cycle = 0; cycle < cycleA; cycle++)
                    matrix = arnold_change(matrix, n);

                for (x=0;x<n;x++)
                    for (y=0;y<n;y++)
                    {
                        I->setPixel(x+j*n,y+start_H_overlay,qRgb(matrix[x][y],matrix[x][y],matrix[x][y]));
                    }

        }
    }

    //Шифруем крайний правый нижний элемент
    if(W_overlay && H_overlay){
        for (x=0;x<W;x++)
                   for (y=0;y<H;y++)
                   {   QColor color(I->pixel(x,y));

                       image_matrix[y][x]=color.green();
                   }
        int start_W_overlay = W - n  ;
        int start_H_overlay = H - n  ;
        for (x=0;x<n;x++)
           for (y=0;y<n;y++)
           {
               matrix[x][y]=image_matrix[y+start_H_overlay][x+start_W_overlay];
           }

           for(int cycle = 0; cycle < cycleA; cycle++)
               matrix = arnold_change(matrix, n);

            for (x=0;x<n;x++)
                for (y=0;y<n;y++)
                {
                    I->setPixel(x+start_W_overlay,y+start_H_overlay,qRgb(matrix[x][y],matrix[x][y],matrix[x][y]));
                }
     }

    //восстановление 3

    //Дешифруем крайний правый нижний элемент
    if(W_overlay && H_overlay){
        for (x=0;x<W;x++)
                   for (y=0;y<H;y++)
                   {   QColor color(I->pixel(x,y));

                       image_matrix[y][x]=color.green();
                   }
        int start_W_overlay = W - n  ;
        int start_H_overlay = H - n  ;
        for (x=0;x<n;x++)
           for (y=0;y<n;y++)
           {
               matrix[x][y]=image_matrix[y+start_H_overlay][x+start_W_overlay];
           }

           for(int cycle = 0; cycle < cycleA; cycle++)
               matrix = back_arnold_change(matrix, n);

            for (x=0;x<n;x++)
                for (y=0;y<n;y++)
                {
                    I->setPixel(x+start_W_overlay,y+start_H_overlay,qRgb(matrix[x][y],matrix[x][y],matrix[x][y]));
                }
     }
    //Дешифруем нижнюю строку
    if(H_overlay){
        for (x=0;x<W;x++)
                   for (y=0;y<H;y++)
                   {   QColor color(I->pixel(x,y));

                       image_matrix[y][x]=color.green();
                   }
        for(int j=0; j<W_full; j++){
            int start_H_overlay = H - n  ;
            for (x=0;x<n;x++)
               for (y=0;y<n;y++)
               {
                   matrix[x][y]=image_matrix[y+start_H_overlay][x+j*n];
               }

               for(int cycle = 0; cycle < cycleA; cycle++)
                    matrix = back_arnold_change(matrix, n);

                for (x=0;x<n;x++)
                    for (y=0;y<n;y++)
                    {
                        I->setPixel(x+j*n,y+start_H_overlay,qRgb(matrix[x][y],matrix[x][y],matrix[x][y]));
                    }

        }
    }
    //Дешифруем крайний столбец
    if(W_overlay){
         for(int i=0; i<H_full; i++){
            for (x=0;x<W;x++)
                       for (y=0;y<H;y++)
                       {   QColor color(I->pixel(x,y));

                           image_matrix[y][x]=color.green();
                       }
            int start_W_overlay = W - n  ;
            for (x=0;x<n;x++)
               for (y=0;y<n;y++)
               {
                   matrix[x][y]=image_matrix[y+i*n][x+start_W_overlay];
               }

               for(int cycle = 0; cycle < cycleA; cycle++)
                    matrix = back_arnold_change(matrix, n);

                for (x=0;x<n;x++)
                    for (y=0;y<n;y++)
                    {
                        I->setPixel(x+start_W_overlay,y+i*n,qRgb(matrix[x][y],matrix[x][y],matrix[x][y]));
                    }
         }
    }

    //Дешифруем главную часть
    for (x=0;x<W;x++)
               for (y=0;y<H;y++)
               {   QColor color(I->pixel(x,y));

                   image_matrix[y][x]=color.green();
               }

    for(int i=0; i<H_full; i++){
        for(int j=0; j<W_full; j++){
            for (x=0;x<n;x++)
               for (y=0;y<n;y++)
               {
                   matrix[x][y]=image_matrix[y+i*n][x+j*n];
               }

               for(int cycle = 0; cycle < cycleA; cycle++)
                 matrix = back_arnold_change(matrix, n);

                for (x=0;x<n;x++)
                        for (y=0;y<n;y++)
                        {
                            I->setPixel(y+j*n,x+i*n,qRgb(matrix[y][x],matrix[y][x],matrix[y][x]));
                        }
        }
    }


    I->save("/home/denis/Pictures/Wallpapers/s3.png","png");
    QImage field = I->scaled(W,H,Qt::IgnoreAspectRatio);
    image->setPixmap(QPixmap::fromImage(field));
}

int** arnold_change(int **matrix, int n)
{
    int **copy = new int *[n];
    for(int i=0;i<n;i++)
        copy[i]=new int[n];
    int temp_x;
    int temp_y;


    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++){
            temp_x = (1*i+1*j)%n;
            temp_y = (1*i+2*j)%n;
            copy[temp_x][temp_y]=matrix[i][j];
        }
    return copy;
}

int** back_arnold_change(int **matrix, int n)
{
    int **copy = new int *[n];
    for(int i=0;i<n;i++)
        copy[i]=new int[n];
    int temp_x;
    int temp_y;

    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++){
            temp_x = (2*i+(-1)*j);
            if(temp_x < 0)
                temp_x = n + temp_x;
            temp_x %=n;
            temp_y = (-1*i+1*j);
            if(temp_y < 0)
                temp_y = n + temp_y;
            temp_y %=n;
            copy[temp_x][temp_y]=matrix[i][j];
        }
    return copy;
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
