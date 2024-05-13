#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) 
{
}

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget()
{

}



void MyGLWidget::paintGL(){
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  viewTransform();
  // SnakeHead
  glBindVertexArray (VAO_models[SNAKE_HEAD]);
  SnakeHeadTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_HEAD].faces().size()*3);
  
  // SnakeBody
  glBindVertexArray (VAO_models[SNAKE_BODY]);
  for (unsigned int i = 0; i < bodyPos.size(); i++)
  {  
	  SnakeBodyTransform(bodyPos[i]);
	  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_BODY].faces().size()*3);
  }

  // SnakeTail
  glBindVertexArray (VAO_models[SNAKE_TAIL]);
  SnakeTailTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_TAIL].faces().size()*3);
  
  // Marble
  glBindVertexArray (VAO_models[MARBLE]);
  MarbleTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[MARBLE].faces().size()*3);

  // Pipe
  for(int i = 0; i < 30; ++i){
    glBindVertexArray (VAO_models[PIPE]);
    PipeTransform(glm::vec3(14.5,0,-14.5+i));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);

  }
  for(int i = 0; i < 30; ++i){
    glBindVertexArray (VAO_models[PIPE]);
    PipeTransform(glm::vec3(14.5-i,0, -14.5));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);

  }
  for(int i = 0; i < 30; ++i){
    glBindVertexArray (VAO_models[PIPE]);
    PipeTransform(glm::vec3(-14.5,0,14.5-i));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);

  }
  for(int i = 0; i < 30; ++i){
    glBindVertexArray (VAO_models[PIPE]);
    PipeTransform(glm::vec3(-14.5+i,0, 14.5));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);

  }
  
  
  // Terra
  glBindVertexArray (VAO_Terra);
  TerraTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void MyGLWidget::TerraTransform(){
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG,glm::vec3(3.0,3.0,3.0));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::SnakeHeadTransform ()
{
  glm::mat4 TG(1.0f);  
  TG = glm::translate(TG, headPos);
  TG = glm::rotate(TG,glm::radians((headAngle-90)),glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_HEAD]));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_HEAD]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::SnakeBodyTransform (glm::vec3 pos)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_BODY]));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_BODY]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::SnakeTailTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, tailPos);
  TG = glm::rotate(TG,glm::radians(tailAngle), glm::vec3(0.0,1.0,0.0));
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_TAIL]));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_TAIL]);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::MarbleTransform ()
{

  float esc = 0.75/1.5;
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, marblePos);
  TG = glm::scale(TG, glm::vec3(esc,esc,esc));
  TG = glm::scale(TG, glm::vec3(escalaModels[MARBLE]));
  TG = glm::translate(TG, -centreBaseModels[MARBLE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::PipeTransform (glm::vec3 pos)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::scale(TG, glm::vec3(1.0,2.0,1.0));
  TG = glm::scale(TG, glm::vec3(escalaModels[PIPE]));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::iniEscena()
{
  radiEscena = sqrt((30*30)+(30*30)+(3*3))/2.0;
  centreEscena = glm::vec3(0,0,0);

  headPos = glm::vec3(0,0,0);
  headAngle = 0;
  bodyPos.clear();	    
  bodyPos.push_back(glm::vec3(-1,0,0));    	
  tailPos = glm::vec3(-2,0,0);
  tailAngle = 0;

  marblePos = glm::vec3(10, 0, 0);

  direction = glm::vec3(1,0,0);
}

void MyGLWidget::iniCamera(){

  obs = glm::vec3(0, radiEscena*2, 0);
  vrp = glm::vec3(0, 1.5, 0);
  up = glm::vec3(0, 0, -1);
  fovini = float(M_PI)/3.0f;
  fov = fovini;
  znear =  radiEscena;
  zfar  = 3*radiEscena;

  teta = glm::radians(45.0);
  pi = glm::radians(0.0);

  prespective = true;
  viewTransform();
}


void MyGLWidget::viewTransform ()
{
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);
  if(prespective){
    View = glm::translate(View, glm::vec3(0.0,0.0,-2*radiEscena));
    View = glm::rotate(View, teta, glm::vec3(1.0,0.0,0.0));
    View = glm::rotate(View, -pi, glm::vec3(0.0,1.0,0.0));
    View = glm::translate(View, -vrp);

  }
  else{
    View = glm::lookAt (obs, vrp, up);

  }
  
  
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  if(prespective){
    if(ra<1) fov = 2*atan(tan(fovini/2.0f)/ra);
    Proj = glm::perspective (fov, ra, znear, zfar);
  }
  else{
    if(ra<1) Proj = glm::ortho(-20.0f,20.0f,-20.0f/ra, 20.0f/ra, znear, zfar);
    else Proj = glm::ortho(-20.0f*ra,20.0f*ra,-20.0f, 20.0f, znear, zfar);
  }
  
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

bool MyGLWidget::checkPosition(glm::vec3 pos)
{
	bool valid = true;

  if(pos == headPos or pos == tailPos) valid = false;

  if (valid){
    int mida = bodyPos.size();
    for (int i = 0; i < mida; ++i){
      if (pos == bodyPos[i]) return false;
    }
    
  if(pos.x >=  13.5 or pos.x <= -13.5) valid = false;
   if(pos.z >=  13.5 or pos.z <= -13.5) valid = false;
  }
  


	return valid; 
}
void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { 
      if(direction.z != 1) {
        direction = glm::vec3(0.0,0.0,-1.0);
        updateSnakeGame();
      }
      break;
    }
    case Qt::Key_Down: { 
      if(direction.z != -1) {
        direction = glm::vec3(0.0,0.0,1.0);
        updateSnakeGame();
      }
      break;
        }
    case Qt::Key_Left: { 
      if(direction.x != 1) {
        direction = glm::vec3(-1.0,0.0,0.0);
        updateSnakeGame();
      }
      break;
        }  
    case Qt::Key_Right: { 
      if(direction.x != -1) {
        direction = glm::vec3(1.0,0.0,0.0);
        updateSnakeGame();
      }
      break;
        }
    case Qt::Key_C: { 
          prespective = !prespective;
          projectTransform();
      break;
        }           
    case Qt::Key_R: { 
      iniEscena();
      iniCamera();
      projectTransform();
      viewTransform();
      break;
        }  
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if(DoingInteractive && prespective){
    pi += (xClick - e->x())*factorAngleX;
    teta -= (yClick - e->y())*factorAngleY;
  }
  xClick = e->x();
  yClick = e->y();

  update ();
}