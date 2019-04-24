// Error.h: interface for the CError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERROR_H__8D3D34FA_3024_4D5F_81B6_D01841D85268__INCLUDED_)
#define AFX_ERROR_H__8D3D34FA_3024_4D5F_81B6_D01841D85268__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



enum ErrID {
	// ����
	NO_ERR=0,
	TEST_ERR=1,
	TEST_TRACE=2,

	// E-STOP ����
	ESTOP=10,


	// ���� ����
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

	// ��ȣ ����
	SGNI=400,

	// �����÷���
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



	// ��ȯ�� �̻�
	INVALID_RETURN=600,

	// Input Error 
	INVALID_INPUT=601,

	// ����� ���� ���� 
	USER_ERR=650,

	// ���Ѱ� �ʰ�
	RANGE_OVER=700,
		OL_REF_CURRENT_IS_OVER_INPUT_VOLT_N_RGST,
		OL_I_RANGE_OVER,
		OL_I_RANGE_UNDER,
		CAL_CHECK_TEST_CONDITION,
		CAL_CHECK_HW,

	SW_LOGIC_ERR = 750,
		SW_NO_INIT_VAR,
		MAN_ERR_TEST_CONDEITION,

	// ���ѽð� �ʰ�
	OVERTIME=800,
		OT_SETNCHK,
		OT_VB_OUTCHECK_ERR,

	// ����̽� ����
	DEVICE=1000,
		// ��� ����
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
		{ SS_DT5V_LOW , "5V ���� �̻�", "", "" },
		{ SS_DT12V_LOW, "12V ���� �̻�", "", "" },
		{ SS_DT24V_LOW, "24V ���� �̻�", "", "" },
		{ SS_DTHV_LOW , "HV ���� �̻�", "", "" },

		{ SS_DT5VB_A_LOW, "BOX A 5VB ���� �̻�", "" , "" },
		{ SS_DT5VC_A_LOW, "BOX A 5VC ���� �̻�", "" , "" },
		{ SS_DT5VB_B_LOW, "BOX B 5VB ���� �̻�", "" , "" },
		{ SS_DT5VC_B_LOW, "BOX B 5VC ���� �̻�", "" , "" },
		{ SS_DT5VB_C_LOW, "BOX C 5VB ���� �̻�", "" , "" },
		{ SS_DT5VC_C_LOW, "BOX C 5VC ���� �̻�", "" , "" },
		{ SS_DT5VB_D_LOW, "BOX D 5VB ���� �̻�", "" , "" },
		{ SS_DT5VC_D_LOW, "BOX D 5VC ���� �̻�", "" , "" },
		{ SS_DT5VB_E_LOW, "BOX E 5VB ���� �̻�", "" , "" },
		{ SS_DT5VC_E_LOW, "BOX E 5VC ���� �̻�", "" , "" },
		{ SS_DT5VB_F_LOW, "BOX F 5VB ���� �̻�", "" , "" },
		{ SS_DT5VC_F_LOW, "BOX F 5VC ���� �̻�", "" , "" },
		{ SS_DT5VB_G_LOW, "BOX G 5VB ���� �̻�", "" , "" },
		{ SS_DT5VC_G_LOW, "BOX G 5VC ���� �̻�", "" , "" },
		{ SS_DT5VB_H_LOW, "BOX H 5VB ���� �̻�", "" , "" },
		{ SS_DT5VC_H_LOW, "BOX H 5VC ���� �̻�", "" , "" },

	{ SGNI, "Signal",  "", "" },

	{ FLAG, "Flag",  "", "" },
	
		{ FLAG_SOURCE_OR_SINK_CANNOT_ON, "Source/Sink ON �̻�", "", "" },
		{ FLAG_SOURCE_CANNOT_OFF,   "Source OFF �̻�", "", "" },
		{ FLAG_SINK_CANNOT_OFF, "Sink OFF �̻�", "", "" },
		{ FLAG_SOURCE_LEAK, "Source Leak �̻�", "", "" },
		{ FLAG_SINK_LEAK, "Sink Leak �̻�", "", "" },
		{ FLAG_ODD_SOURCE_LEAK, "Odd Source Leak �̻�", "", "" },
		{ FLAG_EVEN_SOURCE_LEAK, "Even Source Leak �̻�", "", "" },

		{ FLAG_FILE_NOT_FOUND, "���� ã�� �� ����", "", "" },
		{ FLAG_FILE_CANNOT_OPEN, "���� ���� �̻�", "", "" },
		{ FLAG_FILE_UNPROPER_PARAMETER, "���� ���� �̻�", "", "" },
		{ FLAG_FILE_CHECK_SUM, "���� CheckSum �̻�", "", "" },



	{ INVALID_RETURN, "Invalid return",  "", "" },
	{ INVALID_INPUT, "Invalid Input Parameter",  "", "" },

	{ USER_ERR, "����� ���� ����",  "", "" },

	{ RANGE_OVER, "Range Over",  "", "" },
		{ OL_REF_CURRENT_IS_OVER_INPUT_VOLT_N_RGST, "���� ���� �� �̻�", "", "" },
		{ OL_I_RANGE_OVER, "�ִ� ���� ���� ���� �ʰ�", "", "" },
		{ OL_I_RANGE_UNDER, "�ּ� ���� ���� ���� �ʰ�", "", "" },
		{ CAL_CHECK_TEST_CONDITION, "Calibration Test Condtion ���� ���� üũ �ʿ�", "", "" },
		{ CAL_CHECK_HW, "Calibration Hw Read �� �̻�. H/W üũ �ʿ�", "", "" },

	{ SW_LOGIC_ERR, "SW �� ���� ����. S/Wüũ �ʿ�", "", "" },
		{ SW_NO_INIT_VAR, "�ʱ�ȭ���� ���� ���� ���. S/W üũ �ʿ�", "", "" },
		{ MAN_ERR_TEST_CONDEITION, "ManulTest ���� ���� �̻�", "", "" },

	{ OVERTIME, "Over time",  "", "" },
		{ OT_SETNCHK,  "Souce ���м��� �̻�", "", "" },
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
