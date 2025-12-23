#include "ball.h"
#include "assets.h"
#include "level.h"
#include "paddle.h"
#include "game.h"
#include "raylib.h"

#include <cmath>
#include <numbers>

void spawn_ball()
{
    for (int column = 0; column < current_level.columns; column++) {
        for (int row = 0; row < current_level.rows; row++) {
            if (get_level_cell(row, column) == BALL) {
                set_level_cell(row, column, VOID);
                ball_pos = { static_cast<float>(column), static_cast<float>(row) };
                constexpr float ball_launch_angle_radians = ball_launch_angle_degrees * (std::numbers::pi_v<float> / 180.0f);
                ball_vel.y = -ball_launch_vel_mag * std::sin(ball_launch_angle_radians);
                ball_vel.x = (rand() % 2 == 0) ? ball_launch_vel_mag * std::cos(ball_launch_angle_radians) : -ball_launch_vel_mag * std::cos(ball_launch_angle_radians);
                goto outer_loop_end;
            }
        }
    }
outer_loop_end:;
}

void move_ball()
{
    Vector2 next_ball_pos = {
        ball_pos.x + ball_vel.x,
        ball_pos.y + ball_vel.y
    };

    // 1. WALL and BIG_WALL
    bool hit_wall = is_colliding_with_level_cell(next_ball_pos, ball_size, WALL);
    bool hit_big_wall = is_colliding_with_level_cell(next_ball_pos, ball_size, BIG_WALL);

    if (hit_wall || hit_big_wall) {

        char wall_type = hit_big_wall ? BIG_WALL : WALL;

        if (is_colliding_with_level_cell({ next_ball_pos.x, ball_pos.y }, ball_size, wall_type)) {
            ball_vel.x = -ball_vel.x;
            next_ball_pos.x = std::round(next_ball_pos.x);
        }
        if (is_colliding_with_level_cell({ ball_pos.x, next_ball_pos.y }, ball_size, wall_type)) {
            ball_vel.y = -ball_vel.y;
            next_ball_pos.y = std::round(next_ball_pos.y);
        }

    // 2. Block collision
    } else if (is_colliding_with_level_cell(next_ball_pos, ball_size, BLOCKS) ||
               is_colliding_with_level_cell(next_ball_pos, ball_size, '$')) {

        char hit_type = BLOCKS;
        if (is_colliding_with_level_cell(next_ball_pos, ball_size, '$')) {
            hit_type = '$';
        }

        char& temp = get_colliding_level_cell(next_ball_pos, ball_size, hit_type);

        if (is_colliding_with_level_cell({ next_ball_pos.x, ball_pos.y }, ball_size, hit_type)) {
            ball_vel.x = -ball_vel.x;
            next_ball_pos.x = std::round(next_ball_pos.x);
        }
        if (is_colliding_with_level_cell({ ball_pos.x, next_ball_pos.y }, ball_size, hit_type)) {
            ball_vel.y = -ball_vel.y;
            next_ball_pos.y = std::round(next_ball_pos.y);
        }

        temp = VOID;
        --current_level_blocks;

        if (hit_type == '$') {
            PlaySound(gold_sound);
            player_score += 500;
        } else {
            player_score += 100;
        }

    // 3. Paddle collision
    } else if (is_colliding_with_paddle(next_ball_pos, ball_size)) {
        float paddle_center_x = paddle_pos.x + paddle_size.x / 2.0f;
        float ball_center_x = next_ball_pos.x + ball_size.x / 2.0f;
        float hit_point = (ball_center_x - paddle_center_x) / (paddle_size.x / 2.0f);

        ball_vel.x = hit_point * std::abs(ball_vel.y) * 1.5f;
        ball_vel.y = -std::abs(ball_vel.y);

        if (std::abs(hit_point) > 0.62f) {
            PlaySound(acceleration_sound);
        }
    }
    ball_pos = next_ball_pos;
}

bool is_ball_inside_level()
{
    return is_inside_level(static_cast<int>(ball_pos.y), static_cast<int>(ball_pos.x));
}