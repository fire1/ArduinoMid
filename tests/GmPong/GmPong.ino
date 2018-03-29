#include <Arduino.h>
#include "screen.h"


// Define pins used by potentiometer knobs
#define paddle_l 16
#define paddle_r 15

// Define pins used by buttons
#define button_l 8
#define button_m 21
#define button_r 9


#define ANALOG_MAX 1024
#define WIDTH 240
#define HEIGHT 64

#define PADDLE_SIZE 15

int player1_pos = 0, player1_score = 0;

float ball_x, ball_y;
float ball_xdir, ball_ydir;

void draw_paddle(int x, int y) {
    u8g2.drawBox(x - 1, y - PADDLE_SIZE / 2 + 2, PADDLE_SIZE - 4, 3);
    u8g2.drawBox(x + 0, y - PADDLE_SIZE / 2, PADDLE_SIZE, 3);
    u8g2.drawBox(x + 1, y - PADDLE_SIZE / 2 + 2, PADDLE_SIZE - 4, 3);
}

void draw_ball(int x, int y) {
    u8g2.drawCircle(x, y, 4);
}

void draw_score(int player, int value) {
//    u8g2.setColorIndex(color);
    if (player == 1)
        u8g2.setCursor(7, 0);
    if (player == 2)
        u8g2.setCursor(WIDTH - 10, 0);
    u8g2.print(value);
}

void reset_ball() {
    ball_x = WIDTH / 2;
    ball_y = HEIGHT / 2;
    ball_xdir = 2.5;
    if (random(2) == 0) ball_xdir *= -1;
    ball_ydir = random(0, 5) / 2;
}

void setup(void) {
    lcdPwr.begin();

    pinMode(button_l, INPUT);
    pinMode(button_r, INPUT);


    u8g2.begin();
    u8g2.setFont(u8g2_font_ncenB08_tr);    // choose a suitable font
    delay(50);
    u8g2.drawStr(15, 15, ("Game begin...."));
    u8g2.sendBuffer();
    delay(800);
    reset_ball();
}

uint8_t pos = 15;

void loop() {
    u8g2.clearBuffer();
    draw_ball(ball_x, ball_y);
    draw_paddle(4, player1_pos);

    draw_score(1, player1_score);


    if (!digitalRead(button_r) == HIGH) {
        pos++;
    }


    if (!digitalRead(button_l) == HIGH) {
        pos--;
    }

    player1_pos = pos;

    // If ball goes to top or bottom, just bounce
    if (ball_y < 4 || ball_y > HEIGHT - 4) {
        ball_ydir *= -1;
    }

    // If ball hits player 1's paddle, bounce
    if (ball_x > 4 && ball_x < 8 &&
        ball_y > (player1_pos - PADDLE_SIZE / 2) &&
        ball_y < (player1_pos + PADDLE_SIZE / 2)) {
        ball_xdir *= -1;
        ball_ydir += (ball_y - player1_pos) / PADDLE_SIZE * 2.5;
    }

    ball_x += ball_xdir;
    ball_y += ball_ydir;
    // If ball gets behind player, it's gone
    if (ball_x > WIDTH) {
        player1_score++;
        reset_ball();
    }
    u8g2.sendBuffer();
    delay(15);

    draw_ball(ball_x, ball_y);
    draw_paddle(4, player1_pos);

    draw_score(1, player1_score);
    u8g2.sendBuffer();
    delay(15);
}