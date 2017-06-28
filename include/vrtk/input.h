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
#ifndef VRTK_INPUT_H_
#define VRTK_INPUT_H_

#include <gmath/gmath.h>

namespace vrtk {

/* KEY_* values match X keysyms */
enum {
	KEY_ESC = 27,
	KEY_BACKSPACE = '\b',
	KEY_DELETE = 127,
	KEY_HOME = 0xff50,
	KEY_LEFT = 0xff51,
	KEY_UP = 0xff52,
	KEY_RIGHT = 0xff53,
	KEY_DOWN = 0xff54,
	KEY_PGUP = 0xff55,
	KEY_PGDOWN = 0xff56,
	KEY_END = 0xff57,

	KEY_LSHIFT = 0xffe1,
	KEY_RSHIFT = 0xffe2,
	KEY_LCTRL = 0xffe3,
	KEY_RCTRL = 0xffe4,
	KEY_LALT = 0xffe9,
	KEY_RALT = 0xffea
};

class Widget;

/* distance threshold from initial grab that must be exceeded
 * in order to start a drag, as opposed to triggering an activation
 * when the widget is released
 */
void set_drag_threshold(float dist);
float get_drag_threshold();

void set_keyboard_focus(Widget *w);
Widget *get_keyboard_focus();

void input_keyboard(int key, bool pressed);

void input_ray_pointer(const Vec3 &origin, const Vec3 &dir);
void input_3d_pointer(const Vec3 &pos, const Quat &rot = Quat::identity);

void input_button(int bn, bool pressed);

}	// namespace vrtk

#endif	/* VRTK_INPUT_H_ */
