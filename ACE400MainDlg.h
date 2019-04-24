// ACE400MainDlg.h : header file
//

#if !defined(AFX_ACE400_ShortTestDLG_H__A60D6315_99C4_46C0_A323_42C7B357B0A2__INCLUDED_)
#define AFX_ACE400_ShortTestDLG_H__A60D6315_99C4_46C0_A323_42C7B357B0A2__INCLUDED_

#include "ShortTestDialog.h"	// Added by ClassView
#include "ConfigDialog.h"		

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CACE400MainDlg dialog

class CACE400MainDlg : public CDialog
{
// Construction
public:
	CShortTestDialog	m_ShortTestDlg;
	CConfigDialog		m_ConfigDlg;

	CACE400MainDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CACE400MainDlg)
	enum { IDD = IDD_MAIN_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACE400MainDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CACE400MainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonShortTest();
	afx_msg void OnButtonConfig();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACE400_ShortTestDLG_H__A60D6315_99C4_46C0_A323_42C7B357B0A2__INCLUDED_)
