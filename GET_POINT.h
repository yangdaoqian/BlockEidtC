#pragma once
#include "stdafx.h"
class BLOCK_GET_POINT : public CRhinoGetPoint
{
public:
	BLOCK_GET_POINT();
	void SetPoint(ON_3dPoint point);
	ON_3dPoint POINT() const;
	void OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const ON_2iPoint* p2d);
	void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt);
private:
	ON_3dPoint m_point;
};
