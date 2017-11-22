/** SerDes.c
  Provides the basic interfaces for SerDes Module

  Copyright 2017 NXP

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution. The full text of the license may be found
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Bitops.h>
#include <Library/DebugLib.h>
#include <SerDes.h>
#include <SocSerDes.h>
#include <Soc.h>
#include <Uefi.h>

/**
  Function to get serdes Lane protocol corresponding to
  serdes protocol.

  @param  SerDes    Serdes number.
  @param  Cfg       Serdes Protocol.
  @param  Lane      Serdes Lane number.

  @return           Serdes Lane protocol.

**/
STATIC
SERDES_PROTOCOL
GetSerDesPrtcl (
  IN  INTN          SerDes,
  IN  INTN          Cfg,
  IN  INTN          Lane
  )
{
  SERDES_CONFIG     *Config;

  if (SerDes >= ARRAY_SIZE (SerDesConfigTbl)) {
    return 0;
  }

  Config = SerDesConfigTbl[SerDes];
  while (Config->Protocol) {
    if (Config->Protocol == Cfg) {
      return Config->SrdsLane[Lane];
    }
    Config++;
  }

  return EFI_SUCCESS;
}

/**
  Function to check if inputted protocol is a valid serdes protocol.

  @param  SerDes                   Serdes number.
  @param  Prtcl                    Serdes Protocol to be verified.

  @return EFI_INVALID_PARAMETER    Input parameter in invalid.
  @return EFI_NOT_FOUND            Serdes Protocol not a valid protocol.
  @return EFI_SUCCESS              Serdes Protocol is a valid protocol.

**/
STATIC
EFI_STATUS
CheckSerDesPrtclValid (
  IN  INTN      SerDes,
  IN  UINT32    Prtcl
  )
{
  SERDES_CONFIG *Config;
  INTN          Cnt;

  if (SerDes >= ARRAY_SIZE (SerDesConfigTbl)) {
    return EFI_INVALID_PARAMETER;
  }

  Config = SerDesConfigTbl[SerDes];
  while (Config->Protocol) {
    if (Config->Protocol == Prtcl) {
      DEBUG ((DEBUG_INFO, "Protocol: %x Matched with the one in Table\n", Prtcl));
      break;
    }
    Config++;
  }

  if (!Config->Protocol) {
    return EFI_NOT_FOUND;
  }

  for (Cnt = 0; Cnt < SRDS_MAX_LANES; Cnt++) {
    if (Config->SrdsLane[Cnt] != NONE) {
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Function to fill serdes map information.

  @param  Srds                  Serdes number.
  @param  SerdesProtocolMask    Serdes Protocol Mask.
  @param  SerdesProtocolShift   Serdes Protocol shift value.
  @param  SerDesPrtclMap        Pointer to Serdes Protocol map.

**/
STATIC
VOID
LSSerDesMap (
  IN  UINT32                    Srds,
  IN  UINT32                    SerdesProtocolMask,
  IN  UINT32                    SerdesProtocolShift,
  OUT UINT64                    *SerDesPrtclMap
  )
{
  CCSR_GUR                      *Gur;
  UINT32                        SrdsProt;
  INTN                          Lane;
  UINT32                        Flag;

  Gur = (VOID *)PcdGet64 (PcdGutsBaseAddr);
  *SerDesPrtclMap = 0x0;
  Flag = 0;

  SrdsProt = GurRead ((UINTN)&Gur->RcwSr[RCWSR_INDEX]) & SerdesProtocolMask;
  SrdsProt >>= SerdesProtocolShift;

  DEBUG ((DEBUG_INFO, "Using SERDES%d Protocol: %d (0x%x)\n",
                                   Srds + 1, SrdsProt, SrdsProt));

  if (EFI_SUCCESS != CheckSerDesPrtclValid (Srds, SrdsProt)) {
    DEBUG ((DEBUG_ERROR, "SERDES%d[PRTCL] = 0x%x is not valid\n",
                                   Srds + 1, SrdsProt));
    Flag++;
  }

  for (Lane = 0; Lane < SRDS_MAX_LANES; Lane++) {
    SERDES_PROTOCOL LanePrtcl = GetSerDesPrtcl (Srds, SrdsProt, Lane);
    if (LanePrtcl >= SERDES_PRTCL_COUNT) {
      DEBUG ((DEBUG_ERROR, "Unknown SerDes lane protocol %d\n", LanePrtcl));
      Flag++;
    } else {
      *SerDesPrtclMap |= BIT (LanePrtcl);
    }
  }

  if (Flag) {
    DEBUG ((DEBUG_ERROR, "Could not configure SerDes module!!\n"));
  } else {
    DEBUG ((DEBUG_INFO, "Successfully configured SerDes module!!\n"));
  }
}

/**
  Get lane protocol on provided serdes lane and execute callback function.

  @param  Srds                    Serdes number.
  @param  SerdesProtocolMask      Mask to get Serdes Protocol for Srds
  @param  SerdesProtocolShift     Shift value to get Serdes Protocol for Srds.
  @param  SerDesLaneProbeCallback Pointer Callback function to be called for Lane protocol
  @param  Arg                     Pointer to Arguments to be passed to callback function.

**/
STATIC
VOID
SerDesInstanceProbeLanes (
  IN  UINT32                      Srds,
  IN  UINT32                      SerdesProtocolMask,
  IN  UINT32                      SerdesProtocolShift,
  IN  SERDES_PROBE_LANES_CALLBACK *SerDesLaneProbeCallback,
  IN  VOID                        *Arg
  )
{

  CCSR_GUR                        *Gur;
  UINT32                          SrdsProt;
  INTN                            Lane;

  Gur = (VOID *)PcdGet64 (PcdGutsBaseAddr);;

  SrdsProt = GurRead ((UINTN)&Gur->RcwSr[RCWSR_INDEX]) & SerdesProtocolMask;
  SrdsProt >>= SerdesProtocolShift;

  /*
   * Invoke callback for all lanes in the SerDes instance:
   */
  for (Lane = 0; Lane < SRDS_MAX_LANES; Lane++) {
    SERDES_PROTOCOL LanePrtcl = GetSerDesPrtcl (Srds, SrdsProt, Lane);
    if (LanePrtcl >= SERDES_PRTCL_COUNT || LanePrtcl < NONE) {
      DEBUG ((DEBUG_ERROR, "Unknown SerDes lane protocol %d\n", LanePrtcl));
    }
    else if (LanePrtcl != NONE) {
      SerDesLaneProbeCallback (LanePrtcl, Arg);
    }
  }
}

/**
  Probe all serdes lanes for lane protocol and execute provided callback function.

  @param  SerDesLaneProbeCallback Pointer Callback function to be called for Lane protocol
  @param  Arg                     Pointer to Arguments to be passed to callback function.

**/
VOID
SerDesProbeLanes (
  IN SERDES_PROBE_LANES_CALLBACK *SerDesLaneProbeCallback,
  IN VOID                        *Arg
  )
{
  SerDesInstanceProbeLanes (SRDS_1,
                            RCWSR_SRDS1_PRTCL_MASK,
                            RCWSR_SRDS1_PRTCL_SHIFT,
                            SerDesLaneProbeCallback,
                            Arg);

  if (PcdGetBool (PcdSerdes2Enabled)) {
   SerDesInstanceProbeLanes (SRDS_2,
                             RCWSR_SRDS2_PRTCL_MASK,
                             RCWSR_SRDS2_PRTCL_SHIFT,
                             SerDesLaneProbeCallback,
                             Arg);
  }
}

/**
  Function to return Serdes protocol map for all serdes available on board.

  @param  SerDesPrtclMap   Pointer to Serdes protocl map.

**/
VOID
GetSerdesProtocolMaps (
  OUT UINT64               *SerDesPrtclMap
  )
{
  LSSerDesMap (SRDS_1,
               RCWSR_SRDS1_PRTCL_MASK,
               RCWSR_SRDS1_PRTCL_SHIFT,
               SerDesPrtclMap);

  if (PcdGetBool (PcdSerdes2Enabled)) {
    LSSerDesMap (SRDS_2,
                 RCWSR_SRDS2_PRTCL_MASK,
                 RCWSR_SRDS2_PRTCL_SHIFT,
                 SerDesPrtclMap);
  }

}

BOOLEAN
IsSerDesLaneProtocolConfigured (
  IN UINT64          SerDesPrtclMap,
  IN SERDES_PROTOCOL Device
  )
{
  if (Device >= SERDES_PRTCL_COUNT || Device < NONE) {
    ASSERT (Device > NONE && Device < SERDES_PRTCL_COUNT);
    DEBUG ((DEBUG_ERROR, "Unknown SerDes lane protocol Device %d\n", Device));
  }

  return (SerDesPrtclMap & BIT (Device)) != 0 ;
}
