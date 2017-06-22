#pragma once
/**
* Class FastFT, Faster Fourier Transform.
* This algorithm is for image whose size is the integer power of two.
* We will pad zeros at surroundings to full size to the nearest integer of power of two, if image size isn't equal to that.
* For color image, we simply transform it to gray image and take FFT. Note that it is not true, we should have take FFT for each channel.
*/

#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<complex>
#include<vector>
#include"GDIP.h"




class FastFT
{
public:
	FastFT();
	FastFT(cv::Mat im);
	~FastFT();
	cv::Mat SourceImage;  //Place soure image
	cv::Mat FFTImage;     //Place FFT image
	cv::Mat IFFTImage;    //Place IFFT image



	void FFT(cv::Mat scr, cv::Mat &dstC);  // FFT for an image.
	void IFFT(cv::Mat scrC, cv::Mat&dstC); //IFFT for an image.
	void FFTM(cv::Mat scrC, cv::Mat& dstC); //Pure fast Fourier transform for a two-demensional matrix.
	void FFTRow(std::vector<std::complex<double>>&scrV); //FFT a row vector.
	void InvOrder(cv::Mat scrC, std::vector<std::vector<std::complex<double>>> &scrV);  // Rearrange the order of source image with its inverse order.
	void InvOrderV(std::vector<std::vector<std::complex<double>>> scrC, std::vector<std::vector<std::complex<double>>> &scrV);  //For a vector parameter, rearrange the order of source image with its inverse order.
	void logMagImage(cv::Mat scr, cv::Mat &dst);  //Take the log of a magnitude image for showing at FFT.
//	void ExtCenter(cv::Mat scr, cv::Mat &dst, size_t mr, size_t mc);
private:
	//Some private functions for comparing.
	void Fourier(cv::Mat scr, cv::Mat &dst);      //Discrete Fourier transform is realized by the formula directly
	void FourierM(cv::Mat scrC, cv::Mat& dstC);   //Discrete Fourier Transform's main part.
	void InvFourier(cv::Mat scrC, cv::Mat &dstC); //Inverse discrete Fourier transform is realized by the formula directly
	
private:
	GDIP gip;
};



