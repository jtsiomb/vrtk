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
#include <vector>
#include <algorithm>
#include "widget.h"
#include "shape.h"

namespace vrtk {

class WidgetPriv {
public:
	Widget *parent;
	std::vector<Widget*> children;

	Vec3 pos, scale;
	Quat rot;
	Mat4 xform;
	bool xform_valid;

	Shape *shape;

	void (*draw_func)(const Widget*, void*);
	void *draw_func_cls;

	BoolAnim visible, focused, hover, grabbed, active;
	Vec3 grab_pos;
	Quat grab_rot;
};

Widget::Widget()
{
	priv = new WidgetPriv;
	priv->parent = 0;
	priv->scale = Vec3(1, 1, 1);
	priv->shape = 0;
	priv->draw_func = 0;
	priv->draw_func_cls = 0;
}

Widget::~Widget()
{
	delete priv;
}


void Widget::set_position(const Vec3 &pos)
{
	priv->pos = pos;
	priv->xform_valid = false;
}

const Vec3 &Widget::get_position() const
{
	return priv->pos;
}

void Widget::set_rotation(const Quat &rot)
{
	priv->rot = rot;
	priv->xform_valid = false;
}

const Quat &Widget::get_rotation() const
{
	return priv->rot;
}

void Widget::set_scaling(const Vec3 &scale)
{
	priv->scale = scale;
	priv->xform_valid = false;
}

void Widget::set_scaling(float s)
{
	priv->scale = Vec3(s, s, s);
	priv->xform_valid = false;
}

const Vec3 &Widget::get_scaling() const
{
	return priv->scale;
}

const Mat4 &Widget::get_xform() const
{
	if(!priv->xform_valid) {
		priv->xform.translation(priv->pos);
		priv->xform *= priv->rot.calc_matrix();
		priv->xform.scale(priv->scale);
	}
	return priv->xform;
}

Widget *Widget::get_parent() const
{
	return priv->parent;
}

void Widget::add_child(Widget *c)
{
	Widget *cpar = c->get_parent();
	if(cpar != this) {
		if(cpar) {
			cpar->remove_child(c);
		}
		priv->children.push_back(c);
		c->priv->parent = this;
	}
}

bool Widget::remove_child(Widget *c)
{
	Widget *cpar = c->get_parent();
	if(cpar == this) {
		auto it = std::find(priv->children.begin(), priv->children.end(), c);
		if(it != priv->children.end()) {
			priv->children.erase(it);
			c->priv->parent = 0;
			return true;
		}
	}
	return false;
}

int Widget::num_children() const
{
	return (int)priv->children.size();
}

Widget *Widget::get_child(int idx) const
{
	if(idx < 0 || idx >= num_children()) {
		return 0;
	}
	return priv->children[idx];
}


void Widget::set_shape(Shape *s)
{
	priv->shape = s;
}

Shape *Widget::get_shape() const
{
	return priv->shape;
}

void Widget::set_draw_func(void (*func)(const Widget*, void*), void *cls)
{
	priv->draw_func = func;
	priv->draw_func_cls = cls;
}

void Widget::draw() const
{
	if(priv->draw_func) {
		priv->draw_func(this, priv->draw_func_cls);
	} else if(priv->shape) {
		priv->shape->draw();
	}
}

BoolAnim &Widget::visible()
{
	return priv->visible;
}

BoolAnim &Widget::focused()
{
	return priv->focused;
}

BoolAnim &Widget::hover()
{
	return priv->hover;
}

BoolAnim &Widget::dragged()
{
	return priv->grabbed;
}

BoolAnim &Widget::active()
{
	return priv->active;
}

/* the base Widget class implements all events as empty functions instead
 * of pure virtual ones, to allow the subclasses to override them selectively
 * and leave the rest as nops.
 */
void Widget::on_input_focus(bool focus)
{
	priv->focused = focus;
}

void Widget::on_key_press(int key)
{
}

void Widget::on_key_release(int key)
{
}


void Widget::on_hover(bool hover)
{
	priv->hover = hover;
}

void Widget::on_grab(const Vec3 &pos, const Quat &rot)
{
	priv->grabbed = true;
	priv->grab_pos = pos;
	priv->grab_rot = rot;
}

void Widget::on_release(const Vec3 &pos, const Quat &rot)
{
	priv->grabbed = false;
}

void Widget::on_drag(const Vec3 &pos, const Quat &rot)
{
	priv->grab_pos = pos;
	priv->grab_rot = rot;
}


void Widget::on_activate(const Vec3 &pos, const Quat &rot)
{
	// TODO remind me how goatkit handled activation/deactivation
	priv->active = true;
}


}	// namespace vrtk
