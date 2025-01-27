// Berkeley Open Infrastructure for Network Computing
// http://boinc.berkeley.edu
// Copyright (C) 2014 University of California
//
// This is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation;
// either version 2.1 of the License, or (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// To view the GNU Lesser General Public License visit
// http://www.gnu.org/copyleft/lesser.html
// or write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "stdafx.h"
#include "boinccas.h"
#include "CARevokeBOINCProjectsRights.h"
#include "lsaprivs.h"


#define CUSTOMACTION_NAME               _T("CARevokeBOINCProjectsRights")
#define CUSTOMACTION_PROGRESSTITLE      _T("Validating the BOINC Projects group privilege levels")


/////////////////////////////////////////////////////////////////////
//
// Function:
//
// Description:
//
/////////////////////////////////////////////////////////////////////
CARevokeBOINCProjectsRights::CARevokeBOINCProjectsRights(MSIHANDLE hMSIHandle) :
    BOINCCABase(hMSIHandle, CUSTOMACTION_NAME, CUSTOMACTION_PROGRESSTITLE)
{}


/////////////////////////////////////////////////////////////////////
//
// Function:
//
// Description:
//
/////////////////////////////////////////////////////////////////////
CARevokeBOINCProjectsRights::~CARevokeBOINCProjectsRights()
{
    BOINCCABase::~BOINCCABase();
}


/////////////////////////////////////////////////////////////////////
//
// Function:
//
// Description:
//
/////////////////////////////////////////////////////////////////////
UINT CARevokeBOINCProjectsRights::OnExecution()
{
    PSID        pSid;

	//
    // Obtain the SID of the user/group.
    // Note that we could target a specific machine, but we don't.
    // Specifying NULL for target machine searches for the SID in the
    // following order: well-known, Built-in and local, primary domain,
    // trusted domains.
    //
    if(
        GetAccountSid(
            NULL,                                    // default lookup logic
            L"boinc_projects",                       // account to obtain SID
            &pSid                                    // buffer to allocate to contain resultant SID
            )
    )
    {

        //
        // We only grant the privilege if we succeeded in obtaining the
        // SID. We can actually add SIDs which cannot be looked up, but
        // looking up the SID is a good sanity check which is suitable for
        // most cases.

        // User Rights
        GrantUserRight(pSid, L"SeNetworkLogonRight", FALSE);
        GrantUserRight(pSid, L"SeRemoteInteractiveLogonRight", FALSE);
        GrantUserRight(pSid, L"SeBatchLogonRight", FALSE);
        GrantUserRight(pSid, L"SeInteractiveLogonRight", FALSE);
        GrantUserRight(pSid, L"SeServiceLogonRight", FALSE);
        GrantUserRight(pSid, L"SeDenyNetworkLogonRight", FALSE);
        GrantUserRight(pSid, L"SeDenyInteractiveLogonRight", FALSE);
        GrantUserRight(pSid, L"SeDenyBatchLogonRight", FALSE);
        GrantUserRight(pSid, L"SeDenyServiceLogonRight", FALSE);
        GrantUserRight(pSid, L"SeDenyRemoteInteractiveLogonRight", FALSE);

        // Privileges
        GrantUserRight(pSid, L"SeTcbPrivilege", FALSE);
        GrantUserRight(pSid, L"SeMachineAccountPrivilege", FALSE);
        GrantUserRight(pSid, L"SeIncreaseQuotaPrivilege", FALSE);
        GrantUserRight(pSid, L"SeBackupPrivilege", FALSE);
        GrantUserRight(pSid, L"SeChangeNotifyPrivilege", FALSE);
        GrantUserRight(pSid, L"SeSystemTimePrivilege", FALSE);
        GrantUserRight(pSid, L"SeCreateTokenPrivilege", FALSE);
        GrantUserRight(pSid, L"SeCreatePagefilePrivilege", FALSE);
        GrantUserRight(pSid, L"SeCreateGlobalPrivilege", FALSE);
        GrantUserRight(pSid, L"SeDebugPrivilege", FALSE);
        GrantUserRight(pSid, L"SeEnableDelegationPrivilege", FALSE);
        GrantUserRight(pSid, L"SeRemoteShutdownPrivilege", FALSE);
        GrantUserRight(pSid, L"SeAuditPrivilege", FALSE);
        GrantUserRight(pSid, L"SeImpersonatePrivilege", FALSE);
        GrantUserRight(pSid, L"SeIncreaseBasePriorityPrivilege", FALSE);
        GrantUserRight(pSid, L"SeLoadDriverPrivilege", FALSE);
        GrantUserRight(pSid, L"SeLockMemoryPrivilege", FALSE);
        GrantUserRight(pSid, L"SeSecurityPrivilege", FALSE);
        GrantUserRight(pSid, L"SeSystemEnvironmentPrivilege", FALSE);
        GrantUserRight(pSid, L"SeManageVolumePrivilege", FALSE);
        GrantUserRight(pSid, L"SeProfileSingleProcessPrivilege", FALSE);
        GrantUserRight(pSid, L"SeSystemProfilePrivilege", FALSE);
        GrantUserRight(pSid, L"SeUndockPrivilege", FALSE);
        GrantUserRight(pSid, L"SeAssignPrimaryTokenPrivilege", FALSE);
        GrantUserRight(pSid, L"SeRestorePrivilege", FALSE);
        GrantUserRight(pSid, L"SeShutdownPrivilege", FALSE);
        GrantUserRight(pSid, L"SeSynchAgentPrivilege", FALSE);
        GrantUserRight(pSid, L"SeTakeOwnershipPrivilege", FALSE);
    }
    else
    {
        LogMessage(
            INSTALLMESSAGE_ERROR,
            NULL,
            NULL,
            NULL,
            NULL,
            _T("Failed to be able to obtain the SID for the selected user on the localhost")
        );
        return ERROR_INSTALL_FAILURE;
    }


    //
    // Cleanup any handles and memory allocated during the custom action
    //
    if(pSid != NULL) HeapFree(GetProcessHeap(), 0, pSid);

    return ERROR_SUCCESS;
}


/////////////////////////////////////////////////////////////////////
//
// Function:    RevokeBOINCProjectsRights
//
// Description: This custom action grants the 'boinc_projects' group the
//              required rights.
//
/////////////////////////////////////////////////////////////////////
UINT __stdcall RevokeBOINCProjectsRights(MSIHANDLE hInstall)
{
    UINT uiReturnValue = 0;

    CARevokeBOINCProjectsRights* pCA = new CARevokeBOINCProjectsRights(hInstall);
    uiReturnValue = pCA->Execute();
    delete pCA;

    return uiReturnValue;
}
