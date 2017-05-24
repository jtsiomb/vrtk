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
#ifndef VRTK_SHAPE_H_
#define VRTK_SHAPE_H_

#include <gmath/gmath.h>

namespace vrtk {

class Widget;
class Sphere;
class HitPoint;
class ShapePriv;

enum ShapeType {
	SHAPE_UNKNOWN,
	SHAPE_CAPSULOID,
	SHAPE_MESH
};

class Shape {
private:
	ShapePriv *priv;

public:
	Shape();
	virtual ~Shape();

	virtual ShapeType get_type() const;

	virtual void set_widget(Widget *w);
	virtual Widget *get_widget() const;

	virtual bool contains(const Vec3 &pt) const = 0;
	virtual bool intersect(const Sphere &sph, HitPoint *hit = 0) const = 0;
	virtual bool intersect(const Ray &ray, HitPoint *hit = 0) const = 0;

	virtual void draw() const;
};

}	// namespace vrtk

#endif	/* VRTK_SHAPE_H_ */
