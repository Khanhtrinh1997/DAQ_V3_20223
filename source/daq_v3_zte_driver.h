//--thanhcm3--07--04--22--
#ifndef _DAQ_V3_ZTE_DRIVER_H
#define _DAQ_V3_ZTE_DRIVER_H
//include-----------------------------------------------------------------------
//define------------------------------------------------------------------------
#define REQtimeinfoLen 			18
#define REQmanufacturerinfoLen 	        18
#define REQcommonparainfoLen 	        18
#define REQalarmlevelLen 		18
#define REQacinputinfoLen 		20
#define REQacalarmLen 			20
#define REQacparameterLen 		18
#define REQrectstsLen 			18
#define REQrectinfoLen 			18
#define REQrectalarmLen		 	18
#define REQdcalarmLen 			18
#define REQdcparameterLen 		18
#define REQdcoutputLen 			18
#define REQenvalarmLen 			18
#define	REQenvdataLen			18

// Ban tin  TIMEINFO_REQ yeu cau thong tin ve thoi gian
static const unsigned char REQtimeinfo[REQtimeinfoLen] = {
//~	2	0	0	1	4	0	4	D	0	0	0	0	F	D	A	1	<CR>
0x7e,	0x32,	0x30,	0x30,	0x31,	0x34,	0x30,	0x34,	0x44,	0x30,	0x30,	0x30,	0x30,	0x46,	0x44,	0x41,	0x31,	0x0d
};
// Ban tin  MANUFACTURERINFO_REQ yeu cau thong tin nha san xuat cua thiet bi
static const unsigned char REQmanufacturerinfo[REQmanufacturerinfoLen] = {
//~	2	0	0	1	4	0	5	1	0	0	0	0	F	D	B	3	<CR>
0x7e,	0x32,	0x30,	0x30,	0x31,	0x34,	0x30,	0x35,	0x31,	0x30,	0x30,	0x30,	0x30,	0x46,	0x44,	0x42,	0x33,	0x0d
};
// Ban tin  COMMONPARA_REQ yeu cau thong so chung
static const unsigned char REQcommonparainfo[REQcommonparainfoLen] = {
//~	2	0	0	1	4	0	E	E	0	0	0	0	F	D	8	F	<CR>
0x7e,	0x32,	0x30,	0x30,	0x31,	0x34,	0x30,	0x45,	0x45,	0x30,	0x30,	0x30,	0x30,	0x46,	0x44,	0x38,	0x46,	0x0d
};
// Ban tin  ALARMLEVEL_REQ yeu cau thong tin muc canh bao
static const unsigned char REQalarmlevel[REQalarmlevelLen] = {
//~	2	0	0	1	4	0	F	0	0	0	0	0	F	D	A	3	<CR>
0x7e,	0x32,	0x30,	0x30,	0x31,	0x34,	0x30,	0x46,	0x30,	0x30,	0x30,	0x30,	0x30,	0x46,	0x44,	0x41,	0x33,	0x0d
};
// Ban tin  ACINPUTINFO_REQ yeu cau thong tin dien ap ac dau vao
static const unsigned char REQacinputinfo[REQacinputinfoLen] = {
//~	2	0	0	1	4	0	4	1	E	0	0	2	F	F	F	D	1	1	<CR>
0x7e,	0x32,	0x30,	0x30,	0x31,	0x34,	0x30,	0x34,	0x31,	0x45,	0x30,	0x30,	0x32,	0x46,	0x46,	0x46,	0x44,	0x31,	0x31,	0x0d
};
// Ban tin  ACALARM_REQ yeu cau canh bao dien ap ac dau vao
static const unsigned char REQacalarm[REQacalarmLen] = {
//~	2	0	0	1	4	0	4	4	E	0	0	2	F	F	F	D	0	E	<CR>
0x7e,	0x32,	0x30,	0x30,	0x31,	0x34,	0x30,	0x34,	0x34,	0x45,	0x30,	0x30,	0x32,	0x46,	0x46,	0x46,	0x44,	0x30,	0x45,	0x0d
};
// Ban tin  ACPARAMETER_REQ yeu cau thong so ac
static const unsigned char REQacparameter[REQacparameterLen] = {
//~	2	0	0	1	4	0	4	6	0	0	0	0	F	D	A	F	<CR>
0x7e,	0x32,	0x30,	0x30,	0x31,	0x34,	0x30,	0x34,	0x36,	0x30,	0x30,	0x30,	0x30,	0x46,	0x44,	0x41,	0x46,	0x0d
};

// Ban tin  RECTSTSZTE_REQ yeu cau thong in trang thai rect
static const unsigned char REQrectstszte[REQrectstsLen] = {
//~	2	0	0	1	4	1	4	3	0	0	0	0	F	D	B	1	<CR>
0x7e,	0x32,	0x30,	0x30,	0x31,	0x34,	0x31,	0x34,	0x33,	0x30,	0x30,	0x30,	0x30,	0x46,	0x44,	0x42,	0x31,	0x0d
};

// Ban tin  RECTINFO_REQ yeu cau thong in dong ap rect
static const unsigned char REQrectinfo[REQrectinfoLen] = {
//~	2	0	0	1	4	1	4	1	0	0	0	0	F	D	B	3	<CR>
0x7e,	0x32,	0x30,	0x30,	0x31,	0x34,	0x31,	0x34,	0x31,	0x30,	0x30,	0x30,	0x30,	0x46,	0x44,	0x42,	0x33,	0x0d
};
// Ban tin  RECTALARM_REQ yeu cau thong in canh bao rect
static const unsigned char REQrectalarm[REQrectalarmLen] = {
//~	2	0	0	1	4	1	4	4	0	0	0	0	F	D	B	0	<CR>
0x7e,	0x32,	0x30,	0x30,	0x31,	0x34,	0x31,	0x34,	0x34,	0x30,	0x30,	0x30,	0x30,	0x46,	0x44,	0x42,	0x30,	0x0d
};

// Ban tin  DCOUTPUT_REQ yeu cau thong in dc dau ra
static const unsigned char REQdcoutput[REQdcoutputLen] = {
//~	2	0	0	1	4	2	4	1	0	0	0	0	F	D	B	2	<CR>
0x7e,	0x32,	0x30,	0x30,	0x31,	0x34,	0x32,	0x34,	0x31,	0x30,	0x30,	0x30,	0x30,	0x46,	0x44,	0x42,	0x32,	0x0d
};
// Ban tin  DCALARM_REQ yeu cau thong in canh bao dc
static const unsigned char REQdcalarm[REQdcalarmLen] = {
//~	2	0	0	1	4	2	4	4	0	0	0	0	F	D	A	F	<CR>
0x7e,	0x32,	0x30,	0x30,	0x31,	0x34,	0x32,	0x34,	0x34,	0x30,	0x30,	0x30,	0x30,	0x46,	0x44,	0x41,	0x46,	0x0d
};

// Ban tin  DCPARAMETER_REQ yeu cau thong so dc
static const unsigned char REQdcparameter[REQdcparameterLen] = {
//~	2	0	0	1	4	2	4	6	0	0	0	0	F	D	A	D	<CR>
0x7e,	0x32,	0x30,	0x30,	0x31,	0x34,	0x32,	0x34,	0x36,	0x30,	0x30,	0x30,	0x30,	0x46,	0x44,	0x41,	0x44,	0x0d
};

// Ban tin  ENVDATA_REQ yeu cau gia tri cac thong so moi truong
static const unsigned char REQenvdata[REQenvdataLen] = {
//~	2	0	0	1	8	0	4	1	0	0	0	0	F	D	B	0	<CR>
0x7e,	0x32,	0x30,	0x30,	0x31,	0x38,	0x30,	0x34,	0x31,	0x30,	0x30,	0x30,	0x30,	0x46,	0x44,	0x42,	0x30,	0x0d
};
// Ban tin  ENVALARM_REQ yeu cau thong in canh bao moi truong
static const unsigned char REQenvalarm[REQenvalarmLen] = {
//~	2	0	0	1	8	0	4	4	0	0	0	0	F	D	A	D	<CR>
0x7e,	0x32,	0x30,	0x30,	0x31,	0x38,	0x30,	0x34,	0x34,	0x30,	0x30,	0x30,	0x30,	0x46,	0x44,	0x41,	0x44,	0x0d
};
// Header code cho ban tin cai dat thong so AC
static unsigned char Header_AC_Setcode[13] = {
//~	2	0	0	1	4	0	4	8	6	0	0	A
0x7e,	0x32,	0x30,	0x30,	0x31,	0x34,	0x30,	0x34,	0x38,	0x36,	0x30,	0x30,	0x41
};
// Header code cho ban tin cai dat thong so DC
static unsigned char Header_DC_Setcode[13] = {
//~	2	0	0	1	4	2	4	8	6	0	0	A
0x7e,	0x32,	0x30,	0x30,	0x31,	0x34,	0x32,	0x34,	0x38,	0x36,	0x30,	0x30,	0x41
};
#define AC 1
#define DC 2
#define ACVOLT_UPPERLIMIT		0x80
#define ACVOLT_LOWERLIMIT		0x81
#define ACINPUTCUR_UPPERLIMIT           0x82

#define DCVOLT_UPPERLIMIT 		0x80
#define DCVOLT_LOWERLIMIT 		0x81
#define FLOATVOLT 			0xE0
#define BOOSTVOLT 			0xE1
#define DCLOW 				0xE3
#define LLVD 				0xE4
#define BLVD 				0xE5
#define CHARGINGFACTOR 			0xE6
#define TEMP_COMPENSATION 		0xE8
#define BATT_TEMP_OVER 			0xE9
#define BATT_CAP1 			0xEA
#define BATT_CAP2 			0xEB
#define BATT_CAP3 			0xEC
#define TESTVOLT                        0xE2
#define AUTOTESTDAY                     0xF3
#define TESTSTARTTIME                   0xF5


#define MAC_ALREADY_WRITTEN 14
//function----------------------------------------------------------------------

void ZTE_SetCommand(unsigned char *arrCmd,uint16_t templateCmd, float setNum, unsigned char type);
void EMERV21_SetCommand(unsigned char *arrCmd,uint16_t templateCmd, float setNum, unsigned char type);
//code here


#endif /* _DAQ_V3_ZTE_DRIVER_H */