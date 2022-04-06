// ZLKJ_RHPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "rhinoSdkPlugInDeclare.h"
#include "BLOCK_PLUGIN.h"
#include "Resource.h"

// The plug-in object must be constructed before any plug-in classes derived
// from CRhinoCommand. The #pragma init_seg(lib) ensures that this happens.
#pragma warning(push)
#pragma warning(disable : 4073)
#pragma init_seg(lib)
#pragma warning(pop)

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME(L"BlockEditEX");

// Rhino plug-in id
// Provide a unique uuid for this plug-in.
RHINO_PLUG_IN_ID(L"940d350b-feae-43e2-90a8-ffa44ea8bdc2");

// Rhino plug-in version
// Provide a version number string for this plug-in.
RHINO_PLUG_IN_VERSION(__DATE__ "  " __TIME__)

// Rhino plug-in description
// Provide a description of this plug-in.
RHINO_PLUG_IN_DESCRIPTION(L"BlockEditEX plug-in for Rhinoceros®");

// Rhino plug-in icon resource id
// Provide an icon resource this plug-in.
// Icon resource should contain 16, 24, 32, 48, and 256-pixel image sizes.
RHINO_PLUG_IN_ICON_RESOURCE_ID(IDI_ICON);

// Rhino plug-in developer declarations
// TODO: fill in the following developer declarations with
// your company information. Note, all of these declarations
// must be present or your plug-in will not load.
//
// When completed, delete the following #error directive.
//#error Developer declarations block is incomplete!
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"PANDA");
RHINO_PLUG_IN_DEVELOPER_ADDRESS(L"123 Developer Street\r\nCity State 12345-6789");
RHINO_PLUG_IN_DEVELOPER_COUNTRY(L"China");
RHINO_PLUG_IN_DEVELOPER_PHONE(L"123.456.7890");
RHINO_PLUG_IN_DEVELOPER_FAX(L"123.456.7891");
RHINO_PLUG_IN_DEVELOPER_EMAIL(L"panda_architect@outlook.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE(L"http://www.mycompany.com");
RHINO_PLUG_IN_UPDATE_URL(L"http://www.mycompany.com/support");

// The one and only CZLKJ_RHPlugIn object
static class BLOCK_PLUGIN thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CZLKJ_RHPlugIn definition

BLOCK_PLUGIN& BLOCKPlugIn()
{
	//Return a reference to the one and only CZLKJ_RHPlugIn object

	return thePlugIn;
}

BLOCK_PLUGIN::BLOCK_PLUGIN()
{
	// Description:
	//   CZLKJ_RHPlugIn constructor. The constructor is called when the
	//   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
	//   is loaded, CZLKJ_RHPlugIn::OnLoadPlugIn() is called. The
	//   constructor should be simple and solid. Do anything that might fail in
	//   CZLKJ_RHPlugIn::OnLoadPlugIn().
	// TODO: Add construction code here
	m_doc=RhinoApp().ActiveDoc();
	m_watcher.Register();
	m_plugin_version = RhinoPlugInVersion();
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* BLOCK_PLUGIN::PlugInName() const
{
	// Description:
	//   Plug-in name display string.  This name is displayed by Rhino when
	//   loading the plug-in, in the plug-in help menu, and in the Rhino
	//   interface for managing plug-ins.
	// TODO: Return a short, friendly name for the plug-in.
	return RhinoPlugInName();
}

const wchar_t* BLOCK_PLUGIN::PlugInVersion() const
{
	// Description:
	//   Plug-in version display string. This name is displayed by Rhino
	//   when loading the plug-in and in the Rhino interface for managing
	//   plug-ins.

	// TODO: Return the version number of the plug-in.
	return m_plugin_version;
}

GUID BLOCK_PLUGIN::PlugInID() const
{
	// Description:
	//   Plug-in unique identifier. The identifier is used by Rhino to
	//   manage the plug-ins.

	// TODO: Return a unique identifier for the plug-in.
	// {EF89CFCE-9781-4C20-847A-FF09C6F8AFCA}
	



	return ON_UuidFromString(RhinoPlugInId());
}

/////////////////////////////////////////////////////////////////////////////
// Additional overrides

BOOL BLOCK_PLUGIN::OnLoadPlugIn()
{
	// Description:
	//   Called after the plug-in is loaded and the constructor has been
	//   run. This is a good place to perform any significant initialization,
	//   license checking, and so on.  This function must return TRUE for
	//   the plug-in to continue to load.

	// Remarks:
	//    Plug-ins are not loaded until after Rhino is started and a default document
	//    is created.  Because the default document already exists
	//    CRhinoEventWatcher::On????Document() functions are not called for the default
	//    document.  If you need to do any document initialization/synchronization then
	//    override this function and do it here.  It is not necessary to call
	//    CPlugIn::OnLoadPlugIn() from your derived class.

	// TODO: Add plug-in initialization code here.
	//m_watcher.Register();
	//m_watcher.Enable(TRUE);

	//m_SEC_watcher.Register();
	//m_SEC_watcher.Enable(TRUE);
	BLOCKPlugIn().CTYPE = EDIT_TYPE::NORMAL;
	return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void BLOCK_PLUGIN::OnUnloadPlugIn()
{

	// Description:
	//    Called one time when plug-in is about to be unloaded. By this time,
	//    Rhino's mainframe window has been destroyed, and some of the SDK
	//    managers have been deleted. There is also no active document or active
	//    view at this time. Thus, you should only be manipulating your own objects.
	//    or tools here.

	// TODO: Add plug-in cleanup code here.
}
bool BLOCK_PLUGIN::DisplayDlg()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_dialog)
		DestroyDlg();
	m_dialog = new EDITOR_FORM();
	
	if (m_dialog->Create(IDD_EDITOR, CWnd::FromHandle(RhinoApp().MainWnd())))
	{

		//CWnd* w = CWnd::FromHandle();
		//RECT E;
		//E.left = 0;
		//E.right = 500;
		//E.top = 0;
		//E.bottom = 500;
		//w->GetWindowRect(&E);
		ON_4iRect& rec= ON_4iRect();
	
		RhinoApp().ActiveDoc()->ActiveView()->GetWindowRect(rec);
		m_dialog->ShowWindow(SW_SHOW);
	    const	ON_3dmViewPosition p=RhinoApp().ActiveDoc()->ActiveView()->Position();
		//m_dialog->SetWindowPos(&CWnd::wndTopMost, E.left+200, E.top+200, 0, 0, 1);
		m_dialog->SetWindowPos(&CWnd::wndTop, rec.left + 50,rec.top + 50, 0, 0, 1);
		m_dialog->UpdateWindow();
		
		CRhinoObjectIterator it(CRhinoObjectIterator::normal_objects, CRhinoObjectIterator::active_and_reference_objects);
		it.IncludeLights(true);
		it.IncludeGrips(false);
		const CRhinoObject* obj = 0;
		ON_wString str;
		for (obj = it.First(); obj; obj = it.Next())
		{
			BLOCKPlugIn().APEND_LOCKED_LIST(obj->Id(),-1);
			CRhinoDoc* doc = RhinoApp().ActiveDoc();
			if(doc)
			doc->LockObject(obj, true);
		}

		return true;
	}

	return false;
}
void BLOCK_PLUGIN::APEND_INS_LIST(UUID ID,int s)
{
	if (EDIT_INS_LIST.Search(ID) < 0)
	{
		EDIT_INS_SERIAL.Append(s);
		EDIT_INS_LIST.Append(ID);
	}
}
//void BLOCK_PLUGIN::APEND_B_LIST(UUID ID)
//{
//	if (TEMP_B_LIST.Search(ID) < 0)
//		TEMP_B_LIST.Append(ID);
//}
void BLOCK_PLUGIN::APEND_TREE_LIST(UUID ID,bool c,int s)
{
	if (EDIT_TREE_LIST.Search(ID) < 0)
	{
		EDIT_TREE_SERIAL.Append(s);
		EDIT_TREE_LIST.Append(ID);
	}
	if (EDIT_TREE_BOOL.Search(c) < 0)
		EDIT_TREE_BOOL.Append(c);
}
void BLOCK_PLUGIN::APEND_LOCKED_LIST(UUID ID,int i)
{
	//if (EDIT_LOCKED_LIST.Search(ID) < 0)
		EDIT_LOCKED_LIST.Append(ID);
		EDIT_LOCKED_SERIAL.Append(i);
}

void BLOCK_PLUGIN::REMOVE_INS_LIST(int I)
{
	if (I < 0)
	{
		EDIT_INS_LIST.Remove();
		EDIT_INS_SERIAL.Remove();
	}
	else
	{
		EDIT_INS_LIST.Remove(I);
		EDIT_INS_SERIAL.Remove(I);
	}
}
void BLOCK_PLUGIN::REMOVE_TREE_LIST(int I)
{
	if (I < 0)
	{
		EDIT_TREE_LIST.Remove();
		EDIT_TREE_BOOL.Remove();
		EDIT_TREE_SERIAL.Remove();
	}
	else
	{
		EDIT_TREE_LIST.Remove(I);
		EDIT_TREE_BOOL.Remove(I);
		EDIT_TREE_SERIAL.Remove(I);
	}
}
void BLOCK_PLUGIN::REMOVE_LOCKED_LIST(int I)
{
	if (I < 0)
	{
		EDIT_LOCKED_LIST.Remove();
		EDIT_LOCKED_SERIAL.Remove();
	}
	else
	{
		EDIT_LOCKED_LIST.Remove(I);
		EDIT_LOCKED_SERIAL.Remove(I);
	}
}

void BLOCK_PLUGIN::EMPTY_INS_LIST()
{
	EDIT_INS_LIST.Empty();
	EDIT_INS_SERIAL.Empty();
}
void BLOCK_PLUGIN::EMPTY_TREE_LIST()
{
	EDIT_TREE_LIST.Empty();
	EDIT_TREE_BOOL.Empty();
	EDIT_TREE_SERIAL.Empty();
}
void BLOCK_PLUGIN::EMPTY_LOCKED_LIST()
{
	EDIT_LOCKED_LIST.Empty();
	EDIT_LOCKED_COUNT.Empty();
	EDIT_LOCKED_SERIAL.Empty();
}

void BLOCK_PLUGIN::DestroyDlg()
{
	m_dialog->KillDialog();
	m_dialog = 0;
}
