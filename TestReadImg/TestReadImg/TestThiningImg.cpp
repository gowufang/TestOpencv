//基于索引表的图像细化算法
#include "stdafx.h"

#include "highgui.h"
#include "cv.h"//包含cvCreateStructuringElementEx();颜色空间转换函数cvCvtColor()需要
#include "cxcore.h"

#include <stdlib.h>
#include <string.h>

//基于索引表的细化细化算法
//功能：对图象进行细化
//参数：lpDIBBits：代表图象的一维数组
//      lWidth：图象高度
//      lHeight：图象宽度


//                           代表图象的一维数组      图象宽度     图象高度
bool ThiningDIBSkeleton(unsigned char* imagedata, int lWidth, int lHeight)
{
	/* 	unsigned char* imagedata;
	imagedata = new uchar[sizeof(char) * src->width * src->height]();
	*/
	/*
	deletemark[256]为前人据8领域总结的是否可以被删除的256种情况
	不可以删除用0来表示，能被删除的用1来表示
	*/
	unsigned char deletemark[256] = {
		0, 0, 0, 0, 0, 0, 0, 1,     0, 0, 1, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 1, 1, 1, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0,     1, 0, 0, 0, 1, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0,     1, 0, 1, 1, 1, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,     1, 0, 0, 0, 1, 0, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0,     1, 0, 1, 1, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 0, 1, 1,     0, 0, 0, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 1, 0, 0, 1, 1,
		1, 1, 0, 1, 0, 0, 0, 1,     0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 0, 1, 0, 0, 0, 1,     1, 1, 0, 0, 1, 0, 0, 0,
		0, 1, 1, 1, 0, 0, 1, 1,     0, 0, 0, 1, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 1, 1, 1,
		1, 1, 1, 1, 0, 0, 1, 1,     1, 1, 0, 0, 1, 1, 0, 0,
		1, 1, 1, 1, 0, 0, 1, 1,     1, 1, 0, 0, 1, 1, 0, 0
	};//索引表

	  //循环变量
	  //long i;
	  //long j;


	unsigned char p0, p1, p2, p3, p4, p5, p6, p7;

	unsigned char *pmid, *vergeMark;    // pmid 用来指向二值图像  vergeMark用来指向存放是否为边缘

	unsigned char sum;

	bool bStart = true;

	//动态创建一个长度为sizeof(char) * src->width * src->height的数组
	//并且初始化把数组元素都设置为0
	//长度与imagedata长度一样
	long lLength;
	lLength = lWidth * lHeight; //数组长度
	unsigned char *pTemp = new uchar[sizeof(unsigned char) * lLength]();

	//    P0 P1 P2
	//    P7    P3
	//    P6 P5 P4

	while (bStart)
	{
		bStart = false;
		//每次循环pTemp所有元素都置0
		memset(pTemp, 0, lLength);//初始化pTemp指向的lLength空间全部初始化为0

								  /*---------------首先求边缘点----------------*/
		pmid = (unsigned char *)imagedata + lWidth + 1; // pmid 用来指向二值图像矩阵的第二行	  +1 表示不考虑图像第一行
		vergeMark = (unsigned char *)pTemp + lWidth + 1; //pmidtemp用来指向存放是否为边缘 如果是边缘点 则将其设为1

		for (int i = 1; i < lHeight - 1; i++)  //lHeight -1表示不考虑图像最后一行 第一列
		{
			for (int j = 1; j < lWidth - 1; j++)  //lWidth - 1表示不考虑图像最后一列
			{
				//图像已经是01化，如果是0我们不考虑
				if (*pmid == 0)
				{
					pmid++;
					vergeMark++;
					continue;
				}
				//如果是1，是我们考虑的点 我们需要对周围8个进行判断是否边缘
				p0 = *(pmid - lWidth - 1);
				p1 = *(pmid - lWidth);                  //    P0 P1 P2
				p2 = *(pmid + 1 - lWidth);              //    P7    P3
				p3 = *(pmid + 1);						//    P6 P5 P4
				p4 = *(pmid + lWidth + 1);
				p5 = *(pmid + lWidth);
				p6 = *(pmid + lWidth - 1);
				p7 = *(pmid - 1);
				//p0--到---p7的值不是0就是1
				sum = p0 & p1 & p2 & p3 & p4 & p5 & p6 & p7;//如果是边缘，肯定周围的P0P1P2P3P4P5P6P7 中一定至少有一个为0
				if (sum == 0)
				{
					*vergeMark = 1;// 表记边缘
				}

				pmid++;
				vergeMark++;
			}
			pmid++;//跳过图像最后一列，不考虑
			vergeMark++;

			pmid++;//跳过图像第一列，不考虑
			vergeMark++;
		}

		/*---------------沿着边缘现在开始删除----------------*/
		pmid = (unsigned char *)imagedata + lWidth + 1;
		vergeMark = (unsigned char *)pTemp + lWidth + 1;

		for (long i = 1; i < lHeight - 1; i++)   // 不考虑图像第一行 第一列 最后一行 最后一列
		{
			for (long j = 1; j < lWidth - 1; j++)
			{
				//*vergeMark=0表示这个点不是边缘，即不进行删除
				if (*vergeMark == 0)
				{
					pmid++;
					vergeMark++;

					continue;
				}
				//如果是1，是我们考虑的点 我们需要对周围8个进行判断
				//判断一个点是否能去掉, 要根据它的八个相邻点的情况来判断
				p0 = *(pmid - lWidth - 1);
				p1 = *(pmid - lWidth);                  //    P0 P1 P2
				p2 = *(pmid - lWidth + 1);             //    P7    P3
				p3 = *(pmid + 1);					    //    P6 P5 P4
				p4 = *(pmid + lWidth + 1);
				p5 = *(pmid + lWidth);
				p6 = *(pmid + lWidth - 1);
				p7 = *(pmid - 1);


				/*根据它的八个相邻点的情况形成的索引表进行删除操作
				*
				* 经过预处理后得到待细化的图像是0、1二值图像。
				* 像素值为1的是需要细化的部分，像素值为0的是背景区域。
				*
				* 基于索引表的算法就是"依据一定的判断依据"，所做出的一张表，
				* 因为一个像素的8个邻域，我们可以用8位二进制表示，共有256中可能情况，
				* 因此，索引表的大小一般为256。
				* 根据索引值对应表中的位置存0或1，
				*                  当前像素不能删除存为0，
				*                  可以删除存为1。
				*
				* 然后根据要细化的点的八个邻域的在索引表中情况查询，
				* 若表中元素是1，则删除该点（改为背景）
				* 若是0则保留。
				*
				*
				* ----------------------------------------
				* |p7 | p6 | p5 | p4 | p3 | p2 | p1 | p0 |---索引值对应表中的位置-----》共有256中可能情况
				* ----------------------------------------
		（位置1 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1,
				1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1,
				0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1,
				1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0,
				0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
				1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0,
				1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0（位置256）
				*/
				//此时p0--到---p7的值不是0就是1
				p1 *= 2;
				p2 *= 4;
				p3 *= 8;
				p4 *= 16;
				p5 *= 32;
				p6 *= 64;
				p7 *= 128;

				sum = p0 | p1 | p2 | p3 | p4 | p5 | p6 | p7;
				//  sum = p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7;
				if (deletemark[sum] == 1)//可以删除
				{
					*pmid = 0;
					bStart = true;      //  表明本次扫描进行了细化
				}
				pmid++;
				vergeMark++;
			}

			pmid++;//跳过图像最后一列，不考虑
			vergeMark++;

			pmid++;//跳过图像第一列，不考虑
			vergeMark++;
		}
	}

	//循环结束说明没有可以继续细化的点了，则为细化结束
	delete[]pTemp;
	return true;
}

int main(int argc, char* argv[])
{
	//加载原图像
	IplImage* src = cvLoadImage("ren.jpg", 0);

	//---------------------------预处理二值化------------------------------------
	//对原图像进行二值化  阈值
	cvThreshold(src, src, 130, 255, CV_THRESH_BINARY);
	/*
	图像二值化就是将图像上的像素点的灰度值设置为0或255，
	也就是将整个图像呈现出明显的黑白效果
	CV_THRESH_BINARY  意思为： if src(x,y) > 100
	src(x,y)=255
	else
	src(x,y)=-0
	原图像由0到255---->0或255
	*/

	cvNamedWindow("s", 0);
	cvShowImage("s", src);

	//-----------------------------预处理0/1化-------------------------------------
	/*
	将imagedata指向的与原图像大小一下的数组空间进行0或1赋值
	二值图有原来的0或255----->------0或1
	*/

	//动态创建一个长度为sizeof(char) * src->width * src->height的字符数组
	//并且初始化把数组元素都设置为0
	unsigned char* imagedata;
	imagedata = new uchar[sizeof(char) * src->width * src->height]();

	int x, y;
	for (y = 0; y<src->height; y++)
	{
		/*
		widthStep表示存储一行像素需要的字节数
		偏移到第y行
		*/
		unsigned char* ptr = (unsigned char*)(src->imageData + y*src->widthStep);

		for (x = 0; x<src->width; x++)
		{
			/*
			判断第y行第x列的元素值是否大于0
			大于0时将数组对应的位置元素设置为1
			不大于0则设置为0

			imagedata数组里面不是0就时1
			*/
			imagedata[y*src->width + x] = ptr[x] > 0 ? 1 : 0;
			//经过预处理后得到待细化的图像是0、1二值图像。
		}
	}

	//----------------------------细化-----------------------------------------

	//经过预处理后得到待细化的图像是0、1二值图像。

	//细化一次，和细化多次，由于索引表不变，每次细化效果都一样，想采用多次细化无法消除毛刺
	ThiningDIBSkeleton(imagedata, src->width, src->height);

	//--------------------对细化后的0/1图二值化(0/255)-----------------------------
	/*
	将ptr指向的与原图像大小一下的数组空间进行0或255赋值
	*/
	for (y = 0; y<src->height; y++)
	{
		/*
		widthStep表示存储一行像素需要的字节数
		偏移到第y行
		*/
		unsigned char* ptr = (unsigned char*)(src->imageData + y*src->widthStep);

		for (x = 0; x<src->width; x++)
		{
			/*
			判断第y行第x列的元素值是否大于0
			大于0时将数组对应的位置元素设置为1
			不大于0则设置为0

			ptr指向的空间的值不是0就是255
			也就是细化之后再次变为二值图
			*/
			ptr[x] = imagedata[y*src->width + x]>0 ? 255 : 0;
		}

	}
	//-------------------------------------------------------------------------

	cvNamedWindow("src", 0);
	cvShowImage("src", src);
	cvWaitKey(0);

	delete[]imagedata;

	return 0;
}