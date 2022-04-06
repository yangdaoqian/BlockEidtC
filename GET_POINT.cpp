#include "stdafx.h"
#include "GET_POINT.h"

BLOCK_GET_POINT::BLOCK_GET_POINT()
{

}


void BLOCK_GET_POINT::SetPoint(ON_3dPoint plane)
{
	m_point = plane;
}
//void ZLKJ_GET_PLANE::Set_Y(ON_Plane& p, ON_3dVector& v)
//{
//	ON_3dVector v2 = ON_3dVector(v.x,v.y,v.z);
//	ON_3dVector v0 = CrossProduct(v2,p.zaxis);
//	ON_3dVector v1 = CrossProduct(v0, v2);
//	v0.Unitize();
//	v1.Unitize();
//	v2.Unitize();
//	if (p.zaxis.IsParallelTo(v0, 3.1415926535897931/2)<0)
//	{
//		p.zaxis = -v0;
//		p.yaxis = v2;
//		p.zaxis = -v1;
//	}
//	else
//	{
//		p.zaxis = v0;
//		p.yaxis = v2;
//		p.zaxis = v1;
//	}
//}
//void ZLKJ_GET_PLANE::RotatePlaneY(ON_Plane& p,const ON_3dPoint& v)
//{
//	ON_3dVector v2 = v - p.origin;
//	if (v2.IsNotZero())
//	{
//		Set_Y(p,v2);
//	}
//}
//ON_3dVector ZLKJ_GET_PLANE::CrossProduct(ON_3dVector& v0, ON_3dVector& v1) const
//{
//	ON_3dVector v=ON_3dVector();
//	v.Set(v0.y*v1.z-v1.y*v0.z, v0.z*v1.x - v1.z*v0.x, v0.x*v1.y - v1.x*v0.y);
//	return v;
//}

ON_3dPoint BLOCK_GET_POINT::POINT() const
{
	return m_point;
}
void BLOCK_GET_POINT::OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const ON_2iPoint* p2d)
{
	CRhinoGetPoint::OnMouseMove(vp, flags, pt, p2d);
}

void BLOCK_GET_POINT::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt)
{
	ON_Color cy = ON_Color::SaturatedGreen;
	ON_Color cx = ON_Color::SaturatedMagenta;
	ON_Color cz = ON_Color::Gray250;
	 dp.DrawLine(m_point, pt, cy, 1);
	 dp.DrawPoint(m_point,2, ERhinoPointStyle::RPS_ROUND_CONTROL_POINT,cx);
	 dp.DrawPoint(pt, 2, ERhinoPointStyle::RPS_ROUND_CONTROL_POINT, cz);
	CRhinoGetPoint::DynamicDraw(dp, pt);
}




