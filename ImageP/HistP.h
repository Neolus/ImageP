#pragma once
/**
* Class HistP contains some functions of histogram processing.
*/

#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include"GDIP.h"


class HistP
{
public:
	HistP();
	~HistP();


	void HistEq(cv::Mat scr, cv::Mat &dst, int L, bool isHsi); //Histogram equalization

	cv::Mat Hist2Image(std::vector<std::vector<int>> & hist);  //Transform a histogram to a Mat for showing.
	void GetHist(const cv::Mat& img, std::vector<std::vector<int>> & hist, double a, double b, int L);          // Get the histogram of an Image.
	std::vector<std::vector<int>> GetEqualHist(std::vector<std::vector<int>>hist, int n, double a, double b);   // Compute the equalized histogram.

private:
	void HistEqualize(cv::Mat scr, cv::Mat &dst, std::vector<std::vector<int>> HistE, double a, double b);                // Histogram equlize an image at all channels.
	void HistP::HistEqualize2(cv::Mat scr, cv::Mat &dst, std::vector<std::vector<int>> HistE, double a, double b, int c); // Histogram equlize an image at one channels.


};


