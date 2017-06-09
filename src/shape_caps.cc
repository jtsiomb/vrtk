/*
vrtk - 3D widget toolkit for VR user interfaces
Copyright (C) 2017 John Tsiombikas <nuclear@member.fsf.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <gmath/gmath.h>
#include "shape_caps.h"
#include "geom.h"

namespace vrtk {

class ShapeCapsPriv {
public:
	Vec3 end[2];
	float rad;

	Vec3 axis;	// end[1] - end[0]
	float axis_len;
	bool derived_valid;
};

static void update_derived(ShapeCapsPriv *priv);

ShapeCaps::ShapeCaps()
{
	priv = new ShapeCapsPriv;
	set_capsule(Vec3(0, 0, 0), Vec3(0, 0, 0), 1.0);
}

ShapeCaps::ShapeCaps(const Vec3 &a, const Vec3 &b, float rad)
{
	priv = new ShapeCapsPriv;
	set_capsule(a, b, rad);
}

ShapeCaps::~ShapeCaps()
{
	delete priv;
}

ShapeType ShapeCaps::get_type() const
{
	return SHAPE_CAPSULOID;
}

void ShapeCaps::set_capsule(const Vec3 &a, const Vec3 &b, float rad)
{
	priv->end[0] = a;
	priv->end[1] = b;
	priv->rad = rad;
	priv->derived_valid = false;
}

void ShapeCaps::set_end(int idx, const Vec3 &v)
{
	priv->end[idx] = v;
	priv->derived_valid = false;
}

void ShapeCaps::set_radius(float r)
{
	priv->rad = r;
}

const Vec3 &ShapeCaps::get_end(int idx) const
{
	return priv->end[idx];
}

const float ShapeCaps::get_radius() const
{
	return priv->rad;
}

const Vec3 &ShapeCaps::get_axis() const
{
	update_derived(priv);
	return priv->axis;
}

bool ShapeCaps::contains(const Vec3 &pt) const
{
	float radsq = priv->rad * priv->rad;
	float t = proj_point_line_param(pt, Ray(priv->end[0], priv->axis));
	if(t < 0.0) {
		return length_sq(priv->end[0] - pt) <= radsq;
	}
	if(t > 1.0) {
		return length_sq(priv->end[1] - pt) <= radsq;
	}
	Vec3 projpt = priv->end[0] + priv->axis * t;
	return length_sq(projpt - pt) <= radsq;
}

/* intersection between sphere and capsuloid is the same as containment of a
 * point in a capsuloid of increased radius by as much as the radius of the
 * sphere
 */
bool ShapeCaps::intersect(const Sphere &sph, HitPoint *hit) const
{
	float rad = priv->rad + sph.rad;
	float radsq = rad * rad;
	float t = proj_point_line_param(sph.pos, Ray(priv->end[0], priv->axis));
	if(t < 0.0) {
		return length_sq(priv->end[0] - sph.pos) <= radsq;
	}
	if(t > 1.0) {
		return length_sq(priv->end[1] - sph.pos) <= radsq;
	}
	Vec3 projpt = priv->end[0] + priv->axis * t;
	return length_sq(projpt - sph.pos) <= radsq;
}

bool ShapeCaps::intersect(const Ray &ray, HitPoint *hit) const
{
}

void ShapeCaps::draw() const
{
}

static void update_derived(ShapeCapsPriv *priv)
{
	if(priv->derived_valid) return;

	priv->axis = priv->end[1] - priv->end[0];
	priv->axis_len = length(priv->axis);
	priv->derived_valid = true;
}

}	// namespace vrtk
