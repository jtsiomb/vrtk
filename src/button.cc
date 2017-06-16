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
#include "button.h"
#include "shape_caps.h"

namespace vrtk {

class ButtonPriv {
public:
};

Button::Button()
{
	priv = new ButtonPriv;

	ShapeCaps *shape = new ShapeCaps(Vec3(-1, 0, 0), Vec3(1, 0, 0), 0.5);
	set_shape(shape);
}

Button::~Button()
{
	delete priv;
}

}	// namespace vrtk
