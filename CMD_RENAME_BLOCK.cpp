#include "StdAfx.h"
#include "BLOCK_PLUGIN.h"
//#include "ZLKJ_GET_PLANE.h"
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMarker command
//

#pragma region SampleMarker command

class COMMAND_ZLKJ_BLOCK_RENAME : public CRhinoCommand
{
public:
	COMMAND_ZLKJ_BLOCK_RENAME() = default;
	~COMMAND_ZLKJ_BLOCK_RENAME() = default;
	UUID CommandUUID() override
	{
		//{493a6734-2000-7e32-abb5-f8e7c9d4b1b2}
		static const GUID SampleMarkerCommand_UUID =
		{ 0x493a6734, 0x2000, 0x7e32, { 0xab, 0xb5, 0xf8, 0xe7, 0xc9, 0xd4, 0xb1, 0xb2 } };
		return SampleMarkerCommand_UUID;
	}
	const wchar_t* EnglishCommandName() override { return L"RenameBlockEX"; }
	CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleMarker object
static class COMMAND_ZLKJ_BLOCK_RENAME theSampleMarkerCommand;

CRhinoCommand::result COMMAND_ZLKJ_BLOCK_RENAME::RunCommand(const CRhinoCommandContext& context)
{
	const CRhinoInstanceDefinition* idef0 = 0;
	if (BLOCKPlugIn().m_dialog)
	{
		CRhinoDoc* doc = RhinoApp().ActiveDoc();
		if (doc)
		{
			if (BLOCKPlugIn().EDIT_TREE_LIST.Count() > 0)
			{
				/// <summary>
				/// MODIFY LAST
				/// </summary>
				CRhinoObjRef OR(*(BLOCKPlugIn().EDIT_TREE_LIST.Last()));
				const CRhinoObject* obj0 = OR.Object();
				if (obj0)
				{
					const CRhinoInstanceObject* iref0 = CRhinoInstanceObject::Cast(obj0);
					idef0 = iref0->InstanceDefinition();
				}
			}
		}
	}
	else
	{
		CRhinoGetObject go;
		go.SetCommandPrompt(L"Select blocks to Rename");
		go.SetGeometryFilter(CRhinoGetObject::instance_reference);
		go.GetObjects(0, 0);
		if (go.CommandResult() != CRhinoCommand::success)
			return CRhinoCommand::nothing;
		for (int i = 0; i < go.ObjectCount(); i++)
		{
			const CRhinoInstanceObject* iref = CRhinoInstanceObject::Cast(go.Object(i).Object());
			if (iref)
			{
				idef0= iref->InstanceDefinition();
			}
		}
	}
	if (idef0)
	{
		CRhinoGetString go1;
		go1.SetCommandPrompt(L"Set block new name");
		go1.SetDefaultString(idef0->Name());
		go1.GetString();
		if (go1.CommandResult() != CRhinoCommand::success)
			return CRhinoCommand::nothing;
		if (go1.String() != idef0->Name())
		{
			CRhinoInstanceDefinitionTable& idef_table = context.m_doc.m_instance_definition_table;
			idef_table.SetName(idef0->Index(), go1.String());
			BLOCKPlugIn().m_dialog->update_tree();
		}
	}
	return CRhinoCommand::success;
}