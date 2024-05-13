#include <QProgressBar>
#include <cmath>
#include <QTimer>
#include <QVariant>

class MyQProgressBar: public QProgressBar
{
    Q_OBJECT 
    public:
        MyQProgressBar(QWidget *parent);

        
        
       
    public slots:
        void updateProgress();
        void startSinus();
        void setAmplitude(double amplitude);
        void setPhase(double phase);
        void Anima();
        void activaTimer(bool active);
        
        
        
       
    signals:
        void updateX();

    private:
        bool first;
        QTimer timer;
       
}; 
