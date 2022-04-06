#include "StdAfx.h"
#include "EDITOR_FORM.h"
#include "BLOCK_PLUGIN.h"
//#include "ZLKJ_KEEL_OBJECT.h"
//#include "ZLKJ_GET_PLANE.h"
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMarker command
//

#pragma region SampleMarker command

class COMMAND_BLOCK_EDIT : public CRhinoCommand
{
public:
	COMMAND_BLOCK_EDIT() = default;
	~COMMAND_BLOCK_EDIT() = default;

	UUID CommandUUID() override
	{
		//{8a0ee02d-dc59-4130-a78b-3484d819892c}
		static const GUID SampleMarkerCommand_UUID =
		{ 0x8a0ee02d, 0xdc59, 0x4130, { 0xa7, 0x8b, 0x34, 0x84, 0xd8, 0x19, 0x89, 0x2c } };
		return SampleMarkerCommand_UUID;
	}
	const wchar_t* EnglishCommandName() override { return L"BlockEdit"; }
	CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

};

// The one and only CCommandSampleMarker object
static class COMMAND_BLOCK_EDIT theSampleMarkerCommand;

CRhinoCommand::result COMMAND_BLOCK_EDIT::RunCommand(const CRhinoCommandContext& context)
{
	CRhinoDoc* doc = RhinoApp().ActiveDoc();
	switch (BLOCKPlugIn().CTYPE)
	{
	case EDIT_TYPE::NORMAL:
	{
		BLOCKPlugIn().m_dialog->DO_EDIT(doc);
	}
	break;
	case EDIT_TYPE::RETURN:
	{
		BLOCKPlugIn().m_watcher.Enable(false);
		if (doc)
		{
			if (BLOCKPlugIn().EDIT_TREE_LIST.Count() > 0)
			{
				BLOCKPlugIn().m_dialog->CONTINUE_EDIT(doc);
			}
			else
			{
				BLOCKPlugIn().DestroyDlg();
			}
		}
	}
	break;
	case EDIT_TYPE::END:
	{
		BLOCKPlugIn().m_dialog->END_EDIT(doc);
		BLOCKPlugIn().DestroyDlg();
	}
	break;
	}
	BLOCKPlugIn().CTYPE = EDIT_TYPE::NORMAL;
	doc->Redraw();
	return CRhinoCommand::success;
}
