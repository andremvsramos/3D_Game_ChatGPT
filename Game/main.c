#include "minilibx-linux/mlx.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Player position variables
float pX = 0.0f;
float playerY = 0.0f;
float playerZ = 0.0f;

// Player movement speed
float movementSpeed = 0.1f;

// Gravity acceleration
float gravity = 0.01f;

// Player size
int PLAYER_SIZE = 10; // Adjust this value to change player size

typedef struct {
    void* mlx;
    void* win;
} t_mlx_window;

// Custom mlx_destroy function
void mlx_destroy_custom(void* mlx, void* window) {
    // Clean up and release resources
    mlx_destroy_window(mlx, window);
    mlx_destroy_display(mlx);
    free(mlx);
    printf("Custom mlx_destroy called\n");
}

// Custom mlx_flush function
int mlx_flush_custom(void* param) {
    t_mlx_window* window = (t_mlx_window*)param;

    // Render the player
    int playerRectX = (SCREEN_WIDTH / 2) + (pX * 100);
    int playerRectY = (SCREEN_HEIGHT / 2) + (playerY * 100);

    // Increase player size
    playerRectX -= PLAYER_SIZE / 2;
    playerRectY -= PLAYER_SIZE / 2;

    // Render the player rectangle
    for (int i = 0; i < PLAYER_SIZE; i++) {
        for (int j = 0; j < PLAYER_SIZE; j++) {
            mlx_pixel_put(window->mlx, window->win, playerRectX + i, playerRectY + j, 0xFFFFFF);
        }
    }

    // Update the screen
    mlx_do_sync(window->mlx);

    return 0;
}

int handle_window_close(void* param) {
    t_mlx_window* window = (t_mlx_window*)param;
    mlx_destroy_window(window->mlx, window->win);
    exit(0);
}

int handle_key_press(int keycode, void* param) {
    t_mlx_window* window = (t_mlx_window*)param;

    // Clear the last player position
    mlx_clear_window(window->mlx, window->win);

    // Handle movement keys (WASD and arrow keys)
    switch (keycode) {
        case 119: // 'W' key
        case 65362: // Up arrow key
            playerY -= movementSpeed; // Invert the movement for Up key
            break;
        case 97: // 'A' key
        case 65361: // Left arrow key
            pX -= movementSpeed;
            break;
        case 115: // 'S' key
        case 65364: // Down arrow key
            playerY += movementSpeed; // Invert the movement for Down key
            break;
        case 100: // 'D' key
        case 65363: // Right arrow key
            pX += movementSpeed;
            break;
        case 65307: // Escape key
            mlx_destroy_window(window->mlx, window->win);
            exit(0);
    }

    return 0;
}

int handle_mouse_scroll(int button, int _, int __, void* param) {
    t_mlx_window* window = (t_mlx_window*)param;

    // Handle mouse scroll
    if (button == 4) {
        // Scroll up
        if (PLAYER_SIZE < 0.15 * SCREEN_WIDTH && PLAYER_SIZE < 0.15 * SCREEN_HEIGHT)
            PLAYER_SIZE += 10;
    } else if (button == 5) {
        // Scroll down
        if (PLAYER_SIZE > 5)
            PLAYER_SIZE -= 10;
    }

    return 0;
}

int main() {
    // Initialize the minilibx
    void* mlx = mlx_init();
    if (mlx == NULL) {
        printf("Minilibx initialization failed\n");
        return 1;
    }

    // Create the window
    t_mlx_window window;
    window.mlx = mlx;
    window.win = mlx_new_window(mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "3D Game");
    if (window.win == NULL) {
        printf("Window creation failed\n");
        return 1;
    }

    // Set up the event hooks
    mlx_loop_hook(mlx, mlx_flush_custom, &window);
    mlx_hook(window.win, 17, 0L, handle_window_close, &window);
    mlx_hook(window.win, 2, 1L << 0, handle_key_press, &window);
    mlx_mouse_hook(window.win, handle_mouse_scroll, &window); // Use mlx_mouse_hook instead of mlx_hook

    // Start the event loop
    mlx_loop(mlx);

    // Clean up and exit
    mlx_destroy_custom(mlx, window.win);
    return 0;
}
