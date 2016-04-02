#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
int M=7;
int count=0;
int time=-999;
int allcount=0;
int flag=0;

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent)//QWidget(parent)
{
    //paintArea = new PaintedWidget(parent);
    resize(800,600);
}

MainWindow::~MainWindow()
{
    }
PaintedWidget::PaintedWidget(QWidget *parent)
   : QWidget(parent){

    resize(800,600);
    setWindowTitle(tr("Path Control"));

    this->setMouseTracking(true);
    }




void PaintedWidget::paintEvent(QPaintEvent *)//绘制函数
{

    QPainter painter(this);


     if(count >= 0){
   if(time<=0)
   {for(int j=0;j<allcount-1;j++)//设置笔刷
       {
           QPen redpen;
           redpen.setColor(Qt::red);
           painter.setPen(redpen);
           painter.drawLine(x[0][j+1],y[0][j+1],x[0][j],y[0][j]);


           painter.setBrush(Qt::blue);

           painter.drawEllipse(pp[0][j],3,3);//

   }}
   else if (time<M){   //如果绘制开始，利用定时器触发绘制事件

       //qDebug()<<"time"<<time;


    for(int j=0;j<allcount-1;j++)
    {
        QPen redpen;
        redpen.setColor(Qt::red);
        painter.setPen(redpen);
        painter.drawLine(x[time][j+1],y[time][j+1],x[time][j],y[time][j]);//链接线


        painter.setBrush(Qt::blue);

        painter.drawEllipse(pp[time][j],3,3);//绘制点
        //qDebug()<<"intersaction success"<<x[time][j]<<y[time][j];


}}}}



void PaintedWidget::mousePressEvent(QMouseEvent *event)  //重要函数
{
    setCursor(Qt::PointingHandCursor);    //成员变量？
  // qDebug()<<event->pos()<<"mouse event";
    QPoint current = event->pos();    //event ?have any problem ??
    // pick up the poings clicked and record them in the array
    pp[0][count] = event->pos();
    x[0][count] = pp[0][count].x();
    y[0][count] = pp[0][count].y();

    count++;
    //qDebug()<<x[0];
    update();// update  重要函数
   // qDebug()<<"count"<<count;
    allcount=count;

}

void PaintedWidget::clear()   //重要函数
{


    for(int i=0;i<50;i++)
    {for(int j=0;j<20;j++){
        x[j][i] = y[j][i] = 0;
        pp[j][i].setX(0);  // pp  要作为成员变量
        pp[j][i].setY(0);

    }}
    //table.MyObject_clear1();// C 程序中是否需要 clearSpline

    //update();
}

void PaintedWidget::submitB(){

    table.getinitialsite(x[0],y[0]);

   count=0;

}
void PaintedWidget::submitE(){

    table.getterminationsite(x[0],y[0]);

}
void PaintedWidget::letuspaint1(){
    time=0;


flag=0;

}
void PaintedWidget::letuspaint2(){
    time=0;
flag=1;



}

void MyObject::getinitialsite(int x[],int y[])
{  int i;


    for(i=0;i<=allcount;i++){

        x0[i]=x[i];
        y0[i]=y[i];
       // qDebug()<<"initialiaze success"<<x0[i]<<y0[i];
    }


}

void MyObject::getterminationsite(int x[],int y[])
{  int i;
   // qDebug()<<"termination successx1[]y1[]count "<<count<<"allcount"<<allcount;

    for(i=0;i<=allcount;i++){
        x1[i]=x[i];
        y1[i]=y[i];
        //qDebug()<<"termination successx1[]y1[]"<<x1[i]<<y1[i];
    }
 //qDebug()<<"termination successx1[]y1[]";
}
void MyObject::linear_interpolation()
{
    //qDebug()<<"liner interaption success xt[time][i] yt[time][i]";

  if(time>0||time<=M){
        float t=(float)time/M;
        //这里t是时间，其区间在[0，1]范围内
        for (int i=0;i<allcount;i++){
            xt[time][i]=(1-t)*x0[i]+t*x1[i] ;
        yt[time][i]=(1-t)*y0[i]+t*y1[i] ;
        inter[time][i].setX(xt[time][i]);
        inter[time][i].setY(yt[time][i]);
        //qDebug()<<"liner interaption success xt[time][i] yt[time][i]"<<xt[time][i]<<yt[time][i];
       // qDebug()<<"[time][in linear]"<<time;
    }}
 //qDebug()<<"liner interaption success xt[time][i] yt[time][i]";
}

void MyObject::Vlinear_interpolation()

{    // qDebug()<<"Vector liner interaption success xt[time][i] yt[time][i]";


     if(time>0||time<=M){
         int pa,pa1;
          double t;
          int i,j;
          double xf,yf,xl,yl;
          double a1[20];
          double a0[20];
          double at[20];
          double p0[20];
          double pt[20];
          double p1[20];
          double Pi=3.14;
          i=0;
          for(j=1;j<allcount;j++){
            xf=x0[j]-x0[i];
            yf=y0[j]-y0[i];
            xl=x1[j]-x1[i];
            yl=y1[j]-y1[i];
            //qDebug()<<"Vector liner interaption success xf"<<xf<<"xl"<<xl<<"yf"<<yf<<"yl"<<yl;
            p0[i]=qSqrt(qPow(yf,2)+qPow(xf,2));//r
            p1[i]=qSqrt(qPow(yl,2)+qPow(xl,2));//r
            if(xf<0&&yf>0)a0[i]=qAtan(yf/xf)+Pi;//?
            if(xf<0&&yf<0)a0[i]=qAtan(yf/xf)-Pi;
            if(xf<0&&yf==0)a0[i]=Pi;
            if(xf==0&&yf>0)a0[i]=Pi/2;
            if(xf==0&&yf<0)a0[i]=-Pi/2;
            if(xf>0)a0[i]=qAtan(yf/xf);
            if(xl<0&&yl>0)a1[i]=qAtan(yl/xl)+Pi;
            if(xl<0&&yl<0)a1[i]=qAtan(yl/xl)-Pi;
            if(xl<0&&yl==0)a1[i]=Pi;
            if(xl==0&&yl>0)a1[i]=Pi/2;
            if(xl==0&&yl<0)a1[i]=-Pi/2;//a1
            if(xl>0)a1[i]=qAtan(yl/xl);
            //qDebug()<<"Vector liner interaption success a0[1]" <<a0[i]<<"a1[i]"<<a1[i];

             i++;
          }
           t=(float)time/M;
           //qDebug()<<"t"<<t;

             for(pa =0;pa<allcount-1; pa++) {
                  pt[pa]=(1-t)*p0[pa]+ t*p1[pa];
                 if(a0[pa]-a1[pa]>Pi){
                    at[pa]=(1-t)*a0[pa]+ t*(a1[pa]+2*Pi);//at[

                 }
                 else{
                 if(a0[pa]-a1[pa]<-Pi){
                    at[pa]=(1-t)*a0[pa]+ t*(a1[pa]-2*Pi);
                 }
                 else at[pa]=(1-t)*a0[pa]+ t*a1[pa];}
                // qDebug()<<"Vector liner interaption success at[1]" <<at[pa];
               }
             xt[time][0]=(1-t)*x0[0]+ t*x1[0];
             yt[time][0]=(1-t)*y0[0]+ t*y1[0];
             for(pa1=1;pa1<allcount;pa1++){
                 xt[time][pa1]=xt[time][pa1-1]+pt[pa1-1]*qCos(at[pa1-1]);
                 yt[time][pa1]=yt[time][pa1-1]+pt[pa1-1]*qSin(at[pa1-1]);
            // qDebug()<<"Vector liner interaption success xt[time][i] yt[time][i]"<<xt[time][pa1]<<yt[time][pa1];
             }
}}

void MyObject::setM (int Mvalue)
{
    M = Mvalue;
    //qDebug()<<"m="<<M;
}

void MyObject::MyObject_clear(){
    count=0;
    time=-999;
    for(int i=0;i<50;i++)
    {  x0[i]=0;
        y0[i]=0;
        x1[i]=0;
        y1[i]=0;}
       for(int j=0;j<20;j++){
           for(int  i=0;i<50;i++)
           {xt[j][i]=0;
        yt[j][i]=0;
        inter[j][i].setX(0);
        inter[j][i].setY(0);}
    }}/*
void MyObject::MyObject_clear1(){
    count=0;
    for(int i=0;i<50;i++)
    {  x1[i]=0;
        y1[i]=0;
    }
       for(int j=0;j<20;j++){
           for(int  i=0;i<50;i++)
           {xt[j][i]=0;
        yt[j][i]=0;
        inter[j][i].setX(0);
        inter[j][i].setY(0);}
    }}*/
 void PaintedWidget::slottimedone(){
                                        update();
                                    time=time+1;


                       if(time>0&&time<M){
                           if(flag==0){table.linear_interpolation();}
                           else{table.Vlinear_interpolation();}
                                    for (int i=0;i<=count;i++){
                                       // qDebug()<<"count in similar"<<count;

                                    x[time][i]=table.xt[time][i];
                                    y[time][i]=table.yt[time][i];
                                    //qDebug()<<"submit similiar"<<x[time][i]<<table.xt[time][i];

                                   }  }}
