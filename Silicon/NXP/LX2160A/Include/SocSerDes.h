/** @file
 The Header file of SerDes Module for LX2160A

 Copyright 2017 NXP

 This program and the accompanying materials
 are licensed and made available under the terms and conditions of the BSD License
 which accompanies this distribution. The full text of the license may be found
 http://opensource.org/licenses/bsd-license.php

 THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
 WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __SOC_SERDES_H__
#define __SOC_SERDES_H__

#include <SerDes.h>

SERDES_CONFIG SerDes1ConfigTbl[] = {
  /* SerDes 1 */
  { 0x01, { PCIE2, PCIE2, PCIE2, PCIE2, PCIE1, PCIE1, PCIE1, PCIE1 } },
  { 0x02, { PCIE2, PCIE2, PCIE2, PCIE2, SGMII6, SGMII5, SGMII4, SGMII3 } },
  { 0x03, { PCIE2, PCIE2, PCIE2, PCIE2, XFI6, XFI5, XFI4, XFI3 } },
  { 0x04, { SGMII10, SGMII9, SGMII8, SGMII7, SGMII6, SGMII5, SGMII4, SGMII3 } },
  { 0x05, { XFI10, XFI9, XFI8, XFI7, PCIE1, PCIE1, PCIE1, PCIE1} },
  { 0x06, { SGMII10, SGMII9, SGMII8, SGMII7, SGMII6, SGMII5, XFI4, XFI3 } },
  { 0x07, { SGMII10, SGMII9, SGMII8, SGMII7, XFI6, XFI5, XFI4, XFI3  } },
  { 0x08, { XFI10, XFI9, XFI8, XFI7, XFI6, XFI5, XFI4, XFI3 } },
  { 0x09, { SGMII10, SGMII9, SGMII8, PCIE2, SGMII6, SGMII5, SGMII4, PCIE1 } },
  { 0x0A, { XFI10, XFI9, XFI8, PCIE2, XFI6, XFI5, XFI4, PCIE1 } },
  { 0x0B, { SGMII10, SGMII9, PCIE2, PCIE2, SGMII6, SGMII5, PCIE1, PCIE1 } },
  { 0x0C, { SGMII10, SGMII9, PCIE2, PCIE2, PCIE1, PCIE1, PCIE1, PCIE1 } },
  { 0x0D, { GE100_2, GE100_2, GE100_2, GE100_2, GE100_1, GE100_1, GE100_1, GE100_1 } },
  { 0x0E, { PCIE2, PCIE2, PCIE2, PCIE2, GE100_1, GE100_1, GE100_1, GE100_1 } },
  { 0x0F, { PCIE2, PCIE2, PCIE2, PCIE2, GE50_2, GE50_2, GE50_1, GE50_1 } },
  { 0x10, { PCIE2, PCIE2, PCIE2, PCIE2, GE25_6, GE25_5, GE50_1, GE50_1 } },
  { 0x11, { PCIE2, PCIE2, PCIE2, PCIE2, GE25_6, GE25_5, GE25_4, GE25_3 } },
  { 0x12, { XFI10, XFI9, XFI8, XFI7, GE25_6, GE25_5, XFI4, XFI3 } },
  { 0x13, { GE40_2, GE40_2, GE40_2, GE40_2, GE25_6, GE25_5, XFI4, XFI3 } },
  { 0x14, { GE40_2, GE40_2, GE40_2, GE40_2, GE40_1, GE40_1, GE40_1, GE40_1 } },
  { 0x15, { GE25_10, GE25_9, PCIE2, PCIE2, GE25_6, GE25_5, GE25_4, GE25_3 } },
  { 0x16, { XFI10, XFI9, PCIE2, PCIE2, XFI6, XFI5, XFI4, XFI3 } },
  {}
};

SERDES_CONFIG SerDes2ConfigTbl[] = {
  /* SerDes 2 */
  { 0x01, { PCIE3, PCIE3, SATA1, SATA2, PCIE4, PCIE4, PCIE4, PCIE4 } },
  { 0x02, { PCIE3, PCIE3, PCIE3, PCIE3, PCIE3, PCIE3, PCIE3, PCIE3 } },
  { 0x03, { PCIE3, PCIE3, PCIE3, PCIE3, PCIE4, PCIE4, PCIE4, PCIE4 } },
  { 0x04, { PCIE3, PCIE3, PCIE3, PCIE3, PCIE4, PCIE4, SATA1, SATA2 } },
  { 0x05, { PCIE3, PCIE3, PCIE3, PCIE3, SATA3, SATA4, SATA1, SATA2 } },
  { 0x06, { PCIE3, PCIE3, PCIE3, PCIE3, SGMII15, SGMII16, XFI13, XFI14} },
  { 0x07, { PCIE3, SGMII12, SGMII17, SGMII18, PCIE4, SGMII16, XFI13, XFI14} },
  { 0x08, { 0, 0, SATA1, SATA2, SATA3, SATA4, XFI13, XFI14} },
  { 0x09, { SGMII11, SGMII12, SGMII17, SGMII18, SGMII15, SGMII16, SGMII13, SGMII14 } },
  { 0x0A, { SGMII11, SGMII12, SGMII17, SGMII18, PCIE4, PCIE4, PCIE4, PCIE4 } },
  { 0x0B, { PCIE3, SGMII12, SGMII17, SGMII18, PCIE4, SGMII16, SGMII13, SGMII14 } },
  { 0x0C, { SGMII11, SGMII12, SGMII17, SGMII18, PCIE4, PCIE4, SATA1, SATA2 } },
  { 0x0D, { PCIE3, PCIE3, PCIE3, PCIE3, PCIE4, PCIE4, SGMII13, SGMII14 } },
  { 0x0E, { PCIE3, PCIE3, SGMII17, SGMII18, PCIE4, PCIE4, SGMII13, SGMII14 } },
  {}
};

SERDES_CONFIG SerDes3ConfigTbl[] = {
  /* SerDes 3 */
  { 0x02, { PCIE5, PCIE5, PCIE5, PCIE5, PCIE5, PCIE5, PCIE5, PCIE5 } },
  { 0x03, { PCIE5, PCIE5, PCIE5, PCIE5, PCIE6, PCIE6, PCIE6, PCIE6 } },
  {}
};

SERDES_CONFIG *SerDesConfigTbl[] = {
  SerDes1ConfigTbl,
  SerDes2ConfigTbl,
  SerDes3ConfigTbl
};

#endif /* __SOC_SERDES_H */
