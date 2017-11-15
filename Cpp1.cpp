#include <stdio.h>

typedef struct _BMPFH_ {
	unsigned short   bfType;
	unsigned long   bfSize;
	unsigned short    bfReserved1;
	unsigned short    bfReserved2;
	unsigned long   bfOffBits;
} BMPFH;

typedef struct _BMPIH_{
	unsigned long      biSize;
	long				biWidth;
	long				 biHeight;
	unsigned short       biPlanes;
	unsigned short       biBitCount;
	unsigned long      biCompression;
	unsigned long      biSizeImage;
	long				biXPelsPerMeter;
	long				 biYPelsPerMeter;
	unsigned long      biClrUsed;
	unsigned long      biClrImportant;
} BMPIH;

typedef struct _RGBQUAD_ {
	unsigned char   rgbBlue;
	unsigned char   rgbGreen;
	unsigned char   rgbRed;
	unsigned char   rgbReserved;
} RGBQUAD;

//灰度图像数据;图像宽度;实际存储宽度;图像高度;阈值
void dothreshold(unsigned char* pData,const int width,const int stride,const int height,const unsigned char threshold)
{
	unsigned char* pLine = pData ; //- (height-1)*stride; //第一行
	for (int i=0;i<height;i++)
	{
		unsigned char* pPixel = pLine;
		for (int i=0;i<width;i++)
		{
			*pPixel = (*pPixel)>threshold ? 255 : 0 ;
			pPixel++;
		}
		pLine += stride;
	}
}




int main(int argc,char* argv[])
{
	BMPFH bmpfh;
	BMPIH bmpih;
	FILE* fpImage = fopen("testgray.bmp","rb");
	fread(&bmpfh,sizeof(BMPFH),1,fpImage);
	fread(&bmpih,sizeof(BMPIH),1,fpImage);
	if (bmpih.biBitCount != 8)
	{
		return -1;
	}
	int width = bmpih.biWidth;
	int height = bmpih.biHeight;
	int stride = width ;
	if((width % 4)>0) 
		stride += 4-(width % 4);

	RGBQUAD* pRgbQuad = new RGBQUAD[256];//位图信息头后面为调色板
	if (!pRgbQuad)
	{
		return -1;
	}
	if(fread(pRgbQuad,256 * sizeof(RGBQUAD),1,fpImage)<=0)
		return-1;

	unsigned char* pData = new unsigned char[height*stride];
	if (!pData)
	{
		return -1;
	}
	if(fread(pData,height*stride,1,fpImage)<=0)
		return-1;

	dothreshold(pData,width,stride,height,100);
	fclose(fpImage);

	//save
	FILE* fpRs = fopen("Result.bmp","wb");
	if (!fpRs)
	{
		return -1;
	}
	fwrite(&bmpfh,sizeof(BMPFH),1,fpRs);
	fwrite(&bmpih,sizeof(BMPIH),1,fpRs);
	fwrite(pRgbQuad,256*sizeof(RGBQUAD),1,fpRs);
	fwrite(pData,height*stride,1,fpRs);
	fclose(fpRs);

	return 0;
}