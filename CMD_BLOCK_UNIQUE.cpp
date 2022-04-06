#include "StdAfx.h"
//#include "ZLKJ_KEEL_OBJECT.h"
//#include "ZLKJ_GET_PLANE.h"
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMarker command
//

#pragma region SampleMarker command

class COMMAND_ZLKJ_BLOCK_UNIQUE : public CRhinoCommand
{
public:
	COMMAND_ZLKJ_BLOCK_UNIQUE() = default;
	~COMMAND_ZLKJ_BLOCK_UNIQUE() = default;
	UUID CommandUUID() override
	{
		//{31545a6a-7b7d-4de4-a022-d9933cfee290}
		static const GUID SampleMarkerCommand_UUID =
		{ 0x31545a6a, 0x7b7d, 0x4de4, { 0xa0, 0x22, 0xd9, 0x93, 0x3c, 0xfe, 0xe2, 0x90 } };
		return SampleMarkerCommand_UUID;
	}
	const wchar_t* EnglishCommandName() override { return L"BlockUnique"; }
	CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleMarker object
static class COMMAND_ZLKJ_BLOCK_UNIQUE theSampleMarkerCommand;

CRhinoCommand::result COMMAND_ZLKJ_BLOCK_UNIQUE::RunCommand(const CRhinoCommandContext& context)
{
	CRhinoGetObject go;
	go.SetCommandPrompt(L"Select blocks to make unique");
	go.SetGeometryFilter(CRhinoGetObject::instance_reference);
	go.GetObjects(0, 0);
	if (go.CommandResult() != CRhinoCommand::success)
		return CRhinoCommand::nothing;


	ON_wString name = L"";
	CRhinoStringDictionary nnn;
	CRhinoInstanceDefinitionTable& idef_table = context.m_doc.m_instance_definition_table;

	for (int i = 0; i < go.ObjectCount(); i++)
	{		
		const CRhinoInstanceObject* iref = CRhinoInstanceObject::Cast(go.Object(i).Object());
		if (iref)
		{
			const CRhinoInstanceDefinition* idef = iref->InstanceDefinition();
			ON_wString name = idef->Name();
			int ni = nnn.IndexOf(name);
			int idef_index = idef_table.FindInstanceDefinition(name);
			if (ni < 0)
			{
				const wchar_t* A = L"#1";
			    while (idef_index >= 0)
			    {
				    name.Append(A, 2);
				    idef_index = idef_table.FindInstanceDefinition(name);
			    }
				nnn.Add(idef->Name(),name);
		    }
			else
			{
				idef_index = idef_table.FindInstanceDefinition(nnn[name]);
			}			
		    ON_InstanceDefinition idefn;	
			if (idef_index < 0)
			{
				idefn.SetName(name);
				ON_SimpleArray<const CRhinoObject*> idef_objects(idef->ObjectCount());
				for (int j = 0; j < idef->ObjectCount(); j++)
				{
					const CRhinoObject* obj = idef->Object(j);
					if (obj)
					{
					   idef_objects.Append(obj);
					}
				}
				idef_index = idef_table.AddInstanceDefinition(idefn, idef_objects);
			}
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