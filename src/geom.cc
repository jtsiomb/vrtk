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
#include <float.h>
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

bool intersect(const Ray &ray, const Cylinder &cyl, HitPoint *hit)
{
	/* construct an orthonormal basis, to transform ray to a cylinder-friendly
	 * coordinate system (cylinder axis aligned with the Y axis).
	 */
	Vec3 vj = normalize(cyl.end[1] - cyl.end[0]);
	Vec3 vk = Vec3(0, 0, 1);
	if(fabs(dot(vj, vk)) < 1e-3) {
		vk = Vec3(1, 0, 0);
	}
	Vec3 vi = normalize(cross(vj, vk));
	vk = cross(vi, vj);

	Mat4 xform = Mat4(vi, vj, vk);
	Mat4 inv_xform = transpose(xform);

	Ray lray = inv_xform * ray;

	float a = lray.dir.x * lray.dir.x + lray.dir.z * lray.dir.z;
	float b = 2.0 * lray.dir.x * lray.origin.x + 2.0 * lray.dir.z * lray.origin.z;
	float c = lray.origin.x * lray.origin.x + lray.origin.z * lray.origin.z - cyl.rad * cyl.rad;
	float d = b * b - 4.0 * a * c;

	if(d < 0.0) {
		return false;
	}
	float sqrt_d = sqrt(d);
	float t1 = (-b + sqrt_d) / (2.0 * a);
	float t2 = (-b + sqrt_d) / (2.0 * a);

	if(t1 < EPSILON && t2 < EPSILON) {
		return false;
	}

	Vec3 p1 = lray.origin + lray.dir * t1;
	Vec3 p2 = lray.origin + lray.dir * t2;

	unsigned int valid = 3;
	if(t1 < EPSILON || p1.y < cyl.end[0].y || p1.y >= cyl.end[1].y) {
		valid &= ~1;
		t1 = t2;
	}
	if(t2 < EPSILON || p2.y < cyl.end[0].y || p2.y >= cyl.end[1].y) {
		valid &= ~2;
		t2 = t1;
	}

	if(!valid) {
		return false;
	}

	if(hit) {
		hit->t = t1 < t2 ? t1 : t2;

		hit->pos = ray.origin + ray.dir * hit->t;
		hit->norm = Vec3(1.0, 0.0, 1.0) * hit->pos / cyl.rad;

		hit->norm = xform.upper3x3() * hit->norm;
	}
	return true;
}

bool intersect(const Ray &ray, const AABox &box, HitPoint *hit)
{
	Vec3 param[2] = {box.min, box.max};
	Vec3 inv_dir(1.0 / ray.dir.x, 1.0 / ray.dir.y, 1.0 / ray.dir.z);
	int sign[3] = {inv_dir.x < 0, inv_dir.y < 0, inv_dir.z < 0};

	float tmin = (param[sign[0]].x - ray.origin.x) * inv_dir.x;
	float tmax = (param[1 - sign[0]].x - ray.origin.x) * inv_dir.x;
	float tymin = (param[sign[1]].y - ray.origin.y) * inv_dir.y;
	float tymax = (param[1 - sign[1]].y - ray.origin.y) * inv_dir.y;

	if(tmin > tymax || tymin > tmax) {
		return false;
	}
	if(tymin > tmin) {
		tmin = tymin;
	}
	if(tymax < tmax) {
		tmax = tymax;
	}

	float tzmin = (param[sign[2]].z - ray.origin.z) * inv_dir.z;
	float tzmax = (param[1 - sign[2]].z - ray.origin.z) * inv_dir.z;

	if(tmin > tzmax || tzmin > tmax) {
		return false;
	}
	if(tzmin > tmin) {
		tmin = tzmin;
	}
	if(tzmax < tmax) {
		tmax = tzmax;
	}

	float t = tmin < 1e-4 ? tmax : tmin;
	if(t >= 1e-4) {

		if(hit) {
			hit->t = t;
			hit->pos = ray.origin + ray.dir * t;

			float min_dist = FLT_MAX;
			Vec3 offs = box.min + (box.max - box.min) / 2.0;
			Vec3 local_hit = hit->pos - offs;

			static const Vec3 axis[] = {
				Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1)
			};
			//int tcidx[][2] = {{2, 1}, {0, 2}, {0, 1}};

			for(int i=0; i<3; i++) {
				float dist = fabs((box.max[i] - offs[i]) - fabs(local_hit[i]));
				if(dist < min_dist) {
					min_dist = dist;
					hit->norm = axis[i] * (local_hit[i] < 0.0 ? 1.0 : -1.0);
					//hit->texcoord = Vec2(hit->pos[tcidx[i][0]], hit->pos[tcidx[i][1]]);
				}
			}
		}
		return true;
	}
	return false;

}

float proj_point_line_param(const Vec3 &pt, const Ray &ray)
{
	Vec3 pdir = normalize(pt - ray.origin);
	return dot(pdir, ray.dir);
}

}	// namespace vrtk
