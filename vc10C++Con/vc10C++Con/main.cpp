#include "highgui.h" 
int main( int argc,char** argv ) 
{ 
	//IplImage* img = cvLoadImage( argv[0] ); 
	IplImage* img = cvLoadImage("F:\\QQͼƬ20180506214207.jpg"); 
	//IplImage *img = cvLoadImage(" F:\opencv_study\test1_imread4\lena.bmp"); 
	//������������д������޷���ʾ��ԭ�����ھ���·��֮ǰ���ֿո����vsҪ��Ҳ̫�����ˡ� 
	cvNamedWindow("Example1", CV_WINDOW_AUTOSIZE ); 
	//cvNamedWindow("Example1", 0 ); 
	cvShowImage("Example1", img ); 
	cvWaitKey(0); 
	cvReleaseImage( &img );//����������ʡȥҲ���ԣ���������ϰ��ÿ�ε�����Щ������ʾ�ͷ���Դ�����кô��ġ� 
	cvDestroyWindow("Example1"); 
} 