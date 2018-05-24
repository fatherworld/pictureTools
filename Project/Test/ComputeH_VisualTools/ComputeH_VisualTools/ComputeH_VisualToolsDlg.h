
// ComputeH_VisualToolsDlg.h : header file
//

#pragma once
#include "afxcmn.h"


// CComputeH_VisualToolsDlg dialog
class CComputeH_VisualToolsDlg : public CDialogEx
{
// Construction
public:
	CComputeH_VisualToolsDlg(CWnd* pParent = NULL);	// standard constructor
    ~CComputeH_VisualToolsDlg();	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPUTEH_VISUALTOOLS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    int pointNum;
    float* lPoints;
    float* rPoints;
    int pointChooseStatus;

    float* pPoints;
    float* errs;

    float H[9];

// Implementation
protected:
	HICON m_hIcon;

    //for disp
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    int HisOK;
    int imgIsOK;
    int PointIsOK;
    int projectIsOk;

    void updateHView();
    void checkStatus();

    void showBackground();
    void prepareBackground(unsigned char* left, int wl, int hl, unsigned char* right, int wr, int hr, int channel);

    BMP lBmp;
    BMP rBmp;

    unsigned char* backgroundBuffer;

    int bw;
    int bh;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtLoadh();
    CListCtrl m_pointlist;
    afx_msg void OnDropFiles(HDROP hDropInfo);
    afx_msg void OnBnClickedBtLoadpointspair();
    afx_msg void OnBnClickedBtComputeh();
    afx_msg void OnBnClickedBtProjecterr();
    afx_msg void OnBnClickedBtLoadimage();
    afx_msg void OnBnClickedBtProject();
    afx_msg void OnLvnColumnclickListPoints(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtShowpointspairs();
    afx_msg void OnBnClickedBtImgProject();
    afx_msg void OnBnClickedBtLoadlrpoints();
};
