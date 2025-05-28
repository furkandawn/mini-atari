/*
 * menu_interface.h
 *
 *  Created on: May 26, 2025
 *      Author: furkandawn
 */

//???***???***???*** LAYOUT FOR SOURCE FILES ***???***???***???//
// ===== Includes ===== //
// include display library

// ======= Macros/Constants ===== //
// ----- //

// ===== Static File-Private Variables ===== //

// ===== Public Global Variables ===== //

// ===== Static Function Declarations ===== //

// ===== Public API Function Definitions ===== //

// ===== Static Function Definitions ===== //
// END LAYOUT

#ifndef INC_MENU_INTERFACE_H_
#define INC_MENU_INTERFACE_H_

#include <stdbool.h>

// ===== GLOBAL STATE ENUMS/STRUCTS ===== //
// ----- menu_logic ----- //
typedef enum
{
	MENU_MAIN = 0,
	MENU_SELECTED,
	MENU_LEADERBOARD,
	MENU_PLAYING,
	MENU_PAUSED,
	MENU_GAMEOVER,
	MENU_SAVE
} menu_state_t;

// ----- menu_main ----- //
typedef enum
{
	GAME_SNAKE = 0,
	GAME_TETRIS,
	GAME_3,
	GAME_4,
	GAME_5,
	GAME_COUNT
} game_type_t;

// ----- menu_selected ------ //
typedef enum
{
	MENU_ACTION_START = 0,
	MENU_ACTION_LEADERBOARD,
	MENU_ACTION_BACK,
	MENU_ACTION_COUNT
} menu_action_t;

// ----- menu_paused ----- //
typedef enum
{
	PAUSED_CONTINUE = 0,
	PAUSED_EXIT,
	PAUSED_COUNT
} paused_action_t;

// ----- menu_gameover ----- //
typedef enum
{
	GAMEOVER_PLAY_AGAIN = 0,
	GAMEOVER_MAIN_MENU,
	GAMEOVER_SAVE,
	GAMEOVER_COUNT
} gameover_action_t;

// ----- menu_save ----- //
typedef enum
{
	SAVE_SAVE = 0,
	SAVE_BKSP,
	SAVE_EXIT,
	SAVE_COUNT
} save_action_t;

// ----- menu_leaderboard ----- //
// ----- //

// ********************************************************************************************** //

// ===== GLOBAL STATE VARIABLES ===== //
// ----- menu_logic ----- //
extern menu_state_t current_menu_state;

// ----- menu_main ----- //
extern game_type_t current_game_type;
extern const char *menu_main_items[GAME_COUNT];

// ----- menu_selected ----- //
//----//

// ----- menu_paused ----- //
void game_pause(void);

// ----- menu_gameover ----- //
extern bool gameover_animation_shown;

// ----- menu_save ----- //
// ----- //

// ----- menu_leaderboard ----- //
// ----- //

// ----- joystick/button interrupt logic ----- //
extern bool input_enabled;

#endif /* INC_MENU_INTERFACE_H_ */
