#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <math.h>

#define WIDTH 640
#define HEIGHT 480
#define MAX_BULLETS 5
#define MAX_ENEMIES 5

typedef struct {
    int x, y;
    int radius;
    int isAlive;
} Object;

// Function to check collision between two objects
int checkCollision(int x1, int y1, int x2, int y2, int radius) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    int distance = sqrt(dx * dx + dy * dy);

    if (distance <= radius)
        return 1;
    return 0;
}

int main() {
    // Initialize the graphics window
    int gd, gm;
    gd = DETECT;
initgraph(&gd, &gm, "");

    // Initialize player position and score
    int playerX = WIDTH / 2;
    int playerY = HEIGHT - 50;
    int score = 0;

    // Initialize arrays to store bullets and enemies
    Object bullets[MAX_BULLETS];
    Object enemies[MAX_ENEMIES];

    int i;
    for (i = 0; i < MAX_BULLETS; i++)
        bullets[i].isAlive = 0;

    // Initialize enemies with random positions and set them as alive
    for (i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].x = rand() % (WIDTH - 50);
        enemies[i].y = rand() % (HEIGHT / 2);
        enemies[i].radius = 20;
        enemies[i].isAlive = 1;
    }

    // Gameover flag
    int gameover = 0;

    // Game loop
    while (!gameover) {
cleardevice();

        // Handle player input
        if (kbhit()) {
            char key = getch();
            if (key == 'a' && playerX > 0)
                playerX -= 10;
            else if (key == 'd' && playerX < WIDTH - 50)
                playerX += 10;
            else if (key == ' '){
                // Fire a bullet
                for (i = 0; i < MAX_BULLETS; i++) {
                    if (!bullets[i].isAlive) {
                        bullets[i].x = playerX + 20;
                        bullets[i].y = playerY;
                        bullets[i].isAlive = 1;
                        break;
                    }
                }
            }
        }

        // Draw the player
rectangle(playerX, playerY, playerX + 50, playerY + 30);

        // Update and draw bullets
        for (i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].isAlive) {
                circle(bullets[i].x, bullets[i].y, 5);
                bullets[i].y -= 10;
                if (bullets[i].y< 0)
                    bullets[i].isAlive = 0;
            }
        }

        // Update and draw enemies
        for (i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].isAlive) {
                circle(enemies[i].x, enemies[i].y, enemies[i].radius);
                enemies[i].y += 2;
                if (enemies[i].y> HEIGHT) {
                    // Reset the enemy position once it goes off the screen
                    enemies[i].x = rand() % (WIDTH - 50);
                    enemies[i].y = -50;
                }

                // Check for bullet-enemy collision
                int j;
                for (j = 0; j < MAX_BULLETS; j++) {
                    if (bullets[j].isAlive&& checkCollision(bullets[j].x, bullets[j].y, enemies[i].x, enemies[i].y, enemies[i].radius)) {
                        // Increase score, remove bullet, and reset enemy position
                        score += 10;
                        bullets[j].isAlive = 0;
                        enemies[i].x = rand() % (WIDTH - 50);
                        enemies[i].y = -50;
                    }
                }

                // Check for player-enemy collision
                if (checkCollision(playerX + 25, playerY + 15, enemies[i].x, enemies[i].y, enemies[i].radius)) {
                    gameover = 1; // Game over if collision occurs
                    break;
                }
            }
        }

        // Display the score on the screen
setcolor(YELLOW);
settextstyle(2, 0, 5);
        char scoreStr[20];
sprintf(scoreStr, "Score: %d", score);
outtextxy(10, 10, scoreStr);

delay(10); // Add a small delay to control game speed
    }

    // Game over screen
cleardevice();
settextstyle(3, 0, 5);
outtextxy(WIDTH / 2 - 120, HEIGHT / 2 - 30, "Game Over");
    char finalScoreStr[30];
sprintf(finalScoreStr, "Final Score: %d", score);
outtextxy(WIDTH / 2 - 150, HEIGHT / 2 + 20, finalScoreStr);

getch();
closegraph(); // Close the graphics window

    return 0;
}

