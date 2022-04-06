#include "stdafx.h"
#include "ZLKJ_GET_PLANE.h"

BLOCK_GET_POINT::BLOCK_GET_POINT()
{
	m_draw = false;
	m_grade = 0;
}

void BLOCK_GET_POINT::SetGrade(int grade)
{
	m_grade = grade;
}
void BLOCK_GET_POINT::SetPoint(ON_Plane plane)
{
	m_plane = plane;
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
bool BLOCK_GET_POINT::CalculateGradePoint(CRhinoViewport& vp, const ON_3dPoint& pt)
{
	//ON_3dPoint base_pt = BasePoint();
	//ON_Plane plane = vp.ConstructionPlane().m_plane;
	//plane.SetOrigin(base_pt);

	//ON_3dPoint plane_pt = plane.ClosestPointTo(pt);
	//double run = plane_pt.DistanceTo(base_pt);
	//if (run < 0.000001) // whatever...
	//	return false;

	//double angle = atan(m_grade / 100.0);
	//double rise = run * tan(angle);
	//m_grade_pt = plane_pt + (plane.zaxis * rise);

	return true;
}

ON_3dPoint BLOCK_GET_POINT::GradePoint() const
{
	return m_point;
}
ON_Plane BLOCK_GET_POINT::PLANE() const
{
	return m_plane;
}
void BLOCK_GET_POINT::OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const ON_2iPoint* p2d)
{
	m_draw = CalculateGradePoint(vp, pt);
	if (m_grade == 0)
	{
		m_plane = vp.ConstructionPlane().m_plane;
		m_plane.origin = pt;
	}
	if (m_grade == 1)
	{
		m_plane.xaxis = pt - BasePoint();
	
		ON_3dVector a = m_plane.xaxis;
		ON_3dVector b = -m_plane.zaxis;
		m_plane.yaxis = ON_3dVector(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y);
	}
	if (m_grade == 2)
	{
		m_plane.yaxis = pt - BasePoint();	
		ON_3dVector a = m_plane.xaxis;
		ON_3dVector b = -m_plane.yaxis;
		m_plane.zaxis = ON_3dVector(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y);
		a = m_plane.xaxis;
		b = m_plane.zaxis;
		m_plane.yaxis = ON_3dVector(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y);
	}
	m_plane.xaxis.Unitize();
	m_plane.yaxis.Unitize();
	m_plane.zaxis.Unitize();
	CRhinoGetPoint::OnMouseMove(vp, flags, pt, p2d);
}

void BLOCK_GET_POINT::DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt)
{
	if (m_draw)
	{
		ON_Color cx = ON_Color::SaturatedRed;
		ON_Color cy = ON_Color::SaturatedGreen;
		const CRhinoViewport* vp = dp.GetRhinoVP();
		if (nullptr != vp)
		{
			const int size_in_pixels = 5;
			double pixels_per_unit = 0.0;
			if (vp->View().m_vp.GetWorldToScreenScale(vp->View().TargetPoint(), &pixels_per_unit))
			{
				double units_per_pixel = 100 / pixels_per_unit;
				ON_3dVector vx = units_per_pixel * size_in_pixels * m_plane.xaxis;
				ON_3dVector vy = units_per_pixel * size_in_pixels * m_plane.yaxis;
				dp.DrawLine(m_plane.origin, m_plane.origin + vx, cx, 1, 0xFFFFFFFF);
				dp.DrawLine(m_plane.origin, m_plane.origin + vy, cy, 1, 0xFFFFFFFF);
			}
		}
	}
	CRhinoGetPoint::DynamicDraw(dp, pt);
}




