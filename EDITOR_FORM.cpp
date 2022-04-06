#include "StdAfx.h"
#include "EDITOR_FORM.h"
#include "BLOCK_PLUGIN.h"

IMPLEMENT_SERIAL(EDITOR_FORM, CRhinoDialog, 1)

EDITOR_FORM::EDITOR_FORM(CWnd* pParent /*=NULL*/)
    : CRhinoDialog(EDITOR_FORM::IDD, pParent)
{
}

EDITOR_FORM::~EDITOR_FORM()
{
}

void EDITOR_FORM::DoDataExchange(CDataExchange* pDX)
{
    CRhinoDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BLOCK_TREE, m_tree);
	DDX_Control(pDX, IDC_BLOCK_PREVIEW, m_preview);
}
BEGIN_MESSAGE_MAP(EDITOR_FORM, CRhinoDialog)
    ON_BN_CLICKED(IDC_BUTTON1, &EDITOR_FORM::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &EDITOR_FORM::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &EDITOR_FORM::hide_click)
	ON_BN_CLICKED(IDC_BUTTON4, &EDITOR_FORM::rename_click)
	ON_BN_CLICKED(IDC_BUTTON5, &EDITOR_FORM::relocation_click)
	ON_NOTIFY(TVN_SELCHANGED, IDC_BLOCK_TREE, &EDITOR_FORM::OnTvnSelchangedTree1)

END_MESSAGE_MAP()

BOOL EDITOR_FORM::OnInitDialog()
{

    CRhinoDialog::OnInitDialog();

    // TODO...
	m_preview.SetDrawDecorations(CRhinoUiBlockPreview::draw_world_axes);
	CRhinoObjRef ff(BLOCKPlugIn().m_root_block);
	const CRhinoObject* obj1 = ff.Object();
	if (obj1)
	{
		const CRhinoInstanceObject* iref1 = CRhinoInstanceObject::Cast(obj1);
		if (iref1 && !iref1->IsDeleted())
			AddInstanceDefinitionToTreeCtrl(iref1, TVI_ROOT, TVI_LAST, true);
	}
    return TRUE;
}
void EDITOR_FORM::AddInstanceDefinitionToTreeCtrl(const CRhinoInstanceObject* idef, HTREEITEM hParent, HTREEITEM hInsertAfter, bool bAddDef)
{
	// I use the tree control's "item data" to store information about the instance definition.
	// If the "item data" is >= 0, then that is the index of the instance definition (in the idef table).
	// If the "item data" is < 0, then that is the an instance definition object's runtime serial number
	// that can be used to quickly look up the object so we can draw it highlighted in the preview control.

	if (idef)
	{
		HTREEITEM hRoot = 0;
		if (bAddDef)
		{
			// Only top-level instance definitions get item data >= 0.
			hRoot = m_tree.InsertItem(idef->InstanceDefinition()->Name(), hParent, hInsertAfter);
			m_tree.SetItemData(hRoot, (DWORD_PTR)(-((int)(idef->RuntimeSerialNumber()))));
		}
		else
			hRoot = hParent;

		ON_SimpleArray<const CRhinoObject*> objects;
		idef->InstanceDefinition()->GetObjects(objects);

		// First pass is to add nested blocks
		int i = 0;
		for (i = 0; i < objects.Count(); i++)
		{
			const CRhinoObject* obj = objects[i];
			if (!obj)
				continue;

			const CRhinoInstanceObject* p = CRhinoInstanceObject::Cast(obj);
			if (p)
			{
				// If a nested block, then we want to add the runtime serial number of the
				// instance object. Then, we will recursively call this function but set
				// the bAddRef flag to false
				const CRhinoInstanceDefinition* q = p->InstanceDefinition();
				if (q)
				{
					// Format a display name for the tree
					ON_wString s;
					ON_wString d = q->Name();
					ON_wString n = obj->Attributes().m_name;
					if (n.IsEmpty())
						s = d;
					else
						s.Format(L"%ls - %ls", static_cast<const wchar_t*>(n), static_cast<const wchar_t*>(d));

					HTREEITEM hItem = m_tree.InsertItem(s, hRoot, hInsertAfter);
					m_tree.SetItemData(hItem, (DWORD_PTR)(-((int)(obj->RuntimeSerialNumber()))));
	
					AddInstanceDefinitionToTreeCtrl(p, hItem, hInsertAfter, false);
					objects[i] = 0;
					continue;
				}
			}
		}

		//// Second pass is to add objects
		//for (i = 0; i < objects.Count(); i++)
		//{
		//	const CRhinoObject* obj = objects[i];
		//	if (0 == obj)
		//		continue;

		//	// Format a display name for the tree
		//	ON_wString s;
		//	ON_wString d = obj->ShortDescription(false);
		//	ON_wString n = obj->Attributes().m_name;
		//	if (n.IsEmpty())
		//		s = d;
		//	else
		//		s.Format(L"%ls - %ls", static_cast<const wchar_t*>(n), static_cast<const wchar_t*>(d));

		//	HTREEITEM hItem = m_tree.InsertItem(s, hRoot, hInsertAfter);
		//	m_tree.SetItemData(hItem, (DWORD_PTR)-((int)obj->RuntimeSerialNumber()));
		//}
	}
}

void EDITOR_FORM::END_EDIT(CRhinoDoc* doc)
{
	BLOCKPlugIn().m_watcher.Enable(false);

	if (doc)
	{
		while (BLOCKPlugIn().EDIT_TREE_LIST.Count() > 0)
		{
			/// <summary>
			/// MODIFY LAST
			/// </summary>
			CRhinoObjRef OR(*(BLOCKPlugIn().EDIT_TREE_LIST.Last()));
			const CRhinoObject* obj0 = OR.Object();
			if (!obj0)
			{
				BLOCKPlugIn().REMOVE_TREE_LIST(-1);
				continue;
			}
			const CRhinoInstanceObject* iref0 = CRhinoInstanceObject::Cast(obj0);
			const CRhinoInstanceDefinition* idef0 = iref0->InstanceDefinition();
			ON_SimpleArray<const CRhinoObject*> idef_objects(BLOCKPlugIn().EDIT_INS_LIST.Count());
			for (int j = 0; j < BLOCKPlugIn().EDIT_INS_LIST.Count(); j++)
			{
				CRhinoObjRef ORI(*(BLOCKPlugIn().EDIT_INS_LIST.At(j)));
				const CRhinoObject* obj = ORI.Object();
				if (obj)
				{
					if (obj->IsDeleted())
						continue;
					CRhinoObject* obji = doc->TransformObject(obj, iref0->InstanceXform().Inverse(), false, true, false);
					idef_objects.Append(obji);
				}
			}
			if (BLOCKPlugIn().EDIT_TREE_BOOL.Last())
				if (doc->m_instance_definition_table.ModifyInstanceDefinitionGeometry(idef0->Index(), idef_objects, true))
					update_tree();
			
				/// <summary>
				/// PURGE LAST
				/// </summary>
				for (int i = 0; i < BLOCKPlugIn().EDIT_INS_LIST.Count(); i++)
				{
					CRhinoObjRef rf(*(BLOCKPlugIn().EDIT_INS_LIST.At(i)));
					const CRhinoObject* II = rf.Object();
					doc->PurgeObject(const_cast<CRhinoObject*&>(II));
				}
				BLOCKPlugIn().EMPTY_INS_LIST();

				doc->ShowObject(const_cast<CRhinoObject*&>(obj0));
				BLOCKPlugIn().APEND_INS_LIST(obj0->Id(), *(BLOCKPlugIn().EDIT_TREE_SERIAL.Last()));
				BLOCKPlugIn().REMOVE_TREE_LIST(-1);
			

				/// <summary>
				/// UNLOCK LAST
				/// </summary>		

				if (BLOCKPlugIn().EDIT_TREE_LIST.Count() > 0)
				{
					if (BLOCKPlugIn().EDIT_LOCKED_COUNT.Count() > 0)
					{
						CRhinoObjRef OR1(*(BLOCKPlugIn().EDIT_TREE_LIST.Last()));
						const CRhinoObject* obj1 = OR1.Object();
						const CRhinoInstanceObject* iref1 = CRhinoInstanceObject::Cast(obj1);
						const CRhinoInstanceDefinition* idef1 = iref1->InstanceDefinition();

						int n = *(BLOCKPlugIn().EDIT_LOCKED_COUNT.Last());
						for (int i = 0; i < n; i++)
						{
							CRhinoObjRef rf(*(BLOCKPlugIn().EDIT_LOCKED_LIST.Last()));
							const CRhinoObject* II = rf.Object();
							doc->UnlockObject(const_cast<CRhinoObject*&>(II), true);
							BLOCKPlugIn().APEND_INS_LIST(*(BLOCKPlugIn().EDIT_LOCKED_LIST.Last()), *(BLOCKPlugIn().EDIT_LOCKED_SERIAL.Last()));
							BLOCKPlugIn().REMOVE_LOCKED_LIST(-1);
						}
						BLOCKPlugIn().EDIT_LOCKED_COUNT.Remove();
					}
				}
				else
				{
					BLOCKPlugIn().EMPTY_INS_LIST();
				}
			}
	}
}
void EDITOR_FORM::DO_EDIT(CRhinoDoc* doc)
{
	CRhinoGetObject go;
	go.SetCommandPrompt(L"Select blocks to Edit");
	go.SetGeometryFilter(CRhinoGetObject::instance_reference);
	go.GetObjects(0, 0);
	if (go.CommandResult() != CRhinoCommand::success)
		return;
	CRhinoInstanceDefinitionTable& idef_table = doc->m_instance_definition_table;
	const CRhinoInstanceObject* iref = CRhinoInstanceObject::Cast(go.Object(0).Object());
	if (iref)
	{
		if (iref->InstanceDefinition()->IsLinkedType())
			return;
	}

	//IN EDIT MODE
	if (BLOCKPlugIn().m_dialog > 0 && BLOCKPlugIn().m_dialog->IsWindowVisible())
	{
		if (BLOCKPlugIn().EDIT_TREE_LIST.Count() > 0)
		{
			int index = BLOCKPlugIn().EDIT_INS_LIST.Search(iref->Id());
			if (index > -1)
			{
				UUID* ID = BLOCKPlugIn().EDIT_TREE_LIST.Last();
				CRhinoObjRef OR(*ID);
				const CRhinoInstanceObject* iref0 = CRhinoInstanceObject::Cast(OR.Object());
				const CRhinoInstanceDefinition* idef0 = iref0->InstanceDefinition();
				ON_SimpleArray<const CRhinoObject*> idef_objects(BLOCKPlugIn().EDIT_INS_LIST.Count());
				for (int j = 0; j < BLOCKPlugIn().EDIT_INS_LIST.Count(); j++)
				{
					if (j != index)
					{
						UUID* id = BLOCKPlugIn().EDIT_INS_LIST.At(j);
						BLOCKPlugIn().APEND_LOCKED_LIST(*id, *(BLOCKPlugIn().EDIT_INS_SERIAL.At(j)));
						CRhinoObjRef rf(*id);
						const CRhinoObject* II = rf.Object();
						doc->LockObject(const_cast<CRhinoObject*&>(II), true);
					}
					else
					{
						CRhinoObjRef rf(*(BLOCKPlugIn().EDIT_INS_LIST.At(j)));
						const CRhinoObject* II = rf.Object();
						doc->HideObject(const_cast<CRhinoObject*&>(II), true);
					}
				}
				BLOCKPlugIn().EDIT_LOCKED_COUNT.Append(BLOCKPlugIn().EDIT_INS_LIST.Count() - 1);
				BLOCKPlugIn().EMPTY_INS_LIST();
			}
			else
			{
				BLOCKPlugIn().m_dialog->END_EDIT(doc);
				BLOCKPlugIn().m_dialog->CLEAR(doc);
			}
		}
	}
	else
	{
		doc->HideObject(iref, true);
		if (iref)
			BLOCKPlugIn().m_root_block = iref->Id();
		BLOCKPlugIn().DisplayDlg();
	}
	if (iref)
	{
		BLOCKPlugIn().m_watcher.Enable();
		const CRhinoInstanceDefinition* def = iref->InstanceDefinition();
		ON_SimpleArray<const CRhinoObject*> objects;
		def->GetObjects(objects);
		for (int j = 0; j < objects.Count(); j++)
		{
			const CRhinoObject* obj = objects[j];
			if (obj)
			{
				int SERIAL = -1;
				const CRhinoInstanceObject* p = CRhinoInstanceObject::Cast(obj);

				if (p)
				{
					SERIAL = obj->RuntimeSerialNumber();
				}
				BLOCKPlugIn().APEND_INS_LIST(doc->TransformObject(obj, iref->InstanceXform(), true, false, false)->Id(),SERIAL);
			}
		}
		BLOCKPlugIn().APEND_TREE_LIST(iref->Id(), false, iref->RuntimeSerialNumber());
		doc->HideObject(iref, true);
	}
}
void EDITOR_FORM::CONTINUE_EDIT(CRhinoDoc* doc)
{
	CRhinoObjRef OR(*(BLOCKPlugIn().EDIT_TREE_LIST.Last()));
	const CRhinoObject* obj0 = OR.Object();
	if (obj0)
	{
		const CRhinoInstanceObject* iref0 = CRhinoInstanceObject::Cast(obj0);
		const CRhinoInstanceDefinition* idef0 = iref0->InstanceDefinition();
		ON_SimpleArray<const CRhinoObject*> idef_objects(BLOCKPlugIn().EDIT_INS_LIST.Count());
		for (int j = 0; j < BLOCKPlugIn().EDIT_INS_LIST.Count(); j++)
		{
			CRhinoObjRef ORI(*(BLOCKPlugIn().EDIT_INS_LIST.At(j)));
			const CRhinoObject* obj = ORI.Object();
			if (obj)
			{
				if (obj->IsDeleted())
					continue;
				CRhinoObject* obji = doc->TransformObject(obj, iref0->InstanceXform().Inverse(), false, true, false);
				idef_objects.Append(obji);
			}
		}
		if (BLOCKPlugIn().EDIT_TREE_BOOL.Last())
			if (doc->m_instance_definition_table.ModifyInstanceDefinitionGeometry(idef0->Index(), idef_objects, true))
				update_tree();
		
			/// <summary>
			/// PURGE LAST
			/// </summary>
			for (int i = 0; i < BLOCKPlugIn().EDIT_INS_LIST.Count(); i++)
			{
				CRhinoObjRef rf(*(BLOCKPlugIn().EDIT_INS_LIST.At(i)));
				const CRhinoObject* II = rf.Object();
				doc->PurgeObject(const_cast<CRhinoObject*&>(II));
			}
			BLOCKPlugIn().EMPTY_INS_LIST();

			doc->ShowObject(const_cast<CRhinoObject*&>(obj0));
			BLOCKPlugIn().APEND_INS_LIST(obj0->Id(), *(BLOCKPlugIn().EDIT_TREE_SERIAL.Last()));
			BLOCKPlugIn().REMOVE_TREE_LIST(-1);
		


			/// <summary>
			/// UNLOCK LAST
			/// </summary>		

			if (BLOCKPlugIn().EDIT_TREE_LIST.Count() > 0)
			{
				if (BLOCKPlugIn().EDIT_LOCKED_COUNT.Count() > 0)
				{
					int n = *(BLOCKPlugIn().EDIT_LOCKED_COUNT.Last());
					for (int i = 0; i < n; i++)
					{
						CRhinoObjRef rf(*(BLOCKPlugIn().EDIT_LOCKED_LIST.Last()));
						const CRhinoObject* II = rf.Object();
						doc->UnlockObject(const_cast<CRhinoObject*&>(II), true);
						BLOCKPlugIn().APEND_INS_LIST(*(BLOCKPlugIn().EDIT_LOCKED_LIST.Last()), *(BLOCKPlugIn().EDIT_LOCKED_SERIAL.Last()));
						BLOCKPlugIn().REMOVE_LOCKED_LIST(-1);
					}
					BLOCKPlugIn().EDIT_LOCKED_COUNT.Remove();
				}
			}
			else
			{
				BLOCKPlugIn().EMPTY_INS_LIST();
				BLOCKPlugIn().DestroyDlg();
			}
		
		BLOCKPlugIn().m_watcher.Enable(true);
	}
}
void EDITOR_FORM::CLEAR(CRhinoDoc* doc)
{
	if (BLOCKPlugIn().EDIT_TREE_LIST.Count() > 0)
	{
		CRhinoObjRef OR(*(BLOCKPlugIn().EDIT_TREE_LIST.Last()));
		const CRhinoObject* obj0 = OR.Object();
		if (obj0)
			doc->ShowObject(const_cast<CRhinoObject*&>(obj0));
	}
	for (int i = 0; i < BLOCKPlugIn().EDIT_LOCKED_LIST.Count(); i++)
	{
		CRhinoObjRef rf(*(BLOCKPlugIn().EDIT_LOCKED_LIST.At(i)));
		if (doc)
		{
			const CRhinoObject* obj0 = rf.Object();
			if (obj0)
				doc->UnlockObject(obj0, true);
		}
	}
	for (int i = 0; i < BLOCKPlugIn().EDIT_INS_LIST.Count(); i++)
	{
		CRhinoObjRef rf(*(BLOCKPlugIn().EDIT_INS_LIST.At(i)));
		const CRhinoObject* II = rf.Object();
		if (doc)
		{
			if (II)
				doc->PurgeObject(const_cast<CRhinoObject*&>(II));
		}
	}
	BLOCKPlugIn().EMPTY_INS_LIST();
	BLOCKPlugIn().EMPTY_TREE_LIST();
	BLOCKPlugIn().EMPTY_LOCKED_LIST();
	doc->Redraw();
}


void EDITOR_FORM::OnCancel()
{
	CRhinoDoc* doc = RhinoApp().ActiveDoc();
	BLOCKPlugIn().m_watcher.Enable(false);
	CLEAR(doc);
	if (BLOCKPlugIn().m_dialog)
		BLOCKPlugIn().m_dialog = 0;
	CRhinoDialog::OnCancel();
}
void EDITOR_FORM::OnOK()
{
	BLOCKPlugIn().CTYPE = EDIT_TYPE::END;
	CRhinoDoc* doc = RhinoApp().ActiveDoc();
	doc->ExecuteCommand(L"BlockEdit");
	CRhinoDialog::OnOK();
}




//void EDITOR_FORM::RedrawDocument()
//{
//    CRhinoDoc* doc = RhinoApp().ActiveDoc();
//    if (doc)
//        doc->Redraw(CRhinoView::regenerate_display_hint);
//}


void EDITOR_FORM::OnBnClickedButton1()
{
	BLOCKPlugIn().CTYPE = EDIT_TYPE::RETURN;
	CRhinoDoc* doc = RhinoApp().ActiveDoc();
	doc->ExecuteCommand(L"BlockEdit");
}


void EDITOR_FORM::OnBnClickedButton2()
{
	BLOCKPlugIn().CTYPE = EDIT_TYPE::END;
	CRhinoDoc* doc = RhinoApp().ActiveDoc();
	doc->ExecuteCommand(L"BlockEdit");
}
void EDITOR_FORM::KillDialog()
{
	OnCancel();
}

void EDITOR_FORM::rename_click()
{
	CRhinoDoc* doc = RhinoApp().ActiveDoc();
	doc->ExecuteCommand(L"RenameBlockEX");
}
void EDITOR_FORM::relocation_click()
{
	CRhinoDoc* doc = RhinoApp().ActiveDoc();
	doc->ExecuteCommand(L"RelocationBlockEX");
}
void EDITOR_FORM::hide_click()
{
	CRhinoDoc* doc = RhinoApp().ActiveDoc();
	if (hide_others)
	{
		for (int i = 0; i < BLOCKPlugIn().EDIT_LOCKED_LIST.Count(); i++)
		{
			CRhinoObjRef rf(*(BLOCKPlugIn().EDIT_LOCKED_LIST.At(i)));

			if (doc)
			{
				doc->ShowObject(rf.Object(), true);
				doc->LockObject(rf.Object(), true);
			}
		}
		hide_others = false;
	}
	else
	{
		for (int i = 0; i < BLOCKPlugIn().EDIT_LOCKED_LIST.Count(); i++)
		{
			CRhinoObjRef rf(*(BLOCKPlugIn().EDIT_LOCKED_LIST.At(i)));
			if (doc)
			{
				doc->UnlockObject(rf.Object(), true);
				doc->HideObject(rf.Object(), true);
			}
		}
		hide_others = true;
	}
	doc->Redraw();
}

int EDITOR_FORM::selected_index(HTREEITEM hItem)
{
	HTREEITEM pItem = m_tree.GetParentItem(hItem);
	int idef_index = (int)m_tree.GetItemData(hItem);
		int kidcount = 0;
	if (m_tree.ItemHasChildren(pItem))
	{
		HTREEITEM hKid = m_tree.GetChildItem(pItem);
		while (hKid)
		{
			if (idef_index == (int)m_tree.GetItemData(hItem))
				kidcount++;
			hKid = m_tree.GetNextItem(hKid, TVGN_NEXT);
			if (hKid == hItem)
				return kidcount;
		
		}
	}
}
void EDITOR_FORM::update_tree()
{
	m_tree.DeleteAllItems();
	CRhinoObjRef ff(BLOCKPlugIn().m_root_block);
	const CRhinoObject* obj1 = ff.Object();
	if (obj1)
	{
		const CRhinoInstanceObject* iref1 = CRhinoInstanceObject::Cast(obj1);
		if (iref1 && !iref1->IsDeleted())
			AddInstanceDefinitionToTreeCtrl(iref1, TVI_ROOT, TVI_LAST, true);
	}
}
void EDITOR_FORM::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem = m_tree.GetSelectedItem();
	
	if (hItem)
	{
		m_idef = nullptr;
		const CRhinoObject* idef_obj = nullptr;
		ON_Xform idef_xf(1);
		ON_SimpleArray<int> SS;
		int idef_index = (int)m_tree.GetItemData(hItem);



		if (idef_index < 0)
		{
			// Get subobject to draw
			SS.Append(-idef_index);
			idef_obj=BLOCKPlugIn().m_doc->LookupObjectByRuntimeSerialNumber(-idef_index);
			m_idef= CRhinoInstanceObject::Cast(idef_obj)->InstanceDefinition();
			while (hItem)
			{
				HTREEITEM hParent = m_tree.GetParentItem(hItem);
				if (hParent)
				{
					idef_index = (int)m_tree.GetItemData(hParent);
					if (idef_index < 0)
					{
					      SS.Append(-idef_index);
					}
				}
				hItem = hParent;
			}
		}
		m_preview.SetInstanceDefinitionObject(idef_obj, idef_xf, false);
		m_preview.SetInstanceDefinition(m_idef, true);
		const CRhinoInstanceDefinition* m_idef0 = 0;
			int n=0;
			while (BLOCKPlugIn().EDIT_TREE_LIST.Count()>n && SS.Count() > n)
			{
				int ttt = BLOCKPlugIn().EDIT_TREE_SERIAL[n];
				int sss = SS[SS.Count()-n-1];
				if ( ttt== sss)
				{
					n++;
				}
				else
					break;
			}		
			int k = BLOCKPlugIn().EDIT_TREE_LIST.Count();
			while (k> n)
			{
				BLOCKPlugIn().CTYPE = EDIT_TYPE::RETURN;
				BLOCKPlugIn().m_doc->ExecuteCommand(L"BlockEdit");
				k--;
			}
				while(BLOCKPlugIn().EDIT_INS_LIST.Count() >0&&SS.Count()>n)
				{
					for (int i = 0; i < BLOCKPlugIn().EDIT_INS_SERIAL.Count(); i++)
					{
						int uuu = *(BLOCKPlugIn().EDIT_INS_SERIAL.At(i));
						int fff = SS[SS.Count() - n - 1];
						if (uuu==fff)
						{
							CRhinoObjRef gg(*BLOCKPlugIn().EDIT_INS_LIST.At(i));
							const CRhinoObject* iref = gg.Object();
							iref->Select(true);
							BLOCKPlugIn().CTYPE = EDIT_TYPE::NORMAL;
							BLOCKPlugIn().m_doc->ExecuteCommand(L"BlockEdit");
							break;
						}
					}			
					n++;
				}
	}
	*pResult = 0;
}



