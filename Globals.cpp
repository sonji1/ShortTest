//////////////////////////////////////////////////////////////////////
// Globals.cpp: implementation of the CGlobals class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Globals.h"
#include <mmsystem.h>
#include <afxmt.h>
#include <io.h>
//#include <afx.h>
//#include "psapi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlobals::CGlobals()
{

}

CGlobals::~CGlobals()
{

}



// ���� ���� ����
sFILE_DATA		g_sFile;
WORD			g_wPrintState = 0x0001;		// PRT_BASIC    on 







////////////////////
// �۷ι� �Լ� ����
////////////////////
void DoEvents(DWORD dwMilliseconds)
{
	MSG msg;
	if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	Sleep(dwMilliseconds);
}

void Delay(UINT delaytime, UINT unit)
{
	// �Է°� ������ ����
	// 1->7.5~8.5u  2->9u 4->11u 5->12 7->13u 10->17u
	// 100->106u 200->206u 500->506u

	//static LONGLONG FreqQuadPart=0;
	static double FreqQuadPart=0;
	static LARGE_INTEGER Freq, ZeroCnt, CurCnt;

	// ���ػ� Ÿ�̸��� 1�ʴ� ������ ȹ��
	if(!QueryPerformanceFrequency(&Freq)) return;

	switch(unit)
	{
	case sec:
		FreqQuadPart = (double)Freq.QuadPart;
		break;

	case msec:
		FreqQuadPart = (double)Freq.QuadPart/1000.0;
		break;

	default:
	case usec:		// 1us(micro sec) �� �������� ��ȯ
		FreqQuadPart = (double)Freq.QuadPart/1000000.0;
		break;
	}
	
	//---------------------------------------------------
	// ex) delaytime=200, unit=usec(2) ���
	// 	    200 micro sec ���� DoEvents()�� �ݺ��Ͽ� �����Ѵ�.
	

#if 0		
	// Elapsed time�� ǥ���ϱ�
	__int64	startTime, endTime;
	startTime = GetMicroSecond();

	QueryPerformanceCounter(&ZeroCnt);	// ���۽����� counter �� ȹ��
	do
	{
		DoEvents();
		QueryPerformanceCounter(&CurCnt);	// DoEvents()�� ������ ������ counter �� ȹ��
	}
	while((CurCnt.QuadPart-ZeroCnt.QuadPart)/FreqQuadPart < delaytime);

	endTime = GetMicroSecond();

	MyTrace(PRT_LEVEL1, "			Delay(%d %s), Elapsed time=%d(usec)\n", delaytime,
						(unit == sec) ? "sec" :
					    (unit == msec) ? "msec":  
					    (unit == usec) ? "usec": "Logic Err?",  (endTime - startTime));

#else
	QueryPerformanceCounter(&ZeroCnt);	// ���۽����� counter �� ȹ��
	do
	{
		DoEvents();
		QueryPerformanceCounter(&CurCnt);	// DoEvents()�� ������ ������ counter �� ȹ��
	}
	while((CurCnt.QuadPart-ZeroCnt.QuadPart)/FreqQuadPart < delaytime);

	MyTrace(PRT_LEVEL1, "			Delay(%d %s)\n", delaytime,
						(unit == sec) ? "sec" :
					    (unit == msec) ? "msec":  
					    (unit == usec) ? "usec": "Logic Err?" ) ;

#endif


}

__int64 GetMicroSecond()  //sylee
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER now;

    if( !QueryPerformanceFrequency(&frequency) )
        return (__int64)GetTickCount();

    if( !QueryPerformanceCounter(&now) )
        return (__int64)GetTickCount();

	return((((__int64)now.QuadPart) * ((__int64)1000000)) / ((__int64)frequency.QuadPart));
}



BOOL FileExists(CString strFilePath)
{
	CFileFind aFile;
	BOOL IsExist = aFile.FindFile(strFilePath);
	aFile.Close();
	return IsExist;
}


void GetDateAndTime(char *datetime)
{
	char buf[24];
	memset( buf, 0x00, 24 );

    time_t ltime;
    time( &ltime );

	struct tm *today;
	today = localtime( &ltime );
   
	strftime( buf, 24, "%Y%m%d", today );
 	strcpy(datetime,buf);
}


//
// 2018.06.18:  psapi.h �� psapi.lib�� ���Ե� VisualC++ 6.0 SDK�� ��� �Ʒ� �ڵ�� ��� �Ұ�.
// VC2008���� ī���ؿ��� ����� �ִٰ�� ��.
// https://stackoverflow.com/questions/4560468/compile-an-exe-in-microsoft-visual-c-6-0-using-the-psapi-h
/*
// ���� �� process�� ��밡���� �޸��� ����� �����´�.
SIZE_T GetProcessWorkingSetSize() 
{
	PROCESS_MEMORY_COUNTERS pmc;
	if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
		return 0;
	}
	return pmc.WorkingSetSize;
}

//memory usage
SIZE_T usage = GetProcessWorkingSetSize();
//something
usage = GetProcessWorkingSetSize() - usage;


��ó: http://ospace.tistory.com/514 [JaPa2]
*/


/*
 * Simple utility to call GetProcessMemoryInfo without having 
 * to compile or link with psapi.h / psapi.lib.
 */

typedef struct {
   DWORD cb;
   DWORD PageFaultCount;
   DWORD PeakWorkingSetSize;
   DWORD WorkingSetSize;
   DWORD QuotaPeakPagedPoolUsage;
   DWORD QuotaPagedPoolUsage;
   DWORD QuotaPeakNonPagedPoolUsage;
   DWORD QuotaNonPagedPoolUsage;
   DWORD PagefileUsage;
   DWORD PeakPagefileUsage;
} PROCESS_MEMORY_COUNTERS;

#define Win32MemArgs HANDLE, PROCESS_MEMORY_COUNTERS *, DWORD

extern BOOL WINAPI GetProcessMemoryInfo( Win32MemArgs );

typedef BOOL (WINAPI *GetMemInfo)( Win32MemArgs );

PROCESS_MEMORY_COUNTERS MyGetMemInfo()
{
    PROCESS_MEMORY_COUNTERS pmc;
    HANDLE                  hProc;
    char                   *pf;
    BOOL                    bOK;
    static HINSTANCE        _hLib = (HINSTANCE)0;
    static GetMemInfo       _fcn  = (GetMemInfo)0;

    /*
     * Dynamically Load library (once)
     */
    if ( !_hLib && (_hLib=::LoadLibrary( "psapi.dll" )) ) {
        pf   = "GetProcessMemoryInfo";
        _fcn = (BOOL (WINAPI *)( Win32MemArgs ))::GetProcAddress( _hLib, pf );
    }

    /*
     * Call fcn
     */
    bOK = false;
    if ( _fcn && (hProc=::GetCurrentProcess()) )
        bOK = (*_fcn)( hProc, &pmc, sizeof( pmc ) );

    if ( !bOK )
       memset( &pmc, 0, sizeof( pmc ) );
    return pmc;
}


SIZE_T GetProcessWorkingSetSize() 
{
	PROCESS_MEMORY_COUNTERS pmc;
	//if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
	//	return 0;
	//}

	pmc = MyGetMemInfo();
	
	return pmc.WorkingSetSize;
}

void __PrintMemSize(char* strFunc, int line)
{
	MyTrace(PRT_BASIC, "%s() Line_%d: memsize= %lu\n", 
			strFunc, line, GetProcessWorkingSetSize());

}

void __PrintLine(char* strFile, int line)
{
	MyTrace(PRT_BASIC, "%s Line_%d\n", strFile, line);

}


//2017.11.06  TRACE�� buffer�� 512���� �ۿ� ������� �ʾƼ� �Ʒ��� ���� �ű� �Լ� ����
//  buffer size = 2048
//
static CCriticalSection g_cs;		

void AFX_CDECL MyTrace(PRT_TYPE prtType, LPCTSTR lpszFormat, ...)
{

#ifdef _DEBUG // all AfxTrace output is controlled by afxTraceEnabled
	if (!afxTraceEnabled)
		return;
#endif
	WORD  bitMask = 0x01 << prtType;
	if (!(g_wPrintState & bitMask)) // check print bit_mask
		return;



	// log ���Ͽ� printout �غ� -------
	g_cs.Lock();		// CriticalSection ����

	char date_time[24]; 
	char path[100];
	memset(date_time,0x00,sizeof(date_time));
	memset(path,0x00,sizeof(path));
	GetDateAndTime(date_time);
	CTime t = CTime::GetCurrentTime();
	sprintf(path, "%s\\log_%s_%02d.txt", g_sFile.DataDir, date_time, t.GetHour()); 
				// ex) .\\Data\log_20180608_13.txt   : log_yyyymmdd_hh.txt


	// OUTPUT â�� print out----------------
	int nBuf;
	TCHAR szBuffer[2048];
	va_list args;
	va_start(args, lpszFormat);
	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
	va_end(args);

	// was there an error? was the expanded string too long?
	ASSERT(nBuf >= 0);

#ifdef _DEBUG 
	if ((afxTraceFlags & traceMultiApp) && (AfxGetApp() != NULL))
		afxDump << AfxGetApp()->m_pszExeName << ": ";
	afxDump << szBuffer;
#endif

#if 1
	// log ���Ͽ� print out ----------
	CFile f;
	f.Open( path, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate );
	f.SeekToEnd();	


/*
	// time ��� print --------
	t = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format( "%02d:%02d:%02d ", t.GetHour(), t.GetMinute(), t.GetSecond());
	//strTime.Format( "%02d:%02d:%02d (%12lu) ", t.GetHour(), t.GetMinute(), t.GetSecond(),  GetProcessWorkingSetSize() );

	f.Write( strTime, strTime.GetLength() );
*/

	// data print --------
	CString strTemp = szBuffer;
	strTemp.Replace("\n", "\r\n");		// \n�� \r\n���� ����.
	f.Write( strTemp, strlen(strTemp) );

	f.Close();
#endif

#if 0
	// log ���Ͽ� print out ----------
	CFile f;
	CArchive ar(&f, CArchive::store);
	f.Open( path, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
	f.SeekToEnd(); 	// ���� ���ϳ����� �ڿ� �ٿ� ���� ���� �̵�	

	CString strTime;
	t = CTime::GetCurrentTime();
	strTime.Format( "%02d:%02d:%02d ", t.GetHour(), t.GetMinute(), t.GetSecond() );

	ar.WriteString(strTime);

	CString strTemp = szBuffer;
	strTemp.Replace("\n", "\r\n");		// \n�� \r\n���� ����.
	f.Write( strTemp, strlen(strTemp) );

	ar.WriteString(strTemp);

	ar.Close();
	f.Close();
#endif

	g_cs.Unlock();		// CriticalSection ����
}


/*
//2018.02.14  ���ɰ����� ���� print off�̸� snprintf�� �������� �ʴ� ��� �߰�.
int AFX_CDECL MySnprintf(PRT_TYPE prtType, char* pStr, int strSize, LPCTSTR lpszFormat, ...)
{
#ifdef _DEBUG // all AfxTrace output is controlled by afxTraceEnabled
	if (!afxTraceEnabled)
		return (-1);
#endif
	WORD  bitMask = 0x01 << prtType;
	if (!(g_wPrintState & bitMask)) // check print bit_mask
	{
		return (-1);
	}

	va_list args;
	va_start(args, lpszFormat);

	int nBuf = _vsnprintf((char*)pStr, strSize, lpszFormat, args);

	// was there an error? was the expanded string too long?
	ASSERT(nBuf >= 0);

	va_end(args);

	return nBuf;
}

// MyTrace�� �ٸ� ��. : pStr �����͸� �Է¹޾Ƽ� ȣ�����Ŀ� ����� str�� ������ �� �ִ�.
int AFX_CDECL MyTrace2(PRT_TYPE prtType, char *pStr, int strSize, LPCTSTR lpszFormat, ...)
{
	int nBuf = 0;
#ifdef _DEBUG 
#ifdef _DEBUG // all AfxTrace output is controlled by afxTraceEnabled
	if (!afxTraceEnabled)
		return (-1);
#endif
	WORD  bitMask = 0x01 << prtType;
	if (!(g_wPrintState & bitMask)) // check print bit_mask
	{
		return (-1);
	}

	va_list args;
	va_start(args, lpszFormat);

	//char* pStr = str.GetBuffer(2048);	// �̷��� �ϴϱ� �״´�.. 
	//int	  strSize = str.GetLength();
	nBuf = _vsnprintf(pStr, strSize, lpszFormat, args);

	// was there an error? was the expanded string too long?
	ASSERT(nBuf >= 0);

	if ((afxTraceFlags & traceMultiApp) && (AfxGetApp() != NULL))
		afxDump << AfxGetApp()->m_pszExeName << ": ";
	afxDump << pStr;

	va_end(args);

#endif
	return nBuf;
}


*/
