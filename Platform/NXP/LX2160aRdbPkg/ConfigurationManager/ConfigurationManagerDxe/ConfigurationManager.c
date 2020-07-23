/** @file
  Configuration Manager Dxe

  Copyright (c) 2020 Puresoftware Ltd. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Glossary:
    - Cm or CM   - Configuration Manager
    - Obj or OBJ - Object
**/

#include <IndustryStandard/DebugPort2Table.h>
#include <IndustryStandard/MemoryMappedConfigurationSpaceAccessTable.h>
#include <IndustryStandard/SerialPortConsoleRedirectionTable.h>
#include <Library/ArmLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/ConfigurationManagerProtocol.h>

#include <AcpiTables/Platform.h>
#include "ConfigurationManager.h"

// AML Code Include files generated by iASL Compiler
#include <Dsdt.hex>

/** The platform configuration repository information.
*/
STATIC
EDKII_PLATFORM_REPOSITORY_INFO Lx2160aPlatformRepositoryInfo = {
  /// Configuration Manager information
  { CONFIGURATION_MANAGER_REVISION, CFG_MGR_OEM_ID },

  // ACPI Table List
  {
    // FADT Table
    {
      EFI_ACPI_6_2_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE,
      EFI_ACPI_6_2_FIXED_ACPI_DESCRIPTION_TABLE_REVISION,
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdFadt),
      NULL,
      CFG_MGR_TABLE_ID
    },

    // GTDT Table
    {
      EFI_ACPI_6_2_GENERIC_TIMER_DESCRIPTION_TABLE_SIGNATURE,
      EFI_ACPI_6_2_GENERIC_TIMER_DESCRIPTION_TABLE_REVISION,
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdGtdt),
      NULL,
      CFG_MGR_TABLE_ID
    },

    // MADT Table
    {
      EFI_ACPI_6_2_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE,
      EFI_ACPI_6_2_MULTIPLE_APIC_DESCRIPTION_TABLE_REVISION,
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdMadt),
      NULL,
      CFG_MGR_TABLE_ID
    },

    // DSDT Table
    {
      EFI_ACPI_6_2_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE,
      0,
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdDsdt),
      (EFI_ACPI_DESCRIPTION_HEADER *) &dsdt_aml_code,
      CFG_MGR_TABLE_ID
    }

  },

  // Boot architecture information
  { EFI_ACPI_6_2_ARM_PSCI_COMPLIANT },        // BootArchFlags

  // Power management profile information
  { EFI_ACPI_6_2_PM_PROFILE_MOBILE },         // PowerManagement Profile

  // Generic Timer Info
  {
    // The physical base address for the counter control frame
    TIMER_BASE_ADDRESS,
    // The physical base address for the counter read frame
    TIMER_READ_BASE_ADDRESS,
    // The secure PL1 timer interrupt
    TIMER_SEC_IT,
    // The secure PL1 timer flags
    GTDT_GTIMER_FLAGS,
    // The non-secure PL1 timer interrupt
    TIMER_NON_SEC_IT,
    // The non-secure PL1 timer flags
    GTDT_GTIMER_FLAGS,
    // The virtual timer interrupt
    TIMER_VIRT_IT,
    // The virtual timer flags
    GTDT_GTIMER_FLAGS,
    // The non-secure PL2 timer interrupt
    TIMER_HYP_IT,
    // The non-secure PL2 timer flags
    GTDT_GTIMER_FLAGS
  },

  // Generic Timer Block Information
  {
    {
      // The physical base address for the GT Block Timer structure
      TIMER_GT_BLOCK_0_ADDRESS,
      // The number of timer frames implemented in the GT Block
      PLAT_GTFRAME_COUNT,
      // Reference token for the GT Block timer frame list
      (CM_OBJECT_TOKEN)((UINT8*)&Lx2160aPlatformRepositoryInfo +
          OFFSET_OF (EDKII_PLATFORM_REPOSITORY_INFO, GTBlock0TimerInfo))
    }
  },

  // GTDT Frames
  {
    {
      0,                                    // UINT8 GTFrameNumber
      TIMER_GT_BASE_0_ADDRESS,              // UINT64 CntBaseX
      TIMER_GT_BASE_0_EL0_ADDRESS,          // UINT64 CntEL0BaseX
      TIMER_FRAME0_IT,                      // UINT32 GTxPhysicalTimerGSIV
      GTDT_FRAME_FLAGS,                     // UINT32 GTxPhysicalTimerFlags
      TIMER_FRAME0_IT,                      // UINT32 GTxVirtualTimerGSIV
      GTDT_FRAME_FLAGS,                     // UINT32 GTxVirtualTimerFlags
      0                                     // UINT32 GTxCommonFlags
    }, //Gtdt.Frames[0]
    {
      1,                                    // UINT8 GTFrameNumber
      TIMER_GT_BASE_1_ADDRESS,              // UINT64 CntBaseX
      GT_BLOCK_FRAME_RES_BASE,              // UINT64 CntEL0BaseX
      TIMER_FRAME1_IT,                      // UINT32 GTxPhysicalTimerGSIV
      GTDT_FRAME_FLAGS,                     // UINT32 GTxPhysicalTimerFlags
      0,                                    // UINT32 GTxVirtualTimerGSIV
      0,                                    // UINT32 GTxVirtualTimerFlags
      GTDT_FRAME_COMMON_FLAGS               // UINT32 GTxCommonFlags
    }, //Gtdt.Frames[1]
    {
      2,                                     // UINT8 GTFrameNumber
      TIMER_GT_BASE_2_ADDRESS,               // UINT64 CntBaseX
      TIMER_GT_BASE_2_EL0_ADDRESS,           // UINT64 CntEL0BaseX
      TIMER_FRAME2_IT,                       // UINT32 GTxPhysicalTimerGSIV
      GTDT_FRAME_FLAGS,                      // UINT32 GTxPhysicalTimerFlags
      0,                                     // UINT32 GTxVirtualTimerGSIV
      0,                                     // UINT32 GTxVirtualTimerFlags
      GTDT_FRAME_COMMON_FLAGS                // UINT32 GTxCommonFlags
    },//Gtdt.Frames[2]
    {
      3,                                     // UINT8 GTFrameNumber
      TIMER_GT_BASE_3_ADDRESS,               // UINT64 CntBaseX
      GT_BLOCK_FRAME_RES_BASE,               // UINT64 CntEL0BaseX
      TIMER_FRAME3_IT,                       // UINT32 GTxPhysicalTimerGSIV
      GTDT_FRAME_FLAGS,                      // UINT32 GTxPhysicalTimerFlags
      0,                                     // UINT32 GTxVirtualTimerGSIV
      0,                                     // UINT32 GTxVirtualTimerFlags
      GTDT_FRAME_COMMON_FLAGS                // UINT32 GTxCommonFlags
    }, //Gtdt.Frames[3]
  }, // End of frames and timer blocks

  // watchdogs
  {
      TIMER_WDT0_CONTROL_BASE,                // UINT64 WatchdogControlFramePhysicalAddress
      TIMER_WDT0_REFRESH_BASE,                // UINT64 RefreshFramePhysicalAddress
      TIMER_WDT0_IT,                          // UINT32 WatchdogTimerGSIV
      SBSA_WATCHDOG_FLAGS                     // UINT32 WatchdogTimerFlags
  },

  /* GIC CPU Interface information
     GIC_ENTRY (CPUInterfaceNumber, Mpidr, PmuIrq, VGicIrq, EnergyEfficiency)
   */
  {
    GICC_ENTRY (0,  GET_MPID (0, 0), 23, 0x19, 0),
    GICC_ENTRY (1,  GET_MPID (0, 1), 23, 0x19, 0),
    GICC_ENTRY (2,  GET_MPID (1, 0), 23, 0x19, 0),
    GICC_ENTRY (3,  GET_MPID (1, 1), 23, 0x19, 0),
    GICC_ENTRY (4,  GET_MPID (2, 0), 23, 0x19, 0),
    GICC_ENTRY (5,  GET_MPID (2, 1), 23, 0x19, 0),
    GICC_ENTRY (6,  GET_MPID (3, 0), 23, 0x19, 0),
    GICC_ENTRY (7,  GET_MPID (3, 1), 23, 0x19, 0),
    GICC_ENTRY (8,  GET_MPID (4, 0), 23, 0x19, 0),
    GICC_ENTRY (9,  GET_MPID (4, 1), 23, 0x19, 0),
    GICC_ENTRY (10, GET_MPID (5, 0), 23, 0x19, 0),
    GICC_ENTRY (11, GET_MPID (5, 1), 23, 0x19, 0),
    GICC_ENTRY (12, GET_MPID (6, 0), 23, 0x19, 0),
    GICC_ENTRY (13, GET_MPID (6, 1), 23, 0x19, 0),
    GICC_ENTRY (14, GET_MPID (7, 0), 23, 0x19, 0),
    GICC_ENTRY (15, GET_MPID (7, 1), 23, 0x19, 0)
  },

  // GIC Distributor Info
  {
    GICD_BASE,                                // UINT64  PhysicalBaseAddress
    0,                                        // UINT32  SystemVectorBase
    GIC_VERSION                               // UINT8   GicVersion
  },

  /* GIC Redistributor */
  {
    GICR_BASE,                                // UINT64 DiscoveryRangeBaseAddress
    GICR_LEN                                  // UINT32 DiscoveryRangeLength
  },

  /* GIC ITS */
  {
    0,                                        // UINT32 GIC ITS ID
    GICI_BASE,                                // UINT64 The 64-bit physical address for ITS
    0                                         // UINT32 Populate the GIC ITS affinity in SRAT table.
  },

  2.0                                         // LX2 board revision
};

/** Initialize the platform configuration repository.

  @param [in]  This        Pointer to the Configuration Manager Protocol.

  @retval EFI_SUCCESS   Success
**/
STATIC
EFI_STATUS
EFIAPI
InitializePlatformRepository (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  * CONST This
  )
{
  EDKII_PLATFORM_REPOSITORY_INFO  * PlatformRepo;

  PlatformRepo = This->PlatRepoInfo;

  DEBUG ((DEBUG_INFO, "Lx2 Rev = 0x%x\n", PlatformRepo->Lx2160aRevision));
  return EFI_SUCCESS;
}

/** Return GIC CPU Interface Info.
  @param [in]      This           Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId     The Object ID of the CM object requested
  @param [in]      Token          A unique token for identifying the requested
  CM_ARM_GICC_INFO object.
  @param [in, out] CmObject       Pointer to the Configuration Manager Object
  descriptor describing the requested Object.
  @retval EFI_SUCCESS             Success.
  @retval EFI_INVALID_PARAMETER   A parameter is invalid.
  @retval EFI_NOT_FOUND           The required object information is
  not found.
 **/
EFI_STATUS
EFIAPI
GetGicCInfo (
    IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  * CONST This,
    IN  CONST CM_OBJECT_ID                                  CmObjectId,
    IN  CONST CM_OBJECT_TOKEN                               Token,
    IN  OUT   CM_OBJ_DESCRIPTOR                     * CONST CmObject
    )
{
  EDKII_PLATFORM_REPOSITORY_INFO  * PlatformRepo;

  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }

  PlatformRepo = This->PlatRepoInfo;

  if (Token != (CM_OBJECT_TOKEN)&PlatformRepo->GicCInfo) {
    return EFI_NOT_FOUND;
  }

  CmObject->ObjectId = CmObjectId;
  CmObject->Size = sizeof (PlatformRepo->GicCInfo);
  CmObject->Data = (VOID*)&PlatformRepo->GicCInfo;
  CmObject->Count = sizeof (PlatformRepo->GicCInfo) /
                      sizeof (PlatformRepo->GicCInfo[0]);
  return EFI_SUCCESS;
}

/** Return a GT Block timer frame info list.
  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       A token for identifying the object
  @param [in, out] CmObject    Pointer to the Configuration Manager Object
  descriptor describing the requested Object.
  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
 **/
EFI_STATUS
  EFIAPI
GetGTBlockTimerFrameInfo (
    IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  * CONST This,
    IN  CONST CM_OBJECT_ID                                  CmObjectId,
    IN  CONST CM_OBJECT_TOKEN                               Token,
    IN  OUT   CM_OBJ_DESCRIPTOR                     * CONST CmObject
    )
{
  EDKII_PLATFORM_REPOSITORY_INFO  * PlatformRepo;

  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }

  PlatformRepo = This->PlatRepoInfo;

  if (Token != (CM_OBJECT_TOKEN)&PlatformRepo->GTBlock0TimerInfo) {
    return EFI_NOT_FOUND;
  }

  CmObject->ObjectId = CmObjectId;
  CmObject->Size = sizeof (PlatformRepo->GTBlock0TimerInfo);
  CmObject->Data = (VOID*)&PlatformRepo->GTBlock0TimerInfo;
  CmObject->Count = sizeof (PlatformRepo->GTBlock0TimerInfo) /
                      sizeof (PlatformRepo->GTBlock0TimerInfo[0]);
  return EFI_SUCCESS;
}

/** Return a standard namespace object.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       An optional token identifying the object. If
                               unused this must be CM_NULL_TOKEN.
  @param [in, out] CmObject    Pointer to the Configuration Manager Object
                               descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
GetStandardNameSpaceObject (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  * CONST This,
  IN  CONST CM_OBJECT_ID                                  CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                               Token OPTIONAL,
  IN  OUT   CM_OBJ_DESCRIPTOR                     * CONST CmObject
  )
{
  EFI_STATUS                        Status;
  EDKII_PLATFORM_REPOSITORY_INFO  * PlatformRepo;
  UINT32                            TableCount;

  Status = EFI_SUCCESS;
  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }
  PlatformRepo = This->PlatRepoInfo;

  switch (GET_CM_OBJECT_ID (CmObjectId)) {
    HANDLE_CM_OBJECT (
      EStdObjCfgMgrInfo,
      CmObjectId,
      PlatformRepo->CmInfo,
      1
      );

    case EStdObjAcpiTableList:
      CmObject->ObjectId = CmObjectId;
      TableCount = sizeof (PlatformRepo->CmAcpiTableList) /
                     sizeof (PlatformRepo->CmAcpiTableList[0]);
      CmObject->Data = (VOID*)&PlatformRepo->CmAcpiTableList;
      CmObject->Size = sizeof (PlatformRepo->CmAcpiTableList);
      CmObject->Count = TableCount;

      DEBUG ((
        DEBUG_INFO,
        "EStdObjAcpiTableList: Ptr = 0x%p. Size = %d, Count = %d\n",
        CmObject->Data,
        CmObject->Size,
        CmObject->Count
        ));
      break;

    default: {
      Status = EFI_NOT_FOUND;
      DEBUG ((
        DEBUG_ERROR,
        "ERROR: Object 0x%x. Status = %r\n",
        CmObjectId,
        Status
        ));
      break;
    }
  }

  return Status;
}

/** Return an ARM namespace object.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       An optional token identifying the object. If
                               unused this must be CM_NULL_TOKEN.
  @param [in, out] CmObject    Pointer to the Configuration Manager Object
                               descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
GetArmNameSpaceObject (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  * CONST This,
  IN  CONST CM_OBJECT_ID                                  CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                               Token OPTIONAL,
  IN  OUT   CM_OBJ_DESCRIPTOR                     * CONST CmObject
  )
{
  EFI_STATUS                        Status;
  EDKII_PLATFORM_REPOSITORY_INFO  * PlatformRepo;

  Status = EFI_SUCCESS;
  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }
  PlatformRepo = This->PlatRepoInfo;

  switch (GET_CM_OBJECT_ID (CmObjectId)) {
    HANDLE_CM_OBJECT (
      EArmObjBootArchInfo,
      CmObjectId,
      PlatformRepo->BootArchInfo,
      1
      );
    HANDLE_CM_OBJECT (
      EArmObjPowerManagementProfileInfo,
      CmObjectId,
      PlatformRepo->PmProfileInfo,
      1
      );
    HANDLE_CM_OBJECT (
        EArmObjGenericTimerInfo,
        CmObjectId,
        PlatformRepo->GenericTimerInfo,
        1
        );
    HANDLE_CM_OBJECT (
        EArmObjPlatformGenericWatchdogInfo,
        CmObjectId,
        PlatformRepo->Watchdog,
        1
        );
    HANDLE_CM_OBJECT (
        EArmObjPlatformGTBlockInfo,
        CmObjectId,
        PlatformRepo->GTBlockInfo,
        (sizeof (PlatformRepo->GTBlockInfo) /
         sizeof (PlatformRepo->GTBlockInfo[0]))
        );
    HANDLE_CM_OBJECT_REF_BY_TOKEN (
        EArmObjGTBlockTimerFrameInfo,
        CmObjectId,
        PlatformRepo->GTBlock0TimerInfo,
        (sizeof (PlatformRepo->GTBlock0TimerInfo) /
         sizeof (PlatformRepo->GTBlock0TimerInfo[0])),
        Token,
        GetGTBlockTimerFrameInfo
        );
    HANDLE_CM_OBJECT_REF_BY_TOKEN (
        EArmObjGicCInfo,
        CmObjectId,
        PlatformRepo->GicCInfo,
        ARRAY_SIZE (PlatformRepo->GicCInfo),
        Token,
        GetGicCInfo
        );
    HANDLE_CM_OBJECT (
        EArmObjGicDInfo,
        CmObjectId,
        PlatformRepo->GicDInfo,
        1
        );
    HANDLE_CM_OBJECT (
        EArmObjGicRedistributorInfo,
        CmObjectId,
        PlatformRepo->GicRedistInfo,
        1
        );
    HANDLE_CM_OBJECT (
        EArmObjGicItsInfo,
        CmObjectId,
        PlatformRepo->GicItsInfo,
        1
        );
    default: {
      Status = EFI_NOT_FOUND;
      DEBUG ((
        DEBUG_INFO,
        "INFO: Object 0x%x. Status = %r\n",
        CmObjectId,
        Status
        ));
      break;
    }
  }//switch

  return Status;
}

/** Return an OEM namespace object.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       An optional token identifying the object. If
                               unused this must be CM_NULL_TOKEN.
  @param [in, out] CmObject    Pointer to the Configuration Manager Object
                               descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
GetOemNameSpaceObject (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  * CONST This,
  IN  CONST CM_OBJECT_ID                                  CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                               Token OPTIONAL,
  IN  OUT   CM_OBJ_DESCRIPTOR                     * CONST CmObject
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;
  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }

  switch (GET_CM_OBJECT_ID (CmObjectId)) {
    default: {
      Status = EFI_NOT_FOUND;
      DEBUG ((
        DEBUG_ERROR,
        "ERROR: Object 0x%x. Status = %r\n",
        CmObjectId,
        Status
        ));
      break;
    }
  }

  return Status;
}

/** The GetObject function defines the interface implemented by the
    Configuration Manager Protocol for returning the Configuration
    Manager Objects.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       An optional token identifying the object. If
                               unused this must be CM_NULL_TOKEN.
  @param [in, out] CmObject    Pointer to the Configuration Manager Object
                               descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
Lx2160aPlatformGetObject (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  * CONST This,
  IN  CONST CM_OBJECT_ID                                  CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                               Token OPTIONAL,
  IN  OUT   CM_OBJ_DESCRIPTOR                     * CONST CmObject
  )
{
  EFI_STATUS  Status;

  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }

  switch (GET_CM_NAMESPACE_ID (CmObjectId)) {
    case EObjNameSpaceStandard:
      Status = GetStandardNameSpaceObject (This, CmObjectId, Token, CmObject);
      break;
    case EObjNameSpaceArm:
      Status = GetArmNameSpaceObject (This, CmObjectId, Token, CmObject);
      break;
    case EObjNameSpaceOem:
      Status = GetOemNameSpaceObject (This, CmObjectId, Token, CmObject);
      break;
    default: {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((
        DEBUG_ERROR,
        "ERROR: Unknown Namespace Object = 0x%x. Status = %r\n",
        CmObjectId,
        Status
        ));
      break;
    }
  }

  return Status;
}

/** The SetObject function defines the interface implemented by the
    Configuration Manager Protocol for updating the Configuration
    Manager Objects.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       An optional token identifying the object. If
                               unused this must be CM_NULL_TOKEN.
  @param [in]      CmObject    Pointer to the Configuration Manager Object
                               descriptor describing the Object.

  @retval EFI_UNSUPPORTED  This operation is not supported.
**/
EFI_STATUS
EFIAPI
Lx2160aPlatformSetObject (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  * CONST This,
  IN  CONST CM_OBJECT_ID                                  CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                               Token OPTIONAL,
  IN        CM_OBJ_DESCRIPTOR                     * CONST CmObject
  )
{
  return EFI_UNSUPPORTED;
}

/** A structure describing the configuration manager protocol interface.
*/
STATIC
CONST
EDKII_CONFIGURATION_MANAGER_PROTOCOL Lx2160aConfigManagerProtocol = {
  CREATE_REVISION (1, 0),
  Lx2160aPlatformGetObject,
  Lx2160aPlatformSetObject,
  &Lx2160aPlatformRepositoryInfo
};

/**
  Entrypoint of Configuration Manager Dxe.

  @param  ImageHandle
  @param  SystemTable

  @return EFI_SUCCESS
  @return EFI_LOAD_ERROR
  @return EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
EFIAPI
ConfigurationManagerDxeInitialize (
  IN EFI_HANDLE          ImageHandle,
  IN EFI_SYSTEM_TABLE  * SystemTable
  )
{
  EFI_STATUS  Status;

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gEdkiiConfigurationManagerProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  (VOID*)&Lx2160aConfigManagerProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "ERROR: Failed to get Install Configuration Manager Protocol." \
      " Status = %r\n",
      Status
      ));
    goto error_handler;
  }

  Status = InitializePlatformRepository (
    &Lx2160aConfigManagerProtocol
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "ERROR: Failed to initialize the Platform Configuration Repository." \
      " Status = %r\n",
      Status
      ));
  }

error_handler:
  return Status;
}
