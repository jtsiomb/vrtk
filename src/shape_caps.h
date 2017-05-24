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
#ifndef VRTK_SHAPE_CAPS_H_
#define VRTK_SHAPE_CAPS_H_

#include "shape.h"

namespace vrtk {

class ShapeCapsPriv;

class ShapeCaps : public Shape {
private:
	ShapeCapsPriv *priv;

public:
	ShapeCaps();
	ShapeCaps(const Vec3 &a, const Vec3 &b, float rad);
	~ShapeCaps();

	ShapeType get_type() const;

	void set_capsule(const Vec3 &a, const Vec3 &b, float rad);
	void set_end(int idx, const Vec3 &v);
	void set_radius(float r);

	const Vec3 &get_end(int idx) const;
	const float get_radius() const;

	const Vec3 &get_axis() const;

	bool contains(const Vec3 &pt) const;
	bool intersect(const Sphere &sph, HitPoint *hit = 0) const;
	bool intersect(const Ray &ray, HitPoint *hit = 0) const;

	void draw() const;
};

}	// namespace vrtk

#endif	/* VRTK_SHAPE_CAPS_H_ */
