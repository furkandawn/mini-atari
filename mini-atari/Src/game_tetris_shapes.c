/*
 * game_tetris_shapes.c
 *
 *  Created on: May 13, 2025
 *      Author: furkandawn
 */


#include "game_tetris_shapes.h"

// ----->> includes start

// include OLED Display library


// include mini-atari libraries


// include other


// includes end <<-----

tetrimino_type_t current_tetrimino = TETRIMINO_I;

const tetrimino tetriminos[TETRIMINO_TYPE_COUNT] =
{
		// [0] -> I tetrimino
		{
				.rotation =
				{
						{		// rotation
								{0,1,0},
								{0,1,0},
								{0,1,0}
						},
						{
								{0,0,0},
								{1,1,1},
								{0,0,0}
						},
						{
								{0,0,0},
								{0,0,0},
								{0,0,0}
						},
						{
								{0,0,0},
								{0,0,0},
								{0,0,0}
						}
				},
				.rotation_count = 2
		},

		// [1] -> O tetrimino
		{
				.rotation =
				{
						{
								{1,1,0},
								{1,1,0},
								{0,0,0}
						},
						{
								{0,0,0},
								{0,0,0},
								{0,0,0}
						},
						{
								{0,0,0},
								{0,0,0},
								{0,0,0}
						},
						{
								{0,0,0},
								{0,0,0},
								{0,0,0}
						}
				},
				.rotation_count = 1
		},

		// [2] -> T tetrimino
		{
				.rotation =
				{
						{
								{0,0,0},
								{1,1,1},
								{0,1,0}
						},
						{
								{0,1,0},
								{1,1,0},
								{0,1,0}
						},
						{
								{0,1,0},
								{1,1,1},
								{0,0,0}
						},
						{
								{0,1,0},
								{0,1,1},
								{0,1,0}
						}
				},
				.rotation_count = 4
		},

		// [3] -> S tetrimino
		{
				.rotation =
				{
						{
								{0,0,0},
								{0,1,1},
								{1,1,0}
						},
						{
								{1,0,0},
								{1,1,0},
								{0,1,0}
						},
						{
								{0,0,0},
								{0,0,0},
								{0,0,0}
						},
						{
								{0,0,0},
								{0,0,0},
								{0,0,0}
						}
				},
				.rotation_count = 2
		},

		// [4] -> Z tetrimino
		{
				.rotation =
				{
						{
								{0,0,0},
								{1,1,0},
								{0,1,1}
						},
						{
								{0,0,1},
								{0,1,1},
								{0,1,0}
						},
						{
								{0,0,0},
								{0,0,0},
								{0,0,0}
						},
						{
								{0,0,0},
								{0,0,0},
								{0,0,0}
						}
				},
				.rotation_count = 2
		},

		// [5] -> J tetrimino
		{
				.rotation =
				{
						{
								{0,1,0},
								{0,1,0},
								{1,1,0}
						},
						{
								{1,0,0},
								{1,1,1},
								{0,0,0}
						},
						{
								{0,1,1},
								{0,1,0},
								{0,1,0}
						},
						{
								{0,0,0},
								{1,1,1},
								{0,0,1}
						}

				},
				.rotation_count = 4
		},

		// [6] -> L tetrimino
		{
				.rotation =
				{
						{
								{0,1,0},
								{0,1,0},
								{0,1,1}
						},
						{
								{0,0,0},
								{1,1,1},
								{1,0,0}
						},
						{
								{1,1,0},
								{0,1,0},
								{0,1,0}
						},
						{
								{0,0,1},
								{1,1,1},
								{0,0,0}
						}
				},
				.rotation_count = 4
		},
};
