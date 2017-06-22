#pragma once
/**
* Class DCosT, Discrete cosine tranform.
* Divide image into small blocks then DCT.
* Make sure image size is multiple of block. If not so, please padded zeros before calling this class.
*/


#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<iostream>
#include<complex>
#include<vector>
#include"GDIP.h"

class DCosT
{
public:
	DCosT();
	~DCosT();
	void DCTMat(cv::Mat scr, cv::Mat& dst, size_t nb);               //Discrete Cosine Transform of an image
	void IDCTMat(cv::Mat scr, cv::Mat& dst, double kr, size_t nb);   //Invere Discrete Cosine  Transform of coefficients of DCT.

private:
	void divBlock(cv::Mat scr, std::vector<std::vector<cv::Mat>> &dstV, size_t n);     // Divide imgae Mat into blocks.
	void mergeBlock(std::vector<std::vector<cv::Mat>> &scrV, cv::Mat &dst, size_t n);  // Merge blocks into an image.
	void dctBlock(cv::Mat scrB, cv::Mat& dstB, const std::vector<std::vector<double>>& cosMat);  //Discrete Cosine Transform of a block Mat.
	void idctBlock(cv::Mat scrB, cv::Mat& dstB, const std::vector<std::vector<double>> &cosMat); //Inverse Discrete Cosine Transform of a DCT block Mat.
	std::vector<std::vector<double>> cosMat(size_t n);  // Compute a cosine matrix.
	std::vector<std::vector<double>>  trcTemp(size_t n, double kr); // Compute a template matrix of truncation of DFT coefficients.

};

