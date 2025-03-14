#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

SDL_Texture* loadTexture(const char *file, SDL_Renderer *renderer) {
    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = SDL_LoadBMP(file);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL Error: %s\n", file, SDL_GetError());
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

bool checkCollision(SDL_Rect *a, SDL_Rect *b) {
    if (a->x + a->w > b->x && a->x < b->x + b->w && 
        a->y + a->h > b->y && a->y < b->y + b->h) {
        return true;
    }
    return false;
}

int main(int argc, char *argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("Simple 2D Game Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load player texture (you should have player.bmp image in your directory)
    SDL_Texture *playerTexture = loadTexture("player.bmp", renderer);
    if (playerTexture == NULL) {
        printf("Failed to load player texture!\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Rect playerRect = { 100, 100, 50, 50 }; // player initial position and size
    SDL_Rect boundary = { 0, 0, 800, 600 };  // Screen boundaries

    // Event handling flag
    bool quit = false;
    SDL_Event e;

    // Game loop
    while (!quit) {
        // Event handling
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN) {
                // Handle player movement with keyboard input
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    if (playerRect.y > 0) playerRect.y -= 5; break;
                    case SDLK_DOWN:  if (playerRect.y < boundary.h - playerRect.h) playerRect.y += 5; break;
                    case SDLK_LEFT:  if (playerRect.x > 0) playerRect.x -= 5; break;
                    case SDLK_RIGHT: if (playerRect.x < boundary.w - playerRect.w) playerRect.x += 5; break;
                }
            }
        }

        // Render background color
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue background
        SDL_RenderClear(renderer);

        // Render player sprite
        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

        // Check for collision (in this case, check if player collides with screen boundaries)
        if (checkCollision(&playerRect, &boundary)) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red border when collision occurs
            SDL_RenderDrawRect(renderer, &playerRect);
        }

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
