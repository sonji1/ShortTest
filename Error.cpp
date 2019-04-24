// Error.cpp: implementation of the CError class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "ACE400Cal.h"
#include "Error.h"
#include "Globals.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CError ERR;


CError::CError()
{

}

CError::~CError()
{

}

void CError::Set(int error_no, CString strState)
{	
	m_error_no = error_no;

	m_strState = strState;

#ifndef __PRT_OFF__
	// optional print (to be deleted..?)
	
	//TRACE("ERR.Set: err=%d %s\n", error_no, m_strState);
	//TRACE("Err.Set: err=%d\n", error_no);
#endif

	switch(error_no)
	{
	case NO_ERR:
		break;

	default:
		break;
	}

}

void CError::Reset()
{ 
	m_error_no = NO_ERR;
}
	
BOOL CError::GetMsg(int error_no, CString &title, CString &state, CString &sol)
{
	//TRACE("ERR.GetMsg(err=%d)", error_no);		// to be deleted..

	for (int i=0; i<ERRMSG_TABLE_LEN; i++) {
		if (ErrMsgTable[i].error_no == error_no) {
			title = ErrMsgTable[i].title;
			state = ErrMsgTable[i].state;
			sol = ErrMsgTable[i].sol;
			return TRUE;
		}
	}
    return FALSE;
}


// ȣ����1:  ErrMsg() 			:  MB_OK�� �޽����ڽ����� &  TRACE ���, ȣ������ �ݵ�� ERR.Reset() ����
// ȣ����2:  ErrMsg(MB_OK, FALSE) :  MB_OK�� �޽����ڽ��� ����, ȣ������ �ݵ�� ERR.Reset() ����
// ȣ����3:  ErrMsg(-1, TRUE)		:  TRACE�� ���, ȣ�� ���� ERR.Reset() �������� �ʰ� �����ܰ� return�Ŀ� �ñ�. 
//                                      ErrMsg(-1)�θ� �ᵵ �ǰ�, Errmsg(-1..) ���Ŀ��� return�Ŀ� ErrMsg(MB_OK, FALSE)�� 
//                                      �޽��� �ڽ��� ȣ���ϴ� ������� ������ ������.
//                                      ��ǻ� �� ��� ��� �׳� TRACE(PRT_BASIC, ..) �� ȣ���ص� ��.

// MBoxType: 	MB_OK				(0)
// 				MB_OKCANCEL 		(1)
// 				MB_ABORTRETRYIGNORE	(2)
// 				MB_YESNOCANCEL		(3)
// 				MB_YESNO			(4)
// 				MB_RETRYCANCEL		(5)

int ErrMsg(int nMboxType, BOOL bTraceOn)
{
//	CDlgErrMsg DLG;
//	DLG.ntype = nMboxType;
//	DLG.DoModla();
//	int ret = DLG.ret;

//	if(ERR.Get() != NORMAL)	DISPMSG(strMsg);
//	if(ERR.Get() != NORMAL)	ERRMSGDLG.DoModal();

	//if(ERR.Get() == NO_ERR)	
	//	return 0;
	

//	const char* title; const char* state; const char* sol;
	CString title=""; CString state=""; CString sol="";
	ERR.GetMsg(ERR.Get(), title, state, sol);
//	state = state + "\n\n          " + ERR.m_strState;
	if(state == "") 
		state = ERR.m_strState;

	CString strMsg="", strTemp="";
	strMsg.Format("%s error. [ No. %d ]", title, ERR.Get());

	if(state != "") strTemp.Format("\n\nState : %s", state);
	strMsg += strTemp;
	if(sol != "") strTemp.Format("\n\nSolution : %s", sol), strMsg += strTemp;


	int ret = 0;	

	if (ERR.Get() != NO_ERR)	// Error�� ��쿡�� MsgBox ����Ѵ�.
	{
		// Error �� ���, MBoxType�� ������ ��쿡��  MsgBox�� ����Ѵ�. 		
		// MBoxType�� -1�� �����ϸ� MsgBox�� ������� �ʴ´�. 
		if (nMboxType != -1)	
			ret = AfxMessageBox(strMsg, nMboxType | MB_ICONERROR); // modal
	}

	// bTraceOn == FALSE �̸� TRACE ����� ���� �ʴ´�. 
	// ErrMsg()���� Trace ����� �����ϸ� PC���� return�� ������ ����� ���.  (for debugging)
	// ErrMsg(nMboxType==MB_OK, bTraceOn==FALSE)�� ȣ���ؾ� �Ѵ�.
	if (bTraceOn == TRUE)
	{
		MyTrace(PRT_BASIC, "%s\n", strMsg);	// ����� 512����Ʈ�� ���� �� �����Ƿ�  MyTrace() ���
	}


// MB_ABORTRETRYIGNORE MB_OK MB_OKCANCEL MB_RETRYCANCEL MB_YESNO MB_YESNOCANCEL
// MB_ICONHAND MB_ICONSTOP MB_ICONERROR / MB_ICONQUESTION / MB_ICONEXCLAMATION MB_ICONWARNING / MB_ICONASTERISK MB_ICONINFORMATION
// IDABORT IDCANCEL IDIGNORE IDNO IDOK IDRETRY IDYES

	//ERR.Reset();

	return ret;
}
