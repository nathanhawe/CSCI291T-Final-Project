#include "_fort.h"

_fort::_fort()
{

}

_fort::~_fort()
{

}




void _fort::initFortassets()
{
    GLUquadricObj *quadric, *minercylinder;
    texid1=textureLoader1->loadImages("fortassets/front.bmp");
    texid2=textureLoader1->loadImages("fortassets/left.bmp");
    texid3=textureLoader1->loadImages("fortassets/right.bmp");
    texid4=textureLoader1->loadImages("fortassets/minar_tiles.bmp");
    texid5=textureLoader1->loadImages("fortassets/minar_gliph.bmp");
    texid6=textureLoader1->loadImages("fortassets/dome_gliph.bmp");
    texid7=textureLoader1->loadImages("fortassets/black_stone.bmp");
    texid8=textureLoader1->loadImages("fortassets/big_dome.bmp");
    texid9=textureLoader1->loadImages("fortassets/tree.bmp");
    texid10=textureLoader1->loadImages("fortassets/grass.bmp");
    texid11=textureLoader1->loadImages("fortassets/water.bmp");
    texid12=textureLoader1->loadImages("fortassets/brick.bmp");
    texid13=textureLoader1->loadImages("fortassets/tiles.bmp");
    texid14=textureLoader1->loadImages("fortassets/small_gliphs.bmp");
    texid15=textureLoader1->loadImages("fortassets/red_stone.bmp");
    texid16=textureLoader1->loadImages("fortassets/octagon_side.bmp");
}


void _fort::drawfort()
{

     //glEnable(GL_TEXTURE_2D);{
         GLUquadric* quadric = gluNewQuadric();
        if (!quadric) {
            std::cerr << "Failed to create GLUquadric!" << std::endl;
            return;
        }
        gluQuadricNormals(quadric, GLU_SMOOTH);
        gluQuadricTexture(quadric, GL_TRUE);


         // Wall 1
         glPushMatrix();{
             glRotatef(0,0,0,0);
             glTranslatef(0,0,0);

             //////////////////////////////////////////////////////////////////////////////////////////////////////
             /////////////////////////////////Design of the Wall //////////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D,texid1);
             glEnable(GL_BLEND);
             glColor4f(1,1,1,1);
             glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
             glAlphaFunc(GL_GREATER,.1);
             glEnable( GL_ALPHA_TEST );

             glPushMatrix(); {                                                       // middle wall
                 glNormal3f(0,0,1);
                 glBegin(GL_POLYGON);{
                     glTexCoord2f(0,0);  glVertex3f(-100, 0, 0);
                     glTexCoord2f(0,1);  glVertex3f(-100, 0, 100);
                     glTexCoord2f(1,1); glVertex3f(100, 0, 100);
                     glTexCoord2f(1,0); glVertex3f(100, 0, 0);
                 }glEnd();
             } glPopMatrix();

             glBindTexture(GL_TEXTURE_2D,texid2);
             glPushMatrix(); {                                                       // left wall
                 glNormal3f(0,0,1);
                 glTranslatef(250,0,0);
                 glBegin(GL_POLYGON);{
                     glTexCoord2f(0,0);  glVertex3f(-150, 0, 0);
                     glTexCoord2f(0,1);  glVertex3f(-150, 0, 90);
                     glTexCoord2f(1,1); glVertex3f(-100, 0, 90);
                     glTexCoord2f(1,0); glVertex3f(-100, 0, 0);
                 }glEnd();
             } glPopMatrix();

             glBindTexture(GL_TEXTURE_2D,texid3);
             glPushMatrix(); {                                                       // right wall
                 glNormal3f(0,0,1);
                 glBegin(GL_POLYGON);{
                     glTexCoord2f(0,0); glVertex3f(-150, 0, 0);
                     glTexCoord2f(0,1); glVertex3f(-150, 0, 90);
                     glTexCoord2f(1,1); glVertex3f(-100, 0, 90);
                     glTexCoord2f(1,0); glVertex3f(-100, 0, 0);
                 }glEnd();
             } glPopMatrix();

         }glPopMatrix();

         // Wall 2

         glPushMatrix();{
             glRotatef(90,0,0,1);
             glTranslatef(-167,170,0);

             //////////////////////////////////////////////////////////////////////////////////////////////////////
             /////////////////////////////////Design of the Wall //////////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D,texid1);
             glEnable(GL_BLEND);
             glColor4f(1,1,1,1);
             glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
             glAlphaFunc(GL_GREATER,.1);
             glEnable( GL_ALPHA_TEST );

             glPushMatrix(); {                                                       // middle wall
                 glNormal3f(0,0,1);
                 glBegin(GL_POLYGON);{
                     glTexCoord2f(0,0);  glVertex3f(-100, 0, 0);
                     glTexCoord2f(0,1);  glVertex3f(-100, 0, 100);
                     glTexCoord2f(1,1); glVertex3f(100, 0, 100);
                     glTexCoord2f(1,0); glVertex3f(100, 0, 0);
                 }glEnd();
             } glPopMatrix();

             glBindTexture(GL_TEXTURE_2D,texid2);
             glPushMatrix(); {                                                       // left wall
                 glNormal3f(0,0,1);
                 glTranslatef(250,0,0);
                 glBegin(GL_POLYGON);{
                     glTexCoord2f(0,0);  glVertex3f(-150, 0, 0);
                     glTexCoord2f(0,1);  glVertex3f(-150, 0, 90);
                     glTexCoord2f(1,1); glVertex3f(-100, 0, 90);
                     glTexCoord2f(1,0); glVertex3f(-100, 0, 0);
                 }glEnd();
             } glPopMatrix();

             glBindTexture(GL_TEXTURE_2D,texid3);
             glPushMatrix(); {                                                       // right wall
                 glNormal3f(0,0,1);
                 glBegin(GL_POLYGON);{
                     glTexCoord2f(0,0);  glVertex3f(-150, 0, 0);
                     glTexCoord2f(0,1);  glVertex3f(-150, 0, 90);
                     glTexCoord2f(1,1); glVertex3f(-100, 0, 90);
                     glTexCoord2f(1,0); glVertex3f(-100, 0, 0);
                 }glEnd();
             } glPopMatrix();

         }glPopMatrix();


         // Wall 3
         glPushMatrix();{
             glRotatef(90,0,0,1);
             glTranslatef(-167,-170,0);

             //////////////////////////////////////////////////////////////////////////////////////////////////////
             /////////////////////////////////Design of the Wall //////////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D,texid1);
             glEnable(GL_BLEND);
             glColor4f(1,1,1,1);
             glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
             glAlphaFunc(GL_GREATER,.1);
             glEnable( GL_ALPHA_TEST );

             glPushMatrix(); {                                                       // middle wall
                 glNormal3f(0,0,1);
                 glBegin(GL_POLYGON);{
                     glTexCoord2f(0,0);  glVertex3f(-100, 0, 0);
                     glTexCoord2f(0,1);  glVertex3f(-100, 0, 100);
                     glTexCoord2f(1,1); glVertex3f(100, 0, 100);
                     glTexCoord2f(1,0); glVertex3f(100, 0, 0);
                 }glEnd();
             } glPopMatrix();

             glBindTexture(GL_TEXTURE_2D,texid2);
             glPushMatrix(); {                                                       // left wall
                 glNormal3f(0,0,1);
                 glTranslatef(250,0,0);
                 glBegin(GL_POLYGON);{
                     glTexCoord2f(0,0);  glVertex3f(-150, 0, 0);
                     glTexCoord2f(0,1);  glVertex3f(-150, 0, 90);
                     glTexCoord2f(1,1); glVertex3f(-100, 0, 90);
                     glTexCoord2f(1,0); glVertex3f(-100, 0, 0);
                 }glEnd();
             } glPopMatrix();

             glBindTexture(GL_TEXTURE_2D,texid3);
             glPushMatrix(); {                                                       // right wall
                 glNormal3f(0,0,1);
                 glBegin(GL_POLYGON);{
                     glTexCoord2f(0,0);  glVertex3f(-150, 0, 0);
                     glTexCoord2f(0,1);  glVertex3f(-150, 0, 90);
                     glTexCoord2f(1,1); glVertex3f(-100, 0, 90);
                     glTexCoord2f(1,0); glVertex3f(-100, 0, 0);
                 }glEnd();
             } glPopMatrix();

         }glPopMatrix();

         // Wall 4
         glPushMatrix();{
             glRotatef(0,0,0,0);
             glTranslatef(0,-330,0);

             //////////////////////////////////////////////////////////////////////////////////////////////////////
             /////////////////////////////////Design of the Wall //////////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D,texid1);
             glEnable(GL_BLEND);
             glColor4f(1,1,1,1);
             glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
             glAlphaFunc(GL_GREATER,.1);
             glEnable( GL_ALPHA_TEST );

             glPushMatrix(); {                                                       // middle wall
                 glNormal3f(0,0,1);
                 glBegin(GL_POLYGON);{
                     glTexCoord2f(0,0);  glVertex3f(-100, 0, 0);
                     glTexCoord2f(0,1);  glVertex3f(-100, 0, 100);
                     glTexCoord2f(1,1); glVertex3f(100, 0, 100);
                     glTexCoord2f(1,0); glVertex3f(100, 0, 0);
                 }glEnd();
             } glPopMatrix();

             glBindTexture(GL_TEXTURE_2D,texid2);
             glPushMatrix(); {                                                       // left wall
                 glNormal3f(0,0,1);
                 glTranslatef(250,0,0);
                 glBegin(GL_POLYGON);{
                     glTexCoord2f(0,0);  glVertex3f(-150, 0, 0);
                     glTexCoord2f(0,1);  glVertex3f(-150, 0, 90);
                     glTexCoord2f(1,1); glVertex3f(-100, 0, 90);
                     glTexCoord2f(1,0); glVertex3f(-100, 0, 0);
                 }glEnd();
             } glPopMatrix();

             glBindTexture(GL_TEXTURE_2D,texid3);
             glPushMatrix(); {                                                       // right wall
                 glNormal3f(0,0,1);
                 glBegin(GL_POLYGON);{
                     glTexCoord2f(0,0);  glVertex3f(-150, 0, 0);
                     glTexCoord2f(0,1);  glVertex3f(-150, 0, 90);
                     glTexCoord2f(1,1); glVertex3f(-100, 0, 90);
                     glTexCoord2f(1,0); glVertex3f(-100, 0, 0);
                 }glEnd();
             } glPopMatrix();

         }glPopMatrix();



         // Ceiling
         glPushMatrix();{
             glBindTexture(GL_TEXTURE_2D,texid15);
             glTranslatef(0,0,0);
             glBegin(GL_POLYGON);{
                     glTexCoord2f(0,0);  glVertex3f(-170, 0, 0);
                     glTexCoord2f(0,10);  glVertex3f(-170, -330, 0);
                     glTexCoord2f(10,10); glVertex3f(170, -330, 0);
                     glTexCoord2f(10,0); glVertex3f(170, 0, 0);
             }glEnd();

             glPushMatrix();{
                 glPushMatrix();{
                     glTranslatef(90,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(70,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(50,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(30,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(10,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(-10,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(-30,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(-50,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(-70,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(-90,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
             }glPopMatrix();

             glPushMatrix();{
                 glPushMatrix();{
                     glTranslatef(0,-308,0);
                     glTranslatef(90,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(0,-308,0);
                     glTranslatef(70,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(0,-308,0);
                     glTranslatef(50,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(0,-308,0);
                     glTranslatef(30,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(0,-308,0);
                     glTranslatef(10,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(0,-308,0);
                     glTranslatef(-10,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(0,-308,0);
                     glTranslatef(-30,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(0,-308,0);
                     glTranslatef(-50,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(0,-308,0);
                     glTranslatef(-70,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glTranslatef(0,-308,0);
                     glTranslatef(-90,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
             }glPopMatrix();

             glPushMatrix();{
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,-148,0);
                     glTranslatef(90,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,-148,0);
                     glTranslatef(70,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,-148,0);
                     glTranslatef(50,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,-148,0);
                     glTranslatef(30,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,-148,0);
                     glTranslatef(10,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,-148,0);
                     glTranslatef(-10,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,-148,0);
                     glTranslatef(-30,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,-148,0);
                     glTranslatef(-50,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,-148,0);
                     glTranslatef(-70,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,-148,0);
                     glTranslatef(-90,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
             }glPopMatrix();

             glPushMatrix();{
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,170,0);
                     glTranslatef(90,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,170,0);
                     glTranslatef(70,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,170,0);
                     glTranslatef(50,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,170,0);
                     glTranslatef(30,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,170,0);
                     glTranslatef(10,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,170,0);
                     glTranslatef(-10,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,170,0);
                     glTranslatef(-30,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,170,0);
                     glTranslatef(-50,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,170,0);
                     glTranslatef(-70,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
                 glPushMatrix();{
                     glRotatef(90,0,0,1);
                     glTranslatef(-167,170,0);
                     glTranslatef(-90,-11,0);
                     glutSolidCube(20);
                 }glPopMatrix();
             }glPopMatrix();

         }glPopMatrix();

        // glColor4f(1,1,1,1);


         // Minar 1
         glPushMatrix();{

             glTranslatef(170,0,-50);

             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
             ////////////////////////////////Minar Body Design//////////////////////////////////////////////////////////////
             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////



             glBindTexture(GL_TEXTURE_2D, texid4);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // lower part of the minar
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,0);
                 gluCylinder(quadric, 20, 20, 100, 6, 20);
             }glPopMatrix();

             glBindTexture(GL_TEXTURE_2D, texid5);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // minar gliphs
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,100);
                 gluCylinder(quadric, 20, 20, 50, 6, 20);
             }glPopMatrix();

             glPushMatrix();{                                           // minar carnish
                 glColor3f(0.99,0.62,0.55);
                 glTranslatef(0,0,10);
                 glScalef(2, 2, 0.1);
		         glutSolidSphere(20, 6, 3);
             }glPopMatrix();


             //////////////////////////////////////////////////////////////////////////////////////////////////
             ///////////////////////////////// Minar Dome Design///////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid14);
             glPushMatrix();{                                                        // lower part of the dome
                 glTranslatef(0,0,150);
                 gluCylinder(quadric, 20, 20, 15, 6, 20);
             }glPopMatrix();

             glBindTexture(GL_TEXTURE_2D, texid6);
             glPushMatrix();{                                                        // main dome
                 glColor3f(0.99,0.62,0.55);
                 glTranslatef(0,0,0);
                 glScalef(2, 2, 1.8);
		         gluSphere(quadric, 10, 20, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // cone above the dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,10,-25);
		         glutSolidCone(10,10,20,20);
             }glPopMatrix();

             gluQuadricTexture(quadric, GLU_FALSE);
             glPushMatrix();{                                                        // solid stick above the cone
                 glTranslatef(0,0,-50);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 1, 1, 25, 4, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // sphere in the middle of the stick
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,-42);
                 glScalef(2, 2, 2);
		         glutSolidSphere(2, 20, 20);
             }glPopMatrix();

         }glPopMatrix();

         // Minar 2
         glPushMatrix();{

             glTranslatef(-170,0,-50);

             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
             ////////////////////////////////Minar Body Design//////////////////////////////////////////////////////////////
             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid4);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // lower part of the minar
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,0);
                 gluCylinder(quadric, 20, 20, 100, 6, 20);
             }glPopMatrix();

             glBindTexture(GL_TEXTURE_2D, texid5);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // minar gliphs
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,100);
                 gluCylinder(quadric, 20, 20, 50, 6, 20);
             }glPopMatrix();

             glPushMatrix();{                                           // minar carnish
                 glColor3f(0.99,0.62,0.55);
                 glTranslatef(0,0,10);
                 glScalef(2, 2, 0.1);
		         glutSolidSphere(20, 6, 3);
             }glPopMatrix();


             //////////////////////////////////////////////////////////////////////////////////////////////////
             ///////////////////////////////// Minar Dome Design///////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid14);
             glPushMatrix();{                                                        // lower part of the dome
                 glTranslatef(0,0,150);
                 gluCylinder(quadric, 20, 20, 15, 6, 20);
             }glPopMatrix();

             glBindTexture(GL_TEXTURE_2D, texid6);
             glPushMatrix();{                                                        // main dome
                 glColor3f(0.99,0.62,0.55);
                 glTranslatef(0,0,0);
                 glScalef(2, 2, 1.8);
		         gluSphere(quadric, 10, 20, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // cone above the dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,10,-25);
		         glutSolidCone(10,10,20,20);
             }glPopMatrix();

             gluQuadricTexture(quadric, GLU_FALSE);
             glPushMatrix();{                                                        // solid stick above the cone
                 glTranslatef(0,0,-50);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 1, 1, 25, 4, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // sphere in the middle of the stick
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,-42);
                 glScalef(2, 2, 2);
		         glutSolidSphere(2, 20, 20);
             }glPopMatrix();

         }glPopMatrix();



         // Minar 3
         glPushMatrix();{

             glTranslatef(170,-330,-50);

             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
             ////////////////////////////////Minar Body Design//////////////////////////////////////////////////////////////
             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid4);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // lower part of the minar
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,0);
                 gluCylinder(quadric, 20, 20, 100, 6, 20);
             }glPopMatrix();

             glBindTexture(GL_TEXTURE_2D, texid5);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // minar gliphs
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,100);
                 gluCylinder(quadric, 20, 20, 50, 6, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // minar carnish
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,10);
                 glScalef(2, 2, 0.1);
		         glutSolidSphere(20, 6, 3);
             }glPopMatrix();

             //////////////////////////////////////////////////////////////////////////////////////////////////
             ///////////////////////////////// Minar Dome Design///////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid14);
             glPushMatrix();{                                                        // lower part of the dome
                 glTranslatef(0,0,150);
                 gluCylinder(quadric, 20, 20, 15, 6, 20);
             }glPopMatrix();

             glBindTexture(GL_TEXTURE_2D, texid6);
             glPushMatrix();{                                                        // main dome
                 glColor3f(0.99,0.62,0.55);
                 glTranslatef(0,0,0);
                 glScalef(2, 2, 1.8);
		         gluSphere(quadric, 10, 20, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // cone above the dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,10,-25);
		         glutSolidCone(10,10,20,20);
             }glPopMatrix();

             gluQuadricTexture(quadric, GLU_FALSE);
             glPushMatrix();{                                                        // solid stick above the cone
                 glTranslatef(0,0,-50);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 1, 1, 25, 4, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // sphere in the middle of the stick
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,-42);
                 glScalef(2, 2, 2);
		         glutSolidSphere(2, 20, 20);
             }glPopMatrix();

         }glPopMatrix();


         // Minar 4
         glPushMatrix();{

             glTranslatef(-170,-330,-50);

             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
             ////////////////////////////////Minar Body Design//////////////////////////////////////////////////////////////
             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid4);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // lower part of the minar
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,0);
                 gluCylinder(quadric, 20, 20, 100, 6, 20);
             }glPopMatrix();

             glBindTexture(GL_TEXTURE_2D, texid5);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // minar gliphs
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,100);
                 gluCylinder(quadric, 20, 20, 50, 6, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // minar carnish
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,10);
                 glScalef(2, 2, 0.1);
		         glutSolidSphere(20, 6, 3);
             }glPopMatrix();

             //////////////////////////////////////////////////////////////////////////////////////////////////
             ///////////////////////////////// Minar Dome Design///////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid14);
             glPushMatrix();{                                                        // lower part of the dome
                 glTranslatef(0,0,150);
                 gluCylinder(quadric, 20, 20, 15, 6, 20);
             }glPopMatrix();

             glBindTexture(GL_TEXTURE_2D, texid6);
             glPushMatrix();{                                                        // main dome
                 glColor3f(0.99,0.62,0.55);
                 glTranslatef(0,0,0);
                 glScalef(2, 2, 1.8);
		         gluSphere(quadric, 10, 20, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // cone above the dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,10,-25);
		         glutSolidCone(10,10,20,20);
             }glPopMatrix();

             gluQuadricTexture(quadric, GLU_FALSE);
             glPushMatrix();{                                                        // solid stick above the cone
                 glTranslatef(0,0,-50);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 1, 1, 25, 4, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // sphere in the middle of the stick
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,-42);
                 glScalef(2, 2, 2);
		         glutSolidSphere(2, 20, 20);
             }glPopMatrix();

         }glPopMatrix();



         // Small Minar 1
         glPushMatrix();{

             glTranslatef(90,-8,-160);

             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
             //////////////////////////////// Small Minar Body Design//////////////////////////////////////////////////////////////
             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid4);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // minar gliphs
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,100);
                 gluCylinder(quadric, 10, 10, 50, 6, 20);
             }glPopMatrix();


             glPushMatrix();{                                                     // minar carnish
                 //glColor3f(0.62,0.21,0.17);
                 glTranslatef(0,0,95);
                 glScalef(2, 2, 0.1);
		         glutSolidSphere(8, 6, 3);
             }glPopMatrix();

             //////////////////////////////////////////////////////////////////////////////////////////////////
             ///////////////////////////////// Small Minar Dome Design///////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid6);
             glPushMatrix();{                                                        // main dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,90);
                 glScalef(2, 2, 1.8);
		         gluSphere(quadric, 5.2, 20, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // cone above the dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,80);
		         glutSolidCone(5,10,20,20);
             }glPopMatrix();

             gluQuadricTexture(quadric, GLU_FALSE);
             glPushMatrix();{                                                        // solid stick above the cone
                 glTranslatef(0,0,70);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 1, 1, 25, 4, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // sphere in the middle of the stick
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,70);
                 glScalef(2, 2, 2);
		         glutSolidSphere(1.5, 20, 20);
             }glPopMatrix();

         }glPopMatrix();


         // Small Minar 2
         glPushMatrix();{

             glTranslatef(-90,-8,-160);

             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
             //////////////////////////////// Small Minar Body Design//////////////////////////////////////////////////////////////
             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid4);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // minar gliphs
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,100);
                 gluCylinder(quadric, 10, 10, 50, 6, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // minar carnish
                 //glColor3f(0.62,0.21,0.17);
                 glTranslatef(0,0,95);
                 glScalef(2, 2, 0.1);
		         glutSolidSphere(8, 6, 3);
             }glPopMatrix();

             //////////////////////////////////////////////////////////////////////////////////////////////////
             ///////////////////////////////// Small Minar Dome Design///////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid6);
             glPushMatrix();{                                                        // main dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,90);
                 glScalef(2, 2, 1.8);
		         gluSphere(quadric, 5.2, 20, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // cone above the dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,80);
		         glutSolidCone(5,10,20,20);
             }glPopMatrix();

             gluQuadricTexture(quadric, GLU_FALSE);
             glPushMatrix();{                                                        // solid stick above the cone
                 glTranslatef(0,0,70);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 1, 1, 25, 4, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // sphere in the middle of the stick
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,70);
                 glScalef(2, 2, 2);
		         glutSolidSphere(1.5, 20, 20);
             }glPopMatrix();

         }glPopMatrix();

         // Small Minar 3
         glPushMatrix();{

             glTranslatef(-90,-321,-160);

             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
             //////////////////////////////// Small Minar Body Design//////////////////////////////////////////////////////////////
             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid4);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // minar gliphs
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,100);
                 gluCylinder(quadric, 10, 10, 50, 6, 20);
             }glPopMatrix();

              glPushMatrix();{                                                        // minar carnish
                 //glColor3f(0.62,0.21,0.17);
                 glTranslatef(0,0,95);
                 glScalef(2, 2, 0.1);
		         glutSolidSphere(8, 6, 3);
             }glPopMatrix();

             //////////////////////////////////////////////////////////////////////////////////////////////////
             ///////////////////////////////// Small Minar Dome Design///////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid6);
             glPushMatrix();{                                                        // main dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,90);
                 glScalef(2, 2, 1.8);
		         gluSphere(quadric, 5.2, 20, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // cone above the dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,80);
		         glutSolidCone(5,10,20,20);
             }glPopMatrix();

             gluQuadricTexture(quadric, GLU_FALSE);
             glPushMatrix();{                                                        // solid stick above the cone
                 glTranslatef(0,0,70);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 1, 1, 25, 4, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // sphere in the middle of the stick
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,70);
                 glScalef(2, 2, 2);
		         glutSolidSphere(1.5, 20, 20);
             }glPopMatrix();

         }glPopMatrix();


         // Small Minar 4
         glPushMatrix();{

             glTranslatef(90,-321,-160);

             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
             //////////////////////////////// Small Minar Body Design//////////////////////////////////////////////////////////////
             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid4);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // minar gliphs
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,100);
                 gluCylinder(quadric, 10, 10, 50, 6, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // minar carnish
                 //glColor3f(0.62,0.21,0.17);
                 glTranslatef(0,0,95);
                 glScalef(2, 2, 0.1);
		         glutSolidSphere(8, 6, 3);
             }glPopMatrix();

             //////////////////////////////////////////////////////////////////////////////////////////////////
             ///////////////////////////////// Small Minar Dome Design///////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid6);
             glPushMatrix();{                                                        // main dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,90);
                 glScalef(2, 2, 1.8);
		         gluSphere(quadric, 5.2, 20, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // cone above the dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,80);
		         glutSolidCone(5,10,20,20);
             }glPopMatrix();

             gluQuadricTexture(quadric, GLU_FALSE);
             glPushMatrix();{                                                        // solid stick above the cone
                 glTranslatef(0,0,70);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 1, 1, 25, 4, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // sphere in the middle of the stick
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,70);
                 glScalef(2, 2, 2);
		         glutSolidSphere(1.5, 20, 20);
             }glPopMatrix();

         }glPopMatrix();

         // Small Minar 5
         glPushMatrix();{
             glRotatef(90,0,0,1);
             glTranslatef(-80,-162,-160);

             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
             //////////////////////////////// Small Minar Body Design//////////////////////////////////////////////////////////////
             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid4);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // minar gliphs
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,100);
                 gluCylinder(quadric, 10, 10, 50, 6, 20);
             }glPopMatrix();

            glPushMatrix();{                                                        // minar carnish
                 //glColor3f(0.62,0.21,0.17);
                 glTranslatef(0,0,95);
                 glScalef(2, 2, 0.1);
		         glutSolidSphere(8, 6, 3);
             }glPopMatrix();

             //////////////////////////////////////////////////////////////////////////////////////////////////
             ///////////////////////////////// Small Minar Dome Design///////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid6);
             glPushMatrix();{                                                        // main dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,90);
                 glScalef(2, 2, 1.8);
		         gluSphere(quadric, 5.2, 20, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // cone above the dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,80);
		         glutSolidCone(5,10,20,20);
             }glPopMatrix();

             gluQuadricTexture(quadric, GLU_FALSE);
             glPushMatrix();{                                                        // solid stick above the cone
                 glTranslatef(0,0,70);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 1, 1, 25, 4, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // sphere in the middle of the stick
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,70);
                 glScalef(2, 2, 2);
		         glutSolidSphere(1.5, 20, 20);
             }glPopMatrix();

         }glPopMatrix();


         // Small Minar 6
         glPushMatrix();{
             glRotatef(90,0,0,1);
             glTranslatef(-257,-162,-160);

             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
             //////////////////////////////// Small Minar Body Design//////////////////////////////////////////////////////////////
             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid4);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // minar gliphs
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,100);
                 gluCylinder(quadric, 10, 10, 50, 6, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // minar carnish
                 //glColor3f(0.62,0.21,0.17);
                 glTranslatef(0,0,95);
                 glScalef(2, 2, 0.1);
		         glutSolidSphere(8, 6, 3);
             }glPopMatrix();

             //////////////////////////////////////////////////////////////////////////////////////////////////
             ///////////////////////////////// Small Minar Dome Design///////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid6);
             glPushMatrix();{                                                        // main dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,90);
                 glScalef(2, 2, 1.8);
		         gluSphere(quadric, 5.2, 20, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // cone above the dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,80);
		         glutSolidCone(5,10,20,20);
             }glPopMatrix();

             gluQuadricTexture(quadric, GLU_FALSE);
             glPushMatrix();{                                                        // solid stick above the cone
                 glTranslatef(0,0,70);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 1, 1, 25, 4, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // sphere in the middle of the stick
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,70);
                 glScalef(2, 2, 2);
		         glutSolidSphere(1.5, 20, 20);
             }glPopMatrix();

         }glPopMatrix();


         // Small Minar 7
         glPushMatrix();{
             glRotatef(90,0,0,1);
             glTranslatef(-257,162,-160);

             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
             //////////////////////////////// Small Minar Body Design//////////////////////////////////////////////////////////////
             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid4);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // minar gliphs
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,100);
                 gluCylinder(quadric, 10, 10, 50, 6, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // minar carnish
                 //glColor3f(0.62,0.21,0.17);
                 glTranslatef(0,0,95);
                 glScalef(2, 2, 0.1);
		         glutSolidSphere(8, 6, 3);
             }glPopMatrix();

             //////////////////////////////////////////////////////////////////////////////////////////////////
             ///////////////////////////////// Small Minar Dome Design///////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid6);
             glPushMatrix();{                                                        // main dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,90);
                 glScalef(2, 2, 1.8);
		         gluSphere(quadric, 5.2, 20, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // cone above the dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,80);
		         glutSolidCone(5,10,20,20);
             }glPopMatrix();

             gluQuadricTexture(quadric, GLU_FALSE);
             glPushMatrix();{                                                        // solid stick above the cone
                 glTranslatef(0,0,70);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 1, 1, 25, 4, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // sphere in the middle of the stick
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,70);
                 glScalef(2, 2, 2);
		         glutSolidSphere(1.5, 20, 20);
             }glPopMatrix();

         }glPopMatrix();


         // Small Minar 8
         glPushMatrix();{
             glRotatef(90,0,0,1);
             glTranslatef(-80,+162,-160);

             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
             //////////////////////////////// Small Minar Body Design//////////////////////////////////////////////////////////////
             ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid4);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // minar gliphs
                 glRotatef(90,0,0,1);
                 glTranslatef(0,0,100);
                 gluCylinder(quadric, 10, 10, 50, 6, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // minar carnish
                 //glColor3f(0.62,0.21,0.17);
                 glTranslatef(0,0,95);
                 glScalef(2, 2, 0.1);
		         glutSolidSphere(8, 6, 3);
             }glPopMatrix();

             //////////////////////////////////////////////////////////////////////////////////////////////////
             ///////////////////////////////// Small Minar Dome Design///////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////

             glBindTexture(GL_TEXTURE_2D, texid6);
             glPushMatrix();{                                                        // main dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,90);
                 glScalef(2, 2, 1.8);
		         gluSphere(quadric, 5.2, 20, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // cone above the dome
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,80);
		         glutSolidCone(5,10,20,20);
             }glPopMatrix();

             gluQuadricTexture(quadric, GLU_FALSE);
             glPushMatrix();{                                                        // solid stick above the cone
                 glTranslatef(0,0,70);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 1, 1, 25, 4, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // sphere in the middle of the stick
                 //glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,0,70);
                 glScalef(2, 2, 2);
		         glutSolidSphere(1.5, 20, 20);
             }glPopMatrix();

         }glPopMatrix();



         // Big Dome
         glPushMatrix();{
             glRotatef(180,0,1,0);
             glTranslatef(0,0,-100);
             glBindTexture(GL_TEXTURE_2D, texid16);
             gluQuadricNormals(quadric,GLU_SMOOTH);
             gluQuadricTexture(quadric, GLU_TRUE);
             glPushMatrix();{                                                        // octagon 1
                 glTranslatef(0,-170,100);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 80, 80, 10, 8, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // octagon 1 cover
                 //glColor3f(0.62,0.21,0.17);
                 glTranslatef(0,-170,110);
                 glScalef(2, 2, 0.1);
		         glutSolidSphere(46, 8, 3);
             }glPopMatrix();

             glPushMatrix();{                                                        //  octagon 2
                 glTranslatef(0,-170,110);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 70, 70, 10, 8, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // octagon 2 cover
                 //glColor3f(0.62,0.21,0.17);
                 glTranslatef(0,-170,120);
                 glScalef(2, 2, 0.1);
		         glutSolidSphere(40, 8, 3);
             }glPopMatrix();

             glPushMatrix();{                                                        //  octagon 3
                 glTranslatef(0,-170,120);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 60, 60, 10, 8, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // octagon 3 cover
                 //glColor3f(0.62,0.21,0.17);
                 glTranslatef(0,-170,130);
                 glScalef(2, 2, 0.1);
		         glutSolidSphere(35, 8, 3);
             }glPopMatrix();

             glPushMatrix();{                                                        //  octagon 4
                 glTranslatef(0,-170,130);
                 //glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 50, 50, 30, 8, 2);
             }glPopMatrix();

             glBindTexture(GL_TEXTURE_2D, texid8);
             glPushMatrix();{                                                        // big dome sphere
                 glTranslatef(0,-170,170);
                 glColor3f(0.34,0.91,0.74);
                 gluSphere(quadric, 50, 8, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // cone above the dome
                 glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,-170,215);
		         glutSolidCone(20,20,20,20);
             }glPopMatrix();

             glPushMatrix();{                                                        // solid stick above the cone
                 glTranslatef(0,-170,230);
                 glColor3f(0.34,0.91,0.74);
                 gluCylinder(quadric, 1, 1, 40, 4, 2);
             }glPopMatrix();

             glPushMatrix();{                                                        // sphere in the middle of the stick
                 glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,-170,250);
                 glScalef(2, 2, 2);
		         glutSolidSphere(3, 20, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // sphere in the bottom of the stick
                 glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,-170,240);
                 glScalef(2, 2, 2);
		         glutSolidSphere(1.5, 20, 20);
             }glPopMatrix();

             glPushMatrix();{                                                        // sphere in the top of the stick
                 glColor3f(0.34,0.91,0.74);
                 glTranslatef(0,-170,260);
                 glScalef(2, 2, 2);
		         glutSolidSphere(1.5, 20, 20);
             }glPopMatrix();
         }glPopMatrix();



         gluDeleteQuadric(quadric);
} //glDisable(GL_TEXTURE_2D);


// _sceneA.cpp

void _fort::drawTree(float tr_x, float tr_y, float tr_z)
{
    // First “billboard” pass
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texid9);
        glTranslatef(tr_x, tr_y, tr_z);
        glBegin(GL_POLYGON);
            glTexCoord2f(0,0);  glVertex3f(   0,   0,   0);
            glTexCoord2f(0,0);  glVertex3f(-15,   0,  30);
            glTexCoord2f(0,1);  glVertex3f(-20,   0,  60);
            glTexCoord2f(0,1);  glVertex3f( -5,   0,  90);
            glTexCoord2f(1,1);  glVertex3f(  5,   0,  90);
            glTexCoord2f(1,0);  glVertex3f( 20,   0,  60);
            glTexCoord2f(1,1);  glVertex3f( 15,   0,  30);
        glEnd();
    glPopMatrix();

    // Second pass, rotated 90° about Z
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texid9);
        glTranslatef(tr_x, tr_y, tr_z);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_POLYGON);
            glTexCoord2f(0,0);  glVertex3f(   0,   0,   0);
            glTexCoord2f(0,0);  glVertex3f(-15,   0,  30);
            glTexCoord2f(0,1);  glVertex3f(-20,   0,  60);
            glTexCoord2f(0,1);  glVertex3f( -5,   0,  90);
            glTexCoord2f(1,1);  glVertex3f(  5,   0,  90);
            glTexCoord2f(1,0);  glVertex3f( 20,   0,  60);
            glTexCoord2f(1,1);  glVertex3f( 15,   0,  30);
        glEnd();
    glPopMatrix();
}







