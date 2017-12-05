//�����������ͼ��ϸ���㷨

#include <highgui.h>   
#include <windows.h>   

#include "highgui.h"
#include "cv.h"//����cvCreateStructuringElementEx();��ɫ�ռ�ת������cvCvtColor()��Ҫ
#include "cxcore.h"

#include <stdlib.h>
#include <string.h>

//�����������ϸ��ϸ���㷨
//���ܣ���ͼ�����ϸ��
//������lpDIBBits������ͼ���һά����
//      lWidth��ͼ��߶�
//      lHeight��ͼ����


//                           ����ͼ���һά����      ͼ����     ͼ��߶�
bool ThiningDIBSkeleton(unsigned char* imagedata, int lWidth, int lHeight)
{
	/* 	unsigned char* imagedata;
	imagedata = new uchar[sizeof(char) * src->width * src->height]();
	*/
	/*
	deletemark[256]Ϊǰ�˾�8�����ܽ���Ƿ���Ա�ɾ����256�����
	������ɾ����0����ʾ���ܱ�ɾ������1����ʾ
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
	};//������

	  //ѭ������
	  //long i;
	  //long j;


	unsigned char p0, p1, p2, p3, p4, p5, p6, p7;

	unsigned char *pmid, *vergeMark;    // pmid ����ָ���ֵͼ��  vergeMark����ָ�����Ƿ�Ϊ��Ե

	unsigned char sum;

	bool bStart = true;

	//��̬����һ������Ϊsizeof(char) * src->width * src->height������
	//���ҳ�ʼ��������Ԫ�ض�����Ϊ0
	//������imagedata����һ��
	long lLength;
	lLength = lWidth * lHeight; //���鳤��
	unsigned char *pTemp = new uchar[sizeof(unsigned char) * lLength]();

	//    P0 P1 P2
	//    P7    P3
	//    P6 P5 P4

	while (bStart)
	{
		bStart = false;
		//ÿ��ѭ��pTemp����Ԫ�ض���0
		memset(pTemp, 0, lLength);//��ʼ��pTempָ���lLength�ռ�ȫ����ʼ��Ϊ0

								  /*---------------�������Ե��----------------*/
		pmid = (unsigned char *)imagedata + lWidth + 1; // pmid ����ָ���ֵͼ�����ĵڶ���	  +1 ��ʾ������ͼ���һ��
		vergeMark = (unsigned char *)pTemp + lWidth + 1; //pmidtemp����ָ�����Ƿ�Ϊ��Ե ����Ǳ�Ե�� ������Ϊ1

		for (int i = 1; i < lHeight - 1; i++)  //lHeight -1��ʾ������ͼ�����һ�� ��һ��
		{
			for (int j = 1; j < lWidth - 1; j++)  //lWidth - 1��ʾ������ͼ�����һ��
			{
				//ͼ���Ѿ���01���������0���ǲ�����
				if (*pmid == 0)
				{
					pmid++;
					vergeMark++;
					continue;
				}
				//�����1�������ǿ��ǵĵ� ������Ҫ����Χ8�������ж��Ƿ��Ե
				p0 = *(pmid - lWidth - 1);
				p1 = *(pmid - lWidth);                  //    P0 P1 P2
				p2 = *(pmid + 1 - lWidth);              //    P7    P3
				p3 = *(pmid + 1);						//    P6 P5 P4
				p4 = *(pmid + lWidth + 1);
				p5 = *(pmid + lWidth);
				p6 = *(pmid + lWidth - 1);
				p7 = *(pmid - 1);
				//p0--��---p7��ֵ����0����1
				sum = p0 & p1 & p2 & p3 & p4 & p5 & p6 & p7;//����Ǳ�Ե���϶���Χ��P0P1P2P3P4P5P6P7 ��һ��������һ��Ϊ0
				if (sum == 0)
				{
					*vergeMark = 1;// ��Ǳ�Ե
				}

				pmid++;
				vergeMark++;
			}
			pmid++;//����ͼ�����һ�У�������
			vergeMark++;

			pmid++;//����ͼ���һ�У�������
			vergeMark++;
		}

		/*---------------���ű�Ե���ڿ�ʼɾ��----------------*/
		pmid = (unsigned char *)imagedata + lWidth + 1;
		vergeMark = (unsigned char *)pTemp + lWidth + 1;

		for (long i = 1; i < lHeight - 1; i++)   // ������ͼ���һ�� ��һ�� ���һ�� ���һ��
		{
			for (long j = 1; j < lWidth - 1; j++)
			{
				//*vergeMark=0��ʾ����㲻�Ǳ�Ե����������ɾ��
				if (*vergeMark == 0)
				{
					pmid++;
					vergeMark++;

					continue;
				}
				//�����1�������ǿ��ǵĵ� ������Ҫ����Χ8�������ж�
				//�ж�һ�����Ƿ���ȥ��, Ҫ�������İ˸����ڵ��������ж�
				p0 = *(pmid - lWidth - 1);
				p1 = *(pmid - lWidth);                  //    P0 P1 P2
				p2 = *(pmid - lWidth + 1);             //    P7    P3
				p3 = *(pmid + 1);					    //    P6 P5 P4
				p4 = *(pmid + lWidth + 1);
				p5 = *(pmid + lWidth);
				p6 = *(pmid + lWidth - 1);
				p7 = *(pmid - 1);


				/*�������İ˸����ڵ������γɵ����������ɾ������
				*
				* ����Ԥ�����õ���ϸ����ͼ����0��1��ֵͼ��
				* ����ֵΪ1������Ҫϸ���Ĳ��֣�����ֵΪ0���Ǳ�������
				*
				* ������������㷨����"����һ�����ж�����"����������һ�ű�
				* ��Ϊһ�����ص�8���������ǿ�����8λ�����Ʊ�ʾ������256�п��������
				* ��ˣ�������Ĵ�Сһ��Ϊ256��
				* ��������ֵ��Ӧ���е�λ�ô�0��1��
				*                  ��ǰ���ز���ɾ����Ϊ0��
				*                  ����ɾ����Ϊ1��
				*
				* Ȼ�����Ҫϸ���ĵ�İ˸���������������������ѯ��
				* ������Ԫ����1����ɾ���õ㣨��Ϊ������
				* ����0������
				*
				*
				* ----------------------------------------
				* |p7 | p6 | p5 | p4 | p3 | p2 | p1 | p0 |---����ֵ��Ӧ���е�λ��-----������256�п������
				* ----------------------------------------
				��λ��1 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1,
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
				1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0��λ��256��
				*/
				//��ʱp0--��---p7��ֵ����0����1
				p1 *= 2;
				p2 *= 4;
				p3 *= 8;
				p4 *= 16;
				p5 *= 32;
				p6 *= 64;
				p7 *= 128;

				sum = p0 | p1 | p2 | p3 | p4 | p5 | p6 | p7;
				//  sum = p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7;
				if (deletemark[sum] == 1)//����ɾ��
				{
					*pmid = 0;
					bStart = true;      //  ��������ɨ�������ϸ��
				}
				pmid++;
				vergeMark++;
			}

			pmid++;//����ͼ�����һ�У�������
			vergeMark++;

			pmid++;//����ͼ���һ�У�������
			vergeMark++;
		}
	}

	//ѭ������˵��û�п��Լ���ϸ���ĵ��ˣ���Ϊϸ������
	delete[]pTemp;
	return true;
}
BOOL GetHistogram(unsigned char *pImageData, int nWidth, int nHeight, int nWidthStep,
	int *pHistogram)
{
	int            i = 0;
	int            j = 0;
	unsigned char *pLine = NULL;
	// ���ֱ��ͼ   
	memset(pHistogram, 0, sizeof(int) * 256);

	for (pLine = pImageData, j = 0; j < nHeight; j++, pLine += nWidthStep)
	{
		for (i = 0; i < nWidth; i++)
		{
			pHistogram[pLine[i]]++;
		}
	}
	return TRUE;
}

// ���ȡ��ֵ   
// 1. pImageData   ͼ������   
// 2. nWidth       ͼ����   
// 3. nHeight      ͼ��߶�   
// 4. nWidthStep   ͼ���д�С   
// ����������ֵ   
int Otsu(unsigned char *pImageData, int nWidth, int nHeight, int nWidthStep)
{
	int    i = 0;
	int    j = 0;
	int    nTotal = 0;
	int    nSum = 0;
	int    A = 0;
	int    B = 0;
	double u = 0;
	double v = 0;
	double dVariance = 0;
	double dMaximum = 0;
	int    nThreshold = 0;
	int    nHistogram[256];
	// ��ȡֱ��ͼ   
	GetHistogram(pImageData, nWidth, nHeight, nWidthStep, nHistogram);
	for (i = 0; i < 256; i++)
	{
		nTotal += nHistogram[i];
		nSum += (nHistogram[i] * i);
	}
	for (j = 0; j < 256; j++)
	{
		A = 0;
		B = 0;
		for (i = 0; i < j; i++)
		{
			A += nHistogram[i];
			B += (nHistogram[i] * i);
		}
		if (A > 0)
		{
			u = B / A;
		}
		else
		{
			u = 0;
		}
		if (nTotal - A > 0)
		{
			v = (nSum - B) / (nTotal - A);
		}
		else
		{
			v = 0;
		}
		dVariance = A * (nTotal - A) * (u - v) * (u - v);
		if (dVariance > dMaximum)
		{
			dMaximum = dVariance;
			nThreshold = j;
		}
	}
	return nThreshold;
}



// ��ֵ��   
// 1. pImageData   ͼ������   
// 2. nWidth       ͼ����   
// 3. nHeight      ͼ��߶�   
// 4. nWidthStep   ͼ���д�С   
// 5. nThreshold   ��ֵ   
BOOL Thresholding(unsigned char *pImageData, int nWidth, int nHeight, int nWidthStep,
	unsigned int nThreshold)
{
	int            i = 0;
	int            j = 0;
	unsigned char *pLine = NULL;
	for (pLine = pImageData, j = 0; j < nHeight; j++, pLine += nWidthStep)
	{
		for (i = 0; i < nWidth; i++)
		{
			if (pLine[i] >= nThreshold)
			{
				pLine[i] = 0x00;
			}
			else
			{
				pLine[i] = 0xff;
			}
		}
	}
	return TRUE;
}




//                                        p3  p2  p1
//**********ʹ��zhang���п����㷨����ϸ�� p4  p   p0
//                                        p5  p6  p7
void ZhangThinning(int w, int h, unsigned char *imgBuf)
{
	int neighbor[8];

	BYTE *mark = new BYTE[w*h];
	memset(mark, 0, w*h);

	BOOL loop = TRUE;
	int x, y, k;
	int markNum = 0;

	while (loop)
	{
		loop = FALSE;

		//��һ��
		markNum = 0;
		for (y = 1; y<h - 1; y++)
		{
			for (x = 1; x<w - 1; x++)
			{
				//����1��p������ǰ����
				if (imgBuf[y*w + x] == 0) continue;

				neighbor[0] = imgBuf[y*w + x + 1];
				neighbor[1] = imgBuf[(y - 1)*w + x + 1];
				neighbor[2] = imgBuf[(y - 1)*w + x];
				neighbor[3] = imgBuf[(y - 1)*w + x - 1];
				neighbor[4] = imgBuf[y*w + x - 1];
				neighbor[5] = imgBuf[(y + 1)*w + x - 1];
				neighbor[6] = imgBuf[(y + 1)*w + x];
				neighbor[7] = imgBuf[(y + 1)*w + x + 1];

				//����2��2<=N(p��<=6
				int np = (neighbor[0] + neighbor[1] + neighbor[2] + neighbor[3] + neighbor[4] + neighbor[5] + neighbor[6] + neighbor[7]) / 255;
				if (np<2 || np>6) continue;

				//����3��S(p��=1
				int sp = 0;
				for (int i = 1; i<8; i++)
				{
					if (neighbor[i] - neighbor[i - 1] == 255)
						sp++;
				}
				if (neighbor[0] - neighbor[7] == 255)
					sp++;
				if (sp != 1) continue;

				//����4��p2*p0*p6=0
				if (neighbor[2] & neighbor[0] & neighbor[6] != 0)
					continue;
				//����5��p0*p6*p4=0
				if (neighbor[0] & neighbor[6] & neighbor[4] != 0)
					continue;


				//���ɾ��
				mark[w*y + x] = 1;
				markNum++;
				loop = TRUE;
			}
		}

		//�����ɾ���ĵ���Ϊ����ɫ
		if (markNum>0)
		{
			for (y = 0; y<h; y++)
			{
				for (x = 0; x<w; x++)
				{
					k = y*w + x;
					if (mark[k] == 1)
					{
						imgBuf[k] = 0;
					}
				}
			}
		}


		//�ڶ���
		markNum = 0;
		for (y = 1; y<h - 1; y++)
		{
			for (x = 1; x<w - 1; x++)
			{
				//����1��p������ǰ����
				if (imgBuf[y*w + x] == 0) continue;

				neighbor[0] = imgBuf[y*w + x + 1];
				neighbor[1] = imgBuf[(y - 1)*w + x + 1];
				neighbor[2] = imgBuf[(y - 1)*w + x];
				neighbor[3] = imgBuf[(y - 1)*w + x - 1];
				neighbor[4] = imgBuf[y*w + x - 1];
				neighbor[5] = imgBuf[(y + 1)*w + x - 1];
				neighbor[6] = imgBuf[(y + 1)*w + x];
				neighbor[7] = imgBuf[(y + 1)*w + x + 1];

				//����2��<=N(p)<=6
				int np = (neighbor[0] + neighbor[1] + neighbor[2] + neighbor[3] + neighbor[4] + neighbor[5] + neighbor[6] + neighbor[7]) / 255;
				if (np<2 || np>6) continue;

				//����3��S(p)=1
				int sp = 0;
				for (int i = 1; i<8; i++)
				{
					if (neighbor[i] - neighbor[i - 1] == 255)
						sp++;
				}
				if (neighbor[0] - neighbor[7] == 255)
					sp++;
				if (sp != 1) continue;

				//����4��p2*p0*p4==0
				if (neighbor[2] & neighbor[0] & neighbor[4] != 0)
					continue;
				//����5��p2*p6*p4==0
				if (neighbor[2] & neighbor[6] & neighbor[4] != 0)
					continue;

				//���ɾ��
				mark[w*y + x] = 1;
				markNum++;
				loop = TRUE;
			}
		}

		//�����ɾ���ĵ���Ϊ����ɫ
		for (y = 0; y<h; y++)
		{
			for (x = 0; x<w; x++)
			{
				k = y*w + x;
				if (mark[k] == 1)
				{
					imgBuf[k] = 0;
				}
			}
		}

	}
}




int main(int argc, char* argv[])
{
	//����ԭͼ��
	IplImage* src = cvLoadImage("06100018/00000113(21,430,127,588).jpg", 0);

	//---------------------------Ԥ�����ֵ��------------------------------------
	/*��ԭͼ����ж�ֵ��  ��ֵ
	cvThreshold(src, src, 100, 255, CV_THRESH_BINARY);

	ͼ���ֵ�����ǽ�ͼ���ϵ����ص�ĻҶ�ֵ����Ϊ0��255��
	Ҳ���ǽ�����ͼ����ֳ����Եĺڰ�Ч��
	CV_THRESH_BINARY  ��˼Ϊ�� if src(x,y) > 100
	src(x,y)=255
	else
	src(x,y)=-0
	ԭͼ����0��255---->0��255
	*/

	unsigned int thresValue = Otsu((unsigned char *)src->imageData, src->width, src->height, src->widthStep);

	Thresholding((unsigned char *)src->imageData, src->width, src->height, src->widthStep,
		thresValue);
	
	//��ֵ����ͼ��
	cvNamedWindow("s", 0);
	cvShowImage("s", src);

	//��̬����һ������Ϊsizeof(char) * src->width * src->height���ַ�����
	//���ҳ�ʼ��������Ԫ�ض�����Ϊ0
	unsigned char* imagedata;
	imagedata = new uchar[sizeof(char) * src->width * src->height]();
	






	




	//----------------------------ϸ��-----------------------------------------

	//����Ԥ�����õ���ϸ����ͼ����0��1��ֵͼ��

	//ϸ��һ�Σ���ϸ����Σ������������䣬ÿ��ϸ��Ч����һ��������ö��ϸ���޷�����ë��
	int flag = 0;
	scanf("%d", &flag);
	switch (flag)
	{
	case 0:
		//-----------------------------Ԥ����0/1��-------------------------------------
		/*
		��imagedataָ�����ԭͼ���Сһ�µ�����ռ����0��1��ֵ
		��ֵͼ��ԭ����0��255----->------0��1
		*/

		

		int x, y;
		for (y = 0; y<src->height; y++)
		{
			/*
			widthStep��ʾ�洢һ��������Ҫ���ֽ���
			ƫ�Ƶ���y��
			*/
			unsigned char* ptr = (unsigned char*)(src->imageData + y*src->widthStep);

			for (x = 0; x<src->width; x++)
			{
				/*
				�жϵ�y�е�x�е�Ԫ��ֵ�Ƿ����0
				����0ʱ�������Ӧ��λ��Ԫ������Ϊ1
				������0������Ϊ0

				imagedata�������治��0��ʱ1
				*/
				imagedata[y*src->width + x] = ptr[x] > 0 ? 1 : 0;
				//����Ԥ�����õ���ϸ����ͼ����0��1��ֵͼ��
			}
		}

		ThiningDIBSkeleton(imagedata, src->width, src->height);
		//--------------------��ϸ�����0/1ͼ��ֵ��(0/255)-----------------------------
		/*
		��ptrָ�����ԭͼ���Сһ�µ�����ռ����0��255��ֵ
		*/
		for (y = 0; y<src->height; y++)
		{
			/*
			widthStep��ʾ�洢һ��������Ҫ���ֽ���
			ƫ�Ƶ���y��
			*/
			unsigned char* ptr = (unsigned char*)(src->imageData + y*src->widthStep);

			for (x = 0; x<src->width; x++)//width:������
			{
				/*
				�жϵ�y�е�x�е�Ԫ��ֵ�Ƿ����0
				����0ʱ�������Ӧ��λ��Ԫ������Ϊ1
				������0������Ϊ0

				ptrָ��Ŀռ��ֵ����0����255
				Ҳ����ϸ��֮���ٴα�Ϊ��ֵͼ
				*/
				ptr[x] = imagedata[y*src->width + x]>0 ? 255 : 0;
			}

		}
		//-------------------------------------------------------------------------
		break;
	case 1:
		ZhangThinning(src->width, src->height, (unsigned char *)src->imageData);
		break;
	default:
		break;
	}
	
	




	

	cvNamedWindow("src", 0);
	cvShowImage("src", src);
	cvWaitKey(0);

	if (flag = 0) {
		delete[]imagedata;
	}

	return 0;
}