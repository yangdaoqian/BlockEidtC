// ZLKJ_RH.h : main header file for the ZLKJ_RH DLL.
//

#pragma once

#ifndef __AFXWIN_H__
	//#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h" // main symbols
#include "stdafx.h"
// CZLKJ_RHApp
// See ZLKJ_RHApp.cpp for the implementation of this class
//

class CZLKJ_RH_APP : public CWinApp
{
public:
  // CRITICAL: DO NOT CALL RHINO SDK FUNCTIONS HERE!
  // Only standard MFC DLL instance construction belongs here. 
  // All other significant initialization should take place in
  // CZLKJ_RHPlugIn::OnLoadPlugIn().
	CZLKJ_RH_APP() = default;

  // CRITICAL: DO NOT CALL RHINO SDK FUNCTIONS HERE!
  // Only standard MFC DLL instance initialization belongs here. 
  // All other significant initialization should take place in
  // CZLKJ_RHPlugIn::OnLoadPlugIn().
	BOOL InitInstance() override;
  
  // CRITICAL: DO NOT CALL RHINO SDK FUNCTIONS HERE!
  // Only standard MFC DLL instance clean up belongs here. 
  // All other significant cleanup should take place in either
  // CZLKJ_RHPlugIn::OnSaveAllSettings() or
  // CZLKJ_RHPlugIn::OnUnloadPlugIn().  
	int ExitInstance() override;
  
	DECLARE_MESSAGE_MAP()
};
