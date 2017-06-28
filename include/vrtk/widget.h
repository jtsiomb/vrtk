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
#include "boolanm.h"

namespace vrtk {

class WidgetPriv;
class Shape;

class Widget {
protected:
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

	virtual void set_draw_func(void (*func)(const Widget*, void*), void *cls = 0);

	virtual void draw() const;

	// ---- state ----
	virtual BoolAnim &visible();
	virtual BoolAnim &focused();
	virtual BoolAnim &hover();
	virtual BoolAnim &dragged();
	virtual BoolAnim &active();

	// ---- events ----
	virtual void on_input_focus(bool focus);
	virtual void on_key_press(int key);
	virtual void on_key_release(int key);

	virtual void on_hover(bool hover);
	virtual void on_grab(const Vec3 &pos, const Quat &rot);
	virtual void on_release(const Vec3 &pos, const Quat &rot);
	virtual void on_drag(const Vec3 &pos, const Quat &rot);

	/* a grab, followed by a release without "significant" change of
	 * position and orientation (see: set_drag_threshold in input.h)
	 * causes an activation instead of a drag.
	 */
	virtual void on_activate(const Vec3 &pos, const Quat &rot);
};


}	// namespace vrtk

#endif	/* VRTK_WIDGET_H_ */
