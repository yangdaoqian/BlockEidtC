#pragma once
#include "stdafx.h"

RHINO_SDK_FUNCTION
bool GetShapeParameterOnRail(const ON_Curve& shape, const ON_Curve& rail, double tol, double& t);

RHINO_SDK_FUNCTION
bool Sweep1(const ON_Curve* in_rail, ON_SimpleArray<const ON_Curve*>in_secs, ON_SimpleArray<ON_Brep*>& out_breps);

RHINO_SDK_FUNCTION
void PlaneToPlane(ON_Xform* f,const ON_Plane* po, const ON_Plane* pt);