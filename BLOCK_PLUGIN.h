// ZLKJ_RHPlugIn.h : main header file for the ZLKJ_RH plug-in.
//
//#include "ZLKJ_LOGO_EVENT_WATCHER.h"
#include "EDITOR_FORM.h"
#include "EVENT_WATCHER.h"
#pragma once

// CZLKJ_RHPlugIn
// See ZLKJ_RHPlugIn.cpp for the implementation of this class
//
enum EDIT_TYPE
{
	NORMAL,
	RETURN,
	END
};


class BLOCK_PLUGIN : public CRhinoUtilityPlugIn
{
public:
  // CZLKJ_RHPlugIn constructor. The constructor is called when the
  // plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  // is loaded, CZLKJ_RHPlugIn::OnLoadPlugIn() is called. The
  // constructor should be simple and solid. Do anything that might fail in
  // CZLKJ_RHPlugIn::OnLoadPlugIn().
	BLOCK_PLUGIN();
  
  // CZLKJ_RHPlugIn destructor. The destructor is called to destroy
  // "thePlugIn" when the plug-in is unloaded. Immediately before the
  // DLL is unloaded, CZLKJ_RHPlugIn::OnUnloadPlugin() is called. Do
  // not do too much here. Be sure to clean up any memory you have allocated
  // with onmalloc(), onrealloc(), oncalloc(), or onstrdup().
  ~BLOCK_PLUGIN() = default;

  // Required overrides
  
  // Plug-in name display string. This name is displayed by Rhino when
  // loading the plug-in, in the plug-in help menu, and in the Rhino
  // interface for managing plug-ins. 
  const wchar_t* PlugInName() const override;
  
  // Plug-in version display string. This name is displayed by Rhino
  // when loading the plug-in and in the Rhino interface for 
  // managing plug-ins.
  const wchar_t* PlugInVersion() const override;
  
  // Plug-in unique identifier. The identifier is used by Rhino for
  // managing plug-ins.
  GUID PlugInID() const override;
  
  // Additional overrides
  
  // Called after the plug-in is loaded and the constructor has been
  // run. This is a good place to perform any significant initialization,
  // license checking, and so on.  This function must return TRUE for
  // the plug-in to continue to load.  
  BOOL OnLoadPlugIn() override;
  
  // Called one time when plug-in is about to be unloaded. By this time,
  // Rhino's mainframe window has been destroyed, and some of the SDK
  // managers have been deleted. There is also no active document or active
  // view at this time. Thus, you should only be manipulating your own objects.
  // or tools here.  
  void OnUnloadPlugIn() override;
  bool DisplayDlg();
  void DestroyDlg();

public:
	EDITOR_FORM* m_dialog;
	EDIT_TYPE CTYPE;
	CSampleRhinoEventWatcher m_watcher;
	ON_SimpleArray<UUID> EDIT_INS_LIST;
	ON_SimpleArray<int> EDIT_INS_SERIAL;

	ON_SimpleArray<UUID> EDIT_TREE_LIST;
	ON_SimpleArray<int> EDIT_TREE_SERIAL;
	ON_SimpleArray<bool> EDIT_TREE_BOOL;

	ON_SimpleArray<int> EDIT_LOCKED_COUNT;
	ON_SimpleArray<UUID> EDIT_LOCKED_LIST;
	ON_SimpleArray<int> EDIT_LOCKED_SERIAL;

	bool EIDT_TREE_MODE;
	CRhinoDoc* m_doc;
	UUID m_root_block;
private:
  ON_wString m_plugin_version;

public:
	void APEND_INS_LIST(UUID,int);
	void APEND_TREE_LIST(UUID,bool c,int);
	void APEND_LOCKED_LIST(UUID,int);

	void REMOVE_INS_LIST(int);
	void REMOVE_TREE_LIST(int);
	void REMOVE_LOCKED_LIST(int);

	void EMPTY_INS_LIST();
	void EMPTY_TREE_LIST();
	void EMPTY_LOCKED_LIST();
 /* ZLKJ_LOGO_EVENT_WATCHER m_watcher;
  ZLKJ_SECTION_EVENT_WATCHER m_SEC_watcher;*/
  // TODO: Add additional class information here
};

// Return a reference to the one and only CZLKJ_RHPlugIn object
BLOCK_PLUGIN& BLOCKPlugIn();



