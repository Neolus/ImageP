#pragma once
/**
* Class GDIP, General Functions of Digital Image Processing 
**/
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<iostream>
#include<complex>
#include<vector>


class GDIP
{
public:
	GDIP();
	~GDIP();

	bool IsGray(cv::Mat scr); //Judge an image whether it's a gray image.
	void normlz(cv::Mat scr, cv::Mat &dst); //Normalize an image.

	void Map2CV8U(cv::Mat scr, cv::Mat &dst); //Map double(CV_64F): [min,max]->[0,255]
	void CV64F2CV8U(cv::Mat scr, cv::Mat &dst); //Change data type from double to uchar.


	void BGR2Gray(cv::Mat scr, cv::Mat& dst);  // Transform a RGB image to a gray one.
	cv::Mat BGR2HSV(const cv::Mat & image);    // Transform a BGR image to HSV space.
	cv::Mat BGR2HSV2(const cv::Mat & image);   // Transform a BGR image to HSV space.
	void BGR2HSI(cv::Mat image, cv::Mat &dst); // Transform a BGR image to HSI space.
	void HSI2BGR(cv::Mat scr, cv::Mat &dst);   // Transform a HSI image to BGR space.

	double rms(cv::Mat scr);   //Compute the root-mean-square of an error image.

	void mv2centR(cv::Mat scr, cv::Mat &dst);  // Move an image and let its Fourier transform be centered.
	void mv2centC(cv::Mat scr, cv::Mat &dst);  // Another edition of MV2CENT funtion, for a comlex Mat.

	void real2cmp(cv::Mat r, cv::Mat& cmp);    // Change a real Mat to a complex one.
	void cmp2con(cv::Mat scrC, cv::Mat &dstC); // Change a complex Mat to its conjugate.

	void PadZero(cv::Mat scr, cv::Mat &dst, size_t pr, size_t pc);   //Pad zeros at surroundings of Image Mat
	void ExtCenter(cv::Mat scr, cv::Mat &dst, size_t mr, size_t mc); //Extract the center of a padded image



private:

};

