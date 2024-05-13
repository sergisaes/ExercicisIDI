#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersQuadrat();
}


void MyGLWidget::modelTransformQuadrat(glm::vec3 posicio, glm::vec3 escala)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,posicio);  
  TG = glm::scale(TG,escala);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCano(glm::vec3 posicio, glm::vec3 escala, float rot){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,posicio);  
  TG = glm::rotate(TG,rot,glm::vec3(0.0,0.0,1.0));
  TG = glm::translate(TG,glm::vec3(0.75,(float(0.125/2)),0.0));
  TG = glm::scale(TG,escala);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::modelTransformQuadratRodes(glm::vec3 posicio, glm::vec3 escala, glm::vec3 rotacio, float angle){

  glm::mat4 TG(1.0f);
  
  TG = glm::translate(TG,posicio);
  TG = glm::rotate(TG,angle,rotacio);
  TG = glm::translate(TG,glm::vec3(-0.05f,0.025f,0.f));
  
  TG = glm::scale(TG,escala);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);

}
void MyGLWidget::pintaCosTanc(){
    glUniform3f(colorLoc,verd.x,verd.y,verd.z);
    modelTransformQuadrat(glm::vec3(xcos1,0.125,0.0), glm::vec3(1.f,0.25f,1.f));
    glDrawArrays(GL_TRIANGLES, 0, 6); 
    
    modelTransformQuadrat(glm::vec3(xcos2,0.375f,0.f), glm::vec3(0.5f,0.25f,1.f));
    glDrawArrays(GL_TRIANGLES, 0, 6); 
   
}

void MyGLWidget::pintaCano(){
    glUniform3f(colorLoc,gris.x,gris.y,gris.z);
    modelTransformCano(glm::vec3(xtrabuco,0.25,0.),glm::vec3(0.75f,0.125f,1.f), anguloTrabuco);
    glDrawArrays(GL_TRIANGLES,0,6);
}

void MyGLWidget::pintaRodes(){
    float rot = 0.f;
    float ejex = 0.f;
    for(int j = 0; j < 4;++j){
      
      for(int i = 0; i < 12; ++i){

        if(i%2==0){
          glUniform3f(colorLoc,negre.x,negre.y,negre.z);
        }
        else glUniform3f(colorLoc,gris.x,gris.y,gris.z);

        modelTransformQuadratRodes(glm::vec3(xrodes +ejex,(-0.12f),0.f),glm::vec3(0.1f,0.05f,1.f),glm::vec3(0.f,0.0f,1.f),(rot+angRoda));
        //modelTransformQuadrat(glm::vec3(-0.375f,-0.12f,0.f),glm::vec3(0.1f,0.05f,1.f));
        glDrawArrays(GL_TRIANGLES,0,6);
        rot += float(M_PI/6.f);
      
    }
    ejex += 0.25f;
    }
    
}

void MyGLWidget::pintaTanc(){
  pintaCosTanc();
  pintaCano();
  pintaRodes();

}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Pintem un quadrat
  glBindVertexArray(VAOQuadrat);
  pintaTanc();   		
  glBindVertexArray(0);
    
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Left:
      anguloTrabuco += M_PI/18;
      
    	break;
    case Qt::Key_Right: 
      anguloTrabuco -=  M_PI/18;
      
    	break;
    case Qt::Key_A: 
      xcos1 -= 0.01;
      xcos2 -= 0.01;
      xtrabuco -= 0.01;
      xrodes -= 0.01;
      angRoda += M_PI/180;
    	break;
    case Qt::Key_D: 
      xcos1 += 0.01;
      xcos2 += 0.01;
      xtrabuco += 0.01;
      xrodes += 0.01;
      angRoda -= M_PI/180;
    	break;		
    default: event->ignore(); break;
  }
  if(anguloTrabuco > M_PI) anguloTrabuco = M_PI;
  if(anguloTrabuco < 0) anguloTrabuco = 0;
  update();
}

void MyGLWidget::creaBuffersQuadrat ()
{
  glm::vec3 Vertices[6];  // vèrtexs amb X, Y i Z
  
  // minX = -1.0
  // maxX = 0.0
  // minY = -1.0
  // maxY = 0.0
  
  Vertices[0] = glm::vec3(-1.0, 0.0, 0);
  Vertices[1] = glm::vec3( 0.0, -1.0, 0);
  Vertices[2] = glm::vec3( 0.0, 0.0, 0);
  Vertices[3] = glm::vec3( 0.0, -1.0, 0);
  Vertices[4] = glm::vec3( -1.0, 0.0, 0);
  Vertices[5] = glm::vec3( -1.0, -1.0, 0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar el quadrat
  glGenVertexArrays(1, &VAOQuadrat);
  glBindVertexArray(VAOQuadrat);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}


void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  
  // Obtenim els identificadors dels uniforms
  TGLoc = glGetUniformLocation(program->programId(), "TG"); 

  colorLoc = glGetUniformLocation(program->programId(), "color"); 
}
