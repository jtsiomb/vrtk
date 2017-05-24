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
#ifndef VRTK_WIDGET_H_
#define VRTK_WIDGET_H_

#include <gmath/gmath.h>

namespace vrtk {

class WidgetPriv;
class Shape;

class Widget {
private:
	WidgetPriv *priv;

public:
	Widget();
	Widget(const Widget&) = delete;
	Widget &operator =(const Widget&) = delete;
	virtual ~Widget();

	virtual void set_position(const Vec3 &pos);
	virtual const Vec3 &get_position() const;
	virtual void set_rotation(const Quat &rot);
	virtual const Quat &get_rotation() const;
	virtual void set_scaling(const Vec3 &scale);
	virtual void set_scaling(float s);
	virtual const Vec3 &get_scaling() const;
	virtual const Mat4 &get_xform() const;

	virtual Widget *get_parent() const;
	virtual void add_child(Widget *c);
	virtual bool remove_child(Widget *c);
	virtual int num_children() const;
	virtual Widget *get_child(int idx) const;

	virtual void set_shape(Shape *s);
	virtual Shape *get_shape() const;

	virtual void draw() const = 0;
};


}	// namespace vrtk

#endif	/* VRTK_WIDGET_H_ */
