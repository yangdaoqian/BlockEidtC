#include "StdAfx.h"
#include "BLOCK_PLUGIN.h"
#include "GET_POINT.h"
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMarker command
//

#pragma region SampleMarker command

class COMMAND_ZLKJ_BLOCK_RELOCATION : public CRhinoCommand
{
public:
	COMMAND_ZLKJ_BLOCK_RELOCATION() = default;
	~COMMAND_ZLKJ_BLOCK_RELOCATION() = default;
	UUID CommandUUID() override
	{
		//{c26915f7-db2d-a66b-0274-28d5bede9f88}     
		static const GUID SampleMarkerCommand_UUID =
		{ 0xc26915f7, 0xdb2d, 0xa66b, { 0x02, 0x74, 0x28, 0xd5, 0xbe, 0xde, 0x9f, 0x88 } };
		return SampleMarkerCommand_UUID;
	}
	const wchar_t* EnglishCommandName() override { return L"RelocationBlockEX"; }
	CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleMarker object
static class COMMAND_ZLKJ_BLOCK_RELOCATION theSampleMarkerCommand;

CRhinoCommand::result COMMAND_ZLKJ_BLOCK_RELOCATION::RunCommand(const CRhinoCommandContext& context)
{

	const CRhinoInstanceDefinition* idef0 = 0;
	const CRhinoInstanceObject* iref0 = 0;
	CRhinoDoc* doc = RhinoApp().ActiveDoc();
	if (BLOCKPlugIn().m_dialog)
	{
		BLOCKPlugIn().m_watcher.Enable(false);
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
					iref0 = CRhinoInstanceObject::Cast(obj0);
					if (iref0)
					{
						idef0 = iref0->InstanceDefinition();
					}
				}
				if (idef0)
				{
					BLOCK_GET_POINT g;
					ON_3dPoint p = ON_3dPoint::Origin;
					p.Transform(iref0->InstanceXform());
					g.SetPoint(p);
					g.GetPoint();
					if (g.CommandResult() != CRhinoCommand::success)
						return CRhinoCommand::nothing;
					ON_SimpleArray<UUID> ddd(BLOCKPlugIn().EDIT_INS_LIST.Count());
					ON_SimpleArray<int> sss(BLOCKPlugIn().EDIT_INS_LIST.Count());
				while(BLOCKPlugIn().EDIT_INS_LIST.Count()>0)
					{
						
						CRhinoObjRef ORI(*(BLOCKPlugIn().EDIT_INS_LIST.At(0)));
						const CRhinoObject* obj = ORI.Object();
						if (obj)
						{
							CRhinoObject* obji = doc->TransformObject(obj, ON_Xform::TranslationTransformation(g.Point() - p), true, true, false);
							if (ddd.Search(obji->Id()) < 0)
							{							
							ddd.Append(obji->Id());
							sss.Append(*BLOCKPlugIn().EDIT_INS_SERIAL.At(0));
						    }
						}
						BLOCKPlugIn().REMOVE_INS_LIST(0);
					}
					for (int j = 0; j < ddd.Count(); j++)
					{
						BLOCKPlugIn().APEND_INS_LIST(*ddd.At(j), *sss.At(j));
					}
					ON_SimpleArray<const CRhinoObject*> idef_objects(idef0->ObjectCount());
					for (int j = 0; j < idef0->ObjectCount(); j++)
					{
						const CRhinoObject* obj = idef0->Object(j);
						if (obj)
						{
							if (obj->IsDeleted())
								continue;
							CRhinoObject* obji = doc->TransformObject(obj, ON_Xform::TranslationTransformation(g.Point() - p), false, true, false);
							idef_objects.Append(obji);
						}
					}
					if (doc->m_instance_definition_table.ModifyInstanceDefinitionGeometry(idef0->Index(), idef_objects, true))
						BLOCKPlugIn().m_dialog->update_tree();
				}
			}
		}
		BLOCKPlugIn().m_watcher.Enable(true);
	}

	else
	{
		CRhinoGetObject go;
		go.SetCommandPrompt(L"Select blocks to change base point");
		go.SetGeometryFilter(CRhinoGetObject::instance_reference);
		go.GetObjects(0, 0);
		if (go.CommandResult() != CRhinoCommand::success)
			return CRhinoCommand::nothing;
		for (int i = 0; i < go.ObjectCount(); i++)
		{
			iref0 = CRhinoInstanceObject::Cast(go.Object(i).Object());
			if (iref0)
			{
				idef0 = iref0->InstanceDefinition();
			}
		}
		if (idef0)
		{
			BLOCK_GET_POINT g;
			ON_3dPoint p = ON_3dPoint::Origin;
			p.Transform(iref0->InstanceXform());
			g.SetPoint(p);
			g.GetPoint();
			if (g.CommandResult() != CRhinoCommand::success)
				return CRhinoCommand::nothing;
			ON_SimpleArray<const CRhinoObject*> idef_objects(idef0->ObjectCount());
			for (int j = 0; j < idef0->ObjectCount(); j++)
			{
				const CRhinoObject* obj = idef0->Object(j);
				if (obj)
				{
					if (obj->IsDeleted())
						continue;
					CRhinoObject* obji = doc->TransformObject(obj, ON_Xform::TranslationTransformation(g.Point() - p), false, true, false);
					idef_objects.Append(obji);
				}
			}
			if(doc->m_instance_definition_table.ModifyInstanceDefinitionGeometry(idef0->Index(), idef_objects, true))
				BLOCKPlugIn().m_dialog->update_tree();
		}
	}
	return CRhinoCommand::success;
}