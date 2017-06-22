#include "stdafx.h"
#include "DCosT.h"


DCosT::DCosT()
{
}

DCosT::~DCosT()
{
}


/**
* Discrete Cosine Transform of an image
* Mat SCR is the source image whose type must be CV_64F.
* Mat DST is the transformed Mat whose type is CV_64F.
* NB is the size of patch image in DCT. Default is 8.
**/
void DCosT::DCTMat(cv::Mat scr, cv::Mat& dst, size_t nb = 8)
{
	std::vector<std::vector<cv::Mat>> BV;
	divBlock(scr, BV, nb);  //Divide image to some sub-image blocks whose size is nb*nb.
	std::vector<std::vector<double>> cosM = cosMat(nb); //Compte a cosine matrix. alpha(u)*cos((2x+1)*u*PI/(2n)), see the kernal function of DCT.
	int br = BV.size();   //Row of Blocks.
	int bc = BV[0].size();//Column of Blocks.

	std::vector<std::vector<cv::Mat>> dV(br);//Place result of DCT
	for (int i = 0;i != br;i++)
		dV[i].resize(bc);

	for (int i = 0;i != br;i++)
	{
		for (int j = 0;j != bc;j++)
		{
			dctBlock(BV[i][j], dV[i][j], cosM); //Call function to compute DCT of a block.
		}
	}
	mergeBlock(dV, dst, nb); //Merge blocks to a full image.
}

/**
* Invere Discrete Cosine  Transform of coefficients of DCT.
* Mat SCR is coefficient matrix of DCT, whose type must be CV_64F.
* Mat DST is output image of IDCT whose tyep is CV_64F.
* KR is the ratio of remain coefficients of DCT.
* NB is the size of block image.
**/

void DCosT::IDCTMat(cv::Mat scr, cv::Mat& dst, double kr = 1, size_t nb = 8)
{
	std::vector<std::vector<cv::Mat>> BV;
	divBlock(scr, BV, nb);// Full Mat to blocks.
	std::vector<std::vector<double>> cosM = cosMat(nb);  //Cosine matrix.
	int br = BV.size();   //Row of Blocks.
	int bc = BV[0].size(); //Column of Blocks.

	std::vector<std::vector<double>> trcT = trcTemp(nb, kr);  //TRCT a trunction template 2-dimensional matrix.

	for (int i = 0;i != br;i++)
	{
		for (int j = 0;j != bc;j++)
		{
			for (int r = 0;r != nb;r++)
			{
				double *p = BV[i][j].ptr<double>(r);
				for (int c = 0;c != nb;c++)
				{
					*p++ = (*p)*trcT[r][c];    //Truncate
				}
			}
		}
	}

	std::vector<std::vector<cv::Mat>> IDCV(br); //Matrices of IDCT.
	for (int i = 0;i != br;i++)
		IDCV[i].resize(bc);

	for (int i = 0;i != br;i++)
	{
		for (int j = 0;j != bc;j++)
		{
			idctBlock(BV[i][j], IDCV[i][j], cosM); //Call function to compute IDCT of a block.
		}
	}
	mergeBlock(IDCV, dst, 8); //Merge to a full image.

}

/**
* Compute a template matrix of truncation of DFT coefficients.
* N is the size of Block image.
* KR is the ratio of remain coefficients. Count with zig-zag form the upper-left.
* Return a 2-dimensional vector of truncatin template. left-upper is 1 and lower-right is 0.
**/

std::vector<std::vector<double>>DCosT::trcTemp(size_t n, double kr)
{
	std::vector<std::vector<double>> tp(n);
	for (int i = 0;i != n;i++)
		tp[i].resize(n);

	int tn = n*n;  //Total number of pixels.
	int kn = kr*tn;  //Number of keep pixels. 

	int sn = 0; //Counter

	for (int i = 1;i != 2 * n;i++)  //Traverse by zig-zag.
	{
		if (i <= n)
		{
			if (i % 2 == 0)
			{
				for (int r = 0;r <= i - 1;r++)
				{
					sn++;
					int c = i - 1 - r;
					if (sn <= kn)  tp[r][c] = 1;
					else tp[r][c] = 0;

				}
			}
			else
			{
				for (int r = i - 1;r >= 0;r--)
				{
					sn++;
					int c = i - 1 - r;
					if (sn <= kn)  tp[r][c] = 1;
					else tp[r][c] = 0;


				}
			}
		}
		else
		{
			if (i % 2 == 0)
			{
				for (int c = n - 1;c >= i - n; c--)
				{
					sn++;
					int r = i - 1 - c;
					if (sn <= kn)  tp[r][c] = 1;
					else tp[r][c] = 0;

				}
			}
			else
			{
				for (int c = i - n;c <= n - 1;c++)
				{
					int r = i - 1 - c;
					sn++;
					if (sn <= kn)  tp[r][c] = 1;
					else tp[r][c] = 0;


				}
			}
		}
	}
	return tp;
}

/**
* Divide imgae Mat into blocks.
* Mat SCR is the source image, whose type is CV_64F.
* DSTV, a 2-dimensional vector for placing blocks.
* N is the size of one block.
**/
void DCosT::divBlock(cv::Mat scr, std::vector<std::vector<cv::Mat>> &dstV, size_t n)
{
	size_t nr = scr.rows;
	size_t nc = scr.cols;

	size_t vr = (nr - 1) / n + 1;  //Rows of blocks
	size_t vc = (nc - 1) / n + 1;  //Column of blocks.
	dstV.resize(vr);
	for (int i = 0;i != vr;i++)
		dstV[i].resize(vc);

	for (int i = 0;i != vr;i++)
	{
		for (int j = 0;j != vc;j++)
		{
			dstV[i][j].create(n, n, CV_64F);
			cv::Mat& BlockMat = dstV[i][j];

			for (int r = 0;r != n;r++)  //Each block
			{
				double *p = BlockMat.ptr<double>(r);
				const double *ps = scr.ptr<double>(n * i + r);
				ps += n * j;
				for (int c = 0;c != n;c++)
				{
					*p++ = *ps++;
				}
			}
		}
	}
}

/**
* Merge blocks into an image.
* SCRV is source blocks.
* Mat DST is the output image.
* N is the size of a block.
* Type of source block and DST are CV_64F.
**/
void DCosT::mergeBlock(std::vector<std::vector<cv::Mat>> &scrV, cv::Mat &dst, size_t n)
{
	int br = scrV.size();   //Number of blocks in row
	int bc = scrV[0].size();//Number of blocks in column

	size_t nr = br*n;  //Size of mergered image
	size_t nc = bc*n;
	dst.create(nr, nc, CV_64F);  

	for (int i = 0;i != br;i++)
	{
		for (int j = 0;j != bc;j++)  //Each block
		{
			for (int r = 0;r != n;r++)
			{
				const double *p = scrV[i][j].ptr<double>(r);
				double *pd = dst.ptr<double>(n*i + r);
				pd += j*n;
				for (int c = 0;c != n;c++)
				{
					*pd++ = *p++;
				}
			}

		}
	}
}

/**
* Discrete Cosine Transform of a block Mat.
* Mat SCRB is the soure block. Type of SCRB must be CV_64F.
* Mat DSTB is the output Mat whose size and type are same as SCRB.
* COSMAT is a cosine matrix for saving computation.
*/
void DCosT::dctBlock(cv::Mat scrB, cv::Mat& dstB, const std::vector<std::vector<double>>& cosMat)
{
	size_t nr = scrB.rows;
	size_t nc = scrB.cols;

	dstB.create(nr, nc, CV_64F);

	for (int u = 0;u != nr;u++)
	{
		double *p = dstB.ptr<double>(u);
		for (int v = 0;v != nc;v++)  //Compute T(u,v) by accumulation.
		{
			double dc = 0;
			for (int x = 0;x != nr;x++)
			{
				const double* ps = scrB.ptr<double>(x);
				for (int y = 0;y != nc;y++)
				{
					dc += (*ps)*cosMat[u][x] * cosMat[v][y]; //g(x,y)*alpha(u)*cos((2x+1)*u*PI/(2*n))*alpha(v)*cos((2y+1)*v*PI/(2*n)).
					ps++;
				}
			}
			*p++ = dc;
		}
	}
}

/**
* Inverse Discrete Cosine Transform of a DCT block Mat.
* Mat SCRB is the soure block of DCT coefficients. Type of SCRB must be CV_64F.
* Mat DSTB is the output Mat whose size and type are same as SCRB.
* COSMAT is a cosine matrix for saving computation.
*/
void DCosT::idctBlock(cv::Mat scrB, cv::Mat& dstB, const std::vector<std::vector<double>> &cosMat)
{
	size_t nr = scrB.rows;
	size_t nc = scrB.cols;

	dstB.create(nr, nc, CV_64F);

	for (int x = 0;x != nr;x++)
	{
		double *p = dstB.ptr<double>(x);
		for (int y = 0;y != nc;y++)  //Compute g(x,y) by accumulation. 
		{
			double dc = 0;
			for (int u = 0;u != nr;u++)
			{
				const double* ps = scrB.ptr<double>(u);
				for (int v = 0;v != nc;v++)
				{
					dc += (*ps)*cosMat[u][x] * cosMat[v][y]; //T(u,v)*alpha(u)*cos((2x+1)*u*PI/(2*n))*alpha(v)*cos((2y+1)*v*PI/(2*n)).
					ps++;
				}
			}
			*p++ = dc;
		}
	}
}

/**
* Compute a cosine matrix.  alpha(u)*cos((2x+1)*u*PI/(2*n), where u=0:n-1 and x=0:n-1.
* N is the size of block image.
*/
std::vector<std::vector<double>> DCosT::cosMat(size_t n)
{
	std::vector<std::vector<double>> r(n);
	for (int i = 0;i != n;i++)
		r[i].resize(n);
	double nd = double(n);
	double au = sqrt(1.0 / nd);
	double au2 = sqrt(2.0 / nd);

	for (int u = 0;u != n;u++)
	{
		if (u != 0) au = au2;
		for (int x = 0;x != n;x++)
		{
			r[u][x] = au*cos(((2 * x + 1)*u*CV_PI) / (2 * nd));
		}
	}
	return r;
}

