#include "SFrustum.h"

#include "SSegment.h"
#include "SLine.h"
#include "SRay.h"
#include "SPlane.h"
#include "STriangle.h"
#include "SCircle.h"
#include "SPolygon.h"
#include "SSphere.h"
#include "SCapsule.h"
#include "SPolyhedron.h"

SFrustum::SFrustum()
{
	frustum = new Frustum();
}
