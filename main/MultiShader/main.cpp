#define PI 3.1415926536
#define toRad(x) ((x)*2*PI/360)

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"
#include "GL/glut.h"
#include <vector>
#include <math.h>
#include "textfile.h"
#include "TRIModel.h"


using namespace std;

void changeShader(int s);
GLuint vShader,fShader,prog;

TRIModel csieModel,ballModel,f18Model,csieB1;
TRIModel *TRIobj;
int currentModel=1,currentShader=1;
double thita=0,phi=20,omega=1;
double viewA=0,viewB=0,dis=1000;
double ratio;
bool autoRot=0,disLight=1;
int ch=0;

int ww,wh;

int timeNow,timeOld,frames=0;
float fps=0;

class Shader
    {
    public:
        GLuint vShader,fShader,prog;
        int uniformLoc[4];
        Shader()
            {
            }
        ~Shader()
            {
            glDetachShader(prog,vShader);
            glDetachShader(prog,fShader);
            glDeleteShader(vShader);
            glDeleteShader(fShader);
            glDeleteProgram(prog);    
            }
        void load(char *vspath,char *fspath)
            {
            char *vsSource,*fsSource;
            vsSource=textFileRead(vspath);
            fsSource=textFileRead(fspath);
            vShader=glCreateShader(GL_VERTEX_SHADER);
            fShader=glCreateShader(GL_FRAGMENT_SHADER);
            const char* vsSc=vsSource;
            const char* fsSc=fsSource;
            glShaderSource(vShader,1,&vsSc,NULL);
            glShaderSource(fShader,1,&fsSc,NULL);
            free(vsSource);
            free(fsSource);
            glCompileShader(vShader);
	        glCompileShader(fShader);
	        prog=glCreateProgram();
	        glAttachShader(prog,vShader);
	        glAttachShader(prog,fShader);
	        glLinkProgram(prog);    
            }
        void useProgram()
            {
            glUseProgram(prog);    
            }
        
    };

Shader testS;

int lightNum=0,lightEnabled=0;
class Light
    {
    public:
        bool enabled,exist;
        int i;
        float pos[4],ambient[4],diffuse[4],specular[4];
        float spotDir[4],spotExp,spotCutoff;
        float cAtt,lAtt,qAtt;
        float spotAlpha,spotBeta;
        float r,alpha,beta;
        Light()
            {
            exist=0;
            i=lightNum++;            
            loadDefault();
            }
        ~Light()
            {
            if(enabled)lightEnabled--;
            lightNum--;
            }
        void loadDefault()
            {
            for(int j=0;j<3;j++)
                ambient[j]=diffuse[j]=specular[j]=0;
            ambient[3]=diffuse[3]=specular[3]=1;
            enabled=0;
            spotBeta=alpha=beta=0;
            spotAlpha=180;
            r=700;
            spotExp=0;spotCutoff=180;
            cAtt=1;lAtt=0.0000000001;qAtt=0.0000000000001;
            setPos();
            setColor();
            setAtt(cAtt,lAtt,qAtt);
            }
        void enable()
            {
            if(!enabled&&exist)
                {
                lightEnabled++;
                enabled=1;
                glEnable(GL_LIGHT0+i);
                }
            }
        void disable()
            {
            if(enabled&&exist)
                {
                lightEnabled--;
                enabled=0;
                glDisable(GL_LIGHT0+i);    
                }
            }
        void renew()
            {
            glLightfv(GL_LIGHT0+i,GL_POSITION,pos);
            }
        void increasePos(float nr,float nalpha,float nbeta)
            {
            alpha+=nalpha;beta+=nbeta;
            spotAlpha+=nalpha;spotBeta-=nbeta;
            r+=nr;
            if(r<10)r=10;
            setPos();
            }
        void setPos()
            {
            pos[0]=r*cos(toRad(alpha))*cos(toRad(beta));
            pos[1]=r*cos(toRad(alpha))*sin(toRad(beta));
            pos[2]=r*sin(toRad(alpha));
            pos[3]=1;
            }
        void increaseColor(float ar,float ag,float ab,float dr,float dg,float db,float sr,float sg,float sb)
            {
            ambient[0]+=ar;ambient[1]+=ag;ambient[2]+=ab;
            diffuse[0]+=dr;diffuse[1]+=dg;diffuse[2]+=db;
            specular[0]+=sr;specular[1]+=sg;specular[2]+=sb;
            for(int j=0;j<3;j++)
                {
                if(ambient[j]>1.0)ambient[j]=1.0;
                else if(ambient[j]<0.0)ambient[j]=0.0;
                if(diffuse[j]>1.0)diffuse[j]=1.0;
                else if(diffuse[j]<0.0)diffuse[j]=0.0;
                if(specular[j]>1.0)specular[j]=1.0;
                else if(specular[j]<0.0)specular[j]=0.0;
                }
            setColor();
            }
        void setColor()
            {
            glLightfv(GL_LIGHT0+i,GL_AMBIENT,ambient);
            glLightfv(GL_LIGHT0+i,GL_DIFFUSE,diffuse);
            glLightfv(GL_LIGHT0+i,GL_SPECULAR,specular);
            }
        void increaseAtt(float c,float l,float q)
            {
            cAtt*=c;lAtt*=l;qAtt*=q;
            if(cAtt<=0.02)cAtt=0.02;
            if(lAtt<=0)lAtt=0.0000000001;
            if(qAtt<=0)qAtt=0.0000000000001;
            setAtt(cAtt,lAtt,qAtt);
            }
        void setAtt(float constant,float linear,float quadratic)
            {
            cAtt=constant;lAtt=linear;qAtt=quadratic;
            glLightf(GL_LIGHT0+i,GL_CONSTANT_ATTENUATION,constant);
            glLightf(GL_LIGHT0+i,GL_LINEAR_ATTENUATION,linear);
            glLightf(GL_LIGHT0+i,GL_QUADRATIC_ATTENUATION,quadratic);
            }
        void setSpot(float exp,float cutoff)
            {
            if(exp>128)exp=128;
            else if(exp<0)exp=0;
            if(cutoff>90&&cutoff!=180)cutoff=90;
            else if(cutoff<0)cutoff=0;
            glLightfv(GL_LIGHT0+i,GL_SPOT_DIRECTION,spotDir);
            glLightf(GL_LIGHT0+i,GL_SPOT_EXPONENT,exp);
            //glLight()
            }
        
    };

Light lights[4];
vector<int> lightHandler;

int newLight()
    {
    //if(lightEnabled<4)
        {
        for(int i=0;i<4;i++)if(!lights[i].exist)
            {
            lights[i].exist=1;
            lights[i].loadDefault();
            lightHandler.push_back(i);
            return lightHandler.size()-1;
            }
        }
    return -1;
    }
bool deleteLight(int i)
    {
    if(i>=lightHandler.size()||lights[lightHandler[i]].exist==0)return 0;
    lights[lightHandler[i]].disable();
    lights[lightHandler[i]].exist=0;
    lightHandler.erase(lightHandler.begin()+i);
    return 1;
    }
    
void loadLights()
    {
    for(int i=0;i<lightHandler.size();i++)if(lights[lightHandler[i]].enabled)
        lights[lightHandler[i]].renew();
    }
    
void printBitmapString(void *font,char *str)
    {
    char *strPos=str;
    while(*strPos!=0)
        {
        glutBitmapCharacter(font,*strPos);
        strPos++;
        }
    }

void reshape(int w,int h)
    {
    ww=w;wh=h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(h<100)h=101;
    if(w<100)w=101;
    ratio=1.0*(w-100)/(h-100);
    glViewport(50,50,w-100,h-100);
    gluPerspective(50,ratio,1,100000);
    
    glMatrixMode(GL_MODELVIEW);
    
    }

void idle()
    {
    if(autoRot)
        {
        thita+=omega;
        if(thita>=360*100)thita=0;
        }
    glutPostRedisplay(); 
    } 
    
int xp,yp;
int mouseMode=0,mouseLeft;
int selected=-1;
void display()
    {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    gluLookAt(dis*cos(toRad(viewB))*sin(toRad(viewA)),dis*sin(toRad(viewB)),dis*cos(toRad(viewB))*cos(toRad(viewA)),
		      0.0,0.0,0.0,
			  cos(toRad(viewB+90))*sin(toRad(viewA)),dis*sin(toRad(viewB+90)),dis*cos(toRad(viewB+90))*cos(toRad(viewA)));
	
	loadLights();
	
	if(disLight)
	    {
	    glShadeModel(GL_FLAT);
        glUseProgram(0);
        glDisable(GL_LIGHTING);
        for(int i=0;i<lightHandler.size();i++)if(lights[lightHandler[i]].enabled)
            {
            glPushMatrix();
            glTranslatef(lights[lightHandler[i]].pos[0],lights[lightHandler[i]].pos[1],lights[lightHandler[i]].pos[2]);
            glColor4fv(lights[lightHandler[i]].specular);
            glutSolidSphere(25,10,10);
            glColor4fv(lights[lightHandler[i]].diffuse);
            glutWireSphere(30,8,8);
            glPopMatrix();
            }
        glEnable(GL_LIGHTING);
        changeShader(currentShader);
        }
	
	if(currentShader==3)
	    {
        float mat[16];
        glGetFloatv(GL_MODELVIEW_MATRIX,mat);
        for(int i=0;i<4;i++)
            {
            float lpos[4]={0};
            for(int j=0;j<3;j++)
                //for(int k=0;k<4;k++)
                    //lpos[j]+=lights[i].pos[k]*mat[4*j+k];
                //lpos[j]=lights[i].pos[0]*mat[0+4*j]+lights[i].pos[1]*mat[1+4*j]+lights[i].pos[2]*mat[2+4*j]+lights[i].pos[3]*mat[3+4*j];
                lpos[j]=lights[i].pos[0]*mat[0+j]+lights[i].pos[1]*mat[4+j]+lights[i].pos[2]*mat[8+j]+lights[i].pos[3]*mat[12+j];
            
            //lpos[0]=lights[i].pos[0];lpos[1]=lights[i].pos[1];
            //lpos[2]=lights[i].pos[2]
            lpos[3]=lights[i].enabled;
            glUniform4fv(testS.uniformLoc[i],4,lpos);
            }
        }
		  
    glPushMatrix();
	if(currentModel==2)glTranslated(0,-50,0);
	glRotated(phi,1,0,0);
	glRotated(thita,0,1,0);
	if(currentModel==2||currentModel==3)glRotated(-90,1,0,0);
	if(currentModel==2||currentModel==3)glTranslated(-300,-200,0);
	//if(currentModel==3){glScalef(25,25,25);glTranslated(-100,-10,-15);}
	//if(currentModel==3){glScalef(25,25,25);glTranslated(-100,-10,-15);}
	if(currentModel==4){glScalef(15,15,15);glTranslated(-30,-5,15);}
        
	glBegin(GL_TRIANGLES);
    for(int i=0;i<TRIobj->triangleList.size();i++)
        {
        glColor3fv(TRIobj->triangleList[i].color);
        //glColor3d(1.0*TRIobj->triangleList[i].foreColor[0]/256,1.0*TRIobj->triangleList[i].foreColor[1]/256,1.0*TRIobj->triangleList[i].foreColor[2]/256);
        for(int j=0;j<3;j++)
            {
            //glColor3d(1.0*TRIobj->triangleList[i].foreColor[0]/256,1.0*TRIobj->triangleList[i].foreColor[1]/256,1.0*TRIobj->triangleList[i].foreColor[2]/256);
            glNormal3dv(TRIobj->triangleList[i].normal[j]);
            glVertex3dv(TRIobj->triangleList[i].vertex[j]);
            }
        }
    glEnd();
    glPopMatrix();
    
    frames++;
    timeNow=glutGet(GLUT_ELAPSED_TIME);
    if((timeNow-timeOld)>1000)
        {
        fps=1000.0*frames/(timeNow-timeOld);
        timeOld=timeNow;
        frames=0;
        }
    
    glShadeModel(GL_FLAT);
    glUseProgram(0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glViewport(0,0,ww,wh);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,ww,0,wh);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    if(fps<50)glColor3f(1.0,0,0);
    else glColor3f(0,1.0,0);
    char tmps[100];
    //sprintf(tmps,"FPS : %.1f",fps);
    glRasterPos2i(20,wh-30);
//    printBitmapString(GLUT_BITMAP_HELVETICA_18,tmps);
    
    glColor3f(0.8,0.9,0.9);
    //sprintf(tmps,"model #%d, %d triangles",currentModel,TRIobj->triangleList.size());
    glRasterPos2i(ww/2-120,wh-40);
//    printBitmapString(GLUT_BITMAP_9_BY_15,tmps);
    //sprintf(tmps,"%s shading",currentShader==1?"  Flat":currentShader==2?"Gouraud":currentShader==3?" Phong":"Unknown");
    glRasterPos2i(ww/2-84,wh-23);
//    printBitmapString(GLUT_BITMAP_9_BY_15,tmps);
    glBegin(GL_QUADS);
    if(selected!=-1)
        {
        glColor3f(0.63,0.73,0.22);
        glVertex2s(2,wh-59-selected*22);
        glVertex2s(24,wh-59-selected*22);
        glVertex2s(24,wh-38-selected*22);
        glVertex2s(2,wh-38-selected*22);
        }
    glEnd();
    
/*    for(int i=0;i<lightHandler.size();i++)
        {
        if(lights[lightHandler[i]].enabled)glColor3f(1,1,1);
        else glColor3f(0.4,0.4,0.4);
        glRasterPos2d(9,wh-56-i*22);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'1'+i);
        }    */
    if(selected!=-1)
        {
        glColor3f(0.8,0.9,0.9);    
//        sprintf(tmps,"%cdiffuse: %.2f, %.2f ,%.2f",ch==0?'>':' ',lights[lightHandler[selected]].diffuse[0],lights[lightHandler[selected]].diffuse[1],lights[lightHandler[selected]].diffuse[2]);
        glRasterPos2d(60,35);
//        printBitmapString(GLUT_BITMAP_9_BY_15,tmps);
//        sprintf(tmps,"%cspecular: %.2f, %.2f ,%.2f",ch==1?'>':' ',lights[lightHandler[selected]].specular[0],lights[lightHandler[selected]].specular[1],lights[lightHandler[selected]].specular[2]);    
        glRasterPos2d(60,20);
//        printBitmapString(GLUT_BITMAP_9_BY_15,tmps);
//        sprintf(tmps,"%cattenuation: constant = %.2f, linear = %.1e , quadratic = %.1e",ch==2?'>':' ',lights[lightHandler[selected]].cAtt,lights[lightHandler[selected]].lAtt,lights[lightHandler[selected]].qAtt);    
        glRasterPos2d(60,5);
//        printBitmapString(GLUT_BITMAP_9_BY_15,tmps);
        }
        
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glViewport(50,50,ww-100,wh-100);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    changeShader(currentShader);
    
    glutSwapBuffers();
    }

void shaderInit()
    {
    //testS.load("test.vs","test.fs");
    testS.load("phong.vs","phong.fs");
    testS.uniformLoc[0]=glGetUniformLocation(testS.prog,"lightPos[0]");
    testS.uniformLoc[1]=glGetUniformLocation(testS.prog,"lightPos[1]");
    testS.uniformLoc[2]=glGetUniformLocation(testS.prog,"lightPos[2]");
    testS.uniformLoc[3]=glGetUniformLocation(testS.prog,"lightPos[3]");
    }
    
void changeShader(int s)
    {
    switch(s)
        {
        case 3:
            testS.useProgram(); 
            break;
        case 2:
            glShadeModel(GL_SMOOTH);
            glUseProgram(0);
            break;
        case 1:
            glShadeModel(GL_FLAT);
            glUseProgram(0);
            break;    
        }
    currentShader=s;
    //printf("change shader: %d\n",s);
    }
    
void changeModel(int m)
    {
    switch(m)
        {
        case 2:
            TRIobj=&csieModel;
            break;
        case 1:
            TRIobj=&ballModel;
            break;
        case 3:
            TRIobj=&f18Model;
            break;    
        case 4:
            TRIobj=&csieB1;
        }    
    currentModel=m;
    }

float matA[]={0.15,0.15,0.15,1};
float matD[]={0.6,0.6,0.6,1};
float matS[]={1,1,1,1};

void materialInit()
    {
    glEnable(GL_NORMALIZE);
    
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,30);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,matA);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,matD);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,matS);

    glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    }

void lightInit()
    {
    int i;
    glEnable(GL_LIGHTING);
    i=newLight();
    lights[lightHandler[i]].increaseColor(0.1,0.1,0.1,1,1,1,0.4,0.5,0.9);
    lights[lightHandler[i]].increasePos(-50,130,-50);
    lights[lightHandler[i]].setAtt(0.7,0.0000004,0.0000005);
    lights[lightHandler[i]].enable();
    //cout<<"light #"<<i<<": ";
    /*for(int j=0;j<3;j++)
        cout<<lights[lightHandler[i]].pos[j]<<" ";
        cout<<endl;*/
    
    i=newLight();
    lights[lightHandler[i]].increaseColor(0,0,0,0.3,0.3,0.3,0.18,0.18,0.16);
    lights[lightHandler[i]].increasePos(-30,-10,70);
    lights[lightHandler[i]].setAtt(1,0,0);
    lights[lightHandler[i]].enable();
    /*cout<<"light #"<<i<<": ";
    for(int j=0;j<3;j++)
        cout<<lights[lightHandler[i]].pos[j]<<" ";
        cout<<endl;*/
    
    i=newLight();
    lights[lightHandler[i]].increaseColor(0,0,0,0.8,0.6,0.6,0.9,0.6,0.4);
    lights[lightHandler[i]].increasePos(-60,-10,-70);
    lights[lightHandler[i]].setAtt(0.7,0.0000007,0.0000009);
    lights[lightHandler[i]].enable();
    /*cout<<"light #"<<i<<": ";
    for(int j=0;j<3;j++)
        cout<<lights[lightHandler[i]].pos[j]<<" ";
        cout<<endl;*/
    
    }

void myInit()
    {
    ballModel.loadFromFile("models/balls.tri");
    csieModel.loadFromFile("models/csie.tri");
    f18Model.loadFromFile("models/Car_road.tri");
    csieB1.loadFromFile("models/Easter.tri");

    changeModel(currentModel);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f,0.0f,0.4f,0.0f);
    glewInit();
    if(glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else
        {
		printf("GLSL not supported\n");
		exit(1);
	   }
	/*typedef bool (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
	wglSwapIntervalEXT(1);
	wglSwapIntervalEXT(false);*/
	shaderInit();
	changeShader(currentShader);
	
	materialInit();
	lightInit();
	
	timeNow=timeOld=glutGet(GLUT_ELAPSED_TIME);
    
    }

void keyboard(unsigned char key,int x,int y)
    {
    int l;
    switch(key)
        {
        case 27:
            exit(0);
            break;
        case 'm':
            changeModel((currentModel)%4+1);
            break;
        case 's':
            changeShader((currentShader)%3+1);
            break;
        case 'a':
            autoRot^=1;
            break;
        case 'l':
            disLight^=1;
            break;
        case '9':
            dis-=30;
            if(dis<10)dis=10;
            break;
        case '0':
            dis+=30;
            break;
        case '+':
        case '=':
            omega+=0.2;
            break;
        case '-':
            omega-=0.2;
            if(omega<=0.2)omega=0.2;
            break;
        case '`':
            omega=1;
            viewA=viewB=0;dis=1000;
            thita=0;phi=20;
            for(int i=0;i<lightHandler.size();i++)
                lights[lightHandler[i]].enable();
            break;
        case 'g':
            l=newLight();
            if(l!=-1)
                {
                lights[lightHandler[l]].increaseColor(0,0,0,0.5,0.5,0.5,0.5,0.5,0.5);
                lights[lightHandler[l]].enable();
                selected=l;
                }
            break;
        case 'd':
            if(selected!=-1)
                {
                deleteLight(selected);
                selected=-1;    
                }
            break;
        case '1':
            if(lightHandler.size()>0)
                {
                if(lights[lightHandler[0]].enabled)
                    {
                    lights[lightHandler[0]].disable();
                    if(selected==0)selected=-1;
                    }
                else 
                    {
                    selected=0;
                    lights[lightHandler[0]].enable();
                    }
                }
            break;
        case '2':
            if(lightHandler.size()>1)
                {
                if(lights[lightHandler[1]].enabled)
                    {
                    lights[lightHandler[1]].disable();
                    if(selected==1)selected=-1;
                    }
                else 
                    {
                    selected=1;
                    lights[lightHandler[1]].enable();
                    }
                }
            break;
        case '3':
            if(lightHandler.size()>2)
                {
                if(lights[lightHandler[2]].enabled)
                    {
                    lights[lightHandler[2]].disable();
                    if(selected==2)selected=-1;
                    }
                else 
                    {
                    selected=2;
                    lights[lightHandler[2]].enable();
                    }
                }
            break;
        case '4':
            if(lightHandler.size()>3)
                {
                if(lights[lightHandler[3]].enabled)
                    {
                    lights[lightHandler[3]].disable();
                    if(selected==3)selected=-1;
                    }
                else 
                    {
                    selected=3;
                    lights[lightHandler[3]].enable();
                    }
                }
            break;
        case ',':
            if(selected!=-1)ch=(ch+2)%3;
            break;
        case '.':
            if(selected!=-1)ch=(ch+1)%3;
            break;
        case 'y':
            if(selected!=-1)
                {
                if(ch==0)
                    lights[lightHandler[selected]].increaseColor(0,0,0,0.03,0,0,0,0,0);
                else if(ch==1)
                    lights[lightHandler[selected]].increaseColor(0,0,0,0,0,0,0.03,0,0);  
                else if(ch==2)
                    lights[lightHandler[selected]].increaseAtt(1.05,1,1);     
                }
            break;
        case 'h':
            if(selected!=-1)
                {
                if(ch==0)
                    lights[lightHandler[selected]].increaseColor(0,0,0,-0.03,0,0,0,0,0);
                else if(ch==1)
                    lights[lightHandler[selected]].increaseColor(0,0,0,0,0,0,-0.03,0,0);  
                else if(ch==2)
                    lights[lightHandler[selected]].increaseAtt(1/1.05,1,1);
                }
            break;
        case 'u':
            if(selected!=-1)
                {
                if(ch==0)
                    lights[lightHandler[selected]].increaseColor(0,0,0,0,0.03,0,0,0,0);
                else if(ch==1)
                    lights[lightHandler[selected]].increaseColor(0,0,0,0,0,0,0,0.03,0);  
                else if(ch==2)
                    lights[lightHandler[selected]].increaseAtt(1,1.3,1);     
                }
            break;
        case 'j':
            if(selected!=-1)
                {
                if(ch==0)
                    lights[lightHandler[selected]].increaseColor(0,0,0,0,-0.03,0,0,0,0);
                else if(ch==1)
                    lights[lightHandler[selected]].increaseColor(0,0,0,0,0,0,0,-0.03,0);  
                else if(ch==2)
                    lights[lightHandler[selected]].increaseAtt(1,1/1.3,1);     
                }
            break;
        case 'i':
            if(selected!=-1)
                {
                if(ch==0)
                    lights[lightHandler[selected]].increaseColor(0,0,0,0,0,0.03,0,0,0);
                else if(ch==1)
                    lights[lightHandler[selected]].increaseColor(0,0,0,0,0,0,0,0,0.03);  
                else if(ch==2)
                    lights[lightHandler[selected]].increaseAtt(1,1,1.3);     
                }
            break;
        case 'k':
            if(selected!=-1)
                {
                if(ch==0)
                    lights[lightHandler[selected]].increaseColor(0,0,0,0,0,-0.03,0,0,0);
                else if(ch==1)
                    lights[lightHandler[selected]].increaseColor(0,0,0,0,0,0,0,0,-0.03);  
                else if(ch==2)
                    lights[lightHandler[selected]].increaseAtt(1,1,1/1.3);     
                }
            break;
        }    
       
    }

bool moveLeft;
void motion(int x,int y)
    {
    switch(mouseMode)
        {
        case 1:
            if(mouseLeft==1)
                {
                if(!autoRot)
                    {
                    thita+=0.3*(x-xp);
                    phi+=0.3*(y-yp);
                    }
                }
            else
                {
                viewA-=0.3*(x-xp);
                viewB-=0.3*(y-yp);
                } 
            break;
        case 2:
            if(selected!=-1)
                {
                if(mouseLeft==1)
                    {
                    lights[lightHandler[selected]].increasePos(0,-0.1*(x-xp),0);
                    lights[lightHandler[selected]].increasePos(0,0,0.1*(y-yp));
                    }    
                else
                    lights[lightHandler[selected]].increasePos((moveLeft?1:-1)*0.8*(x-xp),0,0);
                }
            break;
        }
    xp=x;
    yp=y;
    }
    
void mouse(int button,int state,int x,int y)
    {
    switch(button)
        {
        case GLUT_LEFT_BUTTON:
            mouseLeft=1;
            break;
        case GLUT_RIGHT_BUTTON:
            mouseLeft=0;
            break;
        }
    if(state==GLUT_DOWN)    
        {
        xp=x;
        yp=y;    
        if(mouseMode==0)
            {
            GLint viewport[4];
	        double modelview[16];
	        double projection[16];
	        float winX, winY, winZ;
	        double posX, posY, posZ;
	        glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	        glGetDoublev( GL_PROJECTION_MATRIX, projection );
	        glGetIntegerv( GL_VIEWPORT, viewport );
	        winX = (float)x;
	        winY = (float)700-(float)y;
	        glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
	        gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	        
	        selected=-1;
	        for(int i=0;i<lightHandler.size();i++)if(lights[lightHandler[i]].enabled)
	            {
                float sX=posX-lights[lightHandler[i]].pos[0];
                float sY=posY-lights[lightHandler[i]].pos[1];
                float sZ=posZ-lights[lightHandler[i]].pos[2];
                if(sX*sX+sY*sY+sZ*sZ<=32*32)
                    {
                    if(x>ww/2)moveLeft=1;
                    else moveLeft=0;
                    selected=i;
                    break;
                    }    
                }
            if(selected==-1)
	           mouseMode=1;
	        else
	           mouseMode=2;
            }
        }
    if(state==GLUT_UP)
        mouseMode=0;
    }

int main(int argc,char *argv[])
    {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(60,50);
    glutInitWindowSize(900,700);
    glutCreateWindow("Shading examples");

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    
    myInit();
    glutMainLoop();
        
    return 0;
    }