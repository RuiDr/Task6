#include<gl/GLUT.H>
#include<stdio.h>
#include<windows.h>
#include<iostream>
#include<math.h>
using namespace std;
static bool l_button_down = false, r_button_down = false, mid_button_down = false;
static int last_x = -1, last_y = -1;
#define  GLUT_WHEEL_UP		3 // 滚轮操作  
#define  GLUT_WHEEL_DOWN	4
GLfloat modelview_matrix[16];
GLfloat theta[3] = { 0.0,0.0,0.0 };
const int n = 1000;
const GLfloat Pi = 3.1415926536F;
const int win_w = 700, win_h = 700;
GLfloat ex[3] = { 0.0, 0.0, 0.0 }; //观察者位置
GLfloat ey[3] = { 1.0, 0.0, 0.0 };
GLfloat ez[3] = { 4.0, 0.0, 0.0 };
GLfloat fx[3] = { 0.0, 0.0, 0.0 }; //观察对象位置
GLfloat fy[3] = { 0.0, 0.0, 0.0 };
GLfloat fz[3] = { 0.0, 0.0, 0.0 };
GLfloat default_matrix[16];
GLfloat modelview_z_dis;
GLfloat ax = -1.0f; //第一只猪的初始坐标
GLfloat ay = 0.0f;
GLfloat az = 0.0f;
GLfloat mx = 1.0f; //第一只猪缩放倍数
GLfloat my = 1.0f;
GLfloat mz = 1.0f;
GLint view = 0;
GLfloat angle = 0.0f;
void reshape_func(int w,int h);
void display_func();
void drawPig();
void Initial(void);
void onMouse(int button, int state, int x, int y);
void RotateRect();
void absolute_default();
void absolute_scale(GLfloat factor);
void mouse_click_func(int button, int state, int x, int y);
void mouse_move_func(int x, int y);
void absolute_translate(GLfloat x, GLfloat y, GLfloat z);
void absolute_rotate(GLfloat dgree, GLfloat vecx, GLfloat vecy, GLfloat vecz);

void absolute_translate(GLfloat x, GLfloat y, GLfloat z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, z);
	glMultMatrixf(modelview_matrix); // 使变换矩阵左乘到当前矩阵，这样才适合绝对坐标的考虑
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	glPopMatrix();
}
void absolute_rotate(GLfloat dgree, GLfloat vecx, GLfloat vecy, GLfloat vecz)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(.0, .0, -modelview_z_dis);		// 平移回去，注意该句和后两句要倒序来看
	glRotatef(dgree, vecx, vecy, vecz);// 积累旋转量
	glTranslatef(.0, .0, modelview_z_dis);		// 先平移到原点
	glMultMatrixf(modelview_matrix); // 使变换矩阵左乘到当前矩阵，这样才适合绝对坐标的考虑
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	glPopMatrix();
}
void mouse_move_func(int x, int y)
{
	y = win_h - y;
	if (last_x >= 0 && last_y >= 0 && (last_x != x || last_y != y)) {
		GLfloat deltax = GLfloat(x - last_x), deltay = GLfloat(y - last_y);
		if (mid_button_down) {
			absolute_translate(deltax * .1f, deltay * .1f, .0f);
			glutPostRedisplay();
		}
		else if (l_button_down) {
			GLfloat dis = sqrt(deltax*deltax + deltay * deltay);
			absolute_rotate(dis, -deltay / dis, deltax / dis, .0);
			glutPostRedisplay();
		}
	}
	last_x = x; last_y = y;
}
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
	glutSolidSphere(0.06f, 1000, 1000);  //嘴巴
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.5, 0.5);
	glTranslatef(0.02, 0.01, 0.23);
	glutSolidSphere(0.013f, 1000, 1000);  //左鼻孔
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.5, 0.5);
	glTranslatef(-0.02, 0.01, 0.23);
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
	glutSolidSphere(0.05f, 50, 1000);  
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.5, 0.5);
	glTranslatef(-0.30, -0.2, 0.1);
	glScaled(2.0,1.0, 2.0);
	glutSolidSphere(0.05f, 50, 1000);  
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

	//glMatrixMode(GL_MODELVIEW);
	
	//glColor3f(0.0f, 0.0f, 0.0f);
	//glRectf(50.0f, 100.0f, 150.0f, 50.0f);//图形的坐标，绘制一个左上角在（50，100），右下角在（150，50）的矩形
	// 指定哪个矩阵是当前矩阵
	// GL_PROJECTION 投影, GL_MODELVIEW 模型视图, GL_TEXTURE 纹理
	// ?
	//glLoadMatrixf(modelview_matrix);
	// 画图函数	

	glPushMatrix();
	 drawPig();
	glPopMatrix();

	// 交换两个缓冲区指针
	glutSwapBuffers();
}
/*
	单击左键开始旋转，右键暂停
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

void absolute_default()
{
	memcpy(modelview_matrix, default_matrix, sizeof(default_matrix));
}
void absolute_scale(GLfloat factor)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(.0, .0, -modelview_z_dis);		// 平移回去，注意该句和后两句要倒序来看
	glScalef(factor, factor, factor);
	glTranslatef(.0, .0, modelview_z_dis);		// 先平移到原点
	glMultMatrixf(modelview_matrix); // 使变换矩阵左乘到当前矩阵，这样才适合绝对坐标的考虑
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	glPopMatrix();
}
void mouse_click_func(int button, int state, int x, int y)
{
	y = win_h - y;
	switch (button) {
		// 左按钮
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			l_button_down = true;
			last_x = x; last_y = y;
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		}
		else {
			l_button_down = false;
			last_x = -1; last_y = -1;
			glutSetCursor(GLUT_CURSOR_INHERIT);
		}
		break;
		// 中间
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN) {
			mid_button_down = true;
			last_x = x; last_y = y;
			glutSetCursor(GLUT_CURSOR_CYCLE);

		}
		else {
			mid_button_down = false;
			last_x = -1; last_y = -1;
			glutSetCursor(GLUT_CURSOR_INHERIT);
		}
		break;
		// 右边
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			r_button_down = true;
			absolute_default();
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			glutPostRedisplay();
		}
		else {
			r_button_down = false;
			glutSetCursor(GLUT_CURSOR_INHERIT);
		}
		break;
		// 转动
	case GLUT_WHEEL_UP:
		if (state == GLUT_UP) {
			absolute_scale(.9f);
			glutPostRedisplay();
		}
		break;
		// 向下转动
	case GLUT_WHEEL_DOWN:
		if (state == GLUT_UP) {
			absolute_scale(1.1f);
			glutPostRedisplay();
		}
		break;
	}
}
int main(int argc, char*argv[])
{
	glutInit(&argc, argv);
	// 设置初始显示模式，指定RGB颜色模式以及指定双缓存窗口
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("CrazyCake的第一个opengl程序");
	// 显示图像
	glutDisplayFunc(display_func);
	// 改变窗口大小时保持图像比例
	glutReshapeFunc(reshape_func);

	// 设置鼠标事件
	glutMouseFunc(onMouse);
	glutMotionFunc(mouse_move_func);
	Initial();
	glutMainLoop();
}