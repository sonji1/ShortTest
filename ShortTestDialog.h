#if !defined(AFX_SHORT_TEST_DIALOG_H__2ECC17B6_A785_4CA6_B46A_85C1648A9469__INCLUDED_)
#define AFX_SHORT_TEST_DIALOG_H__2ECC17B6_A785_4CA6_B46A_85C1648A9469__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ShortTestDialog.h : header file
//
#include "Globals.h"
#include "Error.h"


#define		MAX_SQUARE_N		100

class	CPinList
{
	// member
public:
	short		waPin[MAX_SQUARE_N];		// content : pin ��ȣ
	int			nPinCnt;		

	// member function
public:
	CPinList()	// ������
	{
		::FillMemory(waPin, sizeof(waPin), -1);
		nPinCnt = 0;
	}

	void initMember()	// �ʱ�ȭ �Լ� 
	{
		::FillMemory(waPin, sizeof(waPin), -1);
		nPinCnt = 0;
	}


	BOOL isExist(short wPin)
	{
		for (int i = 0; i < nPinCnt; i++)
		{
			if (waPin[i] == wPin)
				return TRUE;
		}
		return FALSE;
	}

	BOOL isEmpty()
	{
		if (nPinCnt == 0)
			return TRUE;

		if (waPin[0] == -1)
			return TRUE;

		return FALSE;
	}


	int addPinList(short wPin)
	{
		if (nPinCnt >= MAX_SQUARE_N)
			return -1;

		if (isExist(wPin) == TRUE)
			return -1;

		waPin[nPinCnt] = wPin;

		nPinCnt++;

		return 0;
	}

	CString getStr_PinList()
	{
		CString out = "";
		CString strTemp = ""; 

		for (int i = 0; i < nPinCnt; i++)
		{
			strTemp.Format( " %5d", waPin[i]);
			out += strTemp;
		}

		return out;
	}
};



/////////////////////////////////////////////////////////////////////////////
// CShortTestDialog dialog

class CShortTestDialog : public CDialog
{
// Construction
public:
	CShortTestDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShortTestDialog)
	enum { IDD = IDD_SHORT_TEST_DIALOG };
	CListBox	m_listBadPin;
	CEdit		m_editSquareMtrx;
	CEdit		m_editMeasOut;
	int			m_edit_nSquarePinNum;
	int			m_edit_nSquareN;
	int			m_edit_nShortVbInput;
	int			m_edit_nShortVcInput;
	int			m_edit_nBadPinInput;
	BOOL		m_check_bLinear;
	BOOL		m_check_bExcludeVc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShortTestDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShortTestDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonInputSquarePinNum();
	afx_msg void OnButtonAddBadPin();
	afx_msg void OnButtonResetBadPin();
	afx_msg void OnButtonMeasureShort();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	//-----------
	// member
	
	short		m_waSquareMatrix[MAX_SQUARE_N][MAX_SQUARE_N];		// Short Test ���
	CPinList	m_Bad; 						// Short Test���� ��ƾ� �� �ҷ� Pin List (VB, VC ��� ����)
	
	CPinList	m_foundVbBad;				// Short Test���� ������ Vb Bad(�ҷ�) Pin List 
	CPinList	m_foundVcBad;				// Short Test���� ������ Vc Bad(�ҷ�) Pin List 
	CPinList	m_foundExtraVbBad;			// Vb Bad Pin �� �ι�° �� ������ �ɵ��� -1 ǥ���ϱ� ���� �뵵
	BOOL		m_bFlag_RegistVcBad;		// -1 ǥ���� VC Bad�� �ִٴ� ǥ��
	BOOL		m_bFlag_RegistVbBad;		// -1 ǥ���� VC Bad�� �ִٴ� ǥ��
	BOOL		m_bFlagSQMatrxChanged;		// VC, Extra VB ������ SQ Matrix�� �Ϻΰ� -1�� ����Ǿ����� ǥ��

				// member for CheckShort() Simulation
	short		m_waVbOn[MAX_SQUARE_N];		// Vb Switch�� ���� pin�� Simulation ����Ʈ (content : pin ��ȣ)
	int			m_nVbCount;		
	short		m_waVcOn[MAX_SQUARE_N];		// Vc Switch�� ���� pin�� Simulation ����Ʈ
	int			m_nVcCount;		
	int			m_nCheckShort_Count;		// CheckShort() ȣ�� Ƚ��	


	
	//------------------
	// member function
	
	BOOL 		InitMember();

				// member for Square Matrix
	void 		makeSquare(); 
	CString 	printSquare(); 					// SquareMatrix edit box�� ���
	void 		printSquare_Trace(); 			// Debugâ �� �α����Ͽ� ���

				// member for Y Dir Check 
	void 		DoCheck_YDir(CString &rOut); 
	void		BSearch_VbCheck(CString &rOut, int vbFirst, int vbLast, int vcFirst, int vcLast, BOOL bFirstCall= FALSE);
	void 		LSearch_VbCheck(CString &rOut, int vbFirst, int vbLast, int vcFirst, int vcLast);
	void		BSearch_VcCheck(CString &rOut, int vbFirst, int vbLast, int vcFirst, int vcLast, BOOL bFirstCall= FALSE);
	void 		LSearch_VcCheck(CString &rOut, int vbFirst, int vbLast, int vcFirst, int vcLast);
	int			AddVbBad_PinList(short wPin);
	int			AddVcBad_PinList(short wPin);
	
				// member for X Dir Check 
	void 		DoCheck_XDir(CString &rOut); 
	void 		ColumnLineSearch(CString& rOut, int vcCol);
	
				// member for CheckShort() Simulation
	int 		CheckShort(int vbFirst, int vbLast, int vcFirst, int vcLast); // CheckShort()�� simulate �Ѵ�.
	int			VB_SwOn(short wVbPin);
	int			VC_SwOn(short wVcPin);
	void		VB_Reset();
	void		VC_Reset();
	CString		VB_Print(int start, int end);	// Measureedit box�� ���
	CString		VC_Print(int start, int end);	// Measureedit box�� ���
	void		VB_PrintTrace();				// Debugâ �� �α����Ͽ� ���
	void		VC_PrintTrace();				// Debugâ �� �α����Ͽ� ���

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHORT_TEST_DIALOG_H__2ECC17B6_A785_4CA6_B46A_85C1648A9469__INCLUDED_)
