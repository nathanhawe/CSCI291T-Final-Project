#include "_skybox.h"

_skyBox::_skyBox()
{
    //ctor
}

_skyBox::~_skyBox()
{
    //dtor
}
void _skyBox::skyBoxInit()
{
    glGenTextures(6,tex);

    tex[0] = textures->loadImages("images/forest/front.png");
    tex[1] = textures->loadImages("images/forest/back.png");
    tex[2] = textures->loadImages("images/forest/top.png");
    tex[3] = textures->loadImages("images/forest/bottom.png");
    tex[4] = textures->loadImages("images/forest/right.png");
    tex[5] = textures->loadImages("images/forest/left.png");

    xMin = yMin = 0.0;
    xMax = yMax = 1.0;

    rot.x= rot.y = rot.z = 0.0;
    pos.x= pos.y = 0.0;
    pos.z = -9;
    boxSize.x = boxSize.z =100.0;
    boxSize.y = 20.0;

}

void _skyBox::drawSkyBox()
{
   glPushMatrix();
     // handling translation and scaling
      glColor3f(1.0,1.0,1.0);
     glScalef(boxSize.x,boxSize.y,boxSize.z);
  //   glTranslatef(pos.x,pos.y, pos.z);
     //handling rotations
     glRotatef(rot.x,1,0,0);
     glRotatef(rot.y,0,1,0);
     glRotatef(rot.z,0,0,1);

   //Front Wall
     glBindTexture(GL_TEXTURE_2D,tex[0]);
     glBegin(GL_QUADS);
      glTexCoord2f(xMin,yMin); glVertex3f(-1.0,1.0,1.0);
      glTexCoord2f(xMax,yMin); glVertex3f( 1.0,1.0,1.0);
      glTexCoord2f(xMax,yMax); glVertex3f( 1.0,-1.0,1.0);
      glTexCoord2f(xMin,yMax); glVertex3f(-1.0,-1.0,1.0);
     glEnd();

   //back Wall
     glBindTexture(GL_TEXTURE_2D,tex[1]);
     glBegin(GL_QUADS);
      glTexCoord2f(xMin,yMax); glVertex3f(1.0, -1.0,-1.0);
      glTexCoord2f(xMax,yMax); glVertex3f(-1.0,-1.0,-1.0);
      glTexCoord2f(xMax,yMin); glVertex3f(-1.0, 1.0,-1.0);
      glTexCoord2f(xMin,yMin); glVertex3f( 1.0, 1.0,-1.0);
     glEnd();

    //top wall
     glBindTexture(GL_TEXTURE_2D,tex[2]);
     glBegin(GL_QUADS);
      glTexCoord2f(xMin,yMax); glVertex3f( 1.0,1.0,-1.0);
      glTexCoord2f(xMin,yMin); glVertex3f( 1.0,1.0,1.0);
      glTexCoord2f(xMax,yMin); glVertex3f( -1.0,1.0,1.0);
      glTexCoord2f(xMax,yMax); glVertex3f(-1.0,1.0,-1.0);
     glEnd();

      //bottom wall
     glBindTexture(GL_TEXTURE_2D,tex[3]);
     glBegin(GL_QUADS);
      glTexCoord2f(xMax,yMax); glVertex3f(-1.0,-1.0, 1.0);
      glTexCoord2f(xMin,yMax); glVertex3f( 1.0,-1.0, 1.0);
      glTexCoord2f(xMin,yMin); glVertex3f( 1.0,-1.0,-1.0);
      glTexCoord2f(xMax,yMin); glVertex3f(-1.0,-1.0,-1.0);
     glEnd();

     //left Wall
     glBindTexture(GL_TEXTURE_2D,tex[4]);
     glBegin(GL_QUADS);
      glTexCoord2f(xMin,yMin); glVertex3f(-1.0,1.0,-1.0);
      glTexCoord2f(xMax,yMin); glVertex3f(-1.0,1.0,1.0);
      glTexCoord2f(xMax,yMax); glVertex3f(-1.0,-1.0,1.0);
      glTexCoord2f(xMin,yMax); glVertex3f(-1.0,-1.0,-1.0);
     glEnd();

     //right wall
     glBindTexture(GL_TEXTURE_2D,tex[5]);
     glBegin(GL_QUADS);
      glTexCoord2f(xMax,yMin); glVertex3f( 1.0,1.0,-1.0);
      glTexCoord2f(xMax,yMax); glVertex3f( 1.0,-1.0,-1.0);
      glTexCoord2f(xMin,yMax); glVertex3f( 1.0,-1.0,1.0);
      glTexCoord2f(xMin,yMin); glVertex3f( 1.0,1.0,1.0);
     glEnd();

   glPopMatrix();
}
