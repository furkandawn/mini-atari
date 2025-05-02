# mini-atari

A mini retro-style game console built with the STM32F030R8 microcontroller, SSD1306 0.96' OLED display, 2 dimension analog joystick.
Includes: A menu system, 5 games, game state management, and a modular embedded codebase written in pure C.

STM32 cubeIDE 1.17.0 used for this project.



📌 About the Project ->

I started this project as a personal challenge to:

    ➤Get started with embedded systems
    ➤Build something fun and visual
    ➤Push my understanding of C, memory, and hardware



This project is a game console built on the STM32F030R8T6 microcontroller. It includes:

    ➤A menu system with joystick navigation
    ➤A working Snake game (only 1 game for now)
    ➤Modular code design with future games planned
    ➤Score and level system
    ➤Various types of menus


What I Learned/am learning ->

       ➤STM32 Hardware Programming: 
          •GPIO, ADC (for joystick), delays, memory layout
       ➤SSD1306 OLED Display: 
           •I2C protocol, text rendering, drawing graphics
      ➤C Programming for Embedded: 
           •Memory management, modular header/source file structures
      ➤Game Design: 
          •State machines, logic design for games like Snake
      ➤Code Structuring: 
          •Enum usage, header guards, static vs extern, file separation
      ➤Flash Memory: 
       •SRAM vs Flash, data segment layout, flash writing constraints


Plans for future:
    
      ➤Create a leaderboard system which user will be able to save his score and name directly to the flash memory when he hits a high score in the current game.
      ➤Add special icons for each game at startup.
      ➤Add more games (tetris as 2nd).



🎥Gifs and Videos of the current state

https://www.youtube.com/watch?v=mqlZMSorcHI
➤
➤
➤
➤
📌 Snake Game & Main Menu Gifs


https://github.com/user-attachments/assets/9b9f870b-21a2-4e57-8e17-fdb2bccb818f



https://github.com/user-attachments/assets/867031f0-c9e2-407a-be16-ab84aabc0807



https://github.com/user-attachments/assets/cf425ddc-7228-4562-bc1a-44484f2e8309

