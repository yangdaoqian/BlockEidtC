#pragma once
#include "stdafx.h"
class ZLKJ_GET_PLANE : public CRhinoGetPoint
{
public:
	ZLKJ_GET_PLANE();
	void SetGrade(int grade);
	void SetPlane(ON_Plane plane);
	//void Set_Y(ON_Plane& p,ON_3dVector& v);
	//ON_3dVector CrossProduct(ON_3dVector& v0, ON_3dVector& v1) const;
	//void RotatePlaneY(ON_Plane& p,const ON_3dPoint& v);
	bool CalculateGradePoint(CRhinoViewport& vp, const ON_3dPoint& pt);
	
	ON_3dPoint GradePoint() const;
	ON_Plane PLANE() const;
	void OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const ON_2iPoint* p2d);
	void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt);

	ON_Plane m_plane;
private:
	bool m_draw;
	int m_grade;
	ON_3dPoint m_point;
	
};