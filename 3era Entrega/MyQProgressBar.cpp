#include "MyQProgressBar.h"

double p= 0;
double a = 0.5;
double temps = 0;

MyQProgressBar::MyQProgressBar(QWidget *parent)
    : QProgressBar(parent) {
    setOrientation(Qt::Vertical);
    setRange(-100, 100);
    setTextVisible(true);
    first = true;

   
    
}

void MyQProgressBar::Anima() {
    temps += 0.1;
    
    emit updateProgress();
}

void MyQProgressBar::activaTimer(bool active) {
     if(active){
        timer.start(100);

     }
     else timer.stop();
}

void MyQProgressBar::setAmplitude(double a1) {
    a = a1;
    
    emit updateX();
    emit updateProgress();
}

void MyQProgressBar::setPhase(double p1) {
    p = p1;
    
    emit updateX();
    emit updateProgress();
}

void MyQProgressBar::updateProgress() {
    int xPosValue = this->property("xPos").toInt();
    
   
    double value = 100 *a * std::sin(double(xPosValue) + p + temps);
    emit updateX();
    emit setValue(value);
}

void MyQProgressBar::startSinus() {
    a = 0.5;
    p = 0;
    if(first){
        connect(&timer, SIGNAL(timeout()),this,SLOT(Anima()));
        first = !first;
    }
    
    int xPosValue = this->property("xPos").toInt();
    double value = a * std::sin(xPosValue + p + temps);
    setValue(int(value * 100));
    emit updateX();
    
    
}