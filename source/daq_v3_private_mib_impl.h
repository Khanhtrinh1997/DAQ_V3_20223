/**
 * @file private_mib_impl.h
 * @brief Private MIB module implementation
 *
 * @section License
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Copyright (C) 2010-2022 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Open.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 2.1.4
 **/

#ifndef _PRIVATE_MIB_IMPL_H
#define _PRIVATE_MIB_IMPL_H

//Dependencies
#include "mibs/mib_common.h"

//Private MIB related functions
error_t privateMibInit(void);
error_t privateMibLoad(void *context);
void privateMibUnload(void *context);
void privateMibLock(void);
void privateMibUnlock(void);

//========================================== SiteInfo Function ==========================================//
error_t privateMibSetSiteInfoGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibGetSiteInfoGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);
//power system function ======================================================================================
//alarm info function-----------------------------------------------------------
error_t privateMibGetConnAlarmGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);
error_t privateMibGetDIAlarmGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);
error_t privateMibGetMainAlarmGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);
//rect info fuction-------------------------------------------------------------
error_t privateMibGetRectInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);
error_t privateMibGetNextRectInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);
//ac info fuction---------------------------------------------------------------
error_t privateMibGetAcPhaseEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, MibVariant *value, size_t *valueLen);
error_t privateMibGetNextAcPhaseEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);
//battery info function---------------------------------------------------------
error_t privateMibGetBatteryGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);
//load info function------------------------------------------------------------
error_t privateMibGetLoadGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);
//cfg info function-------------------------------------------------------------
error_t privateMibSetCfgBTSCUEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibGetCfgBTSCUEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);
error_t privateMibGetNextCfgBTSCUEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);

error_t privateMibSetCfgBTPlanTestEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);
error_t privateMibGetCfgBTPlanTestEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, MibVariant *value, size_t *valueLen);
error_t privateMibGetNextCfgBTPlanTestEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);

error_t privateMibSetCfgBTGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);
error_t privateMibGetCfgBTGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);

error_t privateMibSetConfigGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);
error_t privateMibGetConfigGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);
//slave vac info fuction=======================================================================================
error_t private_mib_set_vac_group(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);
error_t private_mib_set_vac_info_entry(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);
error_t private_mib_get_vac_info_entry(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);
error_t private_mib_get_next_vac_info_entry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);
//slave bm info function=======================================================================================
error_t privateMibSetBMInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibGetBMInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);
error_t privateMibGetNextBMInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);
//slave pm info function=======================================================================================
error_t privateMibSetPMGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibSetPMInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibGetPMInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, MibVariant *value, size_t *valueLen);
error_t privateMibGetNextPMInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);
//slave isense info function===================================================================================
error_t privateMibSetISENSEGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibSetISENSEInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibGetISENSEInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);
error_t privateMibGetNextISENSEInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);
//slave pm dc info function====================================================================================
error_t privateMibSet_pm_dc_Group(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibSet_pm_dc_InfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibGet_pm_dc_InfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);
error_t privateMibGetNext_pm_dc_InfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);
//slave smcb info function=====================================================================================
error_t privateMibSetSMCBGroup(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibSetSmcbInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibGetSmcbInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, MibVariant *value, size_t *valueLen);
error_t privateMibGetNextSmcbInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);
//slave fuel info fuction======================================================================================
error_t privateMibSetFUELGroup(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibSetFuelInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibGetFuelInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, MibVariant *value, size_t *valueLen);
error_t privateMibGetNextFuelInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);
//slave libatt info fuction====================================================================================
error_t privateMibSetLIBattGroup(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibSetLIBattInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibGetLIBattInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, MibVariant *value, size_t *valueLen);
error_t privateMibGetNextLIBattInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);
//slave gen info fuction=======================================================================================
error_t privateMibSetGenGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibSetGenInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
error_t privateMibGetGenInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, MibVariant *value, size_t *valueLen);
error_t privateMibGetNextGenInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);
//fan_dpc======================================================================================================
error_t privateMibGet_fan_dpc_Group(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);
error_t privateMibSet_fan_dpc_Group(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
//cnt task======================================================================================================
error_t privateMibGetCntGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen);
//error_t privateMibSetCntGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit);
#endif /*_PRIVATE_MIB_IMPL_H*/
