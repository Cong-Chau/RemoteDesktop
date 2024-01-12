// MainDlg.cpp : implementation file
//

#include "pch.h"
#include "MFCApplication2.h"
#include "afxdialogex.h"
#include "MainDlg.h"


// MainDlg dialog
bool Keylog = false;
IMPLEMENT_DYNAMIC(MainDlg, CDialogEx)

MainDlg::MainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

MainDlg::~MainDlg()
{
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FileName, FileName);
}


BEGIN_MESSAGE_MAP(MainDlg, CDialogEx)
	ON_BN_CLICKED(IDC_PrtScr, &MainDlg::OnBnClickedPrtscr)
	ON_BN_CLICKED(IDC_exit, &MainDlg::OnBnClickedexit)
	ON_BN_CLICKED(IDC_Keylog, &MainDlg::OnBnClickedKeylog)
	ON_BN_CLICKED(IDCANCEL, &MainDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_FileName, &MainDlg::OnEnChangeFilename)
END_MESSAGE_MAP()


// MainDlg message handlers


void MainDlg::OnBnClickedPrtscr()
{
	CString str;
	GetDlgItemText(IDC_FileName, str);
	FILE* f; fopen_s(&f, "C:/Users/Dien/Documents/output.txt", "w");
	fwprintf(f, L"1 %s", str);
	fclose(f);
	// TODO: Add your control notification handler code here
}


void MainDlg::OnBnClickedexit()
{
	FILE* f; fopen_s(&f, "C:/Users/Dien/Documents/output.txt", "w");
	fprintf(f, "0");
	fclose(f);
	CDialogEx::OnCancel();
}


void MainDlg::OnBnClickedKeylog()
{
	CString str;
	GetDlgItemText(IDC_FileName, str);
	FILE* f; fopen_s(&f, "C:/Users/Dien/Documents/output.txt", "w");
	if (!Keylog)
	{
		fwprintf(f, L"2 %s", str); Keylog = 1;
	}

	else {
		fwprintf(f, L"3 %s", str); Keylog = 0;
	}
	fclose(f);
}


void MainDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void MainDlg::OnEnChangeFilename()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
