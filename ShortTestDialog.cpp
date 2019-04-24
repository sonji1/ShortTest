// ShortTestDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ACE400_ShortTest.h"
#include "ShortTestDialog.h"
#include <math.h>
#include <stdlib.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShortTestDialog dialog


CShortTestDialog::CShortTestDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CShortTestDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShortTestDialog)
	m_edit_nSquarePinNum = 0;
	m_edit_nSquareN = 0;
	m_edit_nBadPinInput = 1;
	m_check_bLinear = TRUE;
	m_check_bExcludeVc = TRUE;

	m_bFlag_RegistVcBad = FALSE;
	m_bFlag_RegistVbBad = FALSE;
	m_bFlagSQMatrxChanged = FALSE;
	//}}AFX_DATA_INIT
}


void CShortTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShortTestDialog)
	DDX_Control  (pDX, IDC_LIST_BAD_PIN, 		m_listBadPin);
	DDX_Control  (pDX, IDC_EDIT_SQUARE_MATRIX, 	m_editSquareMtrx);
	DDX_Control  (pDX, IDC_EDIT_MEAS_OUTPUT,    m_editMeasOut);
	DDX_Text     (pDX, IDC_EDIT_SQUARE_PIN_NUM, m_edit_nSquarePinNum);
	DDV_MinMaxInt(pDX, m_edit_nSquarePinNum, 4, 100);
	DDX_Text     (pDX, IDC_EDIT_SQUARE_N, 		m_edit_nSquareN);
	DDX_Text(pDX, IDC_EDIT_BAD_PIN, 			m_edit_nBadPinInput);
	DDV_MinMaxInt(pDX, m_edit_nBadPinInput, 1, 100);
	DDX_Check    (pDX, IDC_CHECK1, 				m_check_bLinear);
	DDX_Check    (pDX, IDC_CHECK_EXCLUDE_VC, 	m_check_bExcludeVc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShortTestDialog, CDialog)
	//{{AFX_MSG_MAP(CShortTestDialog)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_SQUARE_PIN_NUM, 	OnButtonInputSquarePinNum)
	ON_BN_CLICKED(IDC_BUTTON_ADD_BAD_PIN, 			OnButtonAddBadPin)
	ON_BN_CLICKED(IDC_BUTTON_MEASURE, 				OnButtonMeasureShort)
	ON_BN_CLICKED(IDC_BUTTON_RESET_BAD_PIN, 		OnButtonResetBadPin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShortTestDialog message handlers


BOOL CShortTestDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	g_sFile.Init();
	
	if (InitMember() == FALSE)
		return FALSE;

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CShortTestDialog::InitMember()
{

	// Dialog member 초기화
	m_edit_nSquarePinNum = 0;
	m_edit_nSquareN = 0;
	m_edit_nBadPinInput = 1;
	m_listBadPin.ResetContent();			// list box 초기화
	m_editSquareMtrx.SetWindowText("");		// edit box 초기화
	m_editMeasOut.SetWindowText("");		// edit box 초기화


	// 일반 member 초기화
	::FillMemory(m_waSquareMatrix, sizeof(m_waSquareMatrix), -1);

	m_Bad.initMember();	
	m_foundVbBad.initMember();	
	m_foundVcBad.initMember();	

	::FillMemory(m_waVbOn, sizeof(m_waVbOn), -1);
	m_nVbCount = 0;

	::FillMemory(m_waVcOn, sizeof(m_waVcOn), -1);
	m_nVcCount = 0;

	m_nCheckShort_Count = 0;
	m_bFlag_RegistVcBad = FALSE;
	m_bFlag_RegistVbBad = FALSE;
	m_bFlagSQMatrxChanged = FALSE;

	UpdateData(FALSE);
	
	return TRUE;

}

void CShortTestDialog::OnButtonInputSquarePinNum() 
{
	// TODO: Add your control notification handler code here
	//CString strTemp;	
	//GetDlgItemText(IDC_EDIT_TOL_INPUT, strTemp);
	
	UpdateData(TRUE);

	if (m_edit_nSquarePinNum > MAX_SQUARE_N) 
		return;

	double dSquareN = sqrt(m_edit_nSquarePinNum);
	m_edit_nSquareN = (int)dSquareN;

	if (dSquareN > m_edit_nSquareN) 	// 소숫점이 있었다면
		m_edit_nSquareN++; 
	
	
	m_editSquareMtrx.SetWindowText("");		// edit box 초기화

	makeSquare();

	printSquare();
	

	UpdateData(FALSE);
}

// 주어진 Square N 값으로 square를 만들고 초기화한다.
void CShortTestDialog::makeSquare() 
{
	// Make Square
	::FillMemory(m_waSquareMatrix, sizeof(m_waSquareMatrix), -1);

	int pinNo = 1;		// 1부터 시작
	for (int row = 0; row < m_edit_nSquareN ; row++)
	{
		for (int col = 0; col < m_edit_nSquareN ; col++)
		{
		#if 1
			// 여기서는 pinNo를 그냥 linear하게 할당. 
			// 실제로는 BDL에 따라 random 하게 들어온다.
			m_waSquareMatrix[row][col] = pinNo;
		#else				
			// pinNo를 row, col을 바꿔 할당해서 제대로 처리하는지 본다.
			m_waSquareMatrix[col][row] = pinNo;
		#endif	

			pinNo++;

			if (pinNo > m_edit_nSquarePinNum)
				break;
			
		}
		if (pinNo > m_edit_nSquarePinNum)
			break;
	}
}

void CShortTestDialog::printSquare_Trace() 
{
 	// TRACE에 출력
 	
	MyTrace(PRT_BASIC, "\n");
	for (int row = 0; row < m_edit_nSquareN ; row++)
	{
		MyTrace(PRT_BASIC, "\n");
		for (int col = 0; col < m_edit_nSquareN ; col++)
			MyTrace(PRT_BASIC, "%8d,", m_waSquareMatrix[row][col]);

	}


}

CString CShortTestDialog::printSquare() 
{

	//------------------------------------
	// m_editSquareMtrx  edit box 에 출력
	
	CString	out;
	CString strTemp;

	strTemp.Format("\r\n");
	out += strTemp;

	for (int row = 0; row < m_edit_nSquareN ; row++)
	{
		strTemp.Format("\r\n");
		out += strTemp;
		for (int col = 0; col < m_edit_nSquareN ; col++)
		{
			strTemp.Format("%5d,", m_waSquareMatrix[row][col]);
			out += strTemp;
		}

	}
	strTemp.Format("\r\n\r\n");
	out += strTemp;

	m_editSquareMtrx.SetWindowText(out);

	return out;
}


void CShortTestDialog::OnButtonAddBadPin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);		// 화면의 값을 가져온다.
	
	
	// list box에 추가
	CString strTemp;
	strTemp.Format("%3d", m_edit_nBadPinInput);
	int index = m_listBadPin.FindString(-1, strTemp);
	if(index == LB_ERR)		// 기존에 없는 항목이면 
	{
		// Short list box에 추가한다.
		m_listBadPin.InsertString(0, strTemp);

		// VB, VC Bad Pin 실제 리스트에도 추가
		m_Bad.addPinList(m_edit_nBadPinInput);
	}
	
}




void CShortTestDialog::OnButtonResetBadPin() 
{
	// TODO: Add your control notification handler code here
	
	// Bad Pin 리스트 모두 지우기	
	m_Bad.initMember();	

	// list box 리셋
	m_listBadPin.ResetContent();

	// edit box도 초기값으로 돌려 둔다.
	m_edit_nBadPinInput = 1;

	UpdateData(FALSE);		// 실제 UI에 반영
	
}

int CShortTestDialog::VB_SwOn(short wVbPin) 
{
	if (m_nVbCount >= MAX_SQUARE_N)		// range check
		return -1;

	// -1 값도 VB List에 정상적으로 붙이기로 한다.  (wVbPin이 -1이면 리턴하는 부분을 지웠음)
	// -1 값을 List에 넣지 않으면 중간에 -1이 있는 경우 순서가 어긋나서 포인트 search가 이상해짐


	m_waVbOn[m_nVbCount]= wVbPin;

	m_nVbCount++;
	
	return 0;
}

int CShortTestDialog::VC_SwOn(short wVcPin) 
{
	if (m_nVcCount >= MAX_SQUARE_N)		// range check
		return -1;

	// -1 값도 VC List에 정상적으로 붙이기로 한다.  (wVcPin이 -1이면 리턴하는 부분을 지웠음)
	// -1 값을 List에 넣지 않으면 중간에 -1이 있는 경우 순서가 어긋나서 포인트 search가 이상해짐

	m_waVcOn[m_nVcCount]= wVcPin;

	m_nVcCount++;
	
	return 0;
}

void CShortTestDialog::VB_PrintTrace() 
{
	// VB ON  리스트를 디버그로 출력 
	MyTrace(PRT_BASIC, "VB : \n");
	for (int i=0; i < m_nVbCount; i++)
	{
		MyTrace(PRT_BASIC, " %5d", m_waVbOn[i]);

		if (((i + 1) % m_edit_nSquareN) == 0)		// N개씩 찍고 줄을 바꾼다.
			MyTrace(PRT_BASIC, "\n");
	}
	MyTrace(PRT_BASIC, "\n");
	
}

CString CShortTestDialog::VB_Print(int start, int end) 
{
	CString strTemp;
	CString out = "";


	if (start > end)
		return out;

	if (start >= MAX_SQUARE_N || end >= MAX_SQUARE_N)
		return out;

	if (end > (m_nVbCount -1)) 
		return out;

	// VB ON  리스트를 출력 
	strTemp.Format( "VB : ");
	out += strTemp;
	for (int i=start; i <= end; i++)
	{
		strTemp.Format( " %5d", m_waVbOn[i]);
		out += strTemp;

		if ((((i-start) + 1) % m_edit_nSquareN) == 0)		// N개씩 찍고 줄을 바꾼다.
		{
			if ((i-start) != 0)
				strTemp.Format( "\r\n     ");
			out += strTemp;
		}
	}
	strTemp.Format( "\r\n");
	out += strTemp;

	return out;
}

void CShortTestDialog::VC_PrintTrace() 
{
	// VC ON  리스트를 디버그로 출력 
	MyTrace(PRT_BASIC, "VC : \n");
	for (int i=0; i < m_nVcCount; i++)
	{
		MyTrace(PRT_BASIC, " %5d", m_waVcOn[i]);

		if (((i + 1) % m_edit_nSquareN) == 0)		// N개씩 찍고 줄을 바꾼다.
			MyTrace(PRT_BASIC, "\n");
	}
	MyTrace(PRT_BASIC, "\n");
	
}

CString CShortTestDialog::VC_Print(int start, int end) 
{
	CString strTemp;
	CString out = "";

	if (start > end)
		return out;

	if (start >= MAX_SQUARE_N || end >= MAX_SQUARE_N)
		return out;

	if (end > (m_nVcCount -1)) 
		return out;

	// VC ON  리스트를 출력 
	//strTemp.Format( "VC : \r\n");
	strTemp.Format( "VC : ");
	out += strTemp;
	for (int i=start; i <= end; i++)
	{
		strTemp.Format( " %5d", m_waVcOn[i]);
		out += strTemp;

		if ((((i-start) + 1) % m_edit_nSquareN) == 0)		// N개씩 찍고 줄을 바꾼다.
		{
			if ((i-start) != 0)
				strTemp.Format( "\r\n     ");
			out += strTemp;
		}
	}
	strTemp.Format( "\r\n");
	out += strTemp;

	return out;
}

void CShortTestDialog::VB_Reset() 
{
	// VB ON  리스트를 초기화
	::FillMemory(m_waVbOn, sizeof(m_waVbOn), -1);
	m_nVbCount = 0;

	
}

void CShortTestDialog::VC_Reset() 
{
	// VC On 리스트를 초기화
	::FillMemory(m_waVcOn, sizeof(m_waVcOn), -1);
	m_nVcCount = 0;
	
}

void CShortTestDialog::OnButtonMeasureShort() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);		// 화면의 값을 가져온다.  (m_check_bLinear 화면 값 필요)
	CString out = ""; 

	// 이전 측정(Measure)에서의 VB, VC Bad 결과를 리셋한다.
	m_foundVbBad.initMember();		
	m_foundVcBad.initMember();	

	m_bFlag_RegistVcBad = FALSE;
	m_bFlag_RegistVbBad = FALSE;
	
	//-------------------
	// 1. Y 방향 검사 
	DoCheck_YDir(out);

	
	//-------------------
	// 2. X 방향 검사
	DoCheck_XDir(out);
	
	//-------------------------------------
	// 3. 결과를 Measure Output 창에 출력
	m_editMeasOut.SetWindowText(out);


}

void CShortTestDialog::DoCheck_YDir(CString &rOut) 
{
	//------------------
	//  Y 방향 검사 
	//------------------
	// VB에 다수의 Net을 연결하는 방식
	//
	// VC Row를 1부터 N-1까지 옮겨가면서 측정.   VC 앞은 반드시 VB 여야 함.
	// VC Row=1 이면 VB Row는 0
	// VC Row=2 이면 VB Row는 0, 1
	// VC Row=3 이면 VB Row는 0, 1, 2
	// VC Row=N-1 이면 VB Row는 0, 1, 2, .... N-2  (VB, VC 합치면 0~ N-1 -> N개)
	
	CString strTemp = "";
	strTemp.Format("\r\n====================== YDir Check =============================\r\n\r\n", m_nCheckShort_Count);
	rOut += strTemp;

	m_nCheckShort_Count = 0;
	int	 	ret = 0;
	BOOL	bShortFound = FALSE;
	for (int vcRow = 1; vcRow < m_edit_nSquareN ; vcRow++)
	{

		// VC Row가 모두 -1이면 VC로 의미 없고 마지막줄이므로 테스트  중단.  
		// ex) Count 50, n은 8인 경우  마지막줄이 모두 -1이다.  
		if (m_waSquareMatrix[vcRow][0] == -1)
			break;
		

		// VC Row 위치는 지정 
		int		col;
		for (col = 0; col < m_edit_nSquareN ; col++)
			VC_SwOn(m_waSquareMatrix[vcRow][col]);

		// VB Row는  0 ~ (vcRow -1)
		for (int vbRow = 0; vbRow < vcRow ; vbRow++)
		{
			for (int col = 0; col < m_edit_nSquareN ; col++)
				VB_SwOn(m_waSquareMatrix[vbRow][col]);
	
		}

		//---------------------------------------
		// 테스트할 VB List와 VC List를 출력
		rOut += VB_Print(0, (m_nVbCount -1));			// MEAS_OUTPUT edit 창에 출력
		rOut += VC_Print(0, (m_nVcCount -1));
	
		//VB_PrintTrace();		// debug 창과 log 파일에 출력
		//VC_PrintTrace();
		
	
		//-----------------
		// Short Test
		
		// 실제로 VB, VC ON하여 전압측정을 할 수는 없으므로
		// Bad Pin 리스트와 비교해서 리스트에 있는 번호대로 Vb, Vc 패턴이 들어 온다면
		// Fail(-1)로 리턴하고,  그렇지 않으면 Pass(0)로 리턴한다.
		ret = CheckShort(0, m_nVbCount-1, 0,  m_nVcCount-1);		// Simulate CheckShort 
		if (ret < 0 ) 		// Fail 발견
		{
			strTemp.Format("YDir(vcRow=%d) Check Result: Fail\r\n\r\n\r\n", vcRow);
			rOut += strTemp;

			bShortFound = TRUE;		// fail 발견
			strTemp.Format("CheckShort(): Count=%d ------------------\r\n", m_nCheckShort_Count);
			rOut += strTemp;

			//----------------------------------------------------------------------
			// Short를 발견했다면 VB 위치, VC 위치확인 검사를 수행한다.
			
			// BSearch_VbCheck() 를 현재 VB 리스트에 맞게 호출
			BSearch_VbCheck(rOut, 0, m_nVbCount-1, 0,  m_nVcCount-1, TRUE);		// bFirstCall = TRUE
			strTemp.Format("CheckShort(): Count=%d ------------------\r\n\r\n", m_nCheckShort_Count);
			rOut += strTemp;


			// BSearch_VcCheck() 를 현재 VC 리스트에 맞게 호출
			BSearch_VcCheck(rOut, 0, m_nVbCount-1, 0,  m_nVcCount-1, TRUE);		// bFirstCall = TRUE
			strTemp.Format("CheckShort(): Count=%d ------------------\r\n\r\n", m_nCheckShort_Count);
			rOut += strTemp;
		}
		else
		{
			strTemp.Format("YDir(vcRow=%d) Check Result: Pass\r\n\r\n\r\n", vcRow);
			rOut += strTemp;
		}
	

		// Test 완료 후 VB, VC 리스트를 비운다.
		VB_Reset();
		VC_Reset();



		//----------------------------
		// VC Bad SQ Matrix 제외 기능
		//
		// VC Bad 가 등록이 되었다면  SQ Matrix의 해당 위치에 -1 표시한다.
		// ex) m_waVbOn[] = 1, 2, 3, 4, 5 
		//     m_waVcOn[] = 6, 7, 8, 9, 10    일때  
		//      VB Bad = 1, 3  이고 VC Bad가  9여서 Short가 detect된 경우 
		//      VB=1, VC=9 일때 VC=9를 Bad 등록하고 SQ Matrix에서 9를 -1로 바꿔서 
		//      다른 Search에서는 9는 VC 체크 대상에서 제외하도록 한다.  
		//      (주의:  포인트체크를 위해 VC 리스트의 순서는 유지해야 하므로 VC On 리스트에도  9대신 -1이 들어가야 한다.)
		if (m_check_bExcludeVc == TRUE && m_bFlag_RegistVcBad == TRUE)
		{
			// 다음번 체크에서 이전 VC Bad 항목은 제외하기 위해 
			// row 위치는 지정된 vcRow 위치, col 위치를 search해서 VC Vad라면 -1 표시한다.
			for (col = 0; col < m_edit_nSquareN ; col++)
			{
				if (m_foundVcBad.isExist(m_waSquareMatrix[vcRow][col]) == TRUE)
					m_waSquareMatrix[vcRow][col] = -1;
			}

			m_bFlagSQMatrxChanged = TRUE;		// SQ Matrix 변경됐다고 표시
		}
		m_bFlag_RegistVcBad = FALSE;			// 다음번 체크를 위해 Flag를 끈다. 
		
		//---------------------------------
		// Extra VB Bad SQ Matrix 제외 기능
		//
		// Extra VB Bad 가 등록이 되었다면  SQ Matrix의 해당 위치에 -1 표시한다
		// ex) m_waVbOn[] = 1, 2, 3, 4, 5 
		//     m_waVcOn[] = 6, 7, 8, 9, 10    일때  
		//      VB Bad = 1, 3  이고 VC Bad가  9여서 Short가 detect된 경우 
		//      VB=1, VC=9를  Bad 등록하고 VB=3도 Bad를 등록할 때에 VB=3은 추가로 Extra Bad로 등록을 해준다.
		//      이 지점에서 등록된 Extra bad를 SQ Matrix에서 제외해서  vcRow 이동 후에는 체크 안 할 수 있다.
		if (m_foundExtraVbBad.isEmpty() == FALSE)
		{

			for (int vbRow = 0; vbRow < vcRow ; vbRow++)
			{
				for (int col = 0; col < m_edit_nSquareN ; col++)
					if (m_foundExtraVbBad.isExist(m_waSquareMatrix[vbRow][col]) == TRUE)
						m_waSquareMatrix[vbRow][col] = -1;
			}

			// 다음번의  vcRow 턴에서는 이번 VbBad Extra는 필요 없으므로 리스트를 비운다. (중요)
			// m_foundVbBad나 m_foundVcBad는 불량리스트를 저장하기 위한 용도이지만, 
			// m_foundExtraVBBad는 단지 SQ Matrix에 -1 표시를 하기 위한 용도임.
			// SQ Matrix에 -1 표시를 다 했으므로 비워야 한다.
			m_foundExtraVbBad.initMember();		

			m_bFlagSQMatrxChanged = TRUE;		// SQ Matrix 변경됐다고 표시
		}
		m_bFlag_RegistVbBad = FALSE;			// 다음번 체크를 위해 Flag를 끈다. 

		// test print   
		if (m_bFlagSQMatrxChanged == TRUE)
		{
			// VC Bad와 VB Extra Bad를  -1로 표시한 SQ Matrix 를 출력해 본다. 
			strTemp.Format("========== Reassigned SQ Matrix ===================");
			rOut += strTemp;
			rOut += printSquare();		
			strTemp.Format("===================================================\r\n\r\n");
			rOut += strTemp;
			m_bFlagSQMatrxChanged = FALSE;	// 다음번 체크를 위해 Flag를 끈다.
		}

	}


	// Y 방향 검사 결과 Measure 창에 출력
	strTemp.Format("Vb Bad : %s\r\n", m_foundVbBad.getStr_PinList());
	rOut += strTemp;
	strTemp.Format("Vc Bad : %s\r\n\r\n", m_foundVcBad.getStr_PinList());
	rOut += strTemp;
	strTemp.Format("Y Dir CheckShort(): Count=%d. --------  bShortFound: %s\r\n\r\n", m_nCheckShort_Count, (bShortFound)? "YES":"NO");
	rOut += strTemp;

}




// 실제 HW 연결없이 Short Test를 simulate 한 결과를 리턴한다. 
// Bad Pin 리스트와 비교해서 리스트에 있는 번호대로 Vb, Vc 패턴이 들어 온다면
// Fail(-1)로 리턴하고,  그렇지 않으면 Pass(0)로 리턴한다.
int CShortTestDialog::CheckShort(int vbFirst, int vbLast, int vcFirst, int vcLast) 
{
	if (m_Bad.nPinCnt <= 0)		// 등록된 Bad 핀(불량핀) 이 없다면 무조건 pass
		return 0;

	m_nCheckShort_Count++;		// CheckShort()의 호출횟수

	BOOL	bVbShortFound = FALSE;
	BOOL	bVcShortFound = FALSE;

	for (int vbIdx=vbFirst; vbIdx <= vbLast; vbIdx++)
	{
		for (int shrt = 0; shrt < m_Bad.nPinCnt; shrt++)
		{
			if (m_waVbOn[vbIdx]	== m_Bad.waPin[shrt])
				bVbShortFound = TRUE;
		}
	}

	for (int vcIdx=vcFirst; vcIdx <= vcLast; vcIdx++)
	{
		for (int shrt = 0; shrt < m_Bad.nPinCnt; shrt++)
		{
			if (m_waVcOn[vcIdx]	== m_Bad.waPin[shrt])
				bVcShortFound = TRUE;
		}
	}

	if (bVbShortFound && bVcShortFound)
		return -1;		// Short Found  이므로 Fail을 리턴

	return 0;
}



// Binary Search 방식으로 Vb의 Bad Pin 위치를 찾아낸다.
//
// vbFirst, vbLast : m_waVbOn[] 리스트중에서 이번에 search할 범위 
// vcFirst, vcLast : m_waVcOn[] 리스트중에서 이번에 search할 범위 
// bFirstCall      : 첫번째 호출일 때에는 CheckShort()하지 말고 바로 BSearch_VcCheck() 2회 호출로 분기시킬때 사용 
void CShortTestDialog::BSearch_VbCheck(CString &rOut, int vbFirst, int vbLast, int vcFirst, int vcLast, BOOL bFirstCall)
{
	CString strTemp;

	int first = vbFirst;
	int last  = vbLast;

	strTemp.Format("BSearch_VbCheck(%d, %d, %d, %d) \r\n", first, last, vcFirst, vcLast);
	rOut += strTemp;


    if (first > last)
    	return;



	//----------------
	// Check Short   
//	rOut += VB_Print(first, last);		// MEAS_OUTPUT edit 창에 출력
//	rOut += VC_Print(vcFirst, vcLast);	// MEAS_OUTPUT edit 창에 출력

	// 첫번째 호출일 때에는 이미 Vb List를 CheckShort()해서 불량이 감지된 상황이므로 
	// CheckShort()를 skip하고  다음 단계를 진행한다.
	int ret;
	if (bFirstCall == TRUE)
		ret = -1;
	else
		ret = CheckShort(first, last, vcFirst, vcLast);		


	//------------------
	// 마지막 Node 처리
	if (first == last)		
	{
		if (ret < 0 ) 		// Fail: 불량  발견
		{
			// 발견한 VB 위치 (first) 출력 
			strTemp.Format("ret=%d, VB Bad : m_waVbOn[%d] = %d ****%s***\r\n\r\n\r\n", 
										ret, first, m_waVbOn[first],
										(m_bFlag_RegistVbBad == TRUE) ? "ExtraVbBad":""	); 
			rOut += strTemp;
			
			// 발견한 VB Bad pin 값을 m_foundVbBad에 기록.
			AddVbBad_PinList(m_waVbOn[first]);
		}
		return;

	}


	//-----------------------------------
	// 마지막 Node가 아니라면 재귀호출
	if (ret < 0 ) 		// Fail: 불량  발견
	{

		// 갭이 4 이하라면  binary가 아닌 Linear Search.
		// m_check_bLinear 옵션이 켜 있을 때에만 수행한다.
		if (m_check_bLinear == TRUE && (last-first) < 4)	
			LSearch_VbCheck(rOut, first, last, vcFirst, vcLast);

		else
		{

			int mid = (first + last) / 2;
			strTemp.Format("ret=%d %s, BSearch(%d,%d)/ BSearch(%d,%d)\r\n\r\n", 
					ret, (ret < 0) ? "(Bad)":"", first, mid, mid+1,last); 
			rOut+= strTemp;

	    	// 구체적 위치를  찾기 위해  반반으로 잘라서 2회 재귀호출
	    	BSearch_VbCheck(rOut, first, mid, vcFirst, vcLast);	  
	    	BSearch_VbCheck(rOut, mid+1, last, vcFirst, vcLast);  
		}
	}


}

// 일반 Linear Search (1대 1 비교) 방식으로 Vb의 Bad Pin 위치를 찾아낸다.
void CShortTestDialog::LSearch_VbCheck(CString &rOut, int vbFirst, int vbLast, int vcFirst, int vcLast)
{
	CString strTemp;

	int first = vbFirst;
	int last  = vbLast;

	strTemp.Format("LSearch_VbCheck(%d, %d, %d, %d) CheckShort(): Count=%d ------------------\r\n", 
					first, last, vcFirst, vcLast, m_nCheckShort_Count);
	rOut += strTemp;

	//----------------
	// Check Short   
	for (int loc = first; loc <= last; loc++)
	{
		int ret = CheckShort(loc, loc, vcFirst, vcLast);		
		if (ret < 0 ) 		// Fail: 불량  발견
		{
			// 발견한 VB 위치 (loc) 출력 
			strTemp.Format("ret=%d, VB Bad : m_waVbOn[%d] = %d ****%s***\r\n", ret, loc, m_waVbOn[loc],
									(m_bFlag_RegistVbBad == TRUE) ? "ExtraVbBad":""); 
			rOut += strTemp;
			
			// 발견한 VB Bad pin 값을 m_foundVbBad에 기록.
			//m_foundVbBad.addPinList(m_waVbOn[loc]);
			AddVbBad_PinList(m_waVbOn[loc]);
		}

	}

	strTemp.Format("LSearch_VbCheck(%d, %d, %d, %d) CheckShort(): Count=%d ------------------\r\n\r\n", 
					first, last, vcFirst, vcLast, m_nCheckShort_Count);
	rOut += strTemp;


}

//-------------------------------------------------------
// Binary Search 방식으로 VC의 Bad Pin 위치를 찾아낸다.
//
// vbFirst, vbLast : m_waVbOn[] 리스트중에서 이번에 search할 범위 
// vcFirst, vcLast : m_waVcOn[] 리스트중에서 이번에 search할 범위 
// bFirstCall      : 첫번째 호출일 때에는 CheckShort()하지 말고 바로 
//                   두개의 BSearch_VcCheck() 호출로 분기시킬때 사용 
void CShortTestDialog::BSearch_VcCheck(CString &rOut, int vbFirst, int vbLast, int vcFirst, int vcLast, BOOL bFirstCall)
{
	CString strTemp;

	int first = vcFirst;
	int last  = vcLast;

	strTemp.Format("BSearch_VcCheck(%d, %d, %d, %d) \r\n", vbFirst, vbLast, first, last);
	rOut += strTemp;


    if (first > last)
    	return;



	//----------------
	// Check Short   

	// 첫번째 호출일 때에는 이미 Vc List를 CheckShort()해서 불량이 detect된 상황이므로 
	// CheckShort()는 skip하고  다음 단계를 진행한다.
	int ret;
	if (bFirstCall == TRUE)
		ret = -1;
	else
		ret = CheckShort(vbFirst, vbLast, first, last);		


	//------------------
	// 마지막 Node 처리
	if (first == last)		
	{
		if (ret < 0 ) 		// Fail: 불량  발견
		{
			// 발견한 VC 위치 (first) 출력 
			strTemp.Format("ret=%d, VC Bad : m_waVcOn[%d] = %d *******\r\n\r\n\r\n", ret, first, m_waVcOn[first]); 
			rOut += strTemp;
			
			// 발견한 VC Bad pin 값을 m_foundVcBad에 기록.
			AddVcBad_PinList(m_waVcOn[first]);
		}
		return;

	}

	//-----------------------------------
	// 마지막 Node가 아니라면 재귀호출
	if (ret < 0 ) 		// Fail: 불량  발견
	{
		
		// 4 이하는 binary가 아닌 Linear Search 수행.
		if (m_check_bLinear == TRUE && (last-first) < 4)	
			LSearch_VcCheck(rOut, vbFirst, vbLast, first, last);

		else
		{
			int mid = (first + last) / 2;
			strTemp.Format("ret=%d %s, BSearch(%d,%d)/ BSearch(%d,%d)\r\n\r\n", 
					ret, (ret < 0) ? "(Bad)":"", first, mid, mid+1,last); 
			rOut+= strTemp;

    		// 구체적 위치를  찾기 위해  반반으로 잘라서 2회 재귀호출
    		BSearch_VcCheck(rOut, vbFirst, vbLast, first, mid);	  
    		BSearch_VcCheck(rOut, vbFirst, vbLast, mid+1, last);  
		}
	}

}

// 일반 Linear Search (1대 1 비교) 방식으로 Vb의 Bad Pin 위치를 찾아낸다.
void CShortTestDialog::LSearch_VcCheck(CString &rOut, int vbFirst, int vbLast, int vcFirst, int vcLast)
{
	CString strTemp;

	int first = vcFirst;
	int last  = vcLast;

	strTemp.Format("LSearch_VcCheck(%d, %d, %d, %d) CheckShort(): Count=%d ------------------\r\n", 
					vbFirst, vbLast, first, last, m_nCheckShort_Count);
	rOut += strTemp;

	//----------------
	// Check Short   
	for (int loc = first; loc <= last; loc++)
	{
		int ret = CheckShort(vbFirst, vbLast, loc, loc);		
		if (ret < 0 ) 		// Fail: 불량  발견
		{
			// 발견한 VC 위치 (loc) 출력 
			strTemp.Format("ret=%d, VC Bad : m_waVcOn[%d] = %d *******\r\n", ret, loc, m_waVcOn[loc]); 
			rOut += strTemp;
			
			// 발견한 VC Bad pin 값을 m_foundVcBad에 기록.
			AddVcBad_PinList(m_waVcOn[loc]);
		}

	}

	strTemp.Format("LSearch_VcCheck(%d, %d, %d, %d) CheckShort(): Count=%d ------------------\r\n\r\n", 
					vbFirst, vbLast, first, last, m_nCheckShort_Count);
	rOut += strTemp;


}

int CShortTestDialog::AddVbBad_PinList(short wPin) 
{
	// 이 플래그가 이미 켜져 있다면 이번에 추가되는 pin은 Extra Vb Bad 이다.
	// BSearch 내에서 처음 발견된 VB Bad 이외에 다음번 VB Bad는 Extra Vb Bad로 
	// 간주하고 SQ Matrix에 -1 표시해야 한다.  
	if (m_bFlag_RegistVbBad == TRUE)
		m_foundExtraVbBad.addPinList(wPin);

	m_bFlag_RegistVbBad = TRUE;

	return	m_foundVbBad.addPinList(wPin);

}

int CShortTestDialog::AddVcBad_PinList(short wPin) 
{
	m_bFlag_RegistVcBad = TRUE;

	return	m_foundVcBad.addPinList(wPin);

}




//-------------------------------------------------
// DoCheck_YDir을  row(Y), col(X)을 바꿔서 수행
//
// - Y 방향 검사는 같은 row에  vb, vc 가 있는 short는 잡을 수 없으므로
//   X 방향 검사를 추가해서 같은 row의 short를 체크해야 한다.  
//
// - Y 방향 검사에서 나왔던 결과는 대상에서 제외하여 체크 시간을 줄여야 한다.
//   VC에서 제외하고 체크하는 방법을 사용한다.
void CShortTestDialog::DoCheck_XDir(CString &rOut) 
{
	//------------------
	//  X 방향 검사 
	//------------------
	// VB에 다수의 Net을 연결하는 방식
	//
	// VC Col을  1부터 N-1까지 옮겨가면서 측정.   VC 앞은 반드시 VB 여야 함.
	// VC Col=1 이면   VB Col은 0
	// VC Col=2 이면   VB Col은 0, 1
	// VC Col=3 이면   VB Col은 0, 1, 2
	// VC Col=N-1 이면 VB Col은 0, 1, 2, .... N-2  (VB, VC 합치면 0~ N-1 -> N개)
	
	CString strTemp = "";
	strTemp.Format("\r\n====================== XDir Check =============================\r\n\r\n", m_nCheckShort_Count);
	rOut += strTemp;


	int	 	ret = 0;
	BOOL	bShortFound = FALSE;
	for (int vcCol = 1; vcCol < m_edit_nSquareN ; vcCol++)
	{
		// - VC Col이 모두 -1인 경우는 없으므로 열에서  맨 앞의 값이 -1인지 체크는 하지 않는다.
		// - 열의 맨 끝이나, 맨끝 바로 앞의 값이 -1인 경우는 존재하지만 ChechShort()에서 무시하므로 
		//   이 코드 그대로 사용해도 무방하다.
		
		// VC Col 위치는 지정
		int row;
		for (row = 0; row < m_edit_nSquareN ; row++)
			VC_SwOn(m_waSquareMatrix[row][vcCol]);


		// VB Col은  0 ~ (vcCol -1)
		for (int vbCol = 0; vbCol < vcCol ; vbCol++)
		{
			for (int row = 0; row < m_edit_nSquareN ; row++)
				VB_SwOn(m_waSquareMatrix[row][vbCol]);
	
		}


		//---------------------------------------
		// 테스트할 VB List와 VC List를 출력
		rOut += VB_Print(0, (m_nVbCount -1));			// MEAS_OUTPUT edit 창에 출력
		rOut += VC_Print(0, (m_nVcCount -1));
	
		
	
		//-----------------
		// Short Test
		
		// 실제로 VB, VC ON하여 전압측정을 할 수는 없으므로
		// Bad Pin 리스트와 비교해서 리스트에 있는 번호대로 Vb, Vc 패턴이 들어 온다면
		// Fail(-1)로 리턴하고,  그렇지 않으면 Pass(0)로 리턴한다.
		ret = CheckShort(0, m_nVbCount-1, 0,  m_nVcCount-1);		// Simulate CheckShort 
		if (ret < 0 ) 		// Fail 발견
		{
			strTemp.Format("XDir(vcCol=%d) Check Result: Fail\r\n\r\n\r\n", vcCol);
			rOut += strTemp;
			strTemp.Format("CheckShort(): Count=%d ------------------\r\n", m_nCheckShort_Count);
			rOut += strTemp;

			bShortFound = TRUE;		// fail 발견

			//----------------------------------------------------------------------
			// Short를 발견했다면 VB 인 Column 위치를  찾기 위한 라인(Column) 검사를 수행 
			// Short인 VB Column 위치를 찾으면 Point 검사까지 수행하고 돌아온다.
			ColumnLineSearch(rOut, vcCol);

			strTemp.Format("CheckShort(): Count=%d ------------------\r\n", m_nCheckShort_Count);
			rOut += strTemp;

		}
		else
		{
			strTemp.Format("XDir(vcCol=%d) Check Result: Pass\r\n\r\n\r\n", vcCol);
			rOut += strTemp;
		}
	

		// Test 완료 후 Vb, Vc 리스트를 비운다.
		VB_Reset();
		VC_Reset();


		//----------------------------
		// VC Bad SQ Matrix 제외 기능
		//
		// VC Bad 가 등록이 되었다면  SQ Matrix의 해당 위치에 -1 표시한다.
		// ex) m_waVbOn[] = 1, 2, 3, 4, 5 
		//     m_waVcOn[] = 6, 7, 8, 9, 10    일때  
		//      VB Bad = 1, 3  이고 VC Bad가  9여서 Short가 detect된 경우 
		//      VB=1, VC=9 일때 VC=9를 Bad 등록하고 SQ Matrix에서 9를 -1로 바꿔서 
		//      다른 Search에서는 9는 VC 체크 대상에서 제외하도록 한다.  
		//      (주의:  포인트체크를 위해 VC 리스트의 순서는 유지해야 하므로 VC On 리스트에도  9대신 -1이 들어가야 한다.)
		if (m_check_bExcludeVc == TRUE && m_bFlag_RegistVcBad == TRUE)
		{
			// 다음번 체크에서 이전 VC Bad 항목은 제외하기 위해 
			// row 위치는 지정된 vcRow 위치, col 위치를 search해서 VC Vad라면 -1 표시한다.
			for (row = 0; row < m_edit_nSquareN ; row++)
			{
				if (m_foundVcBad.isExist(m_waSquareMatrix[row][vcCol]) == TRUE)
					m_waSquareMatrix[row][vcCol] = -1;
			}

			// VC Bad와 VB Extra Bad를  -1로 표시한 SQ Matrix 를 출력해 본다. 
			strTemp.Format("========== Reassigned SQ Matrix ===================");
			rOut += strTemp;
			rOut += printSquare();		
			strTemp.Format("===================================================\r\n\r\n");
			rOut += strTemp;
		}
		m_bFlag_RegistVcBad = FALSE;			// 다음번 체크를 위해 Flag를 끈다. 

	}

	// X 방향 검사 추가 결과 Measure 창에 출력
	strTemp.Format("\r\nVb Bad : %s\r\n", m_foundVbBad.getStr_PinList());
	rOut += strTemp;
	strTemp.Format("Vc Bad : %s\r\n\r\n", m_foundVcBad.getStr_PinList());
	rOut += strTemp;
	strTemp.Format("X Dir CheckShort(): Count=%d. --------  bShortFound: %s\r\n\r\n", m_nCheckShort_Count, (bShortFound)? "YES":"NO");
	rOut += strTemp;
}

// VB SWOn 리스트를 coloum별로 다시 만들고 column별로 CheckShort()를 수행한다.
//  vcCol : 이전 호출 함수에서 전달한  vcCol 위치, 이 위치 앞까지 vb 라인 comlumn 체크를 한다.
//
// ex)  m_edit_nSquareN = 5,   vcCol = 3
//                vbStart	 vbEnd
//   vbCol = 0:    0 		 4 
//   vbCol = 1:    5 		 9 
//   vbCol = 2:   10		14 
//  
void CShortTestDialog::ColumnLineSearch(CString& rOut, int vcCol) 
{
	CString strTemp;
	int 	ret;


	// VB 라인들 중에서 어느 라인에 Bad가 있는지 찾는다.
	for (int vbCol = 0; vbCol < vcCol  ; vbCol++)
	{

		// VB SwOn 리스트를 Column열만 가지고  다시 만든다.
		//  : 원래 대상이었던 여러개의 VB 라인중 하나만 VB 리스트로  선택
		VB_Reset();	 
		for (int row = 0; row < m_edit_nSquareN ; row++)	
			VB_SwOn(m_waSquareMatrix[row][vbCol]);


		//---------------------
		// Line Check
		ret = CheckShort(0, m_nVbCount-1, 0,  m_nVcCount-1);		

		if (ret < 0) // Short가 있는 컬럼(Line)이라면 해당 컬럼에서 포인트 체크를 수행.
		{
			
			strTemp.Format("\r\nvbCol=%d, Found ColumnLine Fail\r\n", vbCol);
			rOut += strTemp;
			rOut += VB_Print(0, m_nVbCount-1);		// MEAS_OUTPUT edit 창에 출력

			//------------------------
			// Same Row Point Check
			BOOL	bFlag_foundBadPoint = FALSE;
			for (int row = 0; row < m_edit_nSquareN ; row++)
			{
				// 같은 줄(row)의 VB, VC만 체크한다.
				// VB, VC의 row가 다른 경우는 앞서 Y Dir Check에서 다 찾았으므로
				// VB와 같은 row의 VC 가 fail인 경우만 찾으면 된다. 
				ret = CheckShort(row, row, row, row);		
				if (ret < 0)
				{
					bFlag_foundBadPoint = TRUE;

					// 발견한 VB, VC 위치 (loc) 출력 
					strTemp.Format("VB Bad : m_waVbOn[%d] = %d *******\r\n", row, m_waVbOn[row]); 
					rOut += strTemp;
					strTemp.Format("VC Bad : m_waVcOn[%d] = %d *******\r\n", row, m_waVcOn[row]); 
					rOut += strTemp;
					
					// 발견한 VB, VC Bad pin 값을 m_foundVbBad, m_foundVcBad에 기록.
					AddVbBad_PinList(m_waVbOn[row]); // Extra Vb Bad 제외 기능은 XDir에선 미사용
					AddVcBad_PinList(m_waVcOn[row]);
					strTemp.Format("CheckShort(): Count=%d ------------------\r\n\r\n", m_nCheckShort_Count);
					rOut += strTemp;

				}

			}

			// Line Check에서 불량이 나왔는데, Same Row Point 검사를 다 해도 끝까지 불량이 안 나왔다면
			// 일반적인 BSearch, LSearch를 해서 다시 검사한다. 
			// 병렬연결인 회로에서 회로의 범위가 크면 저항이 작아지므로 short가 나오는데, 
			// 범위를 좁히면 저항이 크게 나와서 Short가 안나오는 경계선인 경우 Bsearch,LSearch를 하면 나올 수도 있다.
			if (bFlag_foundBadPoint == FALSE)
			{
				// BSearch_VbCheck() 를 VB 리스트에 맞게 호출
				BSearch_VbCheck(rOut, 0, m_nVbCount-1, 0,  m_nVcCount-1, TRUE);		// bFirstCall = TRUE
				strTemp.Format("CheckShort(): Count=%d ------------------\r\n\r\n", m_nCheckShort_Count);
				rOut += strTemp;


				// BSearch_VcCheck() 를 VC 리스트에 맞게 호출
				BSearch_VcCheck(rOut, 0, m_nVbCount-1, 0,  m_nVcCount-1, TRUE);		// bFirstCall = TRUE
				strTemp.Format("CheckShort(): Count=%d ------------------\r\n\r\n", m_nCheckShort_Count);
				rOut += strTemp;

			
			}
		}


	}

}


