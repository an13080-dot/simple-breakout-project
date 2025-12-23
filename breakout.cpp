#include "assets.h"
#include "ball.h"
#include "game.h"
#include "graphics.h"
#include "level.h"
#include "paddle.h"
#include "raylib.h"

void update()
{
    // 0. Music
    UpdateMusicStream(Menu_sound); // Keeps music playing

    //  music in Menu, stops in game
    if (game_state == menu_state || game_state == instructions_state) {
        if (!IsMusicStreamPlaying(Menu_sound)) {
            PlayMusicStream(Menu_sound);
        }
    } else {
        StopMusicStream(Menu_sound);
    }
    // 1. MENU form
    if (game_state == menu_state) {
        if (IsKeyPressed(KEY_ENTER)) {
            // reset of the level
            current_level_index = 0;

            // 1. reset of the score
            player_score = 0;

            load_level(current_level_index);
            game_state = in_game_state;
        }

        //second. option to instructions
        if (IsKeyPressed(KEY_SPACE)) {
            game_state = instructions_state;
        }
    }
    // INSTRUCTIONS form
    else if (game_state == instructions_state) {
        //Return to menu on tab or escape
        if (IsKeyPressed(KEY_TAB)) {
            game_state = menu_state;
        }
    }
    // 2. PAUSE form
    else if (game_state == paused_state) {
        // RESUME by pressing Escape again
        if (IsKeyPressed(KEY_ESCAPE)) {
            game_state = in_game_state;
        }
    }
    // 3. VICTORY form
    else if (game_state == victory_state) {
        if (IsKeyPressed(KEY_TAB)) {
            load_level(0); // Reset game to Level 1 (Index 0)

            // 2. reset of the scores
            player_score = 0;
            game_state = menu_state;
        }
        // 2. option
        if (IsKeyPressed(KEY_ENTER)) {
            load_level(0);
            // 3. reset of the scores
            player_score = 0;
            game_state = in_game_state;
        }
    }


    // 4. GAME OVER form
    else if (game_state == game_over_state) {
        if (IsKeyPressed(KEY_ENTER)) {
            current_level_index = 0;
            load_level(0); // restart from Level 1
            // 4. reset scores
            player_score = 0;
            game_state = in_game_state;
        }

    // Option 2: Return to Menu
    if (IsKeyPressed(KEY_TAB)) {
        current_level_index = 0;
        load_level(0);
        // 5. reset scores
        player_score = 0;
        game_state = menu_state;
    }
}
    // 5. IN-GAME form
    else {
        // 5000 points
    if (player_score >= 5000 && !milestone_reached) {
        PlaySound(milestone_sound);
        milestone_reached = true;
    }
        // PAUSE form
        if (IsKeyPressed(KEY_ESCAPE)) {
            game_state = paused_state;
        }

        // Paddle Controls
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            move_paddle(-paddle_speed);
        }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            move_paddle(paddle_speed);
        }

        move_ball();

        // Win/Lose Logic
        if (!is_ball_inside_level()) {
            PlaySound(lost_sound);
            // it will go to game over form
            game_state = game_over_state;
        }
        else if (current_level_blocks == 0) {
            PlaySound(win_sound);

            // num of levels
            if (current_level_index >= 5) {
                game_state = victory_state;
            } else {
                // Moving to the next level
                current_level_index++;
                load_level(current_level_index);
            }
        }
    }
}

void draw()
{
    ClearBackground(BLACK);

    if (game_state == menu_state) {
        DrawText("BREAKOUT", 340, 250, 120, RED);
        DrawText("PRESS ENTER TO START", 470, 370, 30, WHITE);
        DrawText("PRESS SPACE TO READ INSTRUCTIONS", 350, 450, 30, WHITE);
    }

    else if (game_state == instructions_state) {
        DrawText("INSTRUCTIONS", 420, 100, 60, RED);

        DrawText("- Use A / D to move.", 300, 250, 30, WHITE);
        DrawText("- Bounce the ball to break the blocks.", 300, 300, 30, WHITE);
        DrawText("- Edges of the paddle accelerate the ball.", 300, 350, 30, WHITE);
        DrawText("- Don't let the ball fall off the screen!", 300, 400, 30, WHITE);
        DrawText("- Press ESC to Pause during the game.", 300, 450, 30, WHITE);

        DrawText("PRESS TAB TO RETURN TO THE MENU", 320, 570, 30, GRAY);
    }
    else if (game_state == paused_state) {
        // 1. shown game behind the pause scene
        draw_level();
        draw_paddle();
        draw_ball();
        draw_ui();

        //Scores in pause
        DrawText(TextFormat("SCORE: %i", player_score), 600, 440, 25, WHITE);

        // 2. The dark overlay
        DrawRectangle(0, 0, 1280, 720, Fade(BLACK, 0.5f));

        // 3. It shows Pause Text and Resume options
        DrawText("PAUSED", 530, 300, 60, RED);
        DrawText("Press ESC to Resume", 510, 380, 25, WHITE);
    }
    else if (game_state == victory_state) {
        DrawText("VICTORY!", 460, 250, 80, GREEN);
        DrawText("Press TAB to return to Menu", 450, 450, 30, WHITE);
        DrawText("Press ENTER to Restart", 450, 400, 30, WHITE);
    }

    else if (game_state == game_over_state) {
        // shows the frozen game  in background
        draw_level();
        draw_paddle();
        draw_ui();

        // Red Overlay
        DrawRectangle(0, 0, 1280, 720, Fade(MAROON, 0.6f));

        DrawText("GAME OVER", 360, 250, 100, WHITE);
        DrawText("PRESS ENTER TO RESTART", 430, 450, 30, LIGHTGRAY);
        DrawText("PRESS TAB FOR MENU", 480, 500, 30, LIGHTGRAY);
    }
    else {
        draw_level();
        draw_paddle();
        draw_ball();
        draw_ui();

        //Scores in game
        DrawText(TextFormat("SCORE: %i", player_score), 50, 20, 20, WHITE);

        if (milestone_reached)
            DrawText("5000 POINTS! YOU'RE A PRO!", 400, 150, 40, GOLD);
        }
    }

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1280, 720, "Breakout");

    //it cahgned the purpose of ESC
    SetExitKey(KEY_NULL);

    SetTargetFPS(60);

    load_fonts();
    load_textures();
    load_level(0);
    load_sounds();

    game_state = menu_state;

    while (!WindowShouldClose()) {
        BeginDrawing();

        draw();
        update();

        EndDrawing();
    }
    CloseWindow();

    unload_sounds();
    unload_level();
    unload_textures();
    unload_fonts();

    return 0;
}