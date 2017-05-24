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
};

Widget::Widget()
{
	priv = new WidgetPriv;
	priv->parent = 0;
	priv->scale = Vec3(1, 1, 1);
	priv->shape = 0;
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


}	// namespace vrtk
