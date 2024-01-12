#pragma once
#include "afxdialogex.h"


// MainDlg dialog

class MainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MainDlg)

public:
	MainDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MainDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit FileName;
	afx_msg void OnBnClickedPrtscr();
	afx_msg void OnBnClickedexit();
	afx_msg void OnBnClickedKeylog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeFilename();
};
