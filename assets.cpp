#include "assets.h"

#include "raylib.h"

Sound paddle_hit_sound;
Sound Pause_sound;
Sound acceleration_sound;
Music Menu_sound;

void load_fonts()
{
    menu_font = LoadFontEx("data/fonts/ARCADECLASSIC.TTF", 256, nullptr, 0);
}

void unload_fonts()
{
    UnloadFont(menu_font);
}

void load_textures()
{
    wall_texture = LoadTexture("data/images/wall.png");
    void_texture = LoadTexture("data/images/void.png");
    block_texture = LoadTexture("data/images/block.png");
    paddle_texture = LoadTexture("data/images/paddle.png");
    ball_sprite = load_sprite("data/images/ball/ball", ".png", 8, true, 10);
}

void unload_textures()
{
    UnloadTexture(wall_texture);
    UnloadTexture(void_texture);
    UnloadTexture(block_texture);
    UnloadTexture(paddle_texture);
    unload_sprite(ball_sprite);
}

void load_sounds()
{
    InitAudioDevice();
    win_sound = LoadSound("data/sounds/win.wav");
    lose_sound = LoadSound("data/sounds/lose.wav");

    paddle_hit_sound = LoadSound("data/sounds/paddle.wav");
    acceleration_sound = LoadSound("data/sounds/acceleration.wav");
    Pause_sound = LoadSound("data/sounds/start.wav");
// for Music
Menu_sound = LoadMusicStream("data/music/Menu.wav");
SetMusicVolume(Menu_sound, 0.5f);
}

void unload_sounds()
{
    UnloadSound(win_sound);
    UnloadSound(lose_sound);
    CloseAudioDevice();
    //My sounds
    UnloadSound(paddle_hit_sound);
    UnloadSound(acceleration_sound);
    UnloadSound(Pause_sound);
    //music
    UnloadMusicStream(Menu_sound);
}
