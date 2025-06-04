/*
 * game_space_invaders_config.h
 *
 *  Created on: Jun 4, 2025
 *      Author: furkandawn
 */

#ifndef INC_GAME_SPACE_INVADERS_CONFIG_H_
#define INC_GAME_SPACE_INVADERS_CONFIG_H_

#include <stdint.h>
#include <game_space_invaders.h>

// ----- Enemy configuration ----- //
#define ENTITY_WIDTH_PX 16
#define ENTITY_HEIGHT_PX 8
#define ENTITY_BITMAP_SIZE (ENTITY_WIDTH_PX * ENTITY_HEIGHT_PX / 8)
#define ENTITY_X_MIN 0
#define ENTITY_X_MAX (DISPLAY_WIDTH - ENTITY_WIDTH_PX)
#define ENTITY_BULLET_SPEED GAME_GRID

// ----- Spaceship configuration ----- //
#define SPACESHIP_WIDTH_PX 8
#define SPACESHIP_HEIGHT_PX 8
#define SPACESHIP_BITMAP_SIZE (SPACESHIP_WIDTH_PX * SPACESHIP_HEIGHT_PX / 8)
#define SPACESHIP_X_MIN 0
#define SPACESHIP_X_MAX (DISPLAY_WIDTH - SPACESHIP_WIDTH_PX)
#define SPACESHIP_Y (DISPLAY_HEIGHT - (2 * SPACESHIP_HEIGHT_PX))
#define SPACESHIP_SPEED GAME_GRID
#define SPACESHIP_BULLET_SPEED ((GAME_GRID * 3) / 2)

extern const spcinv_enemy_t enemy_config[SPCINV_ENEMY_TYPE_COUNT];
extern const uint8_t spaceship_bitmap[SPACESHIP_BITMAP_SIZE];
extern const uint8_t enemy_bitmap[SPCINV_ENEMY_TYPE_COUNT][ENTITY_BITMAP_SIZE];

#endif /* INC_GAME_SPACE_INVADERS_CONFIG_H_ */
