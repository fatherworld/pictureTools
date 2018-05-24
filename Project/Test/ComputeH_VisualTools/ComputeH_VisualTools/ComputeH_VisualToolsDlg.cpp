
// ComputeH_VisualToolsDlg.cpp : implementation file
//

#include "Config.h"

#include "stdafx.h"

#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#include "ComputeH_VisualTools.h"

#include "Format.h"

#define LOG_TAG "COMPUTEH_VISUALTOOLSDLG"
#include "Log.h"
#include "Dump.h"
#include "ErrorTools.h"

#include "BmpHelper.h"
#include "ImageConverter.h"
#include "ImageRotate.h"

#include "AndroidHelper.h"

#include "ComputeH_Direct.h"

#include "ComputeH_VisualToolsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#if 0
#define new DEBUG_NEW
#endif
#endif

// CComputeH_VisualToolsDlg dialog
CComputeH_VisualToolsDlg::CComputeH_VisualToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMPUTEH_VISUALTOOLS_DIALOG, pParent)
{
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    lBmp.data = NULL;
    rBmp.data = NULL;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    lPoints = NULL;
    rPoints = NULL;
    pointChooseStatus = 0;

    pPoints = NULL;
    errs = NULL;

    HisOK = 0;
    imgIsOK = 0;
    PointIsOK = 0;
    projectIsOk = 0;
}

CComputeH_VisualToolsDlg::~CComputeH_VisualToolsDlg() {
    GdiplusShutdown(gdiplusToken);
}

void CComputeH_VisualToolsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_POINTS, m_pointlist);
}

BEGIN_MESSAGE_MAP(CComputeH_VisualToolsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BT_LOADH, &CComputeH_VisualToolsDlg::OnBnClickedBtLoadh)
    ON_WM_DROPFILES()
    ON_BN_CLICKED(IDC_BT_LOADPOINTSPAIR, &CComputeH_VisualToolsDlg::OnBnClickedBtLoadpointspair)
    ON_BN_CLICKED(IDC_BT_COMPUTEH, &CComputeH_VisualToolsDlg::OnBnClickedBtComputeh)
    ON_BN_CLICKED(IDC_BT_PROJECTERR, &CComputeH_VisualToolsDlg::OnBnClickedBtProjecterr)
    ON_BN_CLICKED(IDC_BT_LOADIMAGE, &CComputeH_VisualToolsDlg::OnBnClickedBtLoadimage)
    ON_BN_CLICKED(IDC_BT_PROJECT, &CComputeH_VisualToolsDlg::OnBnClickedBtProject)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_POINTS, &CComputeH_VisualToolsDlg::OnLvnColumnclickListPoints)
    ON_BN_CLICKED(IDC_BT_ShowPointsPairs, &CComputeH_VisualToolsDlg::OnBnClickedBtShowpointspairs)
    ON_BN_CLICKED(IDC_BT_IMG_PROJECT, &CComputeH_VisualToolsDlg::OnBnClickedBtImgProject)
    ON_BN_CLICKED(IDC_BT_LOADLRPOINTS, &CComputeH_VisualToolsDlg::OnBnClickedBtLoadlrpoints)
END_MESSAGE_MAP()


// CComputeH_VisualToolsDlg message handlers

BOOL CComputeH_VisualToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_SHOWNORMAL);

    DragAcceptFiles(TRUE);

	// TODO: Add extra initialization here
    ////////////////////////////////////////////////////////////////////////// list control
    LONG lStyle;
    lStyle = GetWindowLong(m_pointlist.m_hWnd, GWL_STYLE);//获取当前窗口style
    lStyle &= ~LVS_TYPEMASK; //清除显示方式位
    lStyle |= LVS_REPORT; //设置style
    SetWindowLong(m_pointlist.m_hWnd, GWL_STYLE, lStyle);//设置style

    DWORD dwStyle = m_pointlist.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
    dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
    dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
    m_pointlist.SetExtendedStyle(dwStyle); //设置扩展风格

    m_pointlist.DeleteAllItems();//清空
    m_pointlist.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 45);//插入列
    m_pointlist.InsertColumn(1, _T("(x1, y1)"), LVCFMT_CENTER, 100);
    m_pointlist.InsertColumn(2, _T("(x2, y2)"), LVCFMT_CENTER, 100);
    m_pointlist.InsertColumn(4, _T("(px1, py1)"), LVCFMT_CENTER, 100);
    m_pointlist.InsertColumn(5, _T("error"), LVCFMT_CENTER, 100);

    ((CComboBox*)GetDlgItem(IDC_ProjectDirect))->SetCurSel(1);
    ((CComboBox*)GetDlgItem(IDC_ProjectCenter))->SetCurSel(1);

    checkStatus();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CComputeH_VisualToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CComputeH_VisualToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CComputeH_VisualToolsDlg::OnBnClickedBtLoadh()
{
    char* packageData = NULL;
    int length = 0;
    CString path;

    GetDlgItem(IDC_EDIT_H_FILE)->GetWindowText(path);

    char* hPath = NULL;
    int num = WideCharToMultiByte(CP_OEMCP, NULL, path, -1, NULL, NULL, 0, NULL);
    hPath = (char*)calloc(1, num);
    WideCharToMultiByte(CP_OEMCP, NULL, path, -1, hPath, num, 0, NULL);

    int err = readData(hPath, Alva_File, &packageData, &length);
    if(ERROR_CHECK(err)) {
        char temp[2048];
        wchar_t msg[2048];
        sprintf_fl(temp, 2048, "Load [%s] error", hPath);
        MultiByteToWideChar(CP_ACP, 0, temp, -1, msg, 2048);
        AfxMessageBox(msg);
        return;
    }
    char* ptr = packageData;

    char * p = ptr;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            p = strstr(ptr, ",");
            *p = '\0';
            sscanf_fl(ptr, "%f", &(H[i * 3 + j]));
            ptr = p + 1;
        }
        p = strstr(ptr, "\n");
        *p = '\0';

        sscanf_fl(ptr, "%f", &(H[i * 3 + 2]));
        ptr = p + 1;
    }

    updateHView();

    free(hPath);
    free(packageData);

    HisOK = 1;
    checkStatus();
}

void CComputeH_VisualToolsDlg::updateHView() {
    CString strRTS[10] = { _T("") };
    for (int i = 0; i < 9; i++)
    {
        strRTS[i].Format(_T("%f"), H[i]);
    }
    GetDlgItem(IDC_EDIT_H1)->SetWindowText(strRTS[0]);
    GetDlgItem(IDC_EDIT_H2)->SetWindowText(strRTS[1]);
    GetDlgItem(IDC_EDIT_H3)->SetWindowText(strRTS[2]);
    GetDlgItem(IDC_EDIT_H4)->SetWindowText(strRTS[3]);
    GetDlgItem(IDC_EDIT_H5)->SetWindowText(strRTS[4]);
    GetDlgItem(IDC_EDIT_H6)->SetWindowText(strRTS[5]);
    GetDlgItem(IDC_EDIT_H7)->SetWindowText(strRTS[6]);
    GetDlgItem(IDC_EDIT_H8)->SetWindowText(strRTS[7]);
    GetDlgItem(IDC_EDIT_H9)->SetWindowText(strRTS[8]);
}

void CComputeH_VisualToolsDlg::checkStatus() {
    GetDlgItem(IDC_BT_ShowPointsPairs)->EnableWindow(PointIsOK & imgIsOK);
    GetDlgItem(IDC_BT_COMPUTEH)->EnableWindow(PointIsOK);
    GetDlgItem(IDC_BT_PROJECTERR)->EnableWindow(PointIsOK & HisOK);
    GetDlgItem(IDC_BT_PROJECT)->EnableWindow(PointIsOK & HisOK & imgIsOK & projectIsOk);
    GetDlgItem(IDC_ProjectCenter)->EnableWindow(imgIsOK & HisOK);
    GetDlgItem(IDC_ProjectDirect)->EnableWindow(imgIsOK & HisOK);
    GetDlgItem(IDC_BT_IMG_PROJECT)->EnableWindow(imgIsOK & HisOK);
}

void CComputeH_VisualToolsDlg::OnDropFiles(HDROP hDropInfo){
    TCHAR lpszFile[MAX_PATH + 1];
    UINT numFiles;
    POINT dragPt;

    numFiles = DragQueryFile(hDropInfo, 0xFFFF, NULL, 0);
    DragQueryFile(hDropInfo, 0, lpszFile, MAX_PATH);
    DragQueryPoint(hDropInfo, &dragPt);
    DragFinish(hDropInfo);
    do
    {
        CWnd *pWnd = ChildWindowFromPoint(dragPt);
        if (pWnd == NULL)
            break;

        TCHAR chClassName[64];
        if (!GetClassName(pWnd->m_hWnd, chClassName, 64))
            break;
        if (_tcscmp(chClassName, _T("Edit")) != 0)
            break;
        pWnd->SetWindowText(lpszFile);
        if (pWnd->GetDlgCtrlID() == IDC_EDIT_H_FILE) {
            HisOK = 0;
            projectIsOk = 0;
        }
        if (pWnd->GetDlgCtrlID() == IDC_EDIT_POINTSPAIR_FILE) {
            HisOK = 0;
            PointIsOK = 0;
            projectIsOk = 0;
        }
        if (pWnd->GetDlgCtrlID() == IDC_EDIT_LEFT_IMG) {
            imgIsOK = 0;
        }
        if (pWnd->GetDlgCtrlID() == IDC_EDIT_RIGHT_IMG) {
            imgIsOK = 0;
        }
    } while (0);

    checkStatus();

    CDialog::OnDropFiles(hDropInfo);
}

void CComputeH_VisualToolsDlg::OnBnClickedBtLoadpointspair()
{
    char* packageData = NULL;
    int length = 0;
    CString path;

    GetDlgItem(IDC_EDIT_POINTSPAIR_FILE)->GetWindowText(path);

    char* hPath = NULL;
    int num = WideCharToMultiByte(CP_OEMCP, NULL, path, -1, NULL, NULL, 0, NULL);
    hPath = (char*)calloc(1, num);
    WideCharToMultiByte(CP_OEMCP, NULL, path, -1, hPath, num, 0, NULL);

    fstream mapFile;

    char* buf = new char[1024];
    num = 0;

    mapFile.open(hPath, ios::in);

    if(!mapFile.is_open()) {
        char temp[2048];
        wchar_t msg[2048];
        sprintf_fl(temp, 2048, "Load [%s] error", hPath);
        MultiByteToWideChar(CP_ACP, 0, temp, -1, msg, 2048);
        AfxMessageBox(msg);
        return;
    }

    do {
        mapFile.clear(ios::goodbit);
        mapFile.getline(buf, 1024);
        if (mapFile.fail())
            break;
        num++;
    } while (!mapFile.fail());

    pointNum = num - 1;

    if (NULL != lPoints) {
        free(lPoints);
    }
    lPoints = (float*)calloc(1, sizeof(float) * 2 * num);
    if (NULL != rPoints) {
        free(rPoints);
    }
    rPoints = (float*)calloc(1, sizeof(float) * 2 * num);
    if (NULL != pPoints) {
        free(pPoints);
    }
    pPoints = (float*)calloc(1, sizeof(float) * 2 * num);
    if (NULL != errs) {
        free(errs);
    }
    errs = (float*)calloc(1, sizeof(float) * 2 * num);

    num = 0;
    mapFile.clear(ios::goodbit);
    mapFile.seekg(0, ios::beg);

    do {
        mapFile.clear(ios::goodbit);
        mapFile.getline(buf, 1024);
        if (mapFile.fail())
            break;

        sscanf_s(buf, "%f, %f, %f, %f,", &(lPoints[num * 2 + 0]), &(lPoints[num * 2 + 1]),
                                         &(rPoints[num * 2 + 0]), &(rPoints[num * 2 + 1]));
        num++;
    } while (!mapFile.fail());

    mapFile.close();

    CString strIndex = _T("");
    CString strX1Y1 = _T("");
    CString strX2Y2 = _T("");
    CString strpX1pY1 = _T("");
    CString strError = _T("");

    int nRow = 0;

    m_pointlist.DeleteAllItems();

    for (int i = 0; i < pointNum; i++) {
        strIndex.Format(_T("%d"), i + 1);
        nRow = m_pointlist.InsertItem(i, strIndex);//插入行

        strX1Y1.Format(_T("%.1f, %.1f"), lPoints[2 * i], lPoints[2 * i + 1]);
        m_pointlist.SetItemText(nRow, 1, strX1Y1);

        strX2Y2.Format(_T("%.1f, %.1f"), rPoints[2 * i], rPoints[2 * i + 1]);
        m_pointlist.SetItemText(nRow, 2, strX2Y2);

        m_pointlist.SetCheck(i);
    }
    pointChooseStatus = 1;

    PointIsOK = 1;
    checkStatus();
}

void CComputeH_VisualToolsDlg::OnBnClickedBtComputeh()
{
    pComputeH_Direct_Data data = NULL;

    int* loc = (int*)calloc(sizeof(int) * pointNum, 1);
    int  num4Compute = 0;
    for (int i = 0; i < pointNum; i++) {
        if (m_pointlist.GetCheck(i)) {
            loc[num4Compute] = i;
            num4Compute++;
        }
    }

    AllocComputeH_Direct_Data(&data, pointNum);
    setComputeH_Direct_Data(lPoints, rPoints, loc, num4Compute, data);
    ComputeH_Direct(data, H);

    updateHView();
    FreeComputeH_Direct_Data(&data);

    HisOK = 1;
    checkStatus();
}

void CComputeH_VisualToolsDlg::OnBnClickedBtProjecterr()
{
    float sumError = 0.0f;
    CString strpX1pY1 = _T("");
    CString strError = _T("");

    Project2DPointsWithH(H, lPoints, pPoints, pointNum);

    for (int i = 0; i < pointNum; i++)
    {
        float u = pPoints[2 * i + 0];
        float v = pPoints[2 * i + 1];

        float u2 = rPoints[2 * i + 0];
        float v2 = rPoints[2 * i + 1];

        float du = u - u2;
        float dv = v - v2;
        float err = std::sqrt(du * du + dv * dv);
        sumError += err;

        strpX1pY1.Format(_T("%.1f, %.1f"), u, v);
        m_pointlist.SetItemText(i, 3, strpX1pY1);

        strError.Format(_T("%.1f, %.1f"), du, dv);
        m_pointlist.SetItemText(i, 4, strError);
    }

    strError.Format(_T("%.1f"), sumError / pointNum);
    GetDlgItem(IDC_EDIT_AVG_ERR)->SetWindowText(strError);
    projectIsOk = 1;
    checkStatus();
}

void CComputeH_VisualToolsDlg::showBackground() {
    float radio = 1.0f;

    if (NULL != backgroundBuffer) {
        GetDlgItem(IDC_BG_IMG)->MoveWindow((int)(276 * 1.75), (int)(65 * 1.75), (int)(bw * radio), (int)(bh * radio), 1);

        Graphics graphic(GetDlgItem(IDC_BG_IMG)->GetDC()->GetSafeHdc());
        SolidBrush greenBrush(Color(255, 0, 0, 0));
        Rect rect(0, 0, (int)(bw * radio), (int)(bh * radio));
        graphic.FillRectangle(&greenBrush, rect);

        Bitmap* background = new Bitmap(bw, bh, 4 * bw, PixelFormat32bppARGB, backgroundBuffer);

        graphic.DrawImage(background, 0, 0, (int)(bw * radio), (int)(bh * radio));
    }
}

void CComputeH_VisualToolsDlg::prepareBackground(unsigned char* left, int wl, int hl, unsigned char* right, int wr, int hr, int channel) {
    bw = wl + wr;
    bh = max(hl, hr);
    backgroundBuffer = (unsigned char*)realloc(backgroundBuffer, bw * bh * 4);
    memset(backgroundBuffer, 0, bw * bh * 4);

    if (channel == 1) {
        for (int r = 0; r < hl; r++) {
            for (int c = 0; c < wl; c++) {
                backgroundBuffer[r * bw * 4 + c * 4 + 0] = left[r * wl + c];
                backgroundBuffer[r * bw * 4 + c * 4 + 1] = left[r * wl + c];
                backgroundBuffer[r * bw * 4 + c * 4 + 2] = left[r * wl + c];
                backgroundBuffer[r * bw * 4 + c * 4 + 3] = 255;
            }
        }
        for (int r = 0; r < hr; r++) {
            for (int c = 0; c < wr; c++) {
                backgroundBuffer[r * bw * 4 + (wl + c) * 4 + 0] = right[r * wr + c];
                backgroundBuffer[r * bw * 4 + (wl + c) * 4 + 1] = right[r * wr + c];
                backgroundBuffer[r * bw * 4 + (wl + c) * 4 + 2] = right[r * wr + c];
                backgroundBuffer[r * bw * 4 + (wl + c) * 4 + 3] = 255;
            }
        }
    }
    else {
        for (int r = 0; r < hl; r++) {
            for (int c = 0; c < wl; c++) {
                backgroundBuffer[r * bw * 4 + c * 4 + 0] = left[(r * wl + c) * 4 + 2];
                backgroundBuffer[r * bw * 4 + c * 4 + 1] = left[(r * wl + c) * 4 + 1];
                backgroundBuffer[r * bw * 4 + c * 4 + 2] = left[(r * wl + c) * 4 + 0];
                backgroundBuffer[r * bw * 4 + c * 4 + 3] = 255;
            }
        }
        for (int r = 0; r < hr; r++) {
            for (int c = 0; c < wr; c++) {
                backgroundBuffer[r * bw * 4 + (wl + c) * 4 + 0] = right[(r * wr + c) * 4 + 2];
                backgroundBuffer[r * bw * 4 + (wl + c) * 4 + 1] = right[(r * wr + c) * 4 + 1];
                backgroundBuffer[r * bw * 4 + (wl + c) * 4 + 2] = right[(r * wr + c) * 4 + 0];
                backgroundBuffer[r * bw * 4 + (wl + c) * 4 + 3] = 255;
            }
        }
    }
}

void CComputeH_VisualToolsDlg::OnBnClickedBtLoadimage()
{
    CString namet1, namet2;
    ((CEdit*)GetDlgItem(IDC_EDIT_LEFT_IMG))->GetWindowTextW(namet1);
    ((CEdit*)GetDlgItem(IDC_EDIT_RIGHT_IMG))->GetWindowTextW(namet2);

    DWORD num = 0;
    char* name1 = NULL, *name2 = NULL;

    num = WideCharToMultiByte(CP_OEMCP, NULL, namet1, -1, NULL, NULL, 0, NULL);
    name1 = (char*)malloc(num + 1);
    WideCharToMultiByte(CP_OEMCP, NULL, namet1, -1, name1, num, 0, NULL);

    num = WideCharToMultiByte(CP_OEMCP, NULL, namet2, -1, NULL, NULL, 0, NULL);
    name2 = (char*)malloc(num + 1);
    WideCharToMultiByte(CP_OEMCP, NULL, namet2, -1, name2, num, 0, NULL);

    FreeBMP(&lBmp);
    FreeBMP(&rBmp);

    int err = LoadBMP(name1, &lBmp, BIT32);
    if (0 != err) {
        char temp[2048];
        wchar_t msg[2048];
        sprintf_fl(temp, 2048, "Load [%s] error: 0x%x", name1, err);
        MultiByteToWideChar(CP_ACP, 0, temp, -1, msg, 2048);
        AfxMessageBox(msg);
        return;
    }

    err = LoadBMP(name2, &rBmp, BIT32);
    if (0 != err) {
        char temp[2048];
        wchar_t msg[2048];
        sprintf_fl(temp, 2048, "Load [%s] error: 0x%x", name2, err);
        MultiByteToWideChar(CP_ACP, 0, temp, -1, msg, 2048);
        AfxMessageBox(msg);
        return;
    }

    prepareBackground(lBmp.data, lBmp.width, lBmp.height, rBmp.data, rBmp.width, rBmp.height, 4);

    showBackground();

    imgIsOK = 1;
    checkStatus();
}

void CComputeH_VisualToolsDlg::OnBnClickedBtProject()
{
    float radio = 1.0f;

    Graphics graphic(GetDlgItem(IDC_BG_IMG)->GetDC()->GetSafeHdc());
    Pen penG(Color(255, 0, 255, 0)), penR(Color(255, 255, 0, 0)), penB(Color(255, 0, 0, 255), 2);

    for (int i = 0; i < pointNum; i++) {
        if (m_pointlist.GetCheck(i)) {
            Rect rect1((int)((lPoints[i * 2 + 0]) * radio), (int)((lPoints[i * 2 + 1]) * radio),
                3, 3);
            graphic.DrawEllipse(&penB, rect1);
            Rect rect2((int)((lBmp.width + rPoints[i * 2 + 0]) * radio), (int)((rPoints[i * 2 + 1]) * radio),
                3, 3);
            graphic.DrawEllipse(&penB, rect2);
        }
        else {
            Rect rect1((int)((lPoints[i * 2 + 0]) * radio), (int)((lPoints[i * 2 + 1]) * radio),
                3, 3);
            graphic.DrawEllipse(&penG, rect1);
            Rect rect2((int)((lBmp.width + rPoints[i * 2 + 0]) * radio), (int)((rPoints[i * 2 + 1]) * radio),
                3, 3);
            graphic.DrawEllipse(&penG, rect2);
        }
        graphic.DrawLine(&penR, (int)((lPoints[i * 2 + 0]) * radio), (int)((lPoints[i * 2 + 1]) * radio),
            (int)((lBmp.width + pPoints[i * 2 + 0]) * radio), (int)((pPoints[i * 2 + 1]) * radio));
    }
    checkStatus();
}

void CComputeH_VisualToolsDlg::OnLvnColumnclickListPoints(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    int click_column = pNMLV->iSubItem;//点击的列

    BOOL check = 1;
    if (pointChooseStatus) {
        check = 0;
        pointChooseStatus = 0;
    }
    else {
        check = 1;
        pointChooseStatus = 1;
    }

    for (int i = 0; i < pointNum; i++) {
        m_pointlist.SetCheck(i, check);
    }

    *pResult = 0;
}

void CComputeH_VisualToolsDlg::OnBnClickedBtShowpointspairs() {
    float radio = 1.0f;

    Graphics graphic(GetDlgItem(IDC_BG_IMG)->GetDC()->GetSafeHdc());
    Pen penG(Color(255, 0, 255, 0)), penR(Color(255, 255, 0, 0)), penB(Color(255, 0, 0, 255), 2);

    for(int i = 0; i < pointNum; i++) {
        if(m_pointlist.GetCheck(i)) {
            Rect rect1((int)((lPoints[i * 2 + 0]) * radio), (int)((lPoints[i * 2 + 1]) * radio),
                       3, 3);
            graphic.DrawEllipse(&penB, rect1);
            Rect rect2((int)((lBmp.width + rPoints[i * 2 + 0]) * radio), (int)((rPoints[i * 2 + 1]) * radio),
                       3, 3);
            graphic.DrawEllipse(&penB, rect2);
        }
        else {
            Rect rect1((int)((lPoints[i * 2 + 0]) * radio), (int)((lPoints[i * 2 + 1]) * radio),
                       3, 3);
            graphic.DrawEllipse(&penG, rect1);
            Rect rect2((int)((lBmp.width + rPoints[i * 2 + 0]) * radio), (int)((rPoints[i * 2 + 1]) * radio),
                       3, 3);
            graphic.DrawEllipse(&penG, rect2);
        }
        graphic.DrawLine(&penG, (int)((lPoints[i * 2 + 0]             ) * radio), (int)((lPoints[i * 2 + 1]) * radio),
                                (int)((lBmp.width + rPoints[i * 2 + 0]) * radio), (int)((rPoints[i * 2 + 1]) * radio));
    }
}

void CComputeH_VisualToolsDlg::OnBnClickedBtImgProject() {
    int projectDirect = ((CComboBox*)GetDlgItem(IDC_ProjectDirect))->GetCurSel();
    int projectCenter = ((CComboBox*)GetDlgItem(IDC_ProjectCenter))->GetCurSel();

    if(projectDirect == 0) {
        AfxMessageBox(_T("请选择投影方向(Direct)"));
        return;
    }
    if(projectCenter == 0) {
        AfxMessageBox(_T("请选择投影中心(Center)"));
        return;
    }

    unsigned char *lGray = NULL, *rGray = NULL;

    ImageTypeConverter(lBmp.data, &lGray, lBmp.width, lBmp.height, Alva_FMT_BGRA_8888, Alva_FMT_GRAY);
    ImageTypeConverter(rBmp.data, &rGray, rBmp.width, rBmp.height, Alva_FMT_BGRA_8888, Alva_FMT_GRAY);

    if(projectDirect == 1) {
        H_OverlayImageForward(H, lGray, lBmp.width, lBmp.height, rGray, rBmp.width, rBmp.height, projectCenter == 1 ? CT_LeftUpCorner : CT_ImageCenter);
    }
    if(projectDirect == 2) {
        H_OverlayImageBackward(H, lGray, lBmp.width, lBmp.height, rGray, rBmp.width, rBmp.height, projectCenter == 1 ? CT_LeftUpCorner : CT_ImageCenter);
    }

    prepareBackground(lGray, lBmp.width, lBmp.height, rGray, rBmp.width, rBmp.height, 1);

    showBackground();

    free(lGray);
    free(rGray);
}

void CComputeH_VisualToolsDlg::OnBnClickedBtLoadlrpoints() {

    char* packageData = NULL;
    int length = 0;
    CString lpath, rpath;

    GetDlgItem(IDC_EDIT_LEFTPOINTS_FILE)->GetWindowText(lpath);
    GetDlgItem(IDC_EDIT_RIGHTPOINTS_FILE)->GetWindowText(rpath);

    char* hPath = NULL;
    int num;
    int lNum, rNum;

    num = WideCharToMultiByte(CP_OEMCP, NULL, lpath, -1, NULL, NULL, 0, NULL);
    hPath = (char*)calloc(1, num);
    WideCharToMultiByte(CP_OEMCP, NULL, lpath, -1, hPath, num, 0, NULL);
    lNum = load_matrix(hPath, (void**)&lPoints, 2, -1, DUMP_FLOAT);
    free(hPath);

    num = WideCharToMultiByte(CP_OEMCP, NULL, rpath, -1, NULL, NULL, 0, NULL);
    hPath = (char*)calloc(1, num);
    WideCharToMultiByte(CP_OEMCP, NULL, rpath, -1, hPath, num, 0, NULL);
    rNum = load_matrix(hPath, (void**)&rPoints, 2, -1, DUMP_FLOAT);
    free(hPath);

    pointNum = min(lNum, rNum);

    if(NULL != pPoints) {
        free(pPoints);
    }
    pPoints = (float*)calloc(1, sizeof(float) * 2 * pointNum);
    if(NULL != errs) {
        free(errs);
    }
    errs = (float*)calloc(1, sizeof(float) * 2 * pointNum);

    CString strIndex = _T("");
    CString strX1Y1 = _T("");
    CString strX2Y2 = _T("");
    CString strpX1pY1 = _T("");
    CString strError = _T("");

    int nRow = 0;

    m_pointlist.DeleteAllItems();

    int tNum = max(lNum, rNum);

    for(int i = 0; i < tNum; i++) {
        strIndex.Format(_T("%d"), i + 1);
        nRow = m_pointlist.InsertItem(i, strIndex);//插入行

        if(i < lNum) {
            strX1Y1.Format(_T("%.1f, %.1f"), lPoints[2 * i], lPoints[2 * i + 1]);
            m_pointlist.SetItemText(nRow, 1, strX1Y1);
        }

        if(i < rNum) {
            strX2Y2.Format(_T("%.1f, %.1f"), rPoints[2 * i], rPoints[2 * i + 1]);
            m_pointlist.SetItemText(nRow, 2, strX2Y2);
        }

        m_pointlist.SetCheck(i);
    }
    pointChooseStatus = 1;

    PointIsOK = 1;
    checkStatus();
}
