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

	// Dialog member �ʱ�ȭ
	m_edit_nSquarePinNum = 0;
	m_edit_nSquareN = 0;
	m_edit_nBadPinInput = 1;
	m_listBadPin.ResetContent();			// list box �ʱ�ȭ
	m_editSquareMtrx.SetWindowText("");		// edit box �ʱ�ȭ
	m_editMeasOut.SetWindowText("");		// edit box �ʱ�ȭ


	// �Ϲ� member �ʱ�ȭ
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

	if (dSquareN > m_edit_nSquareN) 	// �Ҽ����� �־��ٸ�
		m_edit_nSquareN++; 
	
	
	m_editSquareMtrx.SetWindowText("");		// edit box �ʱ�ȭ

	makeSquare();

	printSquare();
	

	UpdateData(FALSE);
}

// �־��� Square N ������ square�� ����� �ʱ�ȭ�Ѵ�.
void CShortTestDialog::makeSquare() 
{
	// Make Square
	::FillMemory(m_waSquareMatrix, sizeof(m_waSquareMatrix), -1);

	int pinNo = 1;		// 1���� ����
	for (int row = 0; row < m_edit_nSquareN ; row++)
	{
		for (int col = 0; col < m_edit_nSquareN ; col++)
		{
		#if 1
			// ���⼭�� pinNo�� �׳� linear�ϰ� �Ҵ�. 
			// �����δ� BDL�� ���� random �ϰ� ���´�.
			m_waSquareMatrix[row][col] = pinNo;
		#else				
			// pinNo�� row, col�� �ٲ� �Ҵ��ؼ� ����� ó���ϴ��� ����.
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
 	// TRACE�� ���
 	
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
	// m_editSquareMtrx  edit box �� ���
	
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
	UpdateData(TRUE);		// ȭ���� ���� �����´�.
	
	
	// list box�� �߰�
	CString strTemp;
	strTemp.Format("%3d", m_edit_nBadPinInput);
	int index = m_listBadPin.FindString(-1, strTemp);
	if(index == LB_ERR)		// ������ ���� �׸��̸� 
	{
		// Short list box�� �߰��Ѵ�.
		m_listBadPin.InsertString(0, strTemp);

		// VB, VC Bad Pin ���� ����Ʈ���� �߰�
		m_Bad.addPinList(m_edit_nBadPinInput);
	}
	
}




void CShortTestDialog::OnButtonResetBadPin() 
{
	// TODO: Add your control notification handler code here
	
	// Bad Pin ����Ʈ ��� �����	
	m_Bad.initMember();	

	// list box ����
	m_listBadPin.ResetContent();

	// edit box�� �ʱⰪ���� ���� �д�.
	m_edit_nBadPinInput = 1;

	UpdateData(FALSE);		// ���� UI�� �ݿ�
	
}

int CShortTestDialog::VB_SwOn(short wVbPin) 
{
	if (m_nVbCount >= MAX_SQUARE_N)		// range check
		return -1;

	// -1 ���� VB List�� ���������� ���̱�� �Ѵ�.  (wVbPin�� -1�̸� �����ϴ� �κ��� ������)
	// -1 ���� List�� ���� ������ �߰��� -1�� �ִ� ��� ������ ��߳��� ����Ʈ search�� �̻�����


	m_waVbOn[m_nVbCount]= wVbPin;

	m_nVbCount++;
	
	return 0;
}

int CShortTestDialog::VC_SwOn(short wVcPin) 
{
	if (m_nVcCount >= MAX_SQUARE_N)		// range check
		return -1;

	// -1 ���� VC List�� ���������� ���̱�� �Ѵ�.  (wVcPin�� -1�̸� �����ϴ� �κ��� ������)
	// -1 ���� List�� ���� ������ �߰��� -1�� �ִ� ��� ������ ��߳��� ����Ʈ search�� �̻�����

	m_waVcOn[m_nVcCount]= wVcPin;

	m_nVcCount++;
	
	return 0;
}

void CShortTestDialog::VB_PrintTrace() 
{
	// VB ON  ����Ʈ�� ����׷� ��� 
	MyTrace(PRT_BASIC, "VB : \n");
	for (int i=0; i < m_nVbCount; i++)
	{
		MyTrace(PRT_BASIC, " %5d", m_waVbOn[i]);

		if (((i + 1) % m_edit_nSquareN) == 0)		// N���� ��� ���� �ٲ۴�.
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

	// VB ON  ����Ʈ�� ��� 
	strTemp.Format( "VB : ");
	out += strTemp;
	for (int i=start; i <= end; i++)
	{
		strTemp.Format( " %5d", m_waVbOn[i]);
		out += strTemp;

		if ((((i-start) + 1) % m_edit_nSquareN) == 0)		// N���� ��� ���� �ٲ۴�.
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
	// VC ON  ����Ʈ�� ����׷� ��� 
	MyTrace(PRT_BASIC, "VC : \n");
	for (int i=0; i < m_nVcCount; i++)
	{
		MyTrace(PRT_BASIC, " %5d", m_waVcOn[i]);

		if (((i + 1) % m_edit_nSquareN) == 0)		// N���� ��� ���� �ٲ۴�.
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

	// VC ON  ����Ʈ�� ��� 
	//strTemp.Format( "VC : \r\n");
	strTemp.Format( "VC : ");
	out += strTemp;
	for (int i=start; i <= end; i++)
	{
		strTemp.Format( " %5d", m_waVcOn[i]);
		out += strTemp;

		if ((((i-start) + 1) % m_edit_nSquareN) == 0)		// N���� ��� ���� �ٲ۴�.
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
	// VB ON  ����Ʈ�� �ʱ�ȭ
	::FillMemory(m_waVbOn, sizeof(m_waVbOn), -1);
	m_nVbCount = 0;

	
}

void CShortTestDialog::VC_Reset() 
{
	// VC On ����Ʈ�� �ʱ�ȭ
	::FillMemory(m_waVcOn, sizeof(m_waVcOn), -1);
	m_nVcCount = 0;
	
}

void CShortTestDialog::OnButtonMeasureShort() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);		// ȭ���� ���� �����´�.  (m_check_bLinear ȭ�� �� �ʿ�)
	CString out = ""; 

	// ���� ����(Measure)������ VB, VC Bad ����� �����Ѵ�.
	m_foundVbBad.initMember();		
	m_foundVcBad.initMember();	

	m_bFlag_RegistVcBad = FALSE;
	m_bFlag_RegistVbBad = FALSE;
	
	//-------------------
	// 1. Y ���� �˻� 
	DoCheck_YDir(out);

	
	//-------------------
	// 2. X ���� �˻�
	DoCheck_XDir(out);
	
	//-------------------------------------
	// 3. ����� Measure Output â�� ���
	m_editMeasOut.SetWindowText(out);


}

void CShortTestDialog::DoCheck_YDir(CString &rOut) 
{
	//------------------
	//  Y ���� �˻� 
	//------------------
	// VB�� �ټ��� Net�� �����ϴ� ���
	//
	// VC Row�� 1���� N-1���� �Űܰ��鼭 ����.   VC ���� �ݵ�� VB ���� ��.
	// VC Row=1 �̸� VB Row�� 0
	// VC Row=2 �̸� VB Row�� 0, 1
	// VC Row=3 �̸� VB Row�� 0, 1, 2
	// VC Row=N-1 �̸� VB Row�� 0, 1, 2, .... N-2  (VB, VC ��ġ�� 0~ N-1 -> N��)
	
	CString strTemp = "";
	strTemp.Format("\r\n====================== YDir Check =============================\r\n\r\n", m_nCheckShort_Count);
	rOut += strTemp;

	m_nCheckShort_Count = 0;
	int	 	ret = 0;
	BOOL	bShortFound = FALSE;
	for (int vcRow = 1; vcRow < m_edit_nSquareN ; vcRow++)
	{

		// VC Row�� ��� -1�̸� VC�� �ǹ� ���� ���������̹Ƿ� �׽�Ʈ  �ߴ�.  
		// ex) Count 50, n�� 8�� ���  ���������� ��� -1�̴�.  
		if (m_waSquareMatrix[vcRow][0] == -1)
			break;
		

		// VC Row ��ġ�� ���� 
		int		col;
		for (col = 0; col < m_edit_nSquareN ; col++)
			VC_SwOn(m_waSquareMatrix[vcRow][col]);

		// VB Row��  0 ~ (vcRow -1)
		for (int vbRow = 0; vbRow < vcRow ; vbRow++)
		{
			for (int col = 0; col < m_edit_nSquareN ; col++)
				VB_SwOn(m_waSquareMatrix[vbRow][col]);
	
		}

		//---------------------------------------
		// �׽�Ʈ�� VB List�� VC List�� ���
		rOut += VB_Print(0, (m_nVbCount -1));			// MEAS_OUTPUT edit â�� ���
		rOut += VC_Print(0, (m_nVcCount -1));
	
		//VB_PrintTrace();		// debug â�� log ���Ͽ� ���
		//VC_PrintTrace();
		
	
		//-----------------
		// Short Test
		
		// ������ VB, VC ON�Ͽ� ���������� �� ���� �����Ƿ�
		// Bad Pin ����Ʈ�� ���ؼ� ����Ʈ�� �ִ� ��ȣ��� Vb, Vc ������ ��� �´ٸ�
		// Fail(-1)�� �����ϰ�,  �׷��� ������ Pass(0)�� �����Ѵ�.
		ret = CheckShort(0, m_nVbCount-1, 0,  m_nVcCount-1);		// Simulate CheckShort 
		if (ret < 0 ) 		// Fail �߰�
		{
			strTemp.Format("YDir(vcRow=%d) Check Result: Fail\r\n\r\n\r\n", vcRow);
			rOut += strTemp;

			bShortFound = TRUE;		// fail �߰�
			strTemp.Format("CheckShort(): Count=%d ------------------\r\n", m_nCheckShort_Count);
			rOut += strTemp;

			//----------------------------------------------------------------------
			// Short�� �߰��ߴٸ� VB ��ġ, VC ��ġȮ�� �˻縦 �����Ѵ�.
			
			// BSearch_VbCheck() �� ���� VB ����Ʈ�� �°� ȣ��
			BSearch_VbCheck(rOut, 0, m_nVbCount-1, 0,  m_nVcCount-1, TRUE);		// bFirstCall = TRUE
			strTemp.Format("CheckShort(): Count=%d ------------------\r\n\r\n", m_nCheckShort_Count);
			rOut += strTemp;


			// BSearch_VcCheck() �� ���� VC ����Ʈ�� �°� ȣ��
			BSearch_VcCheck(rOut, 0, m_nVbCount-1, 0,  m_nVcCount-1, TRUE);		// bFirstCall = TRUE
			strTemp.Format("CheckShort(): Count=%d ------------------\r\n\r\n", m_nCheckShort_Count);
			rOut += strTemp;
		}
		else
		{
			strTemp.Format("YDir(vcRow=%d) Check Result: Pass\r\n\r\n\r\n", vcRow);
			rOut += strTemp;
		}
	

		// Test �Ϸ� �� VB, VC ����Ʈ�� ����.
		VB_Reset();
		VC_Reset();



		//----------------------------
		// VC Bad SQ Matrix ���� ���
		//
		// VC Bad �� ����� �Ǿ��ٸ�  SQ Matrix�� �ش� ��ġ�� -1 ǥ���Ѵ�.
		// ex) m_waVbOn[] = 1, 2, 3, 4, 5 
		//     m_waVcOn[] = 6, 7, 8, 9, 10    �϶�  
		//      VB Bad = 1, 3  �̰� VC Bad��  9���� Short�� detect�� ��� 
		//      VB=1, VC=9 �϶� VC=9�� Bad ����ϰ� SQ Matrix���� 9�� -1�� �ٲ㼭 
		//      �ٸ� Search������ 9�� VC üũ ��󿡼� �����ϵ��� �Ѵ�.  
		//      (����:  ����Ʈüũ�� ���� VC ����Ʈ�� ������ �����ؾ� �ϹǷ� VC On ����Ʈ����  9��� -1�� ���� �Ѵ�.)
		if (m_check_bExcludeVc == TRUE && m_bFlag_RegistVcBad == TRUE)
		{
			// ������ üũ���� ���� VC Bad �׸��� �����ϱ� ���� 
			// row ��ġ�� ������ vcRow ��ġ, col ��ġ�� search�ؼ� VC Vad��� -1 ǥ���Ѵ�.
			for (col = 0; col < m_edit_nSquareN ; col++)
			{
				if (m_foundVcBad.isExist(m_waSquareMatrix[vcRow][col]) == TRUE)
					m_waSquareMatrix[vcRow][col] = -1;
			}

			m_bFlagSQMatrxChanged = TRUE;		// SQ Matrix ����ƴٰ� ǥ��
		}
		m_bFlag_RegistVcBad = FALSE;			// ������ üũ�� ���� Flag�� ����. 
		
		//---------------------------------
		// Extra VB Bad SQ Matrix ���� ���
		//
		// Extra VB Bad �� ����� �Ǿ��ٸ�  SQ Matrix�� �ش� ��ġ�� -1 ǥ���Ѵ�
		// ex) m_waVbOn[] = 1, 2, 3, 4, 5 
		//     m_waVcOn[] = 6, 7, 8, 9, 10    �϶�  
		//      VB Bad = 1, 3  �̰� VC Bad��  9���� Short�� detect�� ��� 
		//      VB=1, VC=9��  Bad ����ϰ� VB=3�� Bad�� ����� ���� VB=3�� �߰��� Extra Bad�� ����� ���ش�.
		//      �� �������� ��ϵ� Extra bad�� SQ Matrix���� �����ؼ�  vcRow �̵� �Ŀ��� üũ �� �� �� �ִ�.
		if (m_foundExtraVbBad.isEmpty() == FALSE)
		{

			for (int vbRow = 0; vbRow < vcRow ; vbRow++)
			{
				for (int col = 0; col < m_edit_nSquareN ; col++)
					if (m_foundExtraVbBad.isExist(m_waSquareMatrix[vbRow][col]) == TRUE)
						m_waSquareMatrix[vbRow][col] = -1;
			}

			// ��������  vcRow �Ͽ����� �̹� VbBad Extra�� �ʿ� �����Ƿ� ����Ʈ�� ����. (�߿�)
			// m_foundVbBad�� m_foundVcBad�� �ҷ�����Ʈ�� �����ϱ� ���� �뵵������, 
			// m_foundExtraVBBad�� ���� SQ Matrix�� -1 ǥ�ø� �ϱ� ���� �뵵��.
			// SQ Matrix�� -1 ǥ�ø� �� �����Ƿ� ����� �Ѵ�.
			m_foundExtraVbBad.initMember();		

			m_bFlagSQMatrxChanged = TRUE;		// SQ Matrix ����ƴٰ� ǥ��
		}
		m_bFlag_RegistVbBad = FALSE;			// ������ üũ�� ���� Flag�� ����. 

		// test print   
		if (m_bFlagSQMatrxChanged == TRUE)
		{
			// VC Bad�� VB Extra Bad��  -1�� ǥ���� SQ Matrix �� ����� ����. 
			strTemp.Format("========== Reassigned SQ Matrix ===================");
			rOut += strTemp;
			rOut += printSquare();		
			strTemp.Format("===================================================\r\n\r\n");
			rOut += strTemp;
			m_bFlagSQMatrxChanged = FALSE;	// ������ üũ�� ���� Flag�� ����.
		}

	}


	// Y ���� �˻� ��� Measure â�� ���
	strTemp.Format("Vb Bad : %s\r\n", m_foundVbBad.getStr_PinList());
	rOut += strTemp;
	strTemp.Format("Vc Bad : %s\r\n\r\n", m_foundVcBad.getStr_PinList());
	rOut += strTemp;
	strTemp.Format("Y Dir CheckShort(): Count=%d. --------  bShortFound: %s\r\n\r\n", m_nCheckShort_Count, (bShortFound)? "YES":"NO");
	rOut += strTemp;

}




// ���� HW ������� Short Test�� simulate �� ����� �����Ѵ�. 
// Bad Pin ����Ʈ�� ���ؼ� ����Ʈ�� �ִ� ��ȣ��� Vb, Vc ������ ��� �´ٸ�
// Fail(-1)�� �����ϰ�,  �׷��� ������ Pass(0)�� �����Ѵ�.
int CShortTestDialog::CheckShort(int vbFirst, int vbLast, int vcFirst, int vcLast) 
{
	if (m_Bad.nPinCnt <= 0)		// ��ϵ� Bad ��(�ҷ���) �� ���ٸ� ������ pass
		return 0;

	m_nCheckShort_Count++;		// CheckShort()�� ȣ��Ƚ��

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
		return -1;		// Short Found  �̹Ƿ� Fail�� ����

	return 0;
}



// Binary Search ������� Vb�� Bad Pin ��ġ�� ã�Ƴ���.
//
// vbFirst, vbLast : m_waVbOn[] ����Ʈ�߿��� �̹��� search�� ���� 
// vcFirst, vcLast : m_waVcOn[] ����Ʈ�߿��� �̹��� search�� ���� 
// bFirstCall      : ù��° ȣ���� ������ CheckShort()���� ���� �ٷ� BSearch_VcCheck() 2ȸ ȣ��� �б��ų�� ��� 
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
//	rOut += VB_Print(first, last);		// MEAS_OUTPUT edit â�� ���
//	rOut += VC_Print(vcFirst, vcLast);	// MEAS_OUTPUT edit â�� ���

	// ù��° ȣ���� ������ �̹� Vb List�� CheckShort()�ؼ� �ҷ��� ������ ��Ȳ�̹Ƿ� 
	// CheckShort()�� skip�ϰ�  ���� �ܰ踦 �����Ѵ�.
	int ret;
	if (bFirstCall == TRUE)
		ret = -1;
	else
		ret = CheckShort(first, last, vcFirst, vcLast);		


	//------------------
	// ������ Node ó��
	if (first == last)		
	{
		if (ret < 0 ) 		// Fail: �ҷ�  �߰�
		{
			// �߰��� VB ��ġ (first) ��� 
			strTemp.Format("ret=%d, VB Bad : m_waVbOn[%d] = %d ****%s***\r\n\r\n\r\n", 
										ret, first, m_waVbOn[first],
										(m_bFlag_RegistVbBad == TRUE) ? "ExtraVbBad":""	); 
			rOut += strTemp;
			
			// �߰��� VB Bad pin ���� m_foundVbBad�� ���.
			AddVbBad_PinList(m_waVbOn[first]);
		}
		return;

	}


	//-----------------------------------
	// ������ Node�� �ƴ϶�� ���ȣ��
	if (ret < 0 ) 		// Fail: �ҷ�  �߰�
	{

		// ���� 4 ���϶��  binary�� �ƴ� Linear Search.
		// m_check_bLinear �ɼ��� �� ���� ������ �����Ѵ�.
		if (m_check_bLinear == TRUE && (last-first) < 4)	
			LSearch_VbCheck(rOut, first, last, vcFirst, vcLast);

		else
		{

			int mid = (first + last) / 2;
			strTemp.Format("ret=%d %s, BSearch(%d,%d)/ BSearch(%d,%d)\r\n\r\n", 
					ret, (ret < 0) ? "(Bad)":"", first, mid, mid+1,last); 
			rOut+= strTemp;

	    	// ��ü�� ��ġ��  ã�� ����  �ݹ����� �߶� 2ȸ ���ȣ��
	    	BSearch_VbCheck(rOut, first, mid, vcFirst, vcLast);	  
	    	BSearch_VbCheck(rOut, mid+1, last, vcFirst, vcLast);  
		}
	}


}

// �Ϲ� Linear Search (1�� 1 ��) ������� Vb�� Bad Pin ��ġ�� ã�Ƴ���.
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
		if (ret < 0 ) 		// Fail: �ҷ�  �߰�
		{
			// �߰��� VB ��ġ (loc) ��� 
			strTemp.Format("ret=%d, VB Bad : m_waVbOn[%d] = %d ****%s***\r\n", ret, loc, m_waVbOn[loc],
									(m_bFlag_RegistVbBad == TRUE) ? "ExtraVbBad":""); 
			rOut += strTemp;
			
			// �߰��� VB Bad pin ���� m_foundVbBad�� ���.
			//m_foundVbBad.addPinList(m_waVbOn[loc]);
			AddVbBad_PinList(m_waVbOn[loc]);
		}

	}

	strTemp.Format("LSearch_VbCheck(%d, %d, %d, %d) CheckShort(): Count=%d ------------------\r\n\r\n", 
					first, last, vcFirst, vcLast, m_nCheckShort_Count);
	rOut += strTemp;


}

//-------------------------------------------------------
// Binary Search ������� VC�� Bad Pin ��ġ�� ã�Ƴ���.
//
// vbFirst, vbLast : m_waVbOn[] ����Ʈ�߿��� �̹��� search�� ���� 
// vcFirst, vcLast : m_waVcOn[] ����Ʈ�߿��� �̹��� search�� ���� 
// bFirstCall      : ù��° ȣ���� ������ CheckShort()���� ���� �ٷ� 
//                   �ΰ��� BSearch_VcCheck() ȣ��� �б��ų�� ��� 
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

	// ù��° ȣ���� ������ �̹� Vc List�� CheckShort()�ؼ� �ҷ��� detect�� ��Ȳ�̹Ƿ� 
	// CheckShort()�� skip�ϰ�  ���� �ܰ踦 �����Ѵ�.
	int ret;
	if (bFirstCall == TRUE)
		ret = -1;
	else
		ret = CheckShort(vbFirst, vbLast, first, last);		


	//------------------
	// ������ Node ó��
	if (first == last)		
	{
		if (ret < 0 ) 		// Fail: �ҷ�  �߰�
		{
			// �߰��� VC ��ġ (first) ��� 
			strTemp.Format("ret=%d, VC Bad : m_waVcOn[%d] = %d *******\r\n\r\n\r\n", ret, first, m_waVcOn[first]); 
			rOut += strTemp;
			
			// �߰��� VC Bad pin ���� m_foundVcBad�� ���.
			AddVcBad_PinList(m_waVcOn[first]);
		}
		return;

	}

	//-----------------------------------
	// ������ Node�� �ƴ϶�� ���ȣ��
	if (ret < 0 ) 		// Fail: �ҷ�  �߰�
	{
		
		// 4 ���ϴ� binary�� �ƴ� Linear Search ����.
		if (m_check_bLinear == TRUE && (last-first) < 4)	
			LSearch_VcCheck(rOut, vbFirst, vbLast, first, last);

		else
		{
			int mid = (first + last) / 2;
			strTemp.Format("ret=%d %s, BSearch(%d,%d)/ BSearch(%d,%d)\r\n\r\n", 
					ret, (ret < 0) ? "(Bad)":"", first, mid, mid+1,last); 
			rOut+= strTemp;

    		// ��ü�� ��ġ��  ã�� ����  �ݹ����� �߶� 2ȸ ���ȣ��
    		BSearch_VcCheck(rOut, vbFirst, vbLast, first, mid);	  
    		BSearch_VcCheck(rOut, vbFirst, vbLast, mid+1, last);  
		}
	}

}

// �Ϲ� Linear Search (1�� 1 ��) ������� Vb�� Bad Pin ��ġ�� ã�Ƴ���.
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
		if (ret < 0 ) 		// Fail: �ҷ�  �߰�
		{
			// �߰��� VC ��ġ (loc) ��� 
			strTemp.Format("ret=%d, VC Bad : m_waVcOn[%d] = %d *******\r\n", ret, loc, m_waVcOn[loc]); 
			rOut += strTemp;
			
			// �߰��� VC Bad pin ���� m_foundVcBad�� ���.
			AddVcBad_PinList(m_waVcOn[loc]);
		}

	}

	strTemp.Format("LSearch_VcCheck(%d, %d, %d, %d) CheckShort(): Count=%d ------------------\r\n\r\n", 
					vbFirst, vbLast, first, last, m_nCheckShort_Count);
	rOut += strTemp;


}

int CShortTestDialog::AddVbBad_PinList(short wPin) 
{
	// �� �÷��װ� �̹� ���� �ִٸ� �̹��� �߰��Ǵ� pin�� Extra Vb Bad �̴�.
	// BSearch ������ ó�� �߰ߵ� VB Bad �̿ܿ� ������ VB Bad�� Extra Vb Bad�� 
	// �����ϰ� SQ Matrix�� -1 ǥ���ؾ� �Ѵ�.  
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
// DoCheck_YDir��  row(Y), col(X)�� �ٲ㼭 ����
//
// - Y ���� �˻�� ���� row��  vb, vc �� �ִ� short�� ���� �� �����Ƿ�
//   X ���� �˻縦 �߰��ؼ� ���� row�� short�� üũ�ؾ� �Ѵ�.  
//
// - Y ���� �˻翡�� ���Դ� ����� ��󿡼� �����Ͽ� üũ �ð��� �ٿ��� �Ѵ�.
//   VC���� �����ϰ� üũ�ϴ� ����� ����Ѵ�.
void CShortTestDialog::DoCheck_XDir(CString &rOut) 
{
	//------------------
	//  X ���� �˻� 
	//------------------
	// VB�� �ټ��� Net�� �����ϴ� ���
	//
	// VC Col��  1���� N-1���� �Űܰ��鼭 ����.   VC ���� �ݵ�� VB ���� ��.
	// VC Col=1 �̸�   VB Col�� 0
	// VC Col=2 �̸�   VB Col�� 0, 1
	// VC Col=3 �̸�   VB Col�� 0, 1, 2
	// VC Col=N-1 �̸� VB Col�� 0, 1, 2, .... N-2  (VB, VC ��ġ�� 0~ N-1 -> N��)
	
	CString strTemp = "";
	strTemp.Format("\r\n====================== XDir Check =============================\r\n\r\n", m_nCheckShort_Count);
	rOut += strTemp;


	int	 	ret = 0;
	BOOL	bShortFound = FALSE;
	for (int vcCol = 1; vcCol < m_edit_nSquareN ; vcCol++)
	{
		// - VC Col�� ��� -1�� ���� �����Ƿ� ������  �� ���� ���� -1���� üũ�� ���� �ʴ´�.
		// - ���� �� ���̳�, �ǳ� �ٷ� ���� ���� -1�� ���� ���������� ChechShort()���� �����ϹǷ� 
		//   �� �ڵ� �״�� ����ص� �����ϴ�.
		
		// VC Col ��ġ�� ����
		int row;
		for (row = 0; row < m_edit_nSquareN ; row++)
			VC_SwOn(m_waSquareMatrix[row][vcCol]);


		// VB Col��  0 ~ (vcCol -1)
		for (int vbCol = 0; vbCol < vcCol ; vbCol++)
		{
			for (int row = 0; row < m_edit_nSquareN ; row++)
				VB_SwOn(m_waSquareMatrix[row][vbCol]);
	
		}


		//---------------------------------------
		// �׽�Ʈ�� VB List�� VC List�� ���
		rOut += VB_Print(0, (m_nVbCount -1));			// MEAS_OUTPUT edit â�� ���
		rOut += VC_Print(0, (m_nVcCount -1));
	
		
	
		//-----------------
		// Short Test
		
		// ������ VB, VC ON�Ͽ� ���������� �� ���� �����Ƿ�
		// Bad Pin ����Ʈ�� ���ؼ� ����Ʈ�� �ִ� ��ȣ��� Vb, Vc ������ ��� �´ٸ�
		// Fail(-1)�� �����ϰ�,  �׷��� ������ Pass(0)�� �����Ѵ�.
		ret = CheckShort(0, m_nVbCount-1, 0,  m_nVcCount-1);		// Simulate CheckShort 
		if (ret < 0 ) 		// Fail �߰�
		{
			strTemp.Format("XDir(vcCol=%d) Check Result: Fail\r\n\r\n\r\n", vcCol);
			rOut += strTemp;
			strTemp.Format("CheckShort(): Count=%d ------------------\r\n", m_nCheckShort_Count);
			rOut += strTemp;

			bShortFound = TRUE;		// fail �߰�

			//----------------------------------------------------------------------
			// Short�� �߰��ߴٸ� VB �� Column ��ġ��  ã�� ���� ����(Column) �˻縦 ���� 
			// Short�� VB Column ��ġ�� ã���� Point �˻���� �����ϰ� ���ƿ´�.
			ColumnLineSearch(rOut, vcCol);

			strTemp.Format("CheckShort(): Count=%d ------------------\r\n", m_nCheckShort_Count);
			rOut += strTemp;

		}
		else
		{
			strTemp.Format("XDir(vcCol=%d) Check Result: Pass\r\n\r\n\r\n", vcCol);
			rOut += strTemp;
		}
	

		// Test �Ϸ� �� Vb, Vc ����Ʈ�� ����.
		VB_Reset();
		VC_Reset();


		//----------------------------
		// VC Bad SQ Matrix ���� ���
		//
		// VC Bad �� ����� �Ǿ��ٸ�  SQ Matrix�� �ش� ��ġ�� -1 ǥ���Ѵ�.
		// ex) m_waVbOn[] = 1, 2, 3, 4, 5 
		//     m_waVcOn[] = 6, 7, 8, 9, 10    �϶�  
		//      VB Bad = 1, 3  �̰� VC Bad��  9���� Short�� detect�� ��� 
		//      VB=1, VC=9 �϶� VC=9�� Bad ����ϰ� SQ Matrix���� 9�� -1�� �ٲ㼭 
		//      �ٸ� Search������ 9�� VC üũ ��󿡼� �����ϵ��� �Ѵ�.  
		//      (����:  ����Ʈüũ�� ���� VC ����Ʈ�� ������ �����ؾ� �ϹǷ� VC On ����Ʈ����  9��� -1�� ���� �Ѵ�.)
		if (m_check_bExcludeVc == TRUE && m_bFlag_RegistVcBad == TRUE)
		{
			// ������ üũ���� ���� VC Bad �׸��� �����ϱ� ���� 
			// row ��ġ�� ������ vcRow ��ġ, col ��ġ�� search�ؼ� VC Vad��� -1 ǥ���Ѵ�.
			for (row = 0; row < m_edit_nSquareN ; row++)
			{
				if (m_foundVcBad.isExist(m_waSquareMatrix[row][vcCol]) == TRUE)
					m_waSquareMatrix[row][vcCol] = -1;
			}

			// VC Bad�� VB Extra Bad��  -1�� ǥ���� SQ Matrix �� ����� ����. 
			strTemp.Format("========== Reassigned SQ Matrix ===================");
			rOut += strTemp;
			rOut += printSquare();		
			strTemp.Format("===================================================\r\n\r\n");
			rOut += strTemp;
		}
		m_bFlag_RegistVcBad = FALSE;			// ������ üũ�� ���� Flag�� ����. 

	}

	// X ���� �˻� �߰� ��� Measure â�� ���
	strTemp.Format("\r\nVb Bad : %s\r\n", m_foundVbBad.getStr_PinList());
	rOut += strTemp;
	strTemp.Format("Vc Bad : %s\r\n\r\n", m_foundVcBad.getStr_PinList());
	rOut += strTemp;
	strTemp.Format("X Dir CheckShort(): Count=%d. --------  bShortFound: %s\r\n\r\n", m_nCheckShort_Count, (bShortFound)? "YES":"NO");
	rOut += strTemp;
}

// VB SWOn ����Ʈ�� coloum���� �ٽ� ����� column���� CheckShort()�� �����Ѵ�.
//  vcCol : ���� ȣ�� �Լ����� ������  vcCol ��ġ, �� ��ġ �ձ��� vb ���� comlumn üũ�� �Ѵ�.
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


	// VB ���ε� �߿��� ��� ���ο� Bad�� �ִ��� ã�´�.
	for (int vbCol = 0; vbCol < vcCol  ; vbCol++)
	{

		// VB SwOn ����Ʈ�� Column���� ������  �ٽ� �����.
		//  : ���� ����̾��� �������� VB ������ �ϳ��� VB ����Ʈ��  ����
		VB_Reset();	 
		for (int row = 0; row < m_edit_nSquareN ; row++)	
			VB_SwOn(m_waSquareMatrix[row][vbCol]);


		//---------------------
		// Line Check
		ret = CheckShort(0, m_nVbCount-1, 0,  m_nVcCount-1);		

		if (ret < 0) // Short�� �ִ� �÷�(Line)�̶�� �ش� �÷����� ����Ʈ üũ�� ����.
		{
			
			strTemp.Format("\r\nvbCol=%d, Found ColumnLine Fail\r\n", vbCol);
			rOut += strTemp;
			rOut += VB_Print(0, m_nVbCount-1);		// MEAS_OUTPUT edit â�� ���

			//------------------------
			// Same Row Point Check
			BOOL	bFlag_foundBadPoint = FALSE;
			for (int row = 0; row < m_edit_nSquareN ; row++)
			{
				// ���� ��(row)�� VB, VC�� üũ�Ѵ�.
				// VB, VC�� row�� �ٸ� ���� �ռ� Y Dir Check���� �� ã�����Ƿ�
				// VB�� ���� row�� VC �� fail�� ��츸 ã���� �ȴ�. 
				ret = CheckShort(row, row, row, row);		
				if (ret < 0)
				{
					bFlag_foundBadPoint = TRUE;

					// �߰��� VB, VC ��ġ (loc) ��� 
					strTemp.Format("VB Bad : m_waVbOn[%d] = %d *******\r\n", row, m_waVbOn[row]); 
					rOut += strTemp;
					strTemp.Format("VC Bad : m_waVcOn[%d] = %d *******\r\n", row, m_waVcOn[row]); 
					rOut += strTemp;
					
					// �߰��� VB, VC Bad pin ���� m_foundVbBad, m_foundVcBad�� ���.
					AddVbBad_PinList(m_waVbOn[row]); // Extra Vb Bad ���� ����� XDir���� �̻��
					AddVcBad_PinList(m_waVcOn[row]);
					strTemp.Format("CheckShort(): Count=%d ------------------\r\n\r\n", m_nCheckShort_Count);
					rOut += strTemp;

				}

			}

			// Line Check���� �ҷ��� ���Դµ�, Same Row Point �˻縦 �� �ص� ������ �ҷ��� �� ���Դٸ�
			// �Ϲ����� BSearch, LSearch�� �ؼ� �ٽ� �˻��Ѵ�. 
			// ���Ŀ����� ȸ�ο��� ȸ���� ������ ũ�� ������ �۾����Ƿ� short�� �����µ�, 
			// ������ ������ ������ ũ�� ���ͼ� Short�� �ȳ����� ��輱�� ��� Bsearch,LSearch�� �ϸ� ���� ���� �ִ�.
			if (bFlag_foundBadPoint == FALSE)
			{
				// BSearch_VbCheck() �� VB ����Ʈ�� �°� ȣ��
				BSearch_VbCheck(rOut, 0, m_nVbCount-1, 0,  m_nVcCount-1, TRUE);		// bFirstCall = TRUE
				strTemp.Format("CheckShort(): Count=%d ------------------\r\n\r\n", m_nCheckShort_Count);
				rOut += strTemp;


				// BSearch_VcCheck() �� VC ����Ʈ�� �°� ȣ��
				BSearch_VcCheck(rOut, 0, m_nVbCount-1, 0,  m_nVcCount-1, TRUE);		// bFirstCall = TRUE
				strTemp.Format("CheckShort(): Count=%d ------------------\r\n\r\n", m_nCheckShort_Count);
				rOut += strTemp;

			
			}
		}


	}

}


