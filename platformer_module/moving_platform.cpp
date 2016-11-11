/*************************************************************************/
/*  moving_platform.cpp                                                  */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2016 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "moving_platform.h"
#include "object_type_db.h"
#include "core/math/math_funcs.h"
#include "core/math/math_2d.h"
#include "scene/2d/physics_body_2d.h"

void MovingPlatform::_notification(int p_what) {

	if (!is_inside_tree() || get_tree()->is_editor_hint()) return;

	switch (p_what) {

		case NOTIFICATION_READY: {
			set_fixed_process(true);
		} break;
		case NOTIFICATION_FIXED_PROCESS: {

			accum += get_fixed_process_delta_time() * (1.0 / cycle) * Math_PI * 2.0;
			accum = Math::fmod(accum, Math_PI * 2.0);
			double d = Math::sin(accum);
			Matrix32 xf;
			xf[2] = motion * d;
			if(has_node(NodePath("platform")) && get_node(NodePath("platform"))->cast_to<RigidBody2D>())
				get_node(NodePath("platform"))->cast_to<RigidBody2D>()->set_transform(xf);
		}
	}
}

void MovingPlatform::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("set_motion", "motion:Vector2"), &MovingPlatform::set_motion);
	ObjectTypeDB::bind_method(_MD("get_motion:Vector2"), &MovingPlatform::get_motion);

	ObjectTypeDB::bind_method(_MD("set_cycle", "cycle:float"), &MovingPlatform::set_cycle);
	ObjectTypeDB::bind_method(_MD("get_cycle:float"), &MovingPlatform::get_cycle);

	ADD_PROPERTYNZ(PropertyInfo(Variant::VECTOR2, "motion"), _SCS("set_motion"), _SCS("get_motion"));
	ADD_PROPERTYNZ(PropertyInfo(Variant::REAL, "cycle"), _SCS("set_cycle"), _SCS("get_cycle"));
}

void MovingPlatform::set_motion(const Vector2 & p_motion) {

	motion = p_motion;
}

Vector2 MovingPlatform::get_motion() const {

	return motion;
}

void MovingPlatform::set_cycle(double p_cycle) {

	cycle = p_cycle;
}

double MovingPlatform::get_cycle() const {
	return cycle;
}

MovingPlatform::MovingPlatform() {

	motion = Vector2();
	cycle = 1.0;
	accum = 0.0;
}
