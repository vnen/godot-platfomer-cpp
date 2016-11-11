/*************************************************************************/
/*  bullet.cpp                                                           */
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

#include "bullet.h"
#include "object_type_db.h"
#include "scene/animation/animation_player.h"

void Bullet::_on_bullet_body_enter(Object * p_body) {

	if (p_body->has_method("hit_by_bullet")) {
		p_body->callv("hit_by_bullet", Array());
	}
}

void Bullet::_on_Timer_timeout() {

	if(has_node(NodePath("anim")) && get_node(NodePath("anim"))->cast_to<AnimationPlayer>())
		get_node(NodePath("anim"))->cast_to<AnimationPlayer>()->play("shutdown");
}

void Bullet::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("_on_bullet_body_enter", "body"), &Bullet::_on_bullet_body_enter);
	ObjectTypeDB::bind_method(_MD("_on_Timer_timeout"), &Bullet::_on_Timer_timeout);
}