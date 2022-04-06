#include "StdAfx.h"
#include "EVENT_WATCHER.h"
#include "BLOCK_PLUGIN.h"

////////////////////////////////////////////////////////////////
// Document events

void CSampleRhinoEventWatcher::OnCloseDocument(CRhinoDoc&)
{
   // RhinoApp().Print(L"** EVENT: Close Document **\n");
}

void CSampleRhinoEventWatcher::OnNewDocument(CRhinoDoc&)
{
  //  RhinoApp().Print(L"** EVENT: New Document **\n");
}

void CSampleRhinoEventWatcher::OnBeginOpenDocument(CRhinoDoc&, const wchar_t*, BOOL, BOOL)
{
   // RhinoApp().Print(L"** EVENT: Begin Open Document **\n");
}

void CSampleRhinoEventWatcher::OnEndOpenDocument(CRhinoDoc&, const wchar_t*, BOOL, BOOL)
{
   // RhinoApp().Print(L"** EVENT: End Open Document **\n");
}

void CSampleRhinoEventWatcher::OnBeginSaveDocument(CRhinoDoc&, const wchar_t*, BOOL)
{
  //  RhinoApp().Print(L"** EVENT: Begin Save Document **\n");
}

void CSampleRhinoEventWatcher::OnEndSaveDocument(CRhinoDoc&, const wchar_t*, BOOL)
{
   // RhinoApp().Print(L"** EVENT: End Save Document **\n");
}

void CSampleRhinoEventWatcher::OnDocumentPropertiesChanged(CRhinoDoc&)
{
   // RhinoApp().Print(L"** EVENT: Document Properties Changed **\n");
}

void CSampleRhinoEventWatcher::OnAppSettingsChanged(const CRhinoAppSettings&)
{
   // RhinoApp().Print(L"** EVENT: Application Settings Changed **\n");
}

////////////////////////////////////////////////////////////////
// View events

void CSampleRhinoEventWatcher::OnCreateView(CRhinoView*)
{
   // RhinoApp().Print(L"** EVENT: Create View **\n");
}

void CSampleRhinoEventWatcher::OnDestroyView(CRhinoView*)
{
    //RhinoApp().Print(L"** EVENT: Destroy View **\n");
}

void CSampleRhinoEventWatcher::OnSetActiveView(CRhinoView*)
{
   //RhinoApp().Print(L"** EVENT: Set Active View **\n");
}

void CSampleRhinoEventWatcher::OnRenameView(CRhinoView*)
{
   // RhinoApp().Print(L"** EVENT: Rename View **\n");
}

////////////////////////////////////////////////////////////////
// Command events

void CSampleRhinoEventWatcher::OnBeginCommand(const CRhinoCommand& C, const CRhinoCommandContext& X)
{
    ////RhinoApp().Print(L"** EVENT: Undo, Type: %ls **\n", static_cast<const wchar_t*>(C.EnglishCommandName()));
    //if (wcscmp(C.EnglishCommandName(), L"Block") == 0 || wcscmp(C.EnglishCommandName(), L"Paste") == 0)
    //{
    //    BLOCKPlugIn().block_cmd = true;

    //}
}

void CSampleRhinoEventWatcher::OnEndCommand(const CRhinoCommand& C, const CRhinoCommandContext&, CRhinoCommand::result)
{
    //if (wcscmp(C.EnglishCommandName(),L"Block")==0)
    //    BLOCKPlugIn().block_cmd = false;
    //if (wcscmp(C.EnglishCommandName(), L"Paste") == 0)
    //    BLOCKPlugIn().block_cmd = false;
}

////////////////////////////////////////////////////////////////
// Object events

void CSampleRhinoEventWatcher::OnAddObject(CRhinoDoc&, CRhinoObject& OBJ)
{
    if (OBJ.IsNormal())
    {
        BLOCKPlugIn().APEND_INS_LIST(OBJ.Id(),OBJ.RuntimeSerialNumber());
        BLOCKPlugIn().EDIT_TREE_BOOL.Remove();
        BLOCKPlugIn().EDIT_TREE_BOOL.Append(true);
    }
}

void CSampleRhinoEventWatcher::OnDeleteObject(CRhinoDoc&, CRhinoObject& OBJ)
{
    int index = BLOCKPlugIn().EDIT_INS_LIST.Search(OBJ.Id());
    if (index > -1)
    {
        BLOCKPlugIn().REMOVE_INS_LIST(index);
        BLOCKPlugIn().EDIT_TREE_BOOL.Remove();
        BLOCKPlugIn().EDIT_TREE_BOOL.Append(true);
    }
   //RhinoApp().Print(L"** EVENT: Delete Object **\n");
}

void CSampleRhinoEventWatcher::OnReplaceObject(CRhinoDoc& doc, CRhinoObject&, CRhinoObject&)
{

   // RhinoApp().Print(L"** EVENT: Replace Object **\n");
  //  RhinoApp().Print(L"**      UndoActive() = %ls **\n", doc.UndoActive() ? L"TRUE" : L"FALSE");
   // RhinoApp().Print(L"**      RedoActive() = %ls **\n", doc.RedoActive() ? L"TRUE" : L"FALSE");
}

void CSampleRhinoEventWatcher::OnUnDeleteObject(CRhinoDoc&, CRhinoObject& OBJ)
{
    if (OBJ.IsNormal())
    {
        BLOCKPlugIn().APEND_INS_LIST(OBJ.Id(), OBJ.RuntimeSerialNumber());
        BLOCKPlugIn().EDIT_TREE_BOOL.Remove();
        BLOCKPlugIn().EDIT_TREE_BOOL.Append(true);
    }
 //   RhinoApp().Print(L"** EVENT: Undelete Object **\n");
}

void CSampleRhinoEventWatcher::OnPurgeObject(CRhinoDoc&, CRhinoObject& OBJ)
{
    int index = BLOCKPlugIn().EDIT_INS_LIST.Search(OBJ.Id());
    if (index > -1)
    {
        BLOCKPlugIn().REMOVE_INS_LIST(index);
        BLOCKPlugIn().EDIT_TREE_BOOL.Remove();
        BLOCKPlugIn().EDIT_TREE_BOOL.Append(true);
    }
   // RhinoApp().Print(L"** EVENT: Purge Object **\n");
}

void CSampleRhinoEventWatcher::OnSelectObject(CRhinoDoc&, const CRhinoObject&)
{
   // RhinoApp().Print(L"** EVENT: Select Object **\n");
}

void CSampleRhinoEventWatcher::OnSelectObjects(CRhinoDoc&, const ON_SimpleArray<const CRhinoObject*>&)
{
   // RhinoApp().Print(L"** EVENT: Select Objects **\n");
}

void CSampleRhinoEventWatcher::OnDeselectObject(CRhinoDoc&, const CRhinoObject&)
{
   // RhinoApp().Print(L"** EVENT: Deselect Object **\n");
}

void CSampleRhinoEventWatcher::OnDeselectObjects(CRhinoDoc&, const ON_SimpleArray<const CRhinoObject*>&)
{
   // RhinoApp().Print(L"** EVENT: Deselect Objects **\n");
}

void CSampleRhinoEventWatcher::OnDeselectAllObjects(CRhinoDoc&, int)
{
   // RhinoApp().Print(L"** EVENT: Deselect All Objects **\n");
}

void CSampleRhinoEventWatcher::OnModifyObjectAttributes(CRhinoDoc&, CRhinoObject&, const CRhinoObjectAttributes&)
{
    //RhinoApp().Print(L"** EVENT: Modify Object Attributes **\n");
}

////////////////////////////////////////////////////////////////
// Table events

void CSampleRhinoEventWatcher::LayerTableEvent(CRhinoEventWatcher::layer_event, const CRhinoLayerTable&, int, const ON_Layer*)
{
  // RhinoApp().Print(L"** EVENT: Layer Table **\n");
}

void CSampleRhinoEventWatcher::LinetypeTableEvent(CRhinoEventWatcher::linetype_event, const CRhinoLinetypeTable&, int, const ON_Linetype*)
{
    //RhinoApp().Print(L"** EVENT: Linetype Table **\n");
}

void CSampleRhinoEventWatcher::InstanceDefinitionTableEvent(CRhinoEventWatcher::idef_event, const CRhinoInstanceDefinitionTable&, int, const ON_InstanceDefinition*)
{
   // RhinoApp().Print(L"** EVENT: Instance Definition Table **\n");
}

void CSampleRhinoEventWatcher::LightTableEvent(CRhinoEventWatcher::light_event, const CRhinoLightTable&, int, const ON_Light*)
{
   // RhinoApp().Print(L"** EVENT: Light Table **\n");
}

void CSampleRhinoEventWatcher::MaterialTableEvent(CRhinoEventWatcher::material_event, const CRhinoMaterialTable&, int, const ON_Material*)
{
    //RhinoApp().Print(L"** EVENT: Material Table **\n");
}

void CSampleRhinoEventWatcher::TextureMappingTableEvent(CRhinoEventWatcher::texture_mapping_event, const CRhinoTextureMappingTable&, int, const ON_TextureMapping*)
{
    //RhinoApp().Print(L"** EVENT: Texture Mapping Table **\n");
}

void CSampleRhinoEventWatcher::GroupTableEvent(CRhinoEventWatcher::group_event, const CRhinoGroupTable&, int, const ON_Group*)
{
   // RhinoApp().Print(L"** EVENT: Group Table **\n");
}

//void CSampleRhinoEventWatcher::TextStyleTableEvent(CRhinoEventWatcher::text_style_event, const CRhinoTextStyleTable&, int, const ON_TextStyle*)
//{
//  RhinoApp().Print(L"** EVENT: Text Style Table **\n");
//}

void CSampleRhinoEventWatcher::DimStyleTableEvent(CRhinoEventWatcher::dimstyle_event, const CRhinoDimStyleTable&, int, const ON_DimStyle*)
{
   // RhinoApp().Print(L"** EVENT: Dimension Style Table **\n");
}

void CSampleRhinoEventWatcher::HatchPatternTableEvent(CRhinoEventWatcher::hatchpattern_event, const CRhinoHatchPatternTable&, int, const ON_HatchPattern*)
{
    //RhinoApp().Print(L"** EVENT: Hatch Pattern Table **\n");
}

////////////////////////////////////////////////////////////////
// Undo event

void CSampleRhinoEventWatcher::UndoEvent(CRhinoEventWatcher::undo_event type, unsigned int, const CRhinoCommand* C)
{
    if (C)
    {
    ON_wString str;
    switch (type)
    {
    case CRhinoEventWatcher::no_undo_event:
        str = L"No Undo Event";
        break;
    case CRhinoEventWatcher::begin_recording:
        str = L"Begin Recording";
        break;
    case CRhinoEventWatcher::end_recording:
        str = L"End Recording";
        break;
    case CRhinoEventWatcher::begin_undo:
        if (C->EnglishCommandName() == L"BlockEdit")
            BLOCKPlugIn().DestroyDlg();
        str = L"Begin Undo";
        break;
    case CRhinoEventWatcher::end_undo:
        str = L"End Undo";
        break;
    case CRhinoEventWatcher::begin_redo:
        str = L"Begin Undo";
        break;
    case CRhinoEventWatcher::end_redo:
        str = L"End Redo";
        break;
    case CRhinoEventWatcher::purge_record:
        str = L"Purge Record";
        break;
    }
}
   // RhinoApp().Print(L"** EVENT: Undo, Type: %ls **\n", static_cast<const wchar_t*>(str));
}
