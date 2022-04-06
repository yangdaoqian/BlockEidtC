#include "StdAfx.h"
//#include "ZLKJ_KEEL_OBJECT.h"
//#include "ZLKJ_GET_PLANE.h"
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMarker command
//

#pragma region SampleMarker command

class COMMAND_ZLKJ_BLOCK_REPLACE : public CRhinoCommand
{
public:
	COMMAND_ZLKJ_BLOCK_REPLACE() = default;
	~COMMAND_ZLKJ_BLOCK_REPLACE() = default;
	UUID CommandUUID() override
	{
		//{44a0fd2a-e4f2-423f-ab67-705d8462da39}
		static const GUID SampleMarkerCommand_UUID =
		{ 0x44a0fd2a, 0xe4f2, 0x423f, { 0xab, 0x67, 0x70, 0x5d, 0x84, 0x62, 0xda, 0x39 } };
		return SampleMarkerCommand_UUID;
	}
	const wchar_t* EnglishCommandName() override { return L"ReplaceBlockEX"; }
	CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleMarker object
static class COMMAND_ZLKJ_BLOCK_REPLACE theSampleMarkerCommand;

CRhinoCommand::result COMMAND_ZLKJ_BLOCK_REPLACE::RunCommand(const CRhinoCommandContext& context)
{
	CRhinoGetObject go;
	go.SetCommandPrompt(L"Select blocks to Replace");
	go.SetGeometryFilter(CRhinoGetObject::instance_reference);
	go.GetObjects(0, 0);
	if (go.CommandResult() != CRhinoCommand::success)
		return CRhinoCommand::nothing;

	CRhinoGetObject go1;
	go1.SetCommandPrompt(L"Select block as reference");
	go1.SetGeometryFilter(CRhinoGetObject::instance_reference);
	go1.GetObjects(1, 1);
	if (go1.CommandResult() != CRhinoCommand::success)
		return CRhinoCommand::nothing;

	const CRhinoInstanceObject* irefr = CRhinoInstanceObject::Cast(go1.Object(0).Object());
	CRhinoInstanceDefinitionTable& idef_table = context.m_doc.m_instance_definition_table;

	for (int i = 0; i < go.ObjectCount(); i++)
	{
		const CRhinoInstanceObject* iref = CRhinoInstanceObject::Cast(go.Object(i).Object());
		if (iref)
		{
			int idef_index = irefr->InstanceDefinition()->Index();
			if (idef_index >= 0)
			{
				idef_table.AddInstanceObject(idef_index, iref->InstanceXform());
			}
		}
	}
	for (int i = 0; i < go.ObjectCount(); i++)
	{
		const CRhinoInstanceObject* iref = CRhinoInstanceObject::Cast(go.Object(i).Object());
		if (iref)
		{
			unsigned int doc_serial_numnber = context.m_doc.RuntimeSerialNumber();
			const CRhinoObject* pConstRhinoObject = CRhinoObject::FromId(doc_serial_numnber, iref->Id());
			if (nullptr != pConstRhinoObject)
			{
				CRhinoObject* pRhinoObject = const_cast<CRhinoObject*>(pConstRhinoObject);
				bool rc = context.m_doc.PurgeObject(pRhinoObject);
			}
		}
	}
	return CRhinoCommand::success;
}