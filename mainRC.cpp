#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "glut.h"
#include "Matrices.h"

struct ImageStr
{
	float *image;
	int    width;
	int    height;
};
#define EPSILON 0.000001
#define WIDTH       100
#define HEIGTH      100
#define WIDTH_W     500
#define HEIGTH_W    600
#define WIDTH_DST   400
#define HEIGHT_DST  400

ImageStr imageSrcStr;
ImageStr imageDstStr;
float   Image[WIDTH * HEIGTH * 4];
float   imageDst[WIDTH_DST * HEIGHT_DST * 4];
const int Dim[] = { WIDTH / 2, WIDTH / 2, WIDTH / 2 };     //体数据大小
const int Dim0 = Dim[0];
const int Dim1 = Dim[1];
const int Dim2 = Dim[2];
float   CData[WIDTH / 2 * WIDTH / 2 * WIDTH / 2 * 4];
bool    mouseLeftDown;
bool    mouseRightDown;
float   mouseX;
float   mouseY;
float   cameraAngleX;
float   cameraAngleY;
float   cameraAngleZ;
float   cameraDistance;
Matrix4 matrixView;
Matrix4 matrixRotate;

void GenerateVolume(int *Data, const int *Dim);//生成体数据
void GenCube(int x, int y, int z, int side, int density, int *Data, const int *Dim);//生成正方体数据
void GenSphere(int x, int y, int z, int radius, int density, int *Data, const int *Dim);//生成球体数据
void Classify(float* CData, int *Data, const int *Dim);//数据分类
void RotationMatrix(float *R, float *eye, float *center, float *up, float *T);//求取图像空间到物体空间变换的旋转矩阵
void Composite(float *rgba, int x0, int y0, float *CData, const int *Dim, const float *R);//合成像素颜色值
bool Intersection(float *startpos, float *pos, float *dir, const int *Dim);//求光线与包围盒交点坐标
void TrInterpolation(float *rgba, float *pos, float *CData, const int *Dim);//三线性插值
bool CheckinBox(float *point, const int *Dim);         //判断点是否在包围盒内
void MatrixmulVec(float *c, const float *a, float *b);  //矩阵向量乘积
void CrossProd(float *c, float *a, float *b);     //向量叉乘
void Normalize(float *norm, float *a);           //向量归一化
void GeneratePicture(const float *r, float *image, float *cdata, const int *dim);
void Zoom(const ImageStr *imagedst, ImageStr *imagesrc);

void keyboardCB(unsigned char key, int x, int y);
void mouseCB(int button, int stat, int x, int y);
void mouseMotionCB(int x, int y);
void timerCB(int millisec);
void Rotate(void);
void Mydisplay(void);                               //显示图像

int main(int argc, char **argv)
{
	int *Data = (int *)malloc(sizeof(int) * Dim[0] * Dim[1] * Dim[2]);
	mouseLeftDown = mouseRightDown = false;
	mouseX = mouseY = 0;
	cameraAngleX = cameraAngleY = 0;
	cameraDistance = 200;
	imageSrcStr.image = Image;
	imageSrcStr.width = WIDTH;
	imageSrcStr.height = HEIGTH;
	imageDstStr.image = imageDst;
	imageDstStr.width = WIDTH_DST;
	imageDstStr.height = HEIGHT_DST;

	matrixView.identity();
	matrixView.rotate(30, 1, 0, 0);
	matrixView.rotate(30, 0, 1, 0);
	matrixView.translate(0, 0, -cameraDistance);
	matrixView.invertOrtho();

	GenerateVolume(Data, Dim);//生成原始体数据
	Classify(CData, Data, Dim);//对体数据分类
	GeneratePicture(matrixView.get(), Image, CData, Dim);

	free(Data);

	//使用OpenGL显示此二维图像
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH_W, HEIGTH_W);
	glutInitWindowPosition(600, 200);
	glutCreateWindow("Ray-Casting");
	glClearColor(1, 1, 1, 1);      //背景设为白色

	float ww = WIDTH_W, hw = HEIGTH_W, wp = WIDTH_DST, hp = HEIGHT_DST;
	glRasterPos2f((ww - wp) / ww - 1, (hw - hp) / hw - 1);
	glutDisplayFunc(Mydisplay); //显示图像
	glutTimerFunc(30, timerCB, 30);

	glutKeyboardFunc(keyboardCB);
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);

	glutMainLoop();
}

void Rotate(void)
{
	matrixRotate.identity();
	matrixRotate.translate(0, 0, -cameraDistance);
	matrixRotate.transpose();
	matrixView = matrixRotate * matrixView;

	matrixRotate.identity();
	matrixRotate.rotate(cameraAngleX, 1, 0, 0);
	matrixRotate.rotate(cameraAngleY, 0, 1, 0);
	matrixRotate.rotate(cameraAngleZ, 0, 0, 1);
	matrixView = matrixRotate * matrixView;

	matrixRotate.identity();
	matrixRotate.translate(0, 0, cameraDistance);
	matrixRotate.transpose();
	matrixView = matrixRotate * matrixView;
	cameraAngleX = 0;
	cameraAngleY = 0;
	cameraAngleZ = 0;

	GeneratePicture(matrixView.get(), Image, CData, Dim);
}

void mouseCB(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
			mouseLeftDown = false;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}
}

void mouseMotionCB(int x, int y)
{
	if (mouseLeftDown)
	{
		cameraAngleY = (x - mouseX);
		cameraAngleX = (y - mouseY);
		mouseX = x;
		mouseY = y;
	}
	if (mouseRightDown)
	{
		matrixRotate.identity();
		matrixRotate.translate(0, 0, -cameraDistance);
		matrixRotate.transpose();
		matrixView = matrixRotate * matrixView;

		cameraAngleZ = (y - mouseY);
		mouseY = y;
		cameraDistance += (x - mouseX);
		mouseX = x;

		matrixRotate.identity();
		matrixRotate.translate(0, 0, cameraDistance);
		matrixRotate.transpose();
		matrixView = matrixRotate * matrixView;
	}
}

void keyboardCB(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'x':
		cameraAngleX = 3;
		break;
	case 'X':
		cameraAngleX = -3;
		break;
	case 'y':
		cameraAngleY = 3;
		break;
	case 'Y':
		cameraAngleY = -3;
		break;
	case 'z':
		cameraAngleZ = 3;
		break;
	case 'Z':
		cameraAngleZ = -3;
		break;
	case 27:              // ESCAPE
		exit(0);
		break;
	default:
		break;
	}
}

//显示函数
void Mydisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	Rotate();
	glDrawPixels(WIDTH_DST, HEIGHT_DST, GL_RGBA, GL_FLOAT, imageDstStr.image);//使用OpenGL的绘图函数
	glFlush();
}

void timerCB(int millisec)
{
	glutTimerFunc(millisec, timerCB, millisec);
	glutPostRedisplay();
}

void GeneratePicture(const float *r, float *image, float *cdata, const int *dim)
{
	float *linepx = image;

	for (int j = -HEIGTH / 2; j < HEIGTH / 2; j++)//逐个合成像素值
	{
		for (int i = -WIDTH / 2; i < WIDTH / 2; i++)
		{
			Composite(linepx, i, j, cdata, dim, r);
			linepx += 4;
		}
	}
	Zoom(&imageDstStr, &imageSrcStr);
}

void Zoom(const ImageStr *imagedst, ImageStr *imagesrc)
{
	float rw, rh;

	rw = imagesrc->width / (float)imagedst->width;
	rh = imagesrc->height / (float)imagedst->height;

	for (int i = 0; i < imagedst->height; i++)
	{
		for (int j = 0; j < imagedst->width; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				imagedst->image[4 * (i * imagedst->width + j) + k]
					= imagesrc->image[4 * ((int)(rh * i) * imagesrc->width + (int)(rw * j)) + k];
			}
		}
	}
}

//生成体数据
//生成一个大正方体，内部包含一个球体，球体中间又包含一个小正方体
//Data:体积数据
//Dim:体数据大小
void GenerateVolume(int *Data, const int *Dim)
{
	GenCube(-Dim[0] / 2, -Dim[1] / 2, -Dim[2] / 2, Dim[0], 100, Data, Dim);  //大正方体
	GenSphere(0, 0, 0, Dim[0] / 3, 200, Data, Dim);      //球体
	GenCube(-Dim[0] / 8, -Dim[0] / 8, -Dim[0] / 8, Dim[0] / 4, 300, Data, Dim);     //小正方体
}

//生成正方体数据
//x,y,z:正方体左下角坐标
//side:正方体边长
//density:正方体对应的标量值
//Data:体积数据
//Dim:体数据大小
void GenCube(int x, int y, int z, int side, int density, int *Data, const int *Dim)
{
	int max_x = x + side;
	int max_y = y + side;
	int max_z = z + side;
	int Dimxy = Dim[0] * Dim[1];
	int xoffset = Dim[0] / 2;
	int yoffset = Dim[1] / 2;
	int zoffset = Dim[2] / 2;

	for (int k = z; k < max_z; k++)
	{
		for (int j = y; j < max_y; j++)
		{
			for (int i = x; i < max_x; i++)
			{
				Data[(k + zoffset) * Dimxy + (j + yoffset) * Dim[0] + (i + xoffset)] = density;
			}
		}
	}
}

//生成球体数据
//x,y,z:球心坐标
//radius:球半径
//density:球体对应的标量值
//Data:体积数据
//Dim:体数据大小
void GenSphere(int x, int y, int z, int radius, int density, int *Data, const int *Dim)
{
	int radius2 = radius * radius;
	int Dimxy = Dim[0] * Dim[1];
	int xoffset = Dim[0] / 2;
	int yoffset = Dim[1] / 2;
	int zoffset = Dim[2] / 2;

	for (int k = -zoffset; k < zoffset; k++)
	{
		for (int j = -yoffset; j < yoffset; j++)
		{
			for (int i = -xoffset; i < xoffset; i++)
			{
				if ((i - x) * (i - x) + (j - y) * (j - y) + (k - z) * (k - z) <= radius2)
				{
					Data[(k + zoffset) * Dimxy + (j + yoffset) * Dim[0] + (i + xoffset)] = density;
				}
			}
		}
	}
}

//数据分类
//将原始体数据的标量值映射为颜色和不透明度
//这里处理的比较简单，直接将之前生成的数据分三类：大正方体白色、球体红色、小正方体黄色
//CData:分类后体数据
//Data:原始体数据
//Dim:体数据大小
void Classify(float *CData, int *Data, const int *Dim)
{
	int *LinePS = Data;
	float *LinePD = CData;

	for (int k = 0; k < Dim[2]; k++)
	{
		for (int j = 0; j < Dim[1]; j++)
		{
			for (int i = 0; i < Dim[0]; i++)
			{
				if (LinePS[0] <= 100)
				{
					//白色
					LinePD[0] = 1.0;
					LinePD[1] = 1.0;
					LinePD[2] = 1.0;
					LinePD[3] = 0.02;
				}
				else if (LinePS[0] <= 200)
				{
					//红色
					LinePD[0] = 1.0;
					LinePD[1] = 0.0;
					LinePD[2] = 0.0;
					LinePD[3] = 0.05;
				}
				else
				{
					//黄色
					LinePD[0] = 1.0;
					LinePD[1] = 1.0;
					LinePD[2] = 0.0;
					LinePD[3] = 0.2;
				}
				if (j < Dim[1] / 10 && k < Dim[2] / 10)
				{
					//x轴红色
					LinePD[0] = 1.0;
					LinePD[1] = 0.0;
					LinePD[2] = 0.0;
					LinePD[3] = 0.2;
				}
				if (i < Dim[0] / 10 && k < Dim[2] / 10)
				{
					//y轴绿色
					LinePD[0] = 0.0;
					LinePD[1] = 1.0;
					LinePD[2] = 0.0;
					LinePD[3] = 0.2;
				}
				if (i < Dim[0] / 10 && j < Dim[1] / 10)
				{
					//z轴蓝色
					LinePD[0] = 0.0;
					LinePD[1] = 0.0;
					LinePD[2] = 1.0;
					LinePD[3] = 0.2;
				}
				LinePD += 4;
				LinePS++;
			}
		}
	}
}

//求取从图像空间到物体空间变换的旋转矩阵
//功能类似于OpenGL中的gluLookAt函数
//参考：http://blog.csdn.net/popy007/article/details/5120158
//R:旋转矩阵
//eye:视点位置
//center:物体参考点位置
//up:相机朝上的方向
void RotationMatrix(float *R, float *eye, float *center, float *up, float *T)
{
	float XX[3], YY[3], ZZ[3];      //图像空间的基向量
	ZZ[0] = eye[0] - center[0];
	ZZ[1] = eye[1] - center[1];
	ZZ[2] = eye[2] - center[2];
	CrossProd(XX, up, ZZ);
	CrossProd(YY, ZZ, XX);
	Normalize(XX, XX);
	Normalize(YY, YY);
	Normalize(ZZ, ZZ);
	//由图像空间基向量构成旋转矩阵
	R[0] = XX[0]; R[1] = YY[0]; R[2] = ZZ[0]; R[3] = T[0];
	R[4] = XX[1]; R[5] = YY[1]; R[6] = ZZ[1]; R[7] = T[1];
	R[8] = XX[2]; R[9] = YY[2]; R[10] = ZZ[2]; R[11] = T[2];
}

//合成像素值
//rgba:合成颜色值
//x0,y0:二维图像像素坐标
//CData:分类后体数据
//Dim:体数据大小
//R:旋转矩阵（换用于图像空间到物体空间的转换）
//T:平移向量（同上）
void Composite(float *rgba, int x0, int y0, float *CData, const int *Dim, const float *R)
{
	int stepsize = 1;                                       //采样步长
	float cumcolor[4];                                      //累计颜色值
	cumcolor[0] = cumcolor[1] = cumcolor[2] = cumcolor[3] = 0.0;
	float pos[3], dir[3];                                   //投射光线起点、方向
	float startpos[3];                                      //光线与包围盒近视点处的交点坐标
	float samplepos[3];                                     //采样点坐标
	float samplecolor[4];                                   //采样点颜色
	//采用平行投影，故在图像空间中投射光线的方向(0,0,-1),起点(x0,y0,0)
	pos[0] = x0;
	pos[1] = y0;
	pos[2] = 0;
	//将光线描述转换到物体空间
	dir[0] = -R[2];
	dir[1] = -R[6];
	dir[2] = -R[10];                                        //光线方向在物体空间的表达
	MatrixmulVec(pos, R, pos);

	if (Intersection(startpos, pos, dir, Dim))               //判断光线与包围盒是否相交
	{
		samplepos[0] = startpos[0];
		samplepos[1] = startpos[1];
		samplepos[2] = startpos[2];
		while (CheckinBox(samplepos, Dim) && cumcolor[3] < 1)        //当光线射出包围盒或累计不透明度超过1时中止合成
		{
			TrInterpolation(samplecolor, samplepos, CData, Dim);    //三线性插值获得采样点处的颜色及不透明度
			//合成颜色及不透明度,采用的是从前到后的合成公式
			cumcolor[0] += samplecolor[0] * samplecolor[3] * (1 - cumcolor[3]);//R
			cumcolor[1] += samplecolor[1] * samplecolor[3] * (1 - cumcolor[3]);//G
			cumcolor[2] += samplecolor[2] * samplecolor[3] * (1 - cumcolor[3]);//B
			cumcolor[3] += samplecolor[3] * (1 - cumcolor[3]);				//A

			//            cout[0] = cout[0] * (1 - cumcolor[3]) + samplecolor[0] * samplecolor[3];
			//            cout[1] = cout[1] * (1 - cumcolor[3]) + samplecolor[1] * samplecolor[3];
			//            cout[2] = cout[2] * (1 - cumcolor[3]) + samplecolor[2] * samplecolor[3];
			//            cout[3] *= (1 - cumcolor[3]);

			samplepos[0] += dir[0] * stepsize;
			samplepos[1] += dir[1] * stepsize;
			samplepos[2] += dir[2] * stepsize;
			//            K++;
		}
		rgba[0] = cumcolor[0];
		rgba[1] = cumcolor[1];
		rgba[2] = cumcolor[2];
		rgba[3] = cumcolor[3];

		//        rgba[0] = cout[0];
		//        rgba[1] = cout[1];
		//        rgba[2] = cout[2];
		//        rgba[3] = 1 - cout[3];

		return;
	}
	rgba[0] = rgba[1] = rgba[2] = rgba[3] = 0;      //若光线与包围盒不相交，赋白色
}

//判断投射光线与包围盒是否相交（若相交，求靠近视点处的交点坐标）
//思路：将包围盒6个面无限扩展，并分成3组，即平行于XOY,YOZ,ZOX平面的各有2个；
//求光线与6个平面的交点，从每组的2个交点中选出距离视点较近者，这样得到3个候
//选交点；从这3个候选交点中选出距离视点最远的那个。最后判断这个点是否落在包
//围盒内，若在，即为光线与包围盒的靠近视点处的交点。
//stratpos:靠近视点处的交点坐标
//pos:光线起点坐标
//dir:光线方向向量
//Dim:包围盒右上角坐标（左下角坐标为（0,0,0））
bool Intersection(float *startpos, float *pos, float *dir, const int *Dim)
{
	float nearscale = -1000000;
	float scale1, scale2;
	//光线与包围盒平行于YOZ的2个平面交点
	if ((dir[0] <= -EPSILON) || (dir[0] >= EPSILON))
	{
		scale1 = (-Dim[0] / 2 - pos[0]) / dir[0];
		scale2 = (Dim[0] / 2 - 1 - pos[0]) / dir[0];
		//选出靠近视点的交点，并与当前候选点比较，保留较远者
		if (scale1 < scale2)
		{
			if (scale1 > nearscale)
			{
				nearscale = scale1;
			}
		}
		else
		{
			if (scale2 > nearscale)
			{
				nearscale = scale2;
			}
		}
	}
	//光线与包围盒平行于ZOX的2个平面交点
	if ((dir[1] <= -EPSILON) || (dir[1] >= EPSILON))
	{
		scale1 = (-Dim[1] / 2 - pos[1]) / dir[1];
		scale2 = (Dim[1] / 2 - 1 - pos[1]) / dir[1];
		//选出靠近视点的交点，并与当前候选点比较，保留较远者
		if (scale1 < scale2)
		{
			if (scale1 > nearscale)
			{
				nearscale = scale1;
			}
		}
		else
		{
			if (scale2 > nearscale)
			{
				nearscale = scale2;
			}
		}
	}
	//光线与包围盒平行于XOY的2个平面交点
	if ((dir[2] <= -EPSILON) || (dir[2] >= EPSILON))
	{
		scale1 = (-Dim[2] / 2 - pos[2]) / dir[2];
		scale2 = (Dim[2] / 2 - 1 - pos[2]) / dir[2];
		//选出靠近视点的交点，并与当前候选点比较，保留较远者
		if (scale1 < scale2)
		{
			if (scale1 > nearscale)
			{
				nearscale = scale1;
			}
		}
		else
		{
			if (scale2 > nearscale)
			{
				nearscale = scale2;
			}
		}
	}
	startpos[0] = pos[0] + nearscale * dir[0];
	startpos[1] = pos[1] + nearscale * dir[1];
	startpos[2] = pos[2] + nearscale * dir[2];
	return CheckinBox(startpos, Dim);                    //判断该点是否在包围盒内
}

//三线性插值
//rgba:插值结果
//pos:采样点坐标
//CData:分类后体数据
//Dim:体数据大小
void TrInterpolation(float *rgba, float *pos, float *CData, const int *Dim)
{
	int x0, y0, z0, x1, y1, z1;
	float fx, fy, fz;
	float v0, v1, v2, v3, v4, v5, v6;
	int Slicesize = Dim[0] * Dim[1] * 4;
	int Stepsize = Dim[0] * 4;

	//整数部分
	x0 = (int)(pos[0] + Dim[0] / 2);
	y0 = (int)(pos[1] + Dim[1] / 2);
	z0 = (int)(pos[2] + Dim[2] / 2);
	//小数部分
	fx = (pos[0] + Dim[0] / 2) - x0;
	fy = (pos[1] + Dim[1] / 2) - y0;
	fz = (pos[2] + Dim[2] / 2) - z0;
	x1 = x0 + 1;
	y1 = y0 + 1;
	z1 = z0 + 1;
	//防止越界
	if (x1 >= Dim[0]) x1 = Dim[0] - 1;
	if (y1 >= Dim[1]) y1 = Dim[1] - 1;
	if (z1 >= Dim[2]) z1 = Dim[2] - 1;
	for (int i = 0; i < 4; i++)
	{
		//采样点处的值由邻近的8个点插值获得
		v0 = CData[z0 * Slicesize + y0 * Stepsize + 4 * x0 + i] * (1 - fx) + CData[z0 * Slicesize + y0 * Stepsize + 4 * x1 + i] * fx;
		v1 = CData[z0 * Slicesize + y1 * Stepsize + 4 * x0 + i] * (1 - fx) + CData[z0 * Slicesize + y1 * Stepsize + 4 * x1 + i] * fx;
		v2 = CData[z1 * Slicesize + y0 * Stepsize + 4 * x0 + i] * (1 - fx) + CData[z1 * Slicesize + y0 * Stepsize + 4 * x1 + i] * fx;
		v3 = CData[z1 * Slicesize + y1 * Stepsize + 4 * x0 + i] * (1 - fx) + CData[z1 * Slicesize + y1 * Stepsize + 4 * x1 + i] * fx;
		v4 = v0 * (1 - fy) + v1 * fy;
		v5 = v2 * (1 - fy) + v3 * fy;
		v6 = v4 * (1 - fz) + v5 * fz;
		if (v6 > 1)
		{
			v6 = 1; //防止越界
		}
		rgba[i] = v6;
	}
}

//判断点是否在包围盒内
//point:点坐标
//Dim:包围盒右上角坐标（左下角坐标为（0,0,0））
bool CheckinBox(float *point, const int *Dim)
{
	if (point[0] < -Dim[0] / 2 || point[0] >= Dim[0] / 2 || point[1] < -Dim[1] / 2 || point[1] >= Dim[1] / 2 || point[2] < -Dim[2] / 2 || point[2] >= Dim[2] / 2)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//矩阵与向量乘积
//c=a*b
//c:输出向量
//a:输入矩阵
//b:输入向量
void MatrixmulVec(float *c, const float *a, float *b)
{
	float x, y, z;
	x = a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3];
	y = a[4] * b[0] + a[5] * b[1] + a[6] * b[2] + a[7];
	z = a[8] * b[0] + a[9] * b[1] + a[10] * b[2] + a[11];
	c[0] = x;
	c[1] = y;
	c[2] = z;
}

//向量叉乘
//c=a x b
//c:输出向量
//a:输入向量
//b:输入向量
void CrossProd(float *c, float *a, float *b)
{
	float x, y, z;
	x = a[1] * b[2] - b[1] * a[2];
	y = a[2] * b[0] - b[2] * a[0];
	z = a[0] * b[1] - b[0] * a[1];
	c[0] = x;
	c[1] = y;
	c[2] = z;
}

//向量归一化
//norm:归一化结果
//a:输入向量
void Normalize(float *norm, float *a)
{
	float len = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	norm[0] = a[0] / len;
	norm[1] = a[1] / len;
	norm[2] = a[2] / len;
}
