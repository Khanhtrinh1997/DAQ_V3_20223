

#ifndef _CRC32_CAL_H
#define _CRC32_CAL_H

#include "stdint.h"

void crc32cal_Init(void);
void crc32cal_WriteData(const uint8_t *data, uint32_t dataSize);
uint32_t crc32cal_Get32bitResult(void);

#endif /* _CRC32_CAL_H */
