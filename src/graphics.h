#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <SDL.h>
#include "n2DLib.h"

#ifndef COMPILED
extern Texture_nKaruga * image_null;
extern Texture_nKaruga * image_titleScreen;
extern Texture_nKaruga * image_kanji_1;
extern Texture_nKaruga * image_kanji_2;
extern Texture_nKaruga * image_end_of_demo;
extern Texture_nKaruga * image_bossWarning;
extern Texture_nKaruga * image_bg1_0;
extern Texture_nKaruga * image_bg1_1;
extern Texture_nKaruga * image_bg1_2;
extern Texture_nKaruga * image_bg1_3;
extern Texture_nKaruga * image_bg1_4;
extern Texture_nKaruga * image_bg2_0;
extern Texture_nKaruga * image_bg2_1;
extern Texture_nKaruga * image_bg2_2;
extern Texture_nKaruga * image_bg2_3;
extern Texture_nKaruga * image_bg2_4;
extern Texture_nKaruga * image_bg2_5;
extern Texture_nKaruga * image_bg2_6;
extern Texture_nKaruga * image_bg2_7;

/*
 * Boss 1
 */

extern Texture_nKaruga * bossImage_1_body;
extern Texture_nKaruga * bossImage_1_leftarm_armed;
extern Texture_nKaruga * bossImage_1_rightarm_armed1;
extern Texture_nKaruga * bossImage_1_rightarm_armed2;
extern Texture_nKaruga * bossImage_1_leftarm_nonarmed;
extern Texture_nKaruga * bossImage_1_rightarm_nonarmed;

extern Texture_nKaruga * image_boss1_grenade_light;
extern Texture_nKaruga * image_boss1_grenade_shadow;
extern Texture_nKaruga * image_boss1_enemy_ship_shadow;
extern Texture_nKaruga * image_boss1_enemy_ship_light;


/*
 * Boss 2
 */

extern Texture_nKaruga * bossImage_2_body;
extern Texture_nKaruga * bossImage_2_leftShield;
extern Texture_nKaruga * bossImage_2_rightShield;
extern Texture_nKaruga * bossImage_2_leftArm;
extern Texture_nKaruga * bossImage_2_rightArm;
extern Texture_nKaruga * bossImage_2_leftUpperArm;
extern Texture_nKaruga * bossImage_2_rightUpperArm;
extern Texture_nKaruga * bossImage_2_leftWing;
extern Texture_nKaruga * bossImage_2_leftWingOpening_1;
extern Texture_nKaruga * bossImage_2_leftWingOpening_2;
extern Texture_nKaruga * bossImage_2_leftWingOpening_3;
extern Texture_nKaruga * bossImage_2_leftWingOpening_4;
extern Texture_nKaruga * bossImage_2_rightWing;
extern Texture_nKaruga * bossImage_2_rightWingOpening_1;
extern Texture_nKaruga * bossImage_2_rightWingOpening_2;
extern Texture_nKaruga * bossImage_2_rightWingOpening_3;
extern Texture_nKaruga * bossImage_2_rightWingOpening_4;
extern Texture_nKaruga * bossImage_2_rails;
extern Texture_nKaruga * bossImage_2_hitpoint_left_1;
extern Texture_nKaruga * bossImage_2_hitpoint_left_2;
extern Texture_nKaruga * bossImage_2_hitpoint_left_3;
extern Texture_nKaruga * bossImage_2_hitpoint_left_4;
extern Texture_nKaruga * bossImage_2_hitpoint_left_5;
extern Texture_nKaruga * bossImage_2_hitpoint_right_1;
extern Texture_nKaruga * bossImage_2_hitpoint_right_2;
extern Texture_nKaruga * bossImage_2_hitpoint_right_3;
extern Texture_nKaruga * bossImage_2_hitpoint_right_4;
extern Texture_nKaruga * bossImage_2_hitpoint_right_5;


extern Texture_nKaruga * image_boss2_leftShield;
extern Texture_nKaruga * image_boss2_rightShield;
extern Texture_nKaruga * image_boss2_leftUpperArm;


/* GFX */

extern Texture_nKaruga * image_enemy_laser_light;
extern Texture_nKaruga * image_enemy_laser_shadow;

extern Texture_nKaruga * image_box_light_1;
extern Texture_nKaruga * image_box_light_2;
extern Texture_nKaruga * image_box_light_3;
extern Texture_nKaruga * image_box_light_4;
extern Texture_nKaruga * image_box_light_5;
extern Texture_nKaruga * image_box_light_6;
extern Texture_nKaruga * image_box_shadow_1;
extern Texture_nKaruga * image_box_shadow_2;
extern Texture_nKaruga * image_box_shadow_3;
extern Texture_nKaruga * image_box_shadow_4;
extern Texture_nKaruga * image_box_shadow_5;
extern Texture_nKaruga * image_box_shadow_6;
extern Texture_nKaruga * image_box_solid_1;
extern Texture_nKaruga * image_box_solid_2;
extern Texture_nKaruga * image_box_solid_3;
extern Texture_nKaruga * image_box_solid_4;
extern Texture_nKaruga * image_box_solid_5;
extern Texture_nKaruga * image_box_solid_6;
extern Texture_nKaruga * image_chain_hit_light;
extern Texture_nKaruga * image_chain_hit_shadow;
extern Texture_nKaruga * image_door_left;
extern Texture_nKaruga * image_door_right;
extern Texture_nKaruga * image_enemy_bullet_0_light;
extern Texture_nKaruga * image_enemy_bullet_0_shadow;
extern Texture_nKaruga * image_enemy_bullet_1_light;
extern Texture_nKaruga * image_enemy_bullet_1_shadow;
extern Texture_nKaruga * image_enemy_bullet_2_light;
extern Texture_nKaruga * image_enemy_bullet_2_shadow;
extern Texture_nKaruga * image_enemy_generator_light;
extern Texture_nKaruga * image_enemy_generator_shadow;
extern Texture_nKaruga * image_enemy_homing_bullet_light;
extern Texture_nKaruga * image_enemy_homing_bullet_shadow;
extern Texture_nKaruga * image_enemy_ship_0_light;
extern Texture_nKaruga * image_enemy_ship_0_shadow;
extern Texture_nKaruga * image_enemy_ship_1_light;
extern Texture_nKaruga * image_enemy_ship_1_shadow;
extern Texture_nKaruga * image_enemy_ship_2_light;
extern Texture_nKaruga * image_enemy_ship_2_shadow;
extern Texture_nKaruga * image_enemy_ship_3_light;
extern Texture_nKaruga * image_enemy_ship_3_shadow;
extern Texture_nKaruga * image_enemy_ship_4_lightball;
extern Texture_nKaruga * image_enemy_ship_4_light;
extern Texture_nKaruga * image_enemy_ship_4_shadowball;
extern Texture_nKaruga * image_enemy_ship_4_shadow;
extern Texture_nKaruga * image_enemy_ship_5_light;
extern Texture_nKaruga * image_enemy_ship_5_shadow;
extern Texture_nKaruga * image_enemy_ship_6_light;
extern Texture_nKaruga * image_enemy_ship_6_shadow;
extern Texture_nKaruga * image_enemy_wall_light;
extern Texture_nKaruga * image_enemy_wall_shadow;
extern Texture_nKaruga * image_explosion_light_0;
extern Texture_nKaruga * image_explosion_light_1;
extern Texture_nKaruga * image_explosion_light_2;
extern Texture_nKaruga * image_explosion_light_3;
extern Texture_nKaruga * image_explosion_light_4;
extern Texture_nKaruga * image_explosion_light_5;
extern Texture_nKaruga * image_explosion_shadow_0;
extern Texture_nKaruga * image_explosion_shadow_1;
extern Texture_nKaruga * image_explosion_shadow_2;
extern Texture_nKaruga * image_explosion_shadow_3;
extern Texture_nKaruga * image_explosion_shadow_4;
extern Texture_nKaruga * image_explosion_shadow_5;
extern Texture_nKaruga * image_lives;
extern Texture_nKaruga * image_player_bullet_light;
extern Texture_nKaruga * image_player_bullet_shadow;
extern Texture_nKaruga * image_player_explosion_0;
extern Texture_nKaruga * image_player_explosion_10;
extern Texture_nKaruga * image_player_explosion_11;
extern Texture_nKaruga * image_player_explosion_1;
extern Texture_nKaruga * image_player_explosion_2;
extern Texture_nKaruga * image_player_explosion_3;
extern Texture_nKaruga * image_player_explosion_4;
extern Texture_nKaruga * image_player_explosion_5;
extern Texture_nKaruga * image_player_explosion_6;
extern Texture_nKaruga * image_player_explosion_7;
extern Texture_nKaruga * image_player_explosion_8;
extern Texture_nKaruga * image_player_explosion_9;
extern Texture_nKaruga * image_player_homing_bullet_light_0;
extern Texture_nKaruga * image_player_homing_bullet_light_1;
extern Texture_nKaruga * image_player_homing_bullet_light_2;
extern Texture_nKaruga * image_player_homing_bullet_shadow_0;
extern Texture_nKaruga * image_player_homing_bullet_shadow_1;
extern Texture_nKaruga * image_player_homing_bullet_shadow_2;
extern Texture_nKaruga * image_player_ship_invincible_light;
extern Texture_nKaruga * image_player_ship_invincible_shadow;
extern Texture_nKaruga * image_player_ship_light;
extern Texture_nKaruga * image_player_ship_polarityswitch_0_light;
extern Texture_nKaruga * image_player_ship_polarityswitch_0_shadow;
extern Texture_nKaruga * image_player_ship_polarityswitch_1_light;
extern Texture_nKaruga * image_player_ship_polarityswitch_1_shadow;
extern Texture_nKaruga * image_player_ship_shadow;
extern Texture_nKaruga * image_powerslot;
extern Texture_nKaruga * image_prop_wall_left;
extern Texture_nKaruga * image_prop_wall_right;
#endif

#endif
