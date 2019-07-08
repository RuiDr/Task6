#include<gl/GLUT.H>
#include<stdio.h>
#include<windows.h>
#include<iostream>
#include<math.h>
using namespace std;
GLfloat modelview_matrix[16];
const int win_w = 700, win_h = 700;
GLfloat ex[3] = { 0.0, 0.0, 0.0 }; //观察者位置
GLfloat ey[3] = { 1.0, 0.0, 0.0 };
GLfloat ez[3] = { 4.0, 0.0, 0.0 };
GLfloat fx[3] = { 0.0, 0.0, 0.0 }; //观察对象位置
GLfloat fy[3] = { 0.0, 0.0, 0.0 };
GLfloat fz[3] = { 0.0, 0.0, 0.0 };

GLint view = 0;
GLfloat angle = 0.0f;
void reshape_func(int w,int h);
void display_func();
void drawPig();
void Initial(void);
void onMouse(int button, int state, int x, int y);
void RotateRect();

// 画图函数
void drawPig()
{
	// 旋转设置
	glRotatef(angle, 0, 0, 0);
	glPushMatrix();
	glColor3f(1.0, 0.8, 0.8);
	glutSolidSphere(0.25f, 100, 100);  //身体
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.2, 0);
	glColor3f(1.0, 0.8, 0.8);
	glutSolidSphere(0.25f, 100, 100);  //身体
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.05, 0.08, 0.23);
	glutSolidSphere(0.013f, 1000, 1000);  //左眼
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(-0.05, 0.08, 0.23);
	glutSolidSphere(0.013f, 1000, 1000);  //右眼
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(-0.01, 0.01, 0.23);
	glScaled(1.35, -1.0, 1.0);
	glutSolidSphere(0.06f, 1000, 1000);  //嘴巴
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.5, 0.5);
	glTranslatef(0.0175, 0.01, 0.23);
	glutSolidSphere(0.013f, 1000, 1000);  //左鼻孔
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.5, 0.5);
	glTranslatef(-0.0175, 0.01, 0.23);
	glutSolidSphere(0.013f, 1000, 1000);  //右鼻孔
	glPopMatrix();

	// 绘制三角形，耳朵
	glPushMatrix();
	glColor3f(1.0, 0.5, 0.5);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.2, 0.28, 0.15);
	glVertex3f(0.19, 0.06, 0.15);
	glVertex3f(0.07, 0.21, 0.1);

	glVertex3f(0.2, 0.28, 0.15);
	glVertex3f(0.07, 0.21, 0.1);
	glVertex3f(0.12, 0.2, 0.08);

	glVertex3f(0.2, 0.28, 0.15);
	glVertex3f(0.19, 0.06, 0.15);
	glVertex3f(0.12, 0.2, 0.08);
	glEnd(); //左耳
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.5, 0.5);
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.2, 0.28, 0.15);
	glVertex3f(-0.19, 0.06, 0.15);
	glVertex3f(-0.07, 0.21, 0.1);

	glVertex3f(-0.2, 0.28, 0.15);
	glVertex3f(-0.07, 0.21, 0.1);
	glVertex3f(-0.12, 0.2, 0.08);

	glVertex3f(-0.2, 0.28, 0.15);
	glVertex3f(-0.19, 0.06, 0.15);
	glVertex3f(-0.12, 0.2, 0.08);
	glEnd(); //右耳
	glPopMatrix();

	// 手
	glPushMatrix();
	glColor3f(1.0, 0.5, 0.5);
	glTranslatef(0.30, -0.2, 0.1);
	glScaled(2.0,-1.0,2.0);
	glutSolidSphere(0.035f, 50, 1000);  
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.5, 0.5);
	glTranslatef(-0.30, -0.2, 0.1);
	glScaled(2.0,1.0, 2.0);
	glutSolidSphere(0.035f, 50, 1000);  
	glPopMatrix();
	// 脚
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.07, -0.4, 0.1);
	glutSolidSphere(0.05f, 1000, 1000);  //左脚
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(-0.07, -0.4, 0.1);
	glutSolidSphere(0.05f, 1000, 1000);  //右脚
	glPopMatrix();
}
// 初始化设置
void Initial(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);//白色背景，前3个是RGB，最后是Alpha值，用来控制透明，1.0表示完全不透明
	glShadeModel(GL_FLAT);
    //glMatrixMode(GL_PROJECTION);//OpenGL按照三维方式来处理图像，所以需要一个投影变换将三维图形投影到显示器的二维空间中
	//gluOrtho2D(0.0, 200, 0.0, 150.0);//指定使用正投影将一个x坐标在0~200，y坐标0~150范围内的矩形坐标区域投影到显示器窗口
}
// 设置窗口
void reshape_func(int w,int h)
{
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.0, 100.0);
}
// 显示
void display_func()
{
	// 次数
	//static int z = 0;
	//cout << "显示：" << z++ << " " << endl;

	// 表示要清除颜色缓冲
	glClear(GL_COLOR_BUFFER_BIT);
	// 指定显示模式
	glMatrixMode(GL_MODELVIEW);
	// 恢复初始坐标
	glLoadIdentity();
	// 该函数定义一个视图矩阵，并与当前矩阵相乘
	gluLookAt(ex[view], ey[view], ez[view], fx[view], fy[view], fz[view], 0.0, 1.0, 0.0);

	// 画网格线
	glColor3f(0.82, 0.82, 0.82);
	glBegin(GL_LINES);
	float i, j;
	for (i = -50; i <= 50; i = i + 0.2)
	{
		glVertex3f(i, -0.25, -50);
		glVertex3f(i, -0.25, 50);
	}
	for (j = -50; j <= 50; j += 0.2)
	{
		glVertex3f(-50, -0.25, j);
		glVertex3f(50, -0.25, j);
	}
	glEnd();

	glPushMatrix();
	 drawPig();
	glPopMatrix();

	// 交换两个缓冲区指针
	glutSwapBuffers();
}
/*
	单击左键开始旋转，右键停止
*/
void RotateRect()
{
	angle += 0.5;
	if (angle > 360)
	{
		angle = 0.0f;
	}
	Sleep(30);
	display_func();
}
// 鼠标事件
void onMouse(int button,int state,int x,int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		glutIdleFunc(RotateRect);
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		glutIdleFunc(NULL);
	}
}

int main(int argc, char*argv[])
{
	glutInit(&argc, argv);

	// 设置初始显示模式，指定RGB颜色模式以及指定双缓存窗口
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("一只独立特行的猪");
	// 显示图像
	glutDisplayFunc(display_func);
	// 改变窗口大小时保持图像比例
	glutReshapeFunc(reshape_func);

	// 设置鼠标事件
	glutMouseFunc(onMouse);
	Initial();
	glutMainLoop();
}