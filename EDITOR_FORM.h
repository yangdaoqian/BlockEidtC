#pragma once

#include "Resource.h"

class EDITOR_FORM :public CRhinoDialog
{
	DECLARE_SERIAL(EDITOR_FORM)

public:
	EDITOR_FORM(CWnd* pParent = NULL);
	virtual ~EDITOR_FORM();

	// Dialog Data
	enum { IDD = IDD_EDITOR };
	CTreeCtrl m_tree;
	CRhinoUiBlockPreview m_preview;
	void KillDialog();
	void END_EDIT(CRhinoDoc* doc);
	void DO_EDIT(CRhinoDoc* doc);
	void CONTINUE_EDIT(CRhinoDoc* doc);
	void CLEAR(CRhinoDoc* doc);
	void AddInstanceDefinitionToTreeCtrl(const CRhinoInstanceObject * idef, HTREEITEM hParent, HTREEITEM hInsertAfter, bool bAddDef);
	int selected_index(HTREEITEM hItem);
	void update_tree();
protected:
	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void OnOK() override;
	virtual void OnCancel() override;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	DECLARE_MESSAGE_MAP()
	bool hide_others;
    const	CRhinoInstanceDefinition* m_idef;
public:
	afx_msg void hide_click();
	afx_msg void rename_click();
	afx_msg void relocation_click();
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
};