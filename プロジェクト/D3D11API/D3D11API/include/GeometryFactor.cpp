#include "stdafx.h"
#include "GeometryFactor.h"

using namespace std;
using namespace API;


API::GeometryFactor::~GeometryFactor()
{
}

std::shared_ptr<API::Mesh> API::GeometryFactor::CreateCube()
{
	std::shared_ptr<API::Mesh>ret = make_shared<Mesh>();
	ret->Initialize("Geometry/sq.yfm");
	return ret;
}

API::GeometryFactor::GeometryFactor()
{
}
