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
#ifndef GEOM_H_
#define GEOM_H_

#include <gmath/gmath.h>

namespace vrtk {

struct HitPoint {
	float t;
	Vec3 pos, norm;
};


class Sphere {
public:
	Vec3 pos;
	float rad;

	Sphere();
	Sphere(const Vec3 &pos, float rad);
};

bool intersect(const Ray &ray, const Sphere &sph, HitPoint *hit = 0);
bool intersect(const Sphere &s1, const Sphere &s2, HitPoint *hit = 0);

float proj_point_line_param(const Vec3 &pt, const Ray &ray);

}	// namespace vrtk

#endif	/* GEOM_H_ */
