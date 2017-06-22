
// ImagePDlg.h : 头文件

#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include"GDIP.h"
#include"FastFT.h"
#include"DCosT.h"
#include"HistP.h"
#include<fstream>

#pragma once


// CImagePDlg 对话框
class CImagePDlg : public CDialogEx
{
// 构造
public:
	CImagePDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGEP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int CImagePDlg::Mat2CImage(cv::Mat *mat, CImage &img);
	void CImagePDlg::ShowCImage(const cv::Mat& im, int id);

	afx_msg void OnBnClickedOpenImage();  
	afx_msg void OnBnClickedFFT();   // Click FFT.
	afx_msg void OnBnClickedIFFT();  // Click IFFT.
	afx_msg void OnBnClickedDCT();   //Click DCT.
	 
	afx_msg void OnBnClickedIDCT();  //Click IDCT.
	afx_msg void OnBnClickedIDCTError(); //Click IDCTError.

	afx_msg void OnEnChangeEdit1();  //Get the truncation ratio form user.
	afx_msg void OnBnClickedHistGray();  //Click HistRGB.
	afx_msg void OnBnClickedHistRGB();   //Click HistRGB
	afx_msg void OnBnClickedHistHSI();   //Click HistHSI.


	bool isGray;  //A flag whether loaded image is gray.
	cv::Mat scr;  //Place source image.
	cv::Mat scr_gray;  //Gray image of loaded image.
	cv::Mat HistImage; //Histogram equalization image.
	cv::Mat FFTImage;  //FFT image.
	cv::Mat IFFTImage; //IFFT image.
	cv::Mat DCTImage;  //DCT image.
	cv::Mat IDCTImage; //IDCT image.
	double idctR;      //IDCT truncation ratio.
	GDIP gip;          //Class general DIP functions.
	double krr;        //User input number form MFC.

};
