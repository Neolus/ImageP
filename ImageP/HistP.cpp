#include"stdafx.h"
#include"HistP.h"

HistP::HistP()
{
}

HistP::~HistP()
{
}

/**
* Histogram equalization drive function
* SCR is a source image.
* DST is the equalized image.
* L is the number of gray scales of histogram.
* ISHSI is a flag whether to do a HSI processing.
*/
void HistP::HistEq(cv::Mat scr, cv::Mat &dst, int L,bool isHsi=false)
{
	size_t nr = scr.rows;
	size_t nc = scr.cols;
	size_t ch = scr.channels();
	std::vector<std::vector<int>>  hist,histE;
	GetHist(scr, hist, 0, 1, L); //Get an image's histogram
	histE = GetEqualHist(hist, nr*nc, 0, 1); //Compute the equlized histogram
	if (!isHsi)
	{
		HistEqualize(scr, dst, histE,0,1);   //Gray image or RGB, histogram equalization.

	}
	else
	{
		HistEqualize2(scr, dst, histE, 0, 1, 2); //HSI image, histogram equalization.

	}

	
}



/**
* Get the histogram of an Image.
* Mat IMG is the source image whose type must be CV_64F.
* HIST is a 2-dimensional vector for placing histograms of each channel.
* IMG's gray scope is [a,b].
* L is the number of gray scales for counting.
*/
void HistP::GetHist(const cv::Mat& img, std::vector<std::vector<int>> & hist, double a, double b, int L)
{

	int channel = img.channels();
	hist.resize(channel); //BGR channels or gray. 
	for (int c = 0;c != channel;c++)   //Initialize histogram
	{
		hist[c].resize(L);
		for (int i = 0;i != L;i++)
		{
			hist[c][i] = 0;
		}
	}

	int nc = img.cols;
	int nr = img.rows;
	double s = (b - a) / (L - 1);

	for (int i = 0;i != nr;i++)   //Count all points.
	{
		const double * data = img.ptr<double>(i);
		for (int j = 0;j != nc;j++)
		{
			for (int c = 0;c != channel;c++)
			{
				hist[c][((*data + s / 2) - a) / s]++;
				data++;  //data-- fallible, the BGR will changed to RGB. When there is no data, 0 is default.
			}
		}
	}

}

/**
* Transform a histogram to a Mat for showing.
* HIST, Histogram for all channels of an image.
* Return a Mat whose channel=HIST.size() and whose type is CV_8U;
*/
cv::Mat HistP::Hist2Image(std::vector<std::vector<int>> & hist)
{
	cv::Mat HistImg;
	if (hist.size() == 1) //Gray image.
	{
		cv::Mat TempMat(500, hist[0].size(), CV_8U, cv::Scalar(255));
		HistImg = TempMat; //Initialize
		int PMax = 0; //Max value of this histogram
		std::vector<int>::iterator itr, itrEnd;
		itrEnd = hist[0].end();
		for (itr = hist[0].begin();itr != itrEnd;++itr)
		{
			if (PMax < *itr)
			{
				PMax = *itr;
			}
		}
		cv::Mat_<uchar> imatrix = HistImg;
		for (int i = 0;i != hist[0].size();i++)
		{
			int b = double(hist[0][i]) / double(PMax) * 500;  //Normalize.

			for (int j = 500 - b;j != 500;j++) //Black bar form 500-b to 500
			{
				imatrix(j, i) = 0;
			}
		}
	}
	else //Color image.
	{
		cv::Mat TempMat(500, hist[0].size(), CV_8UC3, cv::Scalar(255, 255, 255));
		HistImg = TempMat;
		for (int c = 0;c != HistImg.channels();c++)  //All channels.
		{

			int PMax = 0;
			std::vector<int>::iterator itr, itrEnd;
			itrEnd = hist[c].end();
			for (itr = hist[c].begin();itr != itrEnd;++itr)
			{
				if (PMax < *itr)
				{
					PMax = *itr;
				}
			}
			cv::Mat_<cv::Vec3b> imatrix = HistImg;
			for (int i = 0;i != hist[c].size();i++)
			{
				int b = double(hist[c][i]) / double(PMax) * 500;

				for (int j = 500 - b;j != 500;j++)
				{
					imatrix(j, i)[c] = 0;
				}
			}
		}
	}
	return HistImg;
}

/**
* Compute the equalized histogram.
* HIST is the soure histogram.
* N is the number of pixels in one channel.
* [a,b] is the scope of gray value.
**/
std::vector<std::vector<int>>HistP::GetEqualHist(std::vector<std::vector<int>>hist, int n, double a, double b)  //n is the total number of pixels.
{
	int L = hist[0].size();
	std::vector<std::vector<int>> histE;
	histE.resize(hist.size());
	double s = (b - a) / (L - 1);
	for (int i = 0;i != hist.size();i++)
	{
		histE[i].resize(hist[i].size());
		double sum = 0;
		for (int r = 0;r != L;r++)
		{
			sum += hist[i][r];
			histE[i][r] = (L - 1)*sum / double(n) + 0.5*s; //Histogram equalize core formula.
		}
	}

	return histE;
}


/**
* Histogram equlize an image at all channels.
* Mat SCR is the source image whose data type is CV_64F.
* HISTE is the equalized histogram.
* Retuen the equalized image, whose type is CV_64F.
**/
void HistP::HistEqualize(cv::Mat scr,cv::Mat &dst, std::vector<std::vector<int>> HistE,double a,double b)
{

	dst = scr.clone();
	int nr = dst.rows;
	int nc = dst.cols;
	int channels = dst.channels();
	unsigned L = HistE[0].size();
	double s = (b - a) / (L - 1);



	for (int i = 0;i != nr;i++)
	{
		const double* data_in = scr.ptr<double>(i); //The start adress of i row.
		double* data_out = dst.ptr<double>(i);
		for (int j = 0;j != nc;j++)
		{
			for (int c = 0;c != channels;c++)
			{
				double v = a + s*(HistE[c][(*data_in + s / 2 - a) / s]); //Get a right grayscale value.
				if (v > b) v = b;
				*data_out++ = v;  //Modify pixel value.
				data_in++;
			}
		}
	}
}


/**
* Histogram equlize an image at one channels.
* Mat SCR is the source image whose data type is CV_64F.
* HISTE is the equalized histogram.
* [a,b] is the scope of gray value.
* C is the channel to be equalized. C is 0,1 or 2.
* Retuen the equalized image, whose type is CV_64F.
**/
void HistP::HistEqualize2(cv::Mat scr, cv::Mat &dst, std::vector<std::vector<int>> HistE, double a, double b, int c)
{
	unsigned L = HistE[0].size();
	dst = scr.clone();
	int nr = scr.rows;
	int nc = scr.cols;
//	int channels = scr.channels();
	double s = (b - a) / (L - 1);


	for (int i = 0;i != nr;i++)
	{
		const double* data_in = scr.ptr<double>(i); 
		double* data_out = dst.ptr<double>(i);


		for (int j = 0;j != nc;j++)  
		{

			if (c == 0) //Only choose one channel.
			{
				double v = a + s*(HistE[c][(*data_in + s / 2 - a) / s]);
				if (v > b) v = b;
				data_in++;
				*data_out++ = v;

				data_out++;  //Do nothing.
				data_out++;
				data_in++;
				data_in++;

			}
			if (c == 1)
			{


				data_out++;
				data_in++;
				double v = a + s*(HistE[c][(*data_in + s / 2 - a) / s]);
				data_in++;
				if (v > b) v = b;
				*data_out++ = v;
				data_out++;
				data_in++;
			}
			if (c == 2)
			{
				data_out++;
				data_in++;
				data_out++;
				data_in++;
				double v = a + s*(HistE[c][(*data_in + s / 2 - a) / s]);
				data_in++;
				if (v > b) v = b;
				*data_out++ = v;
			}
		}
	}
}
