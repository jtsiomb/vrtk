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
#include "shape.h"

namespace vrtk {

class ShapePriv {
public:
	Widget *widget;
};

Shape::Shape()
{
	priv = new ShapePriv;
	priv->widget = 0;
}

Shape::~Shape()
{
	delete priv;
}

ShapeType Shape::get_type() const
{
	return SHAPE_UNKNOWN;
}

void Shape::set_widget(Widget *w)
{
	priv->widget = w;
}

Widget *Shape::get_widget() const
{
	return priv->widget;
}

void Shape::draw() const
{
}

}	// namespace vrtk
