/*************************************************************************/
/*  enemy.cpp                                                            */
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

#include "enemy.h"
#include "core/math/math_2d.h"
#include "scene/animation/animation_player.h"
#include "object_type_db.h"

void Enemy::_notification(int p_what) {

	if (!is_inside_tree() || get_tree()->is_editor_hint()) return;

	switch (p_what) {
		case NOTIFICATION_READY: {
			detect_floor_left = get_node(NodePath("detect_floor_left"))->cast_to<RayCast2D>();
			detect_wall_left = get_node(NodePath("detect_wall_left"))->cast_to<RayCast2D>();
			detect_floor_right = get_node(NodePath("detect_floor_right"))->cast_to<RayCast2D>();
			detect_wall_right = get_node(NodePath("detect_wall_right"))->cast_to<RayCast2D>();
			sprite = get_node(NodePath("sprite"))->cast_to<Sprite>();

			if (sprite) {
				set_fixed_process(true);
			}

		} break;

		case NOTIFICATION_FIXED_PROCESS: {

			String new_anim("idle");

			if (state == STATE_WALKING) {

				linear_velocity += GRAVITY_VET * get_fixed_process_delta_time();
				linear_velocity.x = direction * WALK_SPEED;
				linear_velocity = move_and_slide(linear_velocity, FLOOR_NORMAL);

				if (!detect_floor_left->is_colliding() || detect_wall_left->is_colliding()) {
					direction = 1;
				}
				if (!detect_floor_right->is_colliding() || detect_wall_right->is_colliding()) {
					direction = -1;
				}

				sprite->set_scale(Vector2(direction, 1.0));

				new_anim = "walk";
			} else {
				new_anim = "explode";
			}

			if (anim != new_anim) {
				anim = new_anim;
				if(has_node(NodePath("anim")) && get_node(NodePath("anim"))->cast_to<AnimationPlayer>())
					get_node(NodePath("anim"))->cast_to<AnimationPlayer>()->play(anim);
			}
		} break;
	}
}

void Enemy::hit_by_bullet() {
	state = STATE_KILLED;
}

Enemy::Enemy() 
	: GRAVITY_VET(0, 900),
	  FLOOR_NORMAL(0, -1),
	  WALK_SPEED(70)
{
	state = STATE_WALKING;
	linear_velocity = Vector2();
	direction = -1;
	anim = "";
}

void Enemy::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("hit_by_bullet"), &Enemy::hit_by_bullet);
}
