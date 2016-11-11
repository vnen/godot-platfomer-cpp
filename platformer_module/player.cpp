/*************************************************************************/
/*  player.cpp                                                           */
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

#include "player.h"
#include "bullet.h"
#include "scene/2d/sprite.h"
#include "main/input_default.h"
#include "core/math/math_funcs.h"
#include "scene/2d/sample_player_2d.h"
#include "scene/resources/packed_scene.h"
#include "scene/2d/position_2d.h"
#include "scene/animation/animation_player.h"

void Player::_notification(int p_what) {

	if (!is_inside_tree() || get_tree()->is_editor_hint()) return;

	switch (p_what) {
		case NOTIFICATION_READY: {
			sprite = get_node(NodePath("sprite"))->cast_to<Sprite>();

			if (sprite) {
				set_fixed_process(true);
			}
		} break;

		case NOTIFICATION_FIXED_PROCESS: {

			double delta = get_fixed_process_delta_time();

			// increment counters
			onair_time += delta;
			shoot_time += delta;

			// MOVEMENT

			// Apply Gravity
			linear_vel += delta * GRAVITY_VEC;
			// Move and Slide
			linear_vel = move_and_slide(linear_vel, FLOOR_NORMAL, SLOPE_SLIDE_STOP);
			// Detect Floor
			if (is_move_and_slide_on_floor()) {
				onair_time = 0;
			}

			on_floor = onair_time < MIN_ONAIR_TIME;

			// CONTROL

			// Horizontal Movement
			int target_speed = 0;
			if (Input::get_singleton()->is_action_pressed("move_left")) {
				target_speed += -1;
			}
			if (Input::get_singleton()->is_action_pressed("move_right")) {
				target_speed += 1;
			}

			target_speed *= WALK_SPEED;
			linear_vel.x = Math::lerp(linear_vel.x, target_speed, 0.1);

			// Jumping
			if (on_floor && Input::get_singleton()->is_action_just_pressed("jump")) {
				linear_vel.y = -JUMP_SPEED;
				SamplePlayer2D *sound = has_node(NodePath("sound")) ? get_node(NodePath("sound"))->cast_to<SamplePlayer2D>() : NULL;
				if (sound != NULL) {
					sound->play("jump");
				}
			}

			// Shooting

			if (Input::get_singleton()->is_action_just_pressed("shoot")) {

				RES b = ResourceLoader::load("res://bullet.tscn", "PackedScene");
				if (b.is_valid()) {
					Bullet *bullet = b->cast_to<PackedScene>()->instance()->cast_to<Bullet>();

					// use node for shoot position
					if(has_node(NodePath("sprite/bullet_shoot")) && get_node(NodePath("sprite/bullet_shoot"))->cast_to<Position2D>())
						bullet->set_pos(get_node(NodePath("sprite/bullet_shoot"))->cast_to<Position2D>()->get_global_pos());

					if(sprite)
						bullet->set_linear_velocity(Vector2(sprite->get_scale().x * BULLET_VELOCITY, 0));

					bullet->add_collision_exception_with(this); //don't want player to collide with bullet

					get_parent()->add_child(bullet); // don't want bullet to move with me, so add it as child of parent

					if(has_node(NodePath("sound")) && get_node(NodePath("sound"))->cast_to<SamplePlayer2D>())
						get_node(NodePath("sound"))->cast_to<SamplePlayer2D>()->play("shoot");

					shoot_time = 0;
				}
			}

			// ANIMATION

			String new_anim("idle");

			if (on_floor) {
				if (linear_vel.x < -SIDING_CHANGE_SPEED) {
					sprite->set_scale(Vector2(-1, 1));
					new_anim = "run";
				}

				if (linear_vel.x > SIDING_CHANGE_SPEED) {
					sprite->set_scale(Vector2(1, 1));
					new_anim = "run";
				}
			} else {
				if (linear_vel.y < 0) {
					new_anim = "jumping";
				} else {
					new_anim = "falling";
				}
			}

			if (shoot_time < SHOOT_TIME_SHOW_WEAPON) {
				new_anim += "_weapon";
			}

			if (new_anim != anim) {
				anim = new_anim;
				if(has_node(NodePath("anim")) && get_node(NodePath("anim"))->cast_to<AnimationPlayer>())
					get_node(NodePath("anim"))->cast_to<AnimationPlayer>()->play(anim);
			}

		} break;
	}
}

Player::Player() 
	: GRAVITY_VEC(0, 900),
	FLOOR_NORMAL(0, -1),
	SLOPE_SLIDE_STOP(25.0),
	MIN_ONAIR_TIME(0.1),
	WALK_SPEED(250),
	JUMP_SPEED(480),
	SIDING_CHANGE_SPEED(10),
	BULLET_VELOCITY(1000),
	SHOOT_TIME_SHOW_WEAPON(0.2)
{
	linear_vel = Vector2();
	onair_time = 0;
	on_floor = false;
	shoot_time = 99999;

	anim = "";
}

void Player::_bind_methods() {}
