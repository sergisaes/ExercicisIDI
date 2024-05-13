#include "LL2GLWidget.h"

#include <vector>

#include <QTimer>

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();

  protected:
  
    virtual void keyPressEvent (QKeyEvent *event);
    virtual void TerraTransform();
    virtual void paintGL();
    virtual void iniCamera ();
    
    virtual void viewTransform();
    virtual void projectTransform();


    virtual void SnakeHeadTransform();
    virtual void SnakeBodyTransform(glm::vec3 pos);
    virtual void SnakeTailTransform();
    virtual void MarbleTransform();
    virtual void PipeTransform(glm::vec3 pos);
    virtual void iniEscena();
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);
  

    // funció per comprovar si una nova posició de la serp és vàlida
    virtual bool checkPosition(glm::vec3 pos);

    float teta, pi;
    bool prespective;
    float fovini;

  public slots:


  private:
  
    int printOglError(const char file[], int line, const char func[]);
   
};
