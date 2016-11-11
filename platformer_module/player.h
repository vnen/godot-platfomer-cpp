/*************************************************************************/
/*  player.h                                                             */
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

#ifndef PLAYER_H
#define PLAYER_H

#include "scene/2d/physics_body_2d.h"

struct Vector2;
class String;
class Sprite;

class Player : public KinematicBody2D {

	OBJ_TYPE(Player, KinematicBody2D);

public:

	const Vector2 GRAVITY_VEC;
	const Vector2 FLOOR_NORMAL;
	const double SLOPE_SLIDE_STOP;
	const double MIN_ONAIR_TIME;
	const int WALK_SPEED;
	const int JUMP_SPEED;
	const int SIDING_CHANGE_SPEED;
	const int BULLET_VELOCITY;
	const double SHOOT_TIME_SHOW_WEAPON;

	Vector2 linear_vel;
	double onair_time;
	bool on_floor;
	double shoot_time;

	String anim;

	Sprite *sprite;

	void _notification(int p_what);

	Player();

protected:

	static void _bind_methods();
};

#endif // !PLAYER_H
