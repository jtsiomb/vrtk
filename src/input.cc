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
#include "input.h"
#include "widget.h"

namespace vrtk {

static enum PtrMode { PTR_RAY, PTR_3D } ptr_mode;
static Ray ptr_ray;
static Vec3 ptr_pos;
static Quat ptr_rot;

static Widget *kbfocus;

void set_keyboard_focus(Widget *w)
{
	if(kbfocus) {
		kbfocus->on_input_focus(false);
	}
	kbfocus = w;
	w->on_input_focus(true);
}

void input_keyboard(int key, bool pressed)
{
	if(kbfocus) {
		if(pressed) {
			kbfocus->on_key_press(key);
		} else {
			kbfocus->on_key_release(key);
		}
	}
}

void input_ray_pointer(const Vec3 &origin, const Vec3 &dir)
{
	ptr_mode = PTR_RAY;
	ptr_ray.origin = origin;
	ptr_ray.dir = dir;

	// TODO test widgets and generate events
}

void input_3d_pointer(const Vec3 &pos, const Quat &rot)
{
	ptr_mode = PTR_3D;
	ptr_pos = pos;
	ptr_rot = rot;

	// TODO test widgets and generate events
}

void input_button(int bn, bool pressed)
{
	// TODO generate events
}

} // namespace vrtk
