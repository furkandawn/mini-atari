# mini-atari

A mini retro-style game console built with the STM32F030R8 microcontroller, SSD1306 0.96' OLED display, 2 dimensional analog joystick with button, an additional external button.

STM32 cubeIDE 1.18.1 used for this project.

📌 About this Project ->

I started this project as a personal challenge to:

    ➤Get started with embedded systems
    ➤Push my understanding of C, memory, and hardware
    ➤Build something fun and visual
    
    
## FEATURES

    ### GENERAL
    - Menu systems controlled by joystick
    - Modular and extensible architecture
    - Leaderboard system stored in flash memory
    - Simple animations and bitmaps for UX
    
    ### 🎮 Games
    
    #### 🐍 Snake
    - Classic movement and growth logic
    - Collision with self or walls ends the game
    - Speed increases progressively
    
    #### 🧩 Tetris
    - Gravity, kickback.
    - Grid-based movement and rotation
    - Line clearing
    
    #### 🏓 Pong
    - Ball reflection based on randomness
    - Paddle control
    - Player vs simple AI (very simple)
    - Simple score counter
    
    #### 🧱 Breakout
    - Ball physics based on hit angle
    - Paddle control
    - Brick collision and score system
    
    #### 👾 Space Invaders
    - Multiple enemy types: Squid, Crab, Octopus, UFO
    - Bitmap, bullet and movement mechanichs for player and each enemy
    - Pixel perfect bullet - entity collision


## 🔧 How It Works
    - The system boots into a main menu that lists available games.
    - A timer-based RNG is used to seed randomness at startup of the selected game.
    - Games run within isolated state loops and manage their own:
      - Rendering
      - Input
      - Timing
      - Game logic
    - Games return to the menu upon completion or loss.


🎥Videos
    Menu Systems: https://www.youtube.com/watch?v=7HuLqZ83VQw&list=PLDRjpnp750kDVOImY-O_GNV_q2miwJYLG&index=2

GAMES
    1. Snake Game: https://www.youtube.com/watch?v=1EM2BZiJ4zE&list=PLDRjpnp750kDVOImY-O_GNV_q2miwJYLG&index=3
    2. Tetris Game: https://www.youtube.com/watch?v=8NXbPQ_SdIY&list=PLDRjpnp750kDVOImY-O_GNV_q2miwJYLG&index=4
    3. Pong Game: https://www.youtube.com/watch?v=9ESiIYlbD8Q&list=PLDRjpnp750kDVOImY-O_GNV_q2miwJYLG&index=5
    4. Breakout Game: https://www.youtube.com/watch?v=alcsj3yEftw&list=PLDRjpnp750kDVOImY-O_GNV_q2miwJYLG&index=6
    5. Space Invaders Game: https://www.youtube.com/watch?v=4ioF6O9ygYc&list=PLDRjpnp750kDVOImY-O_GNV_q2miwJYLG&index=7

