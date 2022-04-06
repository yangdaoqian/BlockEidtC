#include "StdAfx.h"
#include "BLOCK_PLUGIN.h"
//#include "ZLKJ_GET_PLANE.h"
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMarker command
//

#pragma region SampleMarker command

class COMMAND_ZLKJ_BLOCK_RAPLACE_OBJ : public CRhinoCommand
{
public:
	COMMAND_ZLKJ_BLOCK_RAPLACE_OBJ() = default;
	~COMMAND_ZLKJ_BLOCK_RAPLACE_OBJ() = default;
	UUID CommandUUID() override
	{
		//{b8a2f80a-215c-4c30-a563-077f5ef8a1e8}
		static const GUID SampleMarkerCommand_UUID =
		{ 0xb8a2f80a, 0x215c, 0x4c30, { 0xa5, 0x63, 0x07, 0x7f, 0x5e, 0xf8, 0xa1, 0xe8 } };
		return SampleMarkerCommand_UUID;
	}
	const wchar_t* EnglishCommandName() override { return L"ReplaceBlockGeometry"; }
	CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleMarker object
static class COMMAND_ZLKJ_BLOCK_RAPLACE_OBJ theSampleMarkerCommand;

CRhinoCommand::result COMMAND_ZLKJ_BLOCK_RAPLACE_OBJ::RunCommand(const CRhinoCommandContext& context)
{
	CRhinoGetObject go;
	go.SetCommandPrompt(L"Select block to make Replace Geometry");
	go.SetGeometryFilter(CRhinoGetObject::instance_reference);
	go.GetObjects(1, 1);
	if (go.CommandResult() != CRhinoCommand::success)
		return CRhinoCommand::nothing;
	CRhinoGetObject go1;
	go1.SetCommandPrompt(L"Select block as reference");
	go1.SetGeometryFilter(CRhinoGetObject::instance_reference);
	go1.GetObjects(1, 1);
	if (go1.CommandResult() != CRhinoCommand::success)
		return CRhinoCommand::nothing;
	const CRhinoInstanceObject* irefr = CRhinoInstanceObject::Cast(go.Object(0).Object());
	CRhinoInstanceDefinitionTable& idef_table = context.m_doc.m_instance_definition_table;

	const CRhinoInstanceObject* iref0 = CRhinoInstanceObject::Cast(go1.Object(0).Object());
	const CRhinoInstanceDefinition* idef0 = iref0->InstanceDefinition();
	ON_SimpleArray<const CRhinoObject*> idef_objects(idef0->ObjectCount());
	for (int j = 0; j < idef0->ObjectCount(); j++)
	{
		const CRhinoObject* obj = idef0->Object(j);
		if (obj)
		{
			if (obj->IsDeleted())
				continue;
			idef_objects.Append(obj);
		}
	}
	if (idef_table.ModifyInstanceDefinitionGeometry(irefr->InstanceDefinition()->Index(), idef_objects, true))
	{
		BLOCKPlugIn().m_dialog->update_tree();
    }
	return CRhinoCommand::success;
}