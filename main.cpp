 #include "mainwindow.h"
#include <QApplication>
#include<QPushButton>
#include<QGridLayout>
#include<QVBoxLayout>
#include<QMenuBar>
#include<QMenu>
#include<QSlider>
#include<QSpinBox>
#include<QTimer>
#include<qDebug>
#include<QTimer>


extern int M;
extern int count;
extern int time;
extern int flag;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QWidget*widget= new QWidget();
    PaintedWidget view;
   newAction=new QAction (QIcon(":/image"),QObject::tr("&Open..."),&view);
   QObject::connect(newAction ,&QAction::triggered,&view,&New);
   QMenu*file=w.menuBar()->addMenu(QObject::tr("&File"));
   file->addAction(newAction);
   QPushButton new_btn(&view);
   new_btn.setText("BeginWith");
      QObject::connect(&new_btn,&QPushButton::clicked,&view,&PaintedWidget::clickupdate);
      QObject::connect(&new_btn,&QPushButton::clicked,&view,&PaintedWidget::clear);
   QPushButton end_btn(&view);
   end_btn.setText("EndWith");
   MyObject table;//实例化
   QPushButton finish_btn(&view);
   finish_btn.setText("Finish");

   //QObject::connect(&finish_btn,&QPushButton::clicked,&table,&MyObject::MyObject_clear);
   QObject::connect(&finish_btn,&QPushButton::clicked,&view,&PaintedWidget::clickupdate);
   QObject::connect(&finish_btn,&QPushButton::clicked,&view,&PaintedWidget::clear);
   //QObject::connect(&finish_btn,&QPushButton::clicked,&view,&PaintedWidget::finishall);



   QObject::connect(&end_btn,&QPushButton::clicked,&view,&PaintedWidget::submitB);
   QObject::connect(&end_btn,&QPushButton::clicked,&view,&PaintedWidget::clear);
   QObject::connect(&end_btn,&QPushButton::clicked,&view,&PaintedWidget::clickupdate);


   QPushButton Line_btn(&view);
      Line_btn.setText("LinerInterpolation");
   QPushButton Vline_btn(&view);
      Vline_btn.setText("VectorLinerInterpolation");

 //触发绘制
   QObject::connect(&Line_btn,&QPushButton::clicked,&view,&PaintedWidget::submitE);//获取最终
QObject::connect(&Vline_btn,&QPushButton::clicked,&view,&PaintedWidget::submitE);
QObject::connect(&Line_btn,&QPushButton::clicked,&view,&PaintedWidget::letuspaint1);
QObject::connect(&Vline_btn,&QPushButton::clicked,&view,&PaintedWidget::letuspaint2);
//她在这里传递的是什么值*/void MyObject::linear_interpolation()

   QSlider *sliderM = new QSlider(Qt::Horizontal,&view);  // slider
       sliderM->setRange(0,10);
       sliderM->setValue(5);

        QObject::connect(sliderM,&QSlider::valueChanged,&table,&MyObject::setM);
        QLabel *labelM = new QLabel("Midnumber");
        QSpinBox *boxM = new QSpinBox(&view);
          boxM->setRange(0,10);
         boxM->setValue(5);
        QObject::connect(sliderM,&QSlider::valueChanged,boxM,&QSpinBox::setValue);
            void (QSpinBox::*spinBoxSignal)(int) = &QSpinBox::valueChanged;
            QObject::connect(boxM,spinBoxSignal,sliderM,&QSlider::setValue);
            QTimer*timer = new QTimer(&view);


            QObject::connect(timer,SIGNAL(timeout()),&view,SLOT(slottimedone()));
            timer->start(1000);


QGridLayout*tlayout=new QGridLayout;
   tlayout->addWidget(&new_btn,0,0);
   tlayout->addWidget(&end_btn,0,1);
   tlayout->addWidget(&Line_btn,0,2);
   tlayout->addWidget(&Vline_btn,0,3);
    tlayout->addWidget(&finish_btn,0,4);
   tlayout->addWidget(labelM,1,0);
   tlayout->addWidget(sliderM,1,1);
   tlayout->addWidget(boxM,1,2);
   QVBoxLayout *mainlayout = new QVBoxLayout();
       mainlayout->addWidget(&view);
       mainlayout->addLayout(tlayout);
       mainlayout->setStretchFactor(&view,3);
       mainlayout->setStretchFactor(tlayout,1);

       widget->setLayout(mainlayout);
       w.setCentralWidget(widget);




    w.show(); //if (testTimer->isActive() )

       //Timer->stop();
    return a.exec();
}
