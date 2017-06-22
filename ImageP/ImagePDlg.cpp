
// ImagePDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageP.h"
#include "ImagePDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImagePDlg 对话框



CImagePDlg::CImagePDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IMAGEP_DIALOG, pParent)
	//, kr(0)
	, krr(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImagePDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Scroll(pDX, IDC_SCROLLBAR1, kr);
//	DDV_MinMaxInt(pDX, kr, 0, 100);
	DDX_Text(pDX, IDC_EDIT1, krr);
	DDV_MinMaxDouble(pDX, krr, 0, 1);
}

BEGIN_MESSAGE_MAP(CImagePDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CImagePDlg::OnBnClickedOpenImage)
	ON_BN_CLICKED(IDC_BUTTON2, &CImagePDlg::OnBnClickedDCT)
	ON_BN_CLICKED(IDC_FFT, &CImagePDlg::OnBnClickedFFT)
	ON_BN_CLICKED(IDC_BUTTON3, &CImagePDlg::OnBnClickedIFFT)
	ON_BN_CLICKED(IDC_BUTTON4, &CImagePDlg::OnBnClickedIDCT)
	ON_BN_CLICKED(IDC_BUTTON5, &CImagePDlg::OnBnClickedIDCTError)
	ON_EN_CHANGE(IDC_EDIT1, &CImagePDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON6, &CImagePDlg::OnBnClickedHistGray)
	ON_BN_CLICKED(IDC_BUTTON7, &CImagePDlg::OnBnClickedHistRGB)
	ON_BN_CLICKED(IDC_BUTTON8, &CImagePDlg::OnBnClickedHistHSI)
END_MESSAGE_MAP()


// CImagePDlg 消息处理程序

BOOL CImagePDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	int cx = GetSystemMetrics(SM_CXFULLSCREEN);  //Available screen width
	int cy = GetSystemMetrics(SM_CYFULLSCREEN);  //Available screen height

	int nWidth = GetSystemMetrics(SM_CXSCREEN);  //Full screen width
	int nHeight = GetSystemMetrics(SM_CYSCREEN); //Full screen height
	//CString strScreen;
	//strScreen.Format(L"%d,%d", nWidth, nHeight);
	//MessageBox(strScreen);



	CRect rectDlg0;
	GetClientRect(rectDlg0);
	MoveWindow(0, 0, 0.7*cx, 0.7*cy);  //Reset MFC window's size. 0.7 times of screen.

	//setWindowPos(&wndTop, 0, 0,0.7*cx, 0.7*cy, SWP_NOMOVE);
	//CRect rectDlg;
	//GetClientRect(rectDlg);
	//CString strDlg;
	//strDlg.Format(L"%d,%d,%d,%d", rectDlg.left, rectDlg.top, rectDlg.Width(), rectDlg.Height());
	//MessageBox(strDlg);

	int mxy = MIN(cx, cy);
	
	CStatic *p = (CStatic*)GetDlgItem(IDC_STATIC);
	p->MoveWindow(30, 100, 0.5*mxy, 0.5*mxy);//Move the picture control(30,100) and resize it with 0.5 times of screen.
							 
	//CRect rectCtrl;
	//p->GetWindowRect(rectCtrl);
	//this->ScreenToClient(rectCtrl);

	//CString str;
	//str.Format(L"%d,%d,%d,%d", rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height());
	//MessageBox(str);
	
	krr = 0.5;
	UpdateData(FALSE);
	idctR = krr;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CImagePDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CImagePDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{		
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CImagePDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CImagePDlg::OnBnClickedOpenImage()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filter;
	filter = "所有文件(*.bmp,*.jpg,*.gif,*tif)|*.bmp;*.jpg;*.gif;*.tif| BMP(*.bmp)|*.bmp| JPG(*.jpg)|*.jpg| GIF(*.gif)|*.gif| TIF(*.tiff)|*.tif||";

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		//Clear Previous Image.
		scr.release();
		scr_gray.release();
		HistImage.release();
		FFTImage.release();
		IFFTImage.release(); 
		DCTImage.release();
		IDCTImage.release();

		//Open Windows resource manager.
		CString PicName = dlg.GetPathName();     //Get the path and filename   eg. D:\pic\abc.bmp  
		CString EntName = dlg.GetFileExt();      //Get the file extension.
		EntName.MakeLower();

		CT2CA pszConvertedAnsiString(PicName);
		std::string s1(pszConvertedAnsiString);

		scr = cv::imread(s1);   //OpenCV reads all image as colorful by default.
		isGray=gip.IsGray(scr); //An gray image's three channel will be equal.

		ShowCImage(scr, IDC_STATIC);
		gip.BGR2Gray(scr, scr_gray);
		gip.normlz(scr_gray, scr_gray);
		gip.normlz(scr, scr);

	}
}

/**
* Transform an OpenCV Mat to CImage for supporting MFC
* Reference form the internet.
*/
int CImagePDlg::Mat2CImage(cv::Mat *mat, CImage &img)
{
	if (!mat || mat->empty())
		return -1;
	int nBPP = mat->channels() * 8;
	img.Create(mat->cols, mat->rows, nBPP);
	if (nBPP == 8)
	{
		static RGBQUAD pRGB[256];
		for (int i = 0; i < 256; i++)
			pRGB[i].rgbBlue = pRGB[i].rgbGreen = pRGB[i].rgbRed = i;
		img.SetColorTable(0, 256, pRGB);
	}
	uchar* psrc = mat->data;
	uchar* pdst = (uchar*)img.GetBits();
	int imgPitch = img.GetPitch();
	for (int y = 0; y < mat->rows; y++)
	{
		memcpy(pdst, psrc, mat->cols*mat->channels());//mat->step is incorrect for those images created by roi (sub-images!)
		psrc += mat->step;
		pdst += imgPitch;
	}
	return 0;

	
}

/**
* Show an OpenCV in MFC.
* Mat IM is the OpenCV mat for showing. Type of IM must be CV_8U.
* ID is the ID of a picture control in MFC.
* Part of this funtion reference form the internet.
*/

void CImagePDlg::ShowCImage(const cv::Mat &im, int id)
{
	int cxs = GetSystemMetrics(SM_CXFULLSCREEN);
	int cys = GetSystemMetrics(SM_CYFULLSCREEN);
	int mxy = MIN(cxs, cys);


	CStatic* pStatic = (CStatic*)GetDlgItem(id);
	CRect RectC;
	pStatic->GetClientRect(&RectC);   //Clear Picture control.
	pStatic->GetDC()->FillSolidRect(RectC.left, RectC.top, RectC.Width(), RectC.Height(), RGB(240, 240, 240));  //Reset.
	

	int maxPSize = 0.5*mxy; //Maximum size of picture control.
	size_t nr = im.rows;
	size_t nc = im.cols;
	size_t nm = MAX(nr, nc);


	cv::Mat FitWindow=im.clone();
	
	if (nm > maxPSize) //Image is too large to show.
	{
		cv::Mat im_64;
		gip.normlz(im, im_64);  //CV_8U to CV_64F
		double d1 = maxPSize / double(nm);
	
		cv::Mat tempMat(d1*nr,d1*nc, im_64.type());
		cv::resize(im_64, tempMat, tempMat.size());  //Resize image to maxPSize.
		gip.CV64F2CV8U(tempMat, FitWindow);
		
	}

	CImage ci;
	CImagePDlg::Mat2CImage(&FitWindow, ci);
	CRect rect;//Define a CRect. 
	int cx = ci.GetWidth();  //Width of image. 
	int cy = ci.GetHeight(); //Height of image. 


	GetDlgItem(id)->GetWindowRect(&rect);//Get the picture control's Rect.
	ScreenToClient(&rect);               //User screen to the picture rect.   
	GetDlgItem(id)->MoveWindow(rect.left, rect.top, cx, cy, false);//Move and resize picture control.  
	
	CWnd *pWnd = GetDlgItem(id);//Get the handle of picture control. 
	
	pWnd->GetClientRect(&rect);//Get the Rect of picture control choosen by client. 
	CRect rect2(rect.left, rect.top, cx, cy);
	CDC *pDC = pWnd->GetDC();//Get DC of pictrue control. 

	ci.Draw(pDC->m_hDC, rect2); //Draw image in rect2.  
	ReleaseDC(pDC);//Release DC of picture control.  
}
/**
* Click FFT.
* FFT a loaded image then show its magnitude in a picture control.
*/
void CImagePDlg::OnBnClickedFFT()
{
	// TODO: 在此添加控件通知处理程序代码

	if (scr_gray.data == NULL)  //No image have been loaded.
		MessageBox(CString("Please load an image"));
	else
	{
		
		FastFT fst(scr_gray);

		size_t nr = scr_gray.rows;
		size_t nc = scr_gray.cols;

		size_t mrc = nr;
		if (nc > nr)
			mrc = nc;

		int b = log2(mrc - 1) + 1;  //Row and column are same.
		cv::Mat ScrG = scr_gray.clone();  //Clone the source image.

		gip.PadZero(ScrG, ScrG, pow(2, b), pow(2, b));  //Pad zeros to the nearest integer power of 2.

		fst.FFT(ScrG, FFTImage);  //FFT

		cv::Mat mag,mag1,mag2;
		std::vector<cv::Mat> v(2);
		cv::split(FFTImage, v);  //Split real and imaginary.

		cv::magnitude(v[0], v[1], mag); //Compute magnitude.
		fst.logMagImage(mag, mag1); //Take a log
		gip.Map2CV8U(mag1, mag2);  //Map to [0,255]	

		ShowCImage(mag2, IDC_STATIC);  //Showing
	}
	
}


/**
* Click IFFT.
* Using FFT's result to do IFFT and show the IFFT image.
*/
void CImagePDlg::OnBnClickedIFFT()
{
	// TODO: 在此添加控件通知处理程序代码
	if (FFTImage.data == NULL)
		MessageBox(CString("Please FFT before IFFT"));
	else
	{
		FastFT fst;
		fst.IFFT(FFTImage, IFFTImage);  //IFFT

		std::vector<cv::Mat> v2(2);  
		cv::split(IFFTImage, v2);  //Split real and imaginary.
		gip.ExtCenter(v2[0], IFFTImage, scr.rows, scr.cols);  //Extract the center of image padded zeros.
		gip.CV64F2CV8U(IFFTImage, IFFTImage);   //Chnage CV type.
		ShowCImage(IFFTImage, IDC_STATIC);  //Show

	}
}

/**
* Click DCT.
* DCT a loaded image then show its DCT image in a picture control.
*/
void CImagePDlg::OnBnClickedDCT()
{
	// TODO: 在此添加控件通知处理程序代码

	if (scr_gray.data == NULL)
		MessageBox(CString("Please load an image"));
	else
	{
		DCosT Dc;

		size_t nr = scr_gray.rows;
		size_t nc = scr_gray.cols;

		cv::Mat ScrG = scr_gray.clone();  //clone the source image.
		size_t bs = 8;  //size of block.
		size_t ntr = bs*((nr - 1) / bs + 1);  //Nearest interger of multiple of 8.
		size_t ntc = bs*((nc - 1) / bs + 1);

		gip.PadZero(ScrG, ScrG, ntr, ntc);  //Pad zeros to the nearest interger of multiple of 8.
		Dc.DCTMat(ScrG, DCTImage, bs);  //DCT

		cv::Mat DCTSImage;
		gip.Map2CV8U(DCTImage, DCTSImage);
		ShowCImage(DCTSImage, IDC_STATIC);  //Show
	}
}

/**
* Click IDCT.
* Truncate DCT image by a ratio KRR, then IDCT.
*/
void CImagePDlg::OnBnClickedIDCT()
{
	// TODO: 在此添加控件通知处理程序代码
	if (DCTImage.data == NULL)
		MessageBox(CString("Please DCT before IDCT"));
	else
	{
		DCosT Dc;
		idctR = krr;   //Truncation ratio.
		Dc.IDCTMat(DCTImage, IDCTImage, idctR, 8); //IDCT

		gip.ExtCenter(IDCTImage, IDCTImage, scr.rows, scr.cols); //Extract center
		cv::Mat IDCTSImage;
		gip.CV64F2CV8U(IDCTImage, IDCTSImage);
		ShowCImage(IDCTSImage, IDC_STATIC);  //Show
	}
}

/**
* Click IDCTError.
* Show an error image and compute its RMS.
*/
void CImagePDlg::OnBnClickedIDCTError()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IDCTImage.data == NULL)
		MessageBox(CString("Please IDCT before showing error image"));
	else
	{
		cv::Mat IDCTEr = abs(scr_gray - IDCTImage); //Subtract.
		gip.CV64F2CV8U(IDCTEr, IDCTEr);
		double rms = gip.rms(IDCTEr); //Compute RMS
		ShowCImage(IDCTEr, IDC_STATIC); //Show.
	}

}



/**
* Get the truncation ratio form user.
**/
void CImagePDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	idctR = krr;

}
/**
* Click HistGray.
* Histogram equalization of a gray image.
*/

void CImagePDlg::OnBnClickedHistGray()
{
	// TODO: 在此添加控件通知处理程序代码
	if (scr_gray.data == NULL)  //No image have been loaded.
		MessageBox(CString("Please load an image"));
	else
	{
		HistP hsp;
		hsp.HistEq(scr_gray, HistImage, 256, false);  //Histogram equalization.
		gip.CV64F2CV8U(HistImage, HistImage);
		ShowCImage(HistImage, IDC_STATIC);  //Show
	}

}

/**
* Click HsitRGB.
* Histogram equalization of R, G, and B respectively.
* Note that it is not true for color image. Only for comparison.
*/

void CImagePDlg::OnBnClickedHistRGB()
{
	// TODO: 在此添加控件通知处理程序代码
	if(scr.data==NULL)
		MessageBox(CString("Please load an image"));
	else if(isGray)
		MessageBox(CString("Please load a colorful image"));
	else
	{

		HistP hsp;
		hsp.HistEq(scr, HistImage, 256, false); //RGB
		gip.CV64F2CV8U(HistImage, HistImage);
		ShowCImage(HistImage, IDC_STATIC);
	}
}

/**
* Click HistHSI.
* Histogram equalization of I channel.
*/
void CImagePDlg::OnBnClickedHistHSI()
{
	// TODO: 在此添加控件通知处理程序代码

	if (scr.data == NULL)
		MessageBox(CString("Please load an image"));
	else if (isGray)
		MessageBox(CString("Please load a colorful image"));
	else
	{
		HistP hsp;
		cv::Mat ScrHSI;
		gip.BGR2HSI(scr, ScrHSI);  //RGB to HSI

		hsp.HistEq(ScrHSI, HistImage, 256, true);  //Histogram equalization of I.
		gip.HSI2BGR(HistImage, HistImage);  //HSI to RGB.
		gip.CV64F2CV8U(HistImage, HistImage);
		ShowCImage(HistImage, IDC_STATIC);  //Show
	}
}
