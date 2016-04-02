#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLabel>
#include<QMouseEvent>
#include<QPainter>
#include <QPoint>
#include <QWidget>
#include<QPixmap>
#include<QDebug>
#include<QPropertyAnimation>
#include<Qimage>
#include<QDialog>
#include <QPushButton>
#include<QInputDialog>
#include <QtCore/qmath.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



};
class  MyObject:public QObject{
public:

    QPoint inter[20][50];
    int x0[50];
    int y0[50];
    int xt[20][50];
    int yt[20][50];
    int x1[50];
    int y1[50];
public:
    void getinitialsite(int x[20],int y[20]);
void getterminationsite(int x[20],int y[20]);
void linear_interpolation();
void Vlinear_interpolation();
void MyObject_clear();
//void MyObject_clear2();
void setM(int Mvalue);

};
class PaintedWidget:public QWidget{
    Q_OBJECT

    int count;
        QPoint pp[10][20];//?

        int x[10][20];
        int y[10][20];



public:
       explicit PaintedWidget(QWidget *parent = 0);

       void mousePressEvent(QMouseEvent *event) ;
        void clear();
        //void clear2();
        void clickupdate(){ update();}
        void paintEvent(QPaintEvent *);
        void paintEvent(QPoint p[],int X[],int Y[]);
        void letuspaint1();
        void letuspaint2();
        void submitB();
        void submitE();
        MyObject table;

public slots:
        void slottimedone();


};
static void New();
void New()
{ QDialog dialog;
    dialog.setWindowTitle(QObject::tr("new"));
    dialog.exec();
}
static QAction* newAction;



#endif // MAINWINDOW_H
