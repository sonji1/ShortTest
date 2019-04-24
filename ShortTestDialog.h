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
	short		waPin[MAX_SQUARE_N];		// content : pin 번호
	int			nPinCnt;		

	// member function
public:
	CPinList()	// 생성자
	{
		::FillMemory(waPin, sizeof(waPin), -1);
		nPinCnt = 0;
	}

	void initMember()	// 초기화 함수 
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
	
	short		m_waSquareMatrix[MAX_SQUARE_N][MAX_SQUARE_N];		// Short Test 대상
	CPinList	m_Bad; 						// Short Test에서 잡아야 할 불량 Pin List (VB, VC 모두 포함)
	
	CPinList	m_foundVbBad;				// Short Test에서 감지한 Vb Bad(불량) Pin List 
	CPinList	m_foundVcBad;				// Short Test에서 감지한 Vc Bad(불량) Pin List 
	CPinList	m_foundExtraVbBad;			// Vb Bad Pin 중 두번째 핀 이후의 핀들을 -1 표기하기 위한 용도
	BOOL		m_bFlag_RegistVcBad;		// -1 표기할 VC Bad가 있다는 표시
	BOOL		m_bFlag_RegistVbBad;		// -1 표기할 VC Bad가 있다는 표시
	BOOL		m_bFlagSQMatrxChanged;		// VC, Extra VB 때문에 SQ Matrix의 일부가 -1로 변경되었음을 표시

				// member for CheckShort() Simulation
	short		m_waVbOn[MAX_SQUARE_N];		// Vb Switch를 붙인 pin의 Simulation 리스트 (content : pin 번호)
	int			m_nVbCount;		
	short		m_waVcOn[MAX_SQUARE_N];		// Vc Switch를 붙인 pin의 Simulation 리스트
	int			m_nVcCount;		
	int			m_nCheckShort_Count;		// CheckShort() 호출 횟수	


	
	//------------------
	// member function
	
	BOOL 		InitMember();

				// member for Square Matrix
	void 		makeSquare(); 
	CString 	printSquare(); 					// SquareMatrix edit box에 출력
	void 		printSquare_Trace(); 			// Debug창 및 로그파일에 출력

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
	int 		CheckShort(int vbFirst, int vbLast, int vcFirst, int vcLast); // CheckShort()를 simulate 한다.
	int			VB_SwOn(short wVbPin);
	int			VC_SwOn(short wVcPin);
	void		VB_Reset();
	void		VC_Reset();
	CString		VB_Print(int start, int end);	// Measureedit box에 출력
	CString		VC_Print(int start, int end);	// Measureedit box에 출력
	void		VB_PrintTrace();				// Debug창 및 로그파일에 출력
	void		VC_PrintTrace();				// Debug창 및 로그파일에 출력

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHORT_TEST_DIALOG_H__2ECC17B6_A785_4CA6_B46A_85C1648A9469__INCLUDED_)
