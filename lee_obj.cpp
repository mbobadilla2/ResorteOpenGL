#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <math.h>

 float x,pasox;
struct coordinate{
float x,y,z;
coordinate(float a,float b,float c) : x(a),y(b),z(c) {};
};

struct face{
int facenum;
bool four;
int faces[4];
face(int facen,int f1,int f2,int f3) : facenum(facen){
faces[0]=f1;
faces[1]=f2;
faces[2]=f3;
four=false;
}
face(int facen,int f1,int f2,int f3,int f4) : facenum(facen){
faces[0]=f1;
faces[1]=f2;
faces[2]=f3;
faces[3]=f4;
four=true;
}
};

float angle=0.0;

int loadObject(const char* filename)
{
	std::vector<std::string*> coord;
	std::vector<coordinate*> vertex;
	std::vector<face*> faces;
	std::vector<coordinate*> normals;
	std::ifstream in(filename);
	if(!in.is_open())
	{
		std::cout << "Nor oepened" << std::endl;
		return -1;
	}
	char buf[256];

	while(!in.eof())
	{
		in.getline(buf,256);
		coord.push_back(new std::string(buf));
	}

	for(int i=0;i<coord.size();i++)
	{
		if(coord[i]->c_str()[0]=='#')
		continue;
		else if(coord[i]->c_str()[0]=='v' && coord[i]->c_str()[1]==' ')
		{
			float tmpx,tmpy,tmpz;
			sscanf(coord[i]->c_str(),"v %f %f %f",&tmpx,&tmpy,&tmpz); 
			vertex.push_back(new coordinate(tmpx,tmpy,tmpz));
		}else if(coord[i]->c_str()[0]=='v' && coord[i]->c_str()[1]=='n')
		{
			float tmpx,tmpy,tmpz;
			sscanf(coord[i]->c_str(),"vn %f %f %f",&tmpx,&tmpy,&tmpz);
			normals.push_back(new coordinate(tmpx,tmpy,tmpz));
		}else if(coord[i]->c_str()[0]=='f')
		{
			int a,b,c,d,e;
			if(count(coord[i]->begin(),coord[i]->end(),' ')==3)
			{
				sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b);
				faces.push_back(new face(b,a,c,d));
			}else
			{
				sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b,&e,&b);
				faces.push_back(new face(b,a,c,d,e));
			}
		}
	}

	int num;
	num=glGenLists(1);
	glNewList(num,GL_COMPILE);
	for(int i=0;i<faces.size();i++)
	{
		if(faces[i]->four)
		{
			glBegin(GL_QUADS);

			glNormal3f(normals[faces[i]->facenum-1]->x,normals[faces[i]->facenum-1]->y,normals[faces[i]->facenum-1]->z);

			glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);
			glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
			glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
			glVertex3f(vertex[faces[i]->faces[3]-1]->x,vertex[faces[i]->faces[3]-1]->y,vertex[faces[i]->faces[3]-1]->z);
			glEnd();
		}else
		{
			glBegin(GL_TRIANGLES);
			glNormal3f(normals[faces[i]->facenum-1]->x,normals[faces[i]->facenum-1]->y,normals[faces[i]->facenum-1]->z);
			glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);
			glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
			glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
			glEnd();
		}
	}
	glEndList();

	for(int i=0;i<coord.size();i++)
	delete coord[i];
	for(int i=0;i<faces.size();i++)
	delete faces[i];
	for(int i=0;i<normals.size();i++)
	delete normals[i];
	for(int i=0;i<vertex.size();i++)
	delete vertex[i];
	return num;
}

int cube;
void init()
{
	glClearColor(0.0,0.0,0.0,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,640.0/480.0,1.0,500.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	
	cube=loadObject("resorte.obj");
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float col[]={1.0,1.0,1.0,1.0};
	glLightfv(GL_LIGHT0,GL_DIFFUSE,col);
	x=-5;
	pasox=0.1;
}
float xc=2, incX = -0.01;
void display()
{   
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	float pos[]={-1.0,1.0,-2.0,1.0},y;
	glLightfv(GL_LIGHT0,GL_POSITION,pos);
	y=3*sin(((x*2*3.1416)/(5)));
	glTranslatef(0,0,-30.0);
//	glRotatef(angle,0.0,1.0,0.0);
	glPolygonMode(GL_FRONT, GL_FILL);
    glColorMaterial (GL_FRONT, GL_AMBIENT);	
    glEnable(GL_COLOR_MATERIAL);
	glColor3f(1,0.0,0.0);
	glScalef(1,xc,1);
    glCallList(cube);
	xc+=incX;
	if(xc<=0.5 || xc >= 2){incX*=-1;}
}


int main(int argc,char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE|SDL_OPENGL);
	bool running=true;
	Uint32 start;
	SDL_Event event;
	init();
	while(running)
	{
		start=SDL_GetTicks();
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				running=false;
			break;
			}
		}
		display();
		SDL_GL_SwapBuffers();
		x+=pasox;
		if(x>=5)
		{ pasox=-pasox; x=5+2*pasox;
		};
		if (x<=-5)
		{ pasox=-pasox; x=-5+2*pasox;
		};
		
		angle+=0.5;
		if(angle>360)
		angle-=360;
		if(1000/30>(SDL_GetTicks()-start))
		SDL_Delay(1000/30-(SDL_GetTicks()-start));
		
	}
	SDL_Quit();
	return 0;
}
