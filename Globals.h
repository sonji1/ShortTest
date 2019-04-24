// Globals.h: interface for the CGlobals class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALS_H__FDA16379_4221_4970_8471_B8FBD39D2C1E__INCLUDED_)
#define AFX_GLOBALS_H__FDA16379_4221_4970_8471_B8FBD39D2C1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <direct.h>
#include <io.h>
#include "stdafx.h"
#include <stdarg.h>


// __func__�̳�, __FUNCTION__�� VC60���� ���Ұ��ϹǷ� �Ʒ� define�� �����.
// ��뿹��: 
//  printf("Func:%s,   Line:%d\n", FUNC(main), __LINE__);
//   => �ܼ�â�� "Func:main,  Line:22" �̷��� ��µ�.  
//   ȣ���� ������ function name�� �������� �����ִ� idea.
#define FUNC(fn)	#fn




class CGlobals  
{
public:
	CGlobals();
	virtual ~CGlobals();

};




///////////////////
// ���� ��Ʈ����
///////////////////

typedef struct sFILE_DATA
{
	CString	SystemDir;					// ACE400Analysis ���α׷��� ������ ���丮
	CString ParameterDir;
	CString DataDir;					


	CString ACE400_4WDataDir;			// ACE400�� 4W data Dir ("D:\\log4w");

	CString SystemFileName; 
	CString SystemFile; 

	sFILE_DATA()
	{
		SystemFileName		= "System.";

		ACE400_4WDataDir    = "D:\\log4w";
	}

	void Init()
	{
		// ���� ACE400Cal�� ������ ���丮 ��ġ�� �����´�.
		char buffer[_MAX_PATH];
		if(_getcwd( buffer, _MAX_PATH ) == NULL)
		{
			AfxMessageBox("Root directory not found.");
			return;
		}

		SystemDir.Format("%s", buffer);
		//ParameterDir	= SystemDir + "\\" + "Parameter";
		DataDir			= SystemDir + "\\" + "Data";

		//if( (_access(ParameterDir, 0 )) == -1 )	_mkdir(ParameterDir);
		if( (_access(DataDir, 0 )) == -1 )		
			_mkdir(DataDir);

		SystemFile		= ParameterDir	+ "\\" + SystemFileName;
	}

} sFILE_DATA;



///////////////
// ���� ���� 
///////////////

enum UNIT			{ sec=0, msec=1, usec=2, mA, uA, Mohm, Kohm, ohm, mohm, uohm };// sec=0, msec=1, usec=2 ����
enum UNIT_CONVERT_TYPE	{ 
	MM_TO_UM, UM_TO_MM, SEC_TO_MSEC, MSEC_TO_SEC
};

enum PRT_TYPE { PRT_BASIC, 					// 0x0001
				PRT_LEVEL1, 				// 0x0002
				PRT_LEVEL2, 				// 0x0003
				PRT_LEVEL3, 				// 0x0004
				PRT_DEVICE, 				// 0x0005
				PRT_DEV_D64,				// 0x0006

//				PRT_TIMEOFF = 0x0100		// 0x0100
};

extern WORD			g_wPrintState;
extern sFILE_DATA 	g_sFile;

// BASIC ������ Time header off�� ����ϰ��� �� �� ���
//#define		PRT_TMFBAS		(PRT_BASIC | PRT_TIMEOFF)



////////////////
// ���� �Լ�
////////////////


extern	void		DoEvents(DWORD dwMilliseconds=0);
extern  void		Delay(UINT delaytime=0, UINT unit=usec);
extern  __int64 	GetMicroSecond();  //sylee
extern  BOOL		FileExists(CString strFilePath);
extern  void 		GetDateAndTime(char *datetime);
extern  SIZE_T 		GetProcessWorkingSetSize();
extern  void 		__PrintMemSize(char* strFunc, int line);
extern  void 		__PrintLine(char* strFile, int line);
#define	__LINE_PRT__	(__PrintLine(__FILE__, __LINE__));

extern  void 		AFX_CDECL MyTrace(PRT_TYPE prtType, LPCTSTR lpszFormat, ...);

extern  double 		p_tdist(double t, int df);		// TDist.cpp

//extern  int  		AFX_CDECL MyTrace2(PRT_TYPE prtType, char* pStr, int strSize, LPCTSTR lpszFormat, ...);
//extern  int  		AFX_CDECL MySnprintf(PRT_TYPE prtType, char* pStr, int strSize, LPCTSTR lpszFormat, ...);
//extern  BOOL		CreateFolder(LPCTSTR szFolder);
//extern  BOOL		DeleteSubDir(const TCHAR* sPath);
//extern  void		DeleteFolder(char *strDirectory);



#endif // !defined(AFX_GLOBALS_H__FDA16379_4221_4970_8471_B8FBD39D2C1E__INCLUDED_)
