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
#include <vector>
#include "widgetgroup.h"
#include "shape.h"
#include "geom.h"

namespace vrtk {

class WidgetGroupPriv {
public:
	std::vector<Widget*> widgets;
};

WidgetGroup::WidgetGroup()
{
	priv = new WidgetGroupPriv;
}

WidgetGroup::~WidgetGroup()
{
	int num = priv->widgets.size();
	for(int i=0; i<num; i++) {
		delete priv->widgets[i];
	}
	delete priv;
}

void WidgetGroup::add_widget(Widget *w)
{
	int num = priv->widgets.size();
	for(int i=0; i<num; i++) {
		if(priv->widgets[i] == w) return;
	}

	priv->widgets.push_back(w);
}

bool WidgetGroup::remove_widget(Widget *w)
{
	int num = priv->widgets.size();
	for(int i=0; i<num; i++) {
		if(priv->widgets[i] == w) {
			priv->widgets.erase(priv->widgets.begin() + i);
			return true;
		}
	}
	return false;
}

bool WidgetGroup::contains(const Vec3 &pt) const
{
	int num = priv->widgets.size();
	for(int i=0; i<num; i++) {
		Shape *shape = priv->widgets[i]->get_shape();
		if(shape->contains(pt)) {
			return true;
		}
	}
	return false;
}

bool WidgetGroup::intersect(const Ray &ray, HitPoint *hit) const
{
	HitPoint nearest;
	nearest.obj = 0;
	nearest.t = FLT_MAX;

	int num = priv->widgets.size();
	for(int i=0; i<num; i++) {
		Shape *shape = priv->widgets[i]->get_shape();
		if(shape->intersect(ray, hit) && hit && hit->t < nearest.t) {
			nearest = *hit;
		}
	}

	if(nearest.obj && hit) {
		*hit = nearest;
	}
	return nearest.obj != 0;
}

void WidgetGroup::draw() const
{
	int num = priv->widgets.size();
	for(int i=0; i<num; i++) {
		priv->widgets[i]->draw();
	}
}

}	// namespace vrtk
