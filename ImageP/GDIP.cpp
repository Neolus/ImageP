#include "stdafx.h"
#include"GDIP.h"
#include <limits> 

#define eps std::numeric_limits<double>::min()

GDIP::GDIP()
{
}

GDIP::~GDIP()
{
}


/**
* Judge an image whether it's a gray image.
* Mat SCR is a source image whose data type must be CV_8U or CV_8UC3.
**/
bool GDIP::IsGray(cv::Mat scr)
{
	bool IGray(true);
	int nr = scr.rows;
	int nc = scr.cols;
	int ch = scr.channels();

	if (ch == 1) return IGray;
	else 
	{
		
		for (int r = 0;r != nr;r++)
		{
			uchar *p = scr.ptr<uchar>(r);
			for (int c = 0;c < nc;c++)
			{
				int b = *p++;
				int g = *p++;
				int r = *p++;

				if (b == g&&b == r)  //When R=G=B, is gray.
					IGray = IGray & true;
				else
				{
					IGray = false;
					return IGray;
				}
			}
		}
		return IGray;	
	}

}


/**
* Normalize an CV_8U Mat to CV_64F one.
*/
void GDIP::normlz(cv::Mat scr, cv::Mat &dst)
{
	int nr = scr.rows;
	int nc = scr.cols;
	int ch = scr.channels();
	if(ch==1) dst.create(nr, nc, CV_64F);
	if(ch==3)  dst.create(nr, nc, CV_64FC3);

	for (int r = 0;r != nr;r++)
	{
		uchar *p = scr.ptr<uchar>(r);
		double *pd = dst.ptr<double>(r);
		for (int c = 0;c < nc*ch;c++)
		{
			
			*pd++ = *p / 255.0;;
			p++;
		}
	}
}



/**
* Normalize an CV_64F Mat to CV_8U one.
*/
void GDIP::CV64F2CV8U(cv::Mat scr, cv::Mat &dst)
{
	int nr = scr.rows;
	int nc = scr.cols;
	int ch = scr.channels();
	if (ch == 1) dst.create(nr, nc, CV_8U);
	if (ch == 3)  dst.create(nr, nc, CV_8UC3);
//	dst.create(nr, nc, CV_8U);

	for (int r = 0;r != nr;r++)
	{
		const double *p = scr.ptr<double>(r);
		uchar *pd = dst.ptr<uchar>(r);
		for (int c = 0;c < nc*ch;c++)  //Faster than using one loop for ch.
		{
			*pd++ = *p * 255;
			p++;
		}
	}
}


/**
* Transform a RGB image to a gray one.
*/
void GDIP::BGR2Gray(cv::Mat scr, cv::Mat& dst)
{
	int nr = scr.rows;
	int nc = scr.cols;
	dst.create(nr, nc, CV_8U);

	for (int r = 0;r != nr;r++)
	{
		const uchar *p = scr.ptr<uchar>(r);
		uchar *pd = dst.ptr<uchar>(r);
		for (int c = 0;c < nc;c++)
		{
			int B = *p++;
			int G = *p++;
			int R = *p++;
			int s = 0.114*B + 0.587*G + 0.299*R;  //A typical fumula for transforming.

			if (s > 255) s = 255;
			*pd++ =s;
		}
	}

}


/**
* Map double(CV_64F): [min,max]->[0,255]
**/
void GDIP::Map2CV8U(cv::Mat scr, cv::Mat &dst)
{
	int nr = scr.rows;
	int nc = scr.cols;
	dst.create(nr, nc, CV_8U);
	double mi = 0, ma = 0;


	for (int r = 0;r != nr;r++)
	{
		const double *p = scr.ptr<double>(r);
		for (int c = 0;c < nc;c++)
		{
			double d = *p++;
			if (d > ma) ma = d;  //Maximum
			if (d < mi) mi = d;  //Minimum
		}
	}

	for (int r = 0;r != nr;r++)
	{
		const double *p = scr.ptr<double>(r);
		uchar *pd = dst.ptr<uchar>(r);
		for (int c = 0;c < nc;c++)
		{
			double d = *p++;
			*pd++ = (d - mi) * 255 / (ma - mi);
		}
	}


}



/**
* Move an image and let its Fourier transform be centered.
* SCR is the source image, DST is the output. SCR must be a real one.
*/
void GDIP::mv2centR(cv::Mat scr, cv::Mat &dst)
{
	int nr = scr.rows;
	int nc = scr.cols;

	scr.copyTo(dst);

	for (int r = 0;r != nr;r++)
	{
		double *p = dst.ptr<double>(r);
		for (int c = 0;c < nc;c += 2)  //f(r,c)=(-1)^(r+c)*f(r,c).
		{
			if (c == 0 & (((c + r) % 2) == 0)) c += 1;
			*(p + c) = -*(p + c);
		}
	}
}

/**
* Another edition of MV2CENT funtion, for a comlex Mat.
* Move the image and let its Fourier transform is centered.
* SCR is the source image, DST is the output. SCR must be a complex one.
*/
void GDIP::mv2centC(cv::Mat scr, cv::Mat &dst)
{
	int nr = scr.rows;
	int nc = scr.cols;

	scr.copyTo(dst);

	for (int r = 0;r != nr;r++)
	{
		double *p = dst.ptr<double>(r);
		for (int c = 0;c < 2 * nc;c += 4)
		{
			if (c == 0 & ((c + r) % 2 == 0)) c += 1;
			*(p + c) = -*(p + c);
			*(p + c + 1) = -*(p + c + 1);
		}
	}
}


/**
* Change a real Mat to a complex one.
* Mat R is the source Mat whose type must be CV_64F; 
* Mat CMP is the destination whose type is CV_64FC2.
*/
void GDIP::real2cmp(cv::Mat r, cv::Mat &cmp)
{


	int nr = r.rows;
	int nc = r.cols;
	cmp.create(nr, nc, CV_64FC2); //The 1st channel is real. The 2nd channel is image.

	for (int x = 0;x != nr;x++)
	{
		const double * ps = r.ptr<double>(x);
		double * pd = cmp.ptr<double>(x);
		for (int y = 0;y != nc;++y)
		{
			*pd++ = *ps++;
			*pd++ = 0;
		}
	}
}

/**
* Change a complex Mat to its conjugate.
**/
void GDIP::cmp2con(cv::Mat scrC, cv::Mat &dstC)
{
	int nr = scrC.rows;
	int nc = scrC.cols;
	dstC.create(nr, nc, CV_64FC2);
	for (int x = 0;x != nr;x++)
	{
		const double * ps = scrC.ptr<double>(x);
		double * pd = dstC.ptr<double>(x);
		for (int y = 0;y != nc;++y)
		{
			*pd++ = *ps++;
			*pd++ = -(*ps++);
		}
	}
}


/**
* Pad zeros at surroundings of Image Mat; Put the original one at the center of a new one.
* SCR is the original image; 
* DST is the padded Mat; PC=DST.cols & PR=DST.rols;
* Both types must be CV_64F.
**/
void GDIP::PadZero(cv::Mat scr, cv::Mat &dst, size_t pr, size_t pc)
{

	int nr = scr.rows;
	int nc = scr.cols;

	if (nr == pr&nc == pc)
	{
		dst = scr;
		return;
	}

	int bu = (pr - nr) / 2;      //Rows padded above the original image
//	int bd = (pr + 1 - nr) / 2;  //Rows padded below the original.
	int bl = (pc - nc) / 2;      //Columns padded at the left.
 //	int br= (pc + 1 - nc) / 2;  //Columns padded at the right.
	dst.create(pr, pc, CV_64F);
	for (int r = 0;r != pr;r++)
	{

		double *pd = dst.ptr<double>(r);
		if (r < bu | r >= (nr + bu))
		{
			for (int c = 0;c < pc;c++)
			{
				*pd++ = 0;
			}
		}
		else
		{
			double *p = scr.ptr<double>(r - bu);
			for (int c = 0;c < pc;c++)
			{
				if (c >= bl&&c < bl + nc)
				{
					*(pd + c) = *(p + c - bl);
				}
				else
				{
					*(pd + c) = 0;
				}

			}
		}
	}
}


/**
* Extract the center of a padded image; It is a inverse operation of Function PADZERO
* SCR is the padded image; 
* DST is a small Mat; MC=DST.cols & MR=DST.rols;
* Both types must be CV_64F.
*/
void GDIP::ExtCenter(cv::Mat scr, cv::Mat &dst, size_t mr, size_t mc)
{
	int nr = scr.rows;
	int nc = scr.cols;
	if (nr == mr&nc == mc)  //Sizes are same.
	{
		dst = scr;
		return;
	}

	int bu = (nr - mr) / 2;
	int bl = (nc - mc) / 2;
	dst.create(mr, mc, CV_64F);
	for (int r = 0;r != mr;r++)
	{
		const double *pd = scr.ptr<double>(r + bu);
		pd += bl;
		double *p = dst.ptr<double>(r);
		for (int c = 0;c < mc;c++)
		{
			*p++ = *pd++;
		}
	}
}


/**
* Transform a BGR image to HSV space. CV_8UC3 to CV_8UC3
*/
cv::Mat GDIP::BGR2HSV(const cv::Mat & image)
{
	cv::Mat A(image.size(), CV_8UC3);
	int nr = A.rows;
	int nc = A.cols;
	int channels = A.channels();

	for (int i = 0;i != nr;i++)
	{
		const uchar* data_in = image.ptr<uchar>(i); 
		uchar* data_out = A.ptr<uchar>(i);
		for (int j = 0;j != nc;j++)
		{
			int B = *data_in++;
			int G = *data_in++;
			int R = *data_in++;
			double Ma = MAX(MAX(B, G), R);
			double Mi = MIN(MIN(B, G), R);

			double H;
			if (Ma == R)
				H = 60 * (G - B) / (Ma - Mi);
			if (Ma == G)
				H = 120 + 60 * (B - R) / (Ma - Mi);
			if (Ma == B)
				H = 240 + 60 * (R - G) / (Ma - Mi);

			if (H < 0)
				H = H + 360;

			*data_out++ = uchar(H / 2);
			double S;
			if (Ma == 0)
				S = 0;
			else
				S = (Ma - Mi) / Ma;
			*data_out++ = 255 * S;
			*data_out++ = Ma;

			/*
			double co = 0.5*(R -G+R- B) / (sqrt((R - G)*(R - G) + (R - B)*(G - B)));
			double theta = acos(co);

			if (B <= G)
			*data_out++ = theta/(2*CV_PI);
			else
			*data_out++=(2*CV_PI-theta)/(2*CV_PI);

			*data_out++ = (Ma - Mi) / Ma;
			*data_out++ = Ma / 255;
			*/
		}
	}
	return A;
}

/**
* Transform a BGR image to HSV space. CV_64FC3 to CV_64FC3
*/
cv::Mat GDIP::BGR2HSV2(const cv::Mat & image)
{
	cv::Mat A(image.size(), CV_64FC3);
	int nr = A.rows;
	int nc = A.cols;
	int channels = A.channels();

	for (int i = 0;i != nr;i++)
	{
		const uchar* data_in = image.ptr<uchar>(i); 
		double* data_out = A.ptr<double>(i);
		for (int j = 0;j != nc;j++)
		{
			double B = *data_in++;
			double G = *data_in++;
			double R = *data_in++;

			double Ma = MAX(MAX(B, G), R);
			double Mi = MIN(MIN(B, G), R);


			double H;
			if (Ma == R)
				H = 60 * (G - B) / (Ma - Mi);
			if (Ma == G)
				H = 120 + 60 * (B - R) / (Ma - Mi);
			if (Ma == B)
				H = 240 + 60 * (R - G) / (Ma - Mi);

			if (H < 0)
				H = H + 360;

			H = H / 360;
			*data_out++ = H;

			double S;
			if (Ma == 0)
				S = 0;
			else
				S = (Ma - Mi) / Ma;
			*data_out++ = S;
			*data_out++ = Ma / 255;  //Normalize
		}
	}
	return A;
}

/**
* Normalize A RGB or Gray image to the interval [0,1.0].
* Only consider transforming CV_8U to CV_64F in the case of 1 or 3 channels. 
*/
/*
cv::Mat NormalizeRGB(const cv::Mat& image) 
{
	cv::Mat A(image.size(), CV_64F);
	int nr = A.rows;
	int nc = A.cols;
	int channels = A.channels();
	if (channels == 1) //Gray image CV_8U to CV_64F
	{
		for (int i = 0;i != nr;i++)
		{
			const uchar* data_in = image.ptr<uchar>(i);
			double* data_out = A.ptr<double>(i);
			for (int j = 0;j != nc;j++)
			{
				*data_out++ = *data_in++ / 255.0;
			}
		}
	}
	else if (channels == 3) //RGB image
	{
		for (int i = 0;i != nr;i++)
		{
			const uchar* data_in = image.ptr<uchar>(i);
			double* data_out = A.ptr<double>(i);
			for (int j = 0;j != nc;j++)
			{
				*data_out++ = *data_in++ / 255.0;
				*data_out++ = *data_in++ / 255.0;
				*data_out++ = *data_in++ / 255.0;
			}
		}
	}
	return A;
}

*/

/**
* Transform a BGR image to HSI space. CV_8UC3 to CV_64FC3
*/
void GDIP::BGR2HSI(cv::Mat scr, cv::Mat &dst)
{
	dst.create(scr.size(), CV_64FC3);

	int nr = scr.rows;
	int nc = scr.cols;
	int channels = scr.channels();

	for (int i = 0;i != nr;i++)
	{
		const double* data_in = scr.ptr<double>(i); //The start adress of i row. 
		double* data_out = dst.ptr<double>(i);
		for (int j = 0;j != nc;j++)
		{
			double B = *data_in++;
			double G = *data_in++;
			double R = *data_in++;

			double Ma = MAX(MAX(B, G), R);
			double Mi = MIN(MIN(B, G), R);


			double co = 0.5*(R - G + R - B) / (eps + sqrt((R - G)*(R - G) + (R - B)*(G - B))); 
			if (co > 1 && co < 1 + 1e-8) co = 1; //Two possible computation errors.
			if (co<0 && co>-1e-8) co = 0;


			double theta = acos(co);
			if (B <= G)
				*data_out++ = theta / (2 * CV_PI);
			else
				*data_out++ = (2 * CV_PI - theta) / (2 * CV_PI);


			/*		double H;
			if (Ma == R)
			H = 60 * (G - B) / (Ma - Mi+eps);
			if (Ma == G)
			H = 120 + 60 * (B - R) / (Ma - Mi+eps);
			if (Ma == B)
			H = 240 + 60 * (R - G) / (Ma - Mi+eps);

			if (H < 0)
			H = H + 360;

			H = H/360;
			*data_out++ = H;
			*/
			double S = 1 - 3 * Mi / (R + G + B + eps);

			*data_out++ = S;
			*data_out++ = (R + G + B) / 3;
		}
	}
}


/**
* Transform a HSI image to BGR space. CV_64FC3 to CV_8UC3
*/
void GDIP::HSI2BGR(cv::Mat scr,cv::Mat &dst)
{
	dst.create(scr.size(), CV_64FC3);

	int nr = scr.rows;
	int nc = scr.cols;
	int channels = scr.channels();

	for (int i = 0;i != nr;i++)
	{
		const double* data_in = scr.ptr<double>(i); //The start adress of i row.
		double * data_out = dst.ptr<double>(i);
		for (int j = 0;j != nc;j++)
		{
			double H = *data_in++;
			double S = *data_in++;
			double I = *data_in++;

			H = 2 * CV_PI*H;
			double R = 0, G = 0, B = 0;

			double C1 = I*(1 - S);


			if (H <2 * CV_PI / 3 && H >= 0)  //Compute as formula
			{
				B = C1;
				R = I*(1 + S*cos(H) / cos(CV_PI / 3 - H));
				G = 3 * I - R - B;
			}
			else if (H <4 * CV_PI / 3 && H >= 2 * CV_PI / 3)
			{
				H = H - 2 * CV_PI / 3;
				R = C1;
				G = I*(1 + S*cos(H) / cos(CV_PI / 3 - H));
				B = 3 * I - R - G;
			}
			else if (H <2 * CV_PI && H >= 4 * CV_PI / 3)
			{
				H = H - 4 * CV_PI / 3;
				G = C1;
				B = I*(1 + S*cos(H) / cos(CV_PI / 3 - H));
				R = 3 * I - G - B;
			}

			*data_out++ = MAX(MIN(B, 1), 0); //Scope must be [0,1]
			*data_out++ = MAX(MIN(G, 1), 0);
			*data_out++ = MAX(MIN(R, 1), 0);
		}
	}
}

/**
* Compute the root-mean-square of an error image.
* Mat SCR is an error image of two images. Type of SCR must be CV_8U.
*/
double GDIP::rms(cv::Mat scr)
{
	int nr = scr.rows;
	int nc = scr.cols;
	double er = 0;

	for (int i = 0;i != nr;i++)
	{
		const uchar* p = scr.ptr<uchar>(i); //The start adress of i row.
		for (int j = 0;j != nc;j++)
		{
			double temp = *p++;
			er += temp*temp;
		}
	}
	er = sqrt(er / (nr*nc));

	return er;

}