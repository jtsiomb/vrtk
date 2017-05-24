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
#include "geom.h"

namespace vrtk {

Sphere::Sphere()
{
	rad = 1.0f;
}

Sphere::Sphere(const Vec3 &pos, float rad)
	: pos(pos)
{
	this->rad = rad;
}

#define EPSILON	1e-5f

bool intersect(const Ray &ray, const Sphere &sph, HitPoint *hit)
{
	float a = dot(ray.dir, ray.dir);
	float b = 2.0f * dot(ray.dir, ray.origin - sph.pos);
	float c = dot(ray.origin, ray.origin) + dot(sph.pos, sph.pos) -
		2.0f * dot(ray.origin, sph.pos) - sph.rad * sph.rad;
	float d = b * b - (4.0f * a * c);

	if(d < EPSILON) return false;
	float sqrt_d = (float)sqrt(d);

	float t0 = (-b + sqrt_d) / (2.0f * a);
	float t1 = (-b - sqrt_d) / (2.0f * a);

	if(t1 < t0) {
		float tmp = t0;
		t0 = t1;
		t1 = tmp;
	}

	if(t0 < EPSILON) {
		if(t1 < EPSILON) return false;
		t0 = t1;
	}

	if(hit) {
		hit->t = t0;
		hit->pos = ray.origin + ray.dir * t0;
		hit->norm = normalize(hit->pos - sph.pos);
	}
	return true;
}

bool intersect(const Sphere &s1, const Sphere &s2, HitPoint *hit)
{
	Vec3 dir = s2.pos - s1.pos;
	float dsq = dot(dir, dir);
	float sumrad = s1.rad + s2.rad;

	if(dsq > sumrad * sumrad) {
		return false;
	}

	if(hit) {
		float dist = (float)sqrt(dsq);
		hit->norm = dir / dist;
		hit->pos = s1.pos + hit->norm * s1.rad;
		hit->t = -1.0f;	// invalid
	}
	return true;
}

float proj_point_line_param(const Vec3 &pt, const Ray &ray)
{
	Vec3 pdir = normalize(pt - ray.origin);
	return dot(pdir, ray.dir);
}

}	// namespace vrtk
