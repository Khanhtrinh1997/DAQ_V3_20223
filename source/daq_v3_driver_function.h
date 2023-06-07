//--thanhcm3--07--04--22--
#ifndef _DAQ_V3_DRIVER_FUNCTION_H
#define _DAQ_V3_DRIVER_FUNCTION_H
//include-----------------------------------------------------------------------
//define------------------------------------------------------------------------
//function----------------------------------------------------------------------
//revertIP--------------------------------------
uint32_t revertIP(uint32_t ipaddr);
//function convert--------------------------------------------------------------
uint8_t Checksum8(unsigned char *src,uint32_t len);
uint16_t Checksum16(unsigned char *src,uint32_t len);
void hex2char(unsigned char *src, uint16_t hexnum, uint8_t len);
int8_t char2hex(unsigned char c);
float hex2float(unsigned char* src,uint32_t startbyte);
uint8_t hex2byte(unsigned char* src,uint32_t startbyte);
int16_t iiihex2byte2(unsigned char* src,uint32_t startbyte);
int16_t iihex2byte2(unsigned char* src,uint32_t startbyte);
int16_t ihex2byte2(unsigned char* src,uint32_t startbyte);
uint16_t hex2byte2(unsigned char* src,uint32_t startbyte);
uint32_t hex2byte4(unsigned char* src,uint32_t startbyte);
void u82char(unsigned char *src, uint8_t value, uint8_t len);
void u162char(unsigned char *src, uint16_t value, uint8_t len);
void u322char(unsigned char *src, uint32_t value, uint8_t len);
bool htmlIsValidHexDigit(const char cDigit);
unsigned char htmlHexDigit(const char cDigit);
void RectID_Calculation(char* dst, unsigned char* src);
//HTML--------------------------------------------------------------------------
// html_text_extract---------------------------
uint8_t html_text_extract(const char* pcExtString, uint32_t* pcDst, uint8_t keywordLen);
uint32_t htmlDecodeFormString(const  char *pcEncoded, char *pcDecoded, uint32_t ulLen);
bool htmlDecodeHexEscape(const char *pcEncoded, char *pcDecoded);
uint8_t htmlTextExtract2(const char* pcExtString, int32_t* pcDst, uint8_t keywordLen);


//code here


#endif /* _DAQ_V3_DRIVER_FUNCTION_H */