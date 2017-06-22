#include "stdafx.h"
#include"FastFT.h"


FastFT::FastFT()
{
}

FastFT::FastFT(cv::Mat im)
{
	SourceImage = im;
}

FastFT::~FastFT()
{
}



/**
* Fast Fourier Transform.
* Mat SCR is the source image whose type must be CV_64F. Rows and Columns of SCR must be 2^k.
* Mat DSTC is the complex result of FFT. 
* The first channel of DSTC is the real part. The second channel of DSTC is the imaginary part.
*/
void FastFT::FFT(cv::Mat scr, cv::Mat &dstC)
{
	cv::Mat scrc;
	gip.mv2centR(scr, scrc);  //Move the center by multiplying (-1)^(x+y).
	gip.real2cmp(scrc, scrc);
	FFTM(scrc, dstC);  //Call 2 dimensional fft function. 
	dstC = dstC / (dstC.rows*dstC.cols);
}

/**
* Inverse FFT.
* Mat SCRC is the complex result of FFT. Mat DSTC is the complex image of Inverse FFT.
*/
void FastFT::IFFT(cv::Mat scrC, cv::Mat&dstC)
{
	cv::Mat scrCC;
	gip.cmp2con(scrC, scrCC); //Conjugate
	FFTM(scrCC, dstC);    //Call 2-dimensional FFT function.
	//	dstC = dstC/(dstC.rows*dstC.cols);
	gip.mv2centC(dstC, dstC); //Move center to its origin.
}

/**
* 2 dimensional Fast Fourier Transform.
* Mat SCRC is the complex source image. Mat DSTC is the complex result of FFT.
*/
void FastFT::FFTM(cv::Mat scrC, cv::Mat& dstC)
{
	int nr = scrC.rows;
	int nc = scrC.cols;

	dstC.create(nr, nc, CV_64FC2);
	std::vector<std::vector<std::complex<double>>> scrV; //A 2-dimensional vector for placing image Mat.
	InvOrder(scrC, scrV);  //Place rearranged rows at SCRV.
	//Compute FFT by rows.
	for (int r = 0;r != nr;r++)
	{
		FFTRow(scrV[r]);
	}

	//Compute columns' FFT
	//Transpose the result matrix of FFT at rows.
	std::vector<std::vector<std::complex<double>>> temp;// = scrV;
	temp.resize(nr);
	for (int r = 0;r != nr;r++)
	{
		temp[r].resize(nc);
	}

	for (int r = 0;r != nr;r++)
	{
		for (int c = 0;c != nc;c++)
		{
			temp[r][c] = scrV[c][r];
		}
	}
	/***/
	InvOrderV(temp, temp); //Rearrange.

	for (int c = 0;c != nc;c++) //All rows.
	{
		FFTRow(temp[c]);
	}

	//Transpose to its origin. Rows to columns.
	for (int r = 0;r != nr;r++)
	{
		for (int c = 0;c != nc;c++)
		{
			scrV[r][c] = temp[c][r];
		}
	}

	// Place result of FFT to Mat DSTC.
	for (int r = 0;r != nr;r++)
	{
		double *p = dstC.ptr<double>(r);
		for (int c = 0;c != nr;c++)
		{
			*p++ = real(scrV[r][c]); //Real part.
			*p++ = imag(scrV[r][c]); //Imaginary part.
		}
	}
}



/**
* Rearrange the order of source image with its inverse order.
* Only rearrange by rows.
* Mat SCRC is the complex source image. SCVR is a 2 dimensional vector placed SCRC's inverse data.
**/
void FastFT::InvOrder(cv::Mat scrC, std::vector<std::vector<std::complex<double>>> &scrV)
{
	int nr = scrC.rows;
	int nc = scrC.cols;

	scrV.resize(nr);
	for (int r = 0;r != nr;r++)
		scrV[r].resize(nc);

	size_t lb = log2(nc - 1) + 1;
	std::vector<bool> bv(lb); //bv is the inverse binary.

	for (int c = 0;c != nc;c++)
	{
		for (int k = lb - 1;k >= 0;k--)//Decimal to binary
		{
			if (c&(1 << k))
				bv[k] = 1;
			else bv[k] = 0;
		}

		int InvD = 0;
		for (int k = 0;k != lb;k++) //Binary to decimal
		{
			InvD = InvD * 2 + bv[k];
		}


		for (int r = 0;r != nr;r++)//Rearrange all rows.
		{
			const double*p = scrC.ptr<double>(r);
			double rp = *(p + 2 * InvD);
			double img = *(p + 2 * InvD + 1);
			std::complex<double> tc(rp, img);
			scrV[r][c] = tc;
		}

	}

}

/**
* Rearrange the order of source image with its inverse order.
* Only rearrange by rows.
* Mat SCRC is the complex source vector. SCVR is a 2 dimensional vector placed SCRC's inverse data.
**/
void FastFT::InvOrderV(std::vector<std::vector<std::complex<double>>> scrC, std::vector<std::vector<std::complex<double>>> &scrV)
{
	int nr = scrC.size();
	int nc = scrC[0].size();

	scrV.resize(nr);
	for (int r = 0;r != nr;r++)
		scrV[r].resize(nc);

	size_t lb = log2(nc - 1) + 1;
	std::vector<bool> bv(lb); //bv is the inverse binary.

	for (int c = 0;c != nc;c++)
	{
		for (int k = lb - 1;k >= 0;k--)//Decimal to binary
		{
			if (c&(1 << k))
				bv[k] = 1;
			else bv[k] = 0;
		}

		int InvD = 0;
		for (int k = 0;k != lb;k++) //Binary to decimal
		{
			InvD = InvD * 2 + bv[k];
		}


		for (int r = 0;r != nr;r++)//Rearrange all rows.
		{
			scrV[r][c] = scrC[r][InvD];
		}

	}

}



/**
* FFT a row vector. 
* Using butterfly diagram.
* SCRV is the source and place its FFT at it.
**/
void FastFT::FFTRow(std::vector<std::complex<double>>&scrV)
{
	size_t nr = scrV.size();
	int Lr = log2(nr - 1) + 1; //The number of loop.
			

	for (int k = 0;k != Lr;k++)//For each loop, apply butterfly diagram.
	{
		std::vector<std::complex<double>> temp = scrV;
		if (k == 0) //Donn't Weight.
		{
			for (int c = 0;c<nr;c += 2)
			{
				temp[c] = scrV[c] + scrV[c + 1];
				temp[c + 1] = scrV[c] - scrV[c + 1];
			}
		}
		else
		{

			// Weighting
			int dn = pow(2, k);
			int N = 2 * dn; //N is the number of this part for FFT.
			for (int j = 0;j != nr / N;j++)
			{
				for (int t = 0; t != dn;t++)
				{
					int c = t + dn + j*N;
					double d1 = -2 * CV_PI*t / N;
					std::complex<double> exy(0, d1);
					std::complex<double> d = exp(exy);
					scrV[c] = scrV[c] * d;
				}
			}

			for (int c = 0;c<nr;c += N)  //Compute FFT of each small part.
			{
				for (int t = 0;t != dn;t++)
				{
					int c1 = c + t;
					temp[c1] = scrV[c1] + scrV[c1 + dn];
					temp[c1 + dn] = scrV[c1] - scrV[c1 + dn];
				}
			}
		}
		scrV = temp;
	}
}



/**
* Take the log of a magnitude image for showing at FFT.
* Mat SCR is the soure magnitude image.
* Mat DST is the log(1+SCR).
* Both types are CV_64F.
**/
void FastFT::logMagImage(cv::Mat scr, cv::Mat &dst)
{
	int nr = scr.rows;
	int nc = scr.cols;
	dst.create(nr, nc, CV_64F);

	for (int r = 0;r != nr;r++)
	{
		const double *p = scr.ptr<double>(r);
		double *pd = dst.ptr<double>(r);
		for (int c = 0;c < nc;c++)
		{
			long double d = *p++;

		//Log(1+x)>=0 where x>=0;  //nr*nc*255 may be biger tham maximum double for large image, so multiplication order is important.
			*pd++ = 1 + log2(1 + nr*nc *d* 255);  //multiply nr*nc is an inverse operation of FFT's last line. 
		}
	}
}



/**
* Discrete Fourier Transform with dierect calculatin moethod based on the Fourier transfom formula.
**/
void FastFT::Fourier(cv::Mat scr, cv::Mat &dst)
{
	gip.mv2centR(scr, scr);
	gip.real2cmp(scr, scr);
	FourierM(scr, dst);
	dst = dst / (dst.rows*dst.cols);
}


/**
* Discrete Fourier Transform's main part.
* Mat scrC is a complex source Mat. DSTC is the complex result of DFT.
**/
void FastFT::FourierM(cv::Mat scrC, cv::Mat& dstC)
{

	int nr = scrC.rows;
	int nc = scrC.cols;

	dstC.create(nr, nc, CV_64FC2);

	std::complex<double> m;
	for (int u = 0;u != nr;u++)
	{
		double *pd = dstC.ptr<double>(u);
		for (int v = 0;v != nc;v++)
		{
			m = 0;
			for (int x = 0;x != nr;x++)
			{
				const double * ps = scrC.ptr<double>(x);
				for (int y = 0;y != nc;++y)
				{
					double re = *ps++;
					double img = *ps++;
					std::complex<double> fxy(re, img);
					double d1 = -2 * CV_PI*(double(u)*double(x) / double(nr) + double(v)*double(y) / double(nc));
					std::complex<double> exy(0, d1);
					std::complex<double> d = exp(exy);
					m += fxy*d;
				}
			}
			*pd++ = real(m);  //1st channel for real.
			*pd++ = imag(m);  //2nd channel for imaginary part
		}
	}

}

/**
* Inverse Fourier Transform. Take the direct calculation based on Fourier transform formula.
*/
void FastFT::InvFourier(cv::Mat scrC, cv::Mat &dstC)
{
	gip.cmp2con(scrC, scrC);  //Conjugate
	FourierM(scrC, dstC); //DFT
   //	dstC = dstC / (dstC.rows*dstC.cols);
}
