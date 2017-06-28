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
#ifndef VRTK_UI_H_
#define VRTK_UI_H_

#include "widget.h"

namespace vrtk {

class HitPoint;
class UIPriv;

class UI {
private:
	UIPriv *priv;

public:
	UI();
	~UI();

	void add_widget(Widget *w);
	bool remove_widget(Widget *w);

	bool contains(const Vec3 &pt) const;
	bool intersect(const Ray &ray, HitPoint *hit = 0) const;

	void draw() const;
};


}	// namespace vrtk

#endif	/* VRTK_UI_H_ */
