// Error.h: interface for the CError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERROR_H__8D3D34FA_3024_4D5F_81B6_D01841D85268__INCLUDED_)
#define AFX_ERROR_H__8D3D34FA_3024_4D5F_81B6_D01841D85268__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



enum ErrID {
	// 정상
	NO_ERR=0,
	TEST_ERR=1,
	TEST_TRACE=2,

	// E-STOP 관련
	ESTOP=10,


	// 센서 관련
	SENSOR=100,
		SS_DT5V_LOW,
		SS_DT12V_LOW,
		SS_DT24V_LOW,
		SS_DTHV_LOW,	
		SS_DT5VB_A_LOW,
		SS_DT5VC_A_LOW,
		SS_DT5VB_B_LOW,
		SS_DT5VC_B_LOW,
		SS_DT5VB_C_LOW,
		SS_DT5VC_C_LOW,
		SS_DT5VB_D_LOW,
		SS_DT5VC_D_LOW,
		SS_DT5VB_E_LOW,
		SS_DT5VC_E_LOW,
		SS_DT5VB_F_LOW,
		SS_DT5VC_F_LOW,
		SS_DT5VB_G_LOW,
		SS_DT5VC_G_LOW,
		SS_DT5VB_H_LOW,
		SS_DT5VC_H_LOW,

	// 신호 관련
	SGNI=400,

	// 에러플래그
	FLAG=500,
		FLAG_SOURCE_OR_SINK_CANNOT_ON,
		FLAG_SOURCE_CANNOT_OFF,
		FLAG_SINK_CANNOT_OFF,
		FLAG_SOURCE_LEAK,
		FLAG_SINK_LEAK,
		FLAG_ODD_SOURCE_LEAK,
		FLAG_EVEN_SOURCE_LEAK,

		FLAG_FILE_NOT_FOUND,
		FLAG_FILE_CANNOT_OPEN,
		FLAG_FILE_UNPROPER_PARAMETER,
		FLAG_FILE_CHECK_SUM,



	// 반환값 이상
	INVALID_RETURN=600,

	// Input Error 
	INVALID_INPUT=601,

	// 사용자 유발 에러 
	USER_ERR=650,

	// 제한값 초과
	RANGE_OVER=700,
		OL_REF_CURRENT_IS_OVER_INPUT_VOLT_N_RGST,
		OL_I_RANGE_OVER,
		OL_I_RANGE_UNDER,
		CAL_CHECK_TEST_CONDITION,
		CAL_CHECK_HW,

	SW_LOGIC_ERR = 750,
		SW_NO_INIT_VAR,
		MAN_ERR_TEST_CONDEITION,

	// 제한시간 초과
	OVERTIME=800,
		OT_SETNCHK,
		OT_VB_OUTCHECK_ERR,

	// 디바이스 공통
	DEVICE=1000,
		// 모션 관련
		MOTION=DEVICE+100,
			DIO_INIT_ERR,
			DIO_EXIT_ERR,
			INVALID_RETURN_DIOINPBIT,
			INVALID_RETURN_DIOOUTBIT,
			INVALID_RETURN_DIOINPBYTE,
			INVALID_RETURN_DIOOUTBYTE,
			INVALID_RETURN_DIOINP_MBIT,
			INVALID_RETURN_DIOOUT_MBIT,

};

typedef struct {
    int error_no;
    const char* title;
    const char* state;
    const char* sol;
} sErrMsg;

static sErrMsg ErrMsgTable[] = 
{
	{ NO_ERR, "Normal",  "", "" },
	{ TEST_ERR, "Test",  "", "" },
	{ TEST_TRACE, "Test Trace",  "", "" },

	{ ESTOP, "E-Stop",  "", "" },

	{ SENSOR, "Sensor",  "", "" },
		{ SS_DT5V_LOW , "5V 전원 이상", "", "" },
		{ SS_DT12V_LOW, "12V 전원 이상", "", "" },
		{ SS_DT24V_LOW, "24V 전원 이상", "", "" },
		{ SS_DTHV_LOW , "HV 전원 이상", "", "" },

		{ SS_DT5VB_A_LOW, "BOX A 5VB 전원 이상", "" , "" },
		{ SS_DT5VC_A_LOW, "BOX A 5VC 전원 이상", "" , "" },
		{ SS_DT5VB_B_LOW, "BOX B 5VB 전원 이상", "" , "" },
		{ SS_DT5VC_B_LOW, "BOX B 5VC 전원 이상", "" , "" },
		{ SS_DT5VB_C_LOW, "BOX C 5VB 전원 이상", "" , "" },
		{ SS_DT5VC_C_LOW, "BOX C 5VC 전원 이상", "" , "" },
		{ SS_DT5VB_D_LOW, "BOX D 5VB 전원 이상", "" , "" },
		{ SS_DT5VC_D_LOW, "BOX D 5VC 전원 이상", "" , "" },
		{ SS_DT5VB_E_LOW, "BOX E 5VB 전원 이상", "" , "" },
		{ SS_DT5VC_E_LOW, "BOX E 5VC 전원 이상", "" , "" },
		{ SS_DT5VB_F_LOW, "BOX F 5VB 전원 이상", "" , "" },
		{ SS_DT5VC_F_LOW, "BOX F 5VC 전원 이상", "" , "" },
		{ SS_DT5VB_G_LOW, "BOX G 5VB 전원 이상", "" , "" },
		{ SS_DT5VC_G_LOW, "BOX G 5VC 전원 이상", "" , "" },
		{ SS_DT5VB_H_LOW, "BOX H 5VB 전원 이상", "" , "" },
		{ SS_DT5VC_H_LOW, "BOX H 5VC 전원 이상", "" , "" },

	{ SGNI, "Signal",  "", "" },

	{ FLAG, "Flag",  "", "" },
	
		{ FLAG_SOURCE_OR_SINK_CANNOT_ON, "Source/Sink ON 이상", "", "" },
		{ FLAG_SOURCE_CANNOT_OFF,   "Source OFF 이상", "", "" },
		{ FLAG_SINK_CANNOT_OFF, "Sink OFF 이상", "", "" },
		{ FLAG_SOURCE_LEAK, "Source Leak 이상", "", "" },
		{ FLAG_SINK_LEAK, "Sink Leak 이상", "", "" },
		{ FLAG_ODD_SOURCE_LEAK, "Odd Source Leak 이상", "", "" },
		{ FLAG_EVEN_SOURCE_LEAK, "Even Source Leak 이상", "", "" },

		{ FLAG_FILE_NOT_FOUND, "파일 찾을 수 없음", "", "" },
		{ FLAG_FILE_CANNOT_OPEN, "파일 열기 이상", "", "" },
		{ FLAG_FILE_UNPROPER_PARAMETER, "파일 내용 이상", "", "" },
		{ FLAG_FILE_CHECK_SUM, "파일 CheckSum 이상", "", "" },



	{ INVALID_RETURN, "Invalid return",  "", "" },
	{ INVALID_INPUT, "Invalid Input Parameter",  "", "" },

	{ USER_ERR, "사용자 유발 에러",  "", "" },

	{ RANGE_OVER, "Range Over",  "", "" },
		{ OL_REF_CURRENT_IS_OVER_INPUT_VOLT_N_RGST, "설정 전류 값 이상", "", "" },
		{ OL_I_RANGE_OVER, "최대 전류 설정 범위 초과", "", "" },
		{ OL_I_RANGE_UNDER, "최소 전류 설정 범위 초과", "", "" },
		{ CAL_CHECK_TEST_CONDITION, "Calibration Test Condtion 파일 설정 체크 필요", "", "" },
		{ CAL_CHECK_HW, "Calibration Hw Read 값 이상. H/W 체크 필요", "", "" },

	{ SW_LOGIC_ERR, "SW 논리 오류 있음. S/W체크 필요", "", "" },
		{ SW_NO_INIT_VAR, "초기화되지 않은 변수 사용. S/W 체크 필요", "", "" },
		{ MAN_ERR_TEST_CONDEITION, "ManulTest 설정 조건 이상", "", "" },

	{ OVERTIME, "Over time",  "", "" },
		{ OT_SETNCHK,  "Souce 전압설정 이상", "", "" },
		{ OT_VB_OUTCHECK_ERR,  "VB OUT CHECK ERROR. OverTime", "", "" },


	{ DEVICE, "Device",  "", "" },
		{ MOTION, "Motion",  "", "" },
			{ DIO_INIT_ERR, "DIO_INIT_ERR",  "", "" },
			{ DIO_EXIT_ERR, "DIO_EXIT_ERR",  "", "" },
			{ INVALID_RETURN_DIOINPBIT, "Invalid return from DioInpBit()",  "", "" },
			{ INVALID_RETURN_DIOOUTBIT, "Invalid return from DioOutBit()",  "", "" },
			{ INVALID_RETURN_DIOINPBYTE, "Invalid return from DioInpByte()",  "", "" },
			{ INVALID_RETURN_DIOOUTBYTE, "Invalid return from DioOutByte()",  "", "" },
			{ INVALID_RETURN_DIOINP_MBIT, "Invalid return from DioInpMultiBit()",  "", "" },
			{ INVALID_RETURN_DIOOUT_MBIT, "Invalid return from DioOutMultiBit()",  "", "" },

};

#define ERRMSG_TABLE_LEN  sizeof(ErrMsgTable)/sizeof(ErrMsgTable[0])


class CError  
{
private:
	int m_error_no;

public:
	CString m_strState;

	void	Set(int errcode, CString strState="");
	int		Get() 
	{ 
		//TRACE("ERR.Get() m_error_no =%d\n", m_error_no);
		return m_error_no; 
	}
	void	Reset();
	BOOL	GetMsg(int error_no, CString &title, CString &state, CString &sol);

public:
	CError();
	virtual ~CError();
};


extern CError ERR;

extern	int			ErrMsg(int nMboxType=MB_OK, BOOL bTraceOn=TRUE);

#endif // !defined(AFX_ERROR_H__8D3D34FA_3024_4D5F_81B6_D01841D85268__INCLUDED_)
