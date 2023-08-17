#include "raylib.h"
#include <stdio.h>

enum bodyType {
    CENTER,
    UPDOWN,
    RIGHTLEFT,
    RIGHTDOWN,
    DOWNLEFT,
    LEFTUP,
    UPRIGHT,
    ONLYUP,
    ONLYRIGHT,
    ONLYDOWN,
    ONLYLEFT,
};

enum endType {
    NONE,
    UP,
    RIGHT,
    DOWN,
    LEFT,
};

enum gameState {
    MENU,
    SETTINGS,
    HOWTO,
    PREGAME,
    GAME,
    PAUSED,
    GAMEOVER,
};

struct ani_obj {
    Vector2 pos;
    char texture;
};

bool equals(Vector2 *lhs, Vector2 *rhs) {
    return (*lhs).x == (*rhs).x && (*lhs).y == (*rhs).y;
}

void loadSettings() {
    //FILE *fopen("settings.txt", "rb");
}

void saveSettings() {
    //FILE *fopen("settings.txt", "w");
}




//---------------------------------------------------------------------------------------------
//TODO
//add persistent settings
//add how to page
//---------------------------------------------------------------------------------------------

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    int screenWidth = 1000;
    int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Snake");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    GetFontDefault();

    //WindowSize(1920, 1080);
    //ToggleFullscreen();

    // Textures and texture recs
    RenderTexture2D canvas = LoadRenderTexture(screenWidth, screenHeight);
    RenderTexture2D helpcnv = LoadRenderTexture(300, 300);

    Texture2D appleTexture = LoadTexture("textures/apple.png");

    Texture2D snakeTexture = LoadTexture("textures/snake.png");
    Rectangle snakeTextureRecs[11] = {0};
    for (int i = 0; i < 11; i++) snakeTextureRecs[i] = (Rectangle){20 * i, 20, 20, 20};

    Texture2D snakeHeadTexture = LoadTexture("textures/snake_head.png");
    Rectangle snakeHeadTextureRecs[5] = {0};
    for (int i = 0; i < 5; i++) snakeHeadTextureRecs[i] = (Rectangle){20 * i, 20, 20, 20};

    Texture2D snakeTailTexture = LoadTexture("textures/snake_tail.png");
    Rectangle snakeTailTextureRecs[5] = {0};
    for (int i = 0; i < 5; i++) snakeTailTextureRecs[i] = (Rectangle){20 * i, 20, 20, 20};

    Vector2 apple[4] = {0};
    bool spawned[4] = {0};

    Vector2 snake[1500] = {0};
    Vector2 speed = {0, 0};
    int snakeLength = 4;
    int snakeLongest = 4;
    for (int i = 0; i < snakeLength; i++) snake[i] = (Vector2){500.0f, 300.0f};

    int maxLen = 2;
    Vector2 queue[2] = {0};
    int queuePos = -1;

    enum gameState state = MENU;
    int frame = 0;
    int ani_frame = 0;
    int help_frame = 0;
    int skipFrames = 6;
    int menuPos = 0;
    int settingsPos = 0;

    int activeRes = 1, newActiveRes = 1;
    const char *resolutions[4] = {
        "Small (800 x 500)",
        "Medium (1000 x 600)",
        "Large (1920 x 1080)",
        "Fullscreen"
    };

    int difficulty = 1, newDifficulty = 1;
    const char *difficultys[3] = {
        "Slow",
        "Medium",
        "Fast"
    };

    int numApples = 1, newNumApples = 1;

    struct ani_obj help_ani[9 * 9] = {
        (struct ani_obj){(Vector2){80, 60}, 0}, (struct ani_obj){(Vector2){80, 120}, DOWN}, (struct ani_obj){(Vector2){80, 140}, UPDOWN}, (struct ani_obj){(Vector2){80, 160}, UPRIGHT}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){100, 160}, LEFT},
        (struct ani_obj){(Vector2){80, 60}, 0}, (struct ani_obj){(Vector2){80, 100}, DOWN}, (struct ani_obj){(Vector2){80, 120}, UPDOWN}, (struct ani_obj){(Vector2){80, 140}, UPDOWN}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){80, 160}, UP},
        (struct ani_obj){(Vector2){80, 60}, 0}, (struct ani_obj){(Vector2){80, 80}, DOWN}, (struct ani_obj){(Vector2){80, 100}, UPDOWN}, (struct ani_obj){(Vector2){80, 120}, UPDOWN}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){80, 140}, UP},
        (struct ani_obj){(Vector2){-20, -20}, 0}, (struct ani_obj){(Vector2){80, 60}, DOWN}, (struct ani_obj){(Vector2){80, 80}, UPDOWN}, (struct ani_obj){(Vector2){80, 100}, UPDOWN}, (struct ani_obj){(Vector2){80, 120}, UPDOWN}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){80, 140}, UP},
        (struct ani_obj){(Vector2){-20, -20}, 0}, (struct ani_obj){(Vector2){80, 40}, DOWN}, (struct ani_obj){(Vector2){80, 60}, UPDOWN}, (struct ani_obj){(Vector2){80, 80}, UPDOWN}, (struct ani_obj){(Vector2){80, 100}, UPDOWN}, (struct ani_obj){(Vector2){80, 120}, UPDOWN}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){80, 140}, UP},
        (struct ani_obj){(Vector2){-20, -20}, 0}, (struct ani_obj){(Vector2){100, 40}, LEFT}, (struct ani_obj){(Vector2){80, 40}, RIGHTDOWN}, (struct ani_obj){(Vector2){80, 60}, UPDOWN}, (struct ani_obj){(Vector2){80, 80}, UPDOWN}, (struct ani_obj){(Vector2){80, 100}, UPDOWN}, (struct ani_obj){(Vector2){80, 120}, UPDOWN}, (struct ani_obj){(Vector2){-20, -20}, RIGHTLEFT}, (struct ani_obj){(Vector2){80, 140}, UP},
        (struct ani_obj){(Vector2){-20, -20}, 0}, (struct ani_obj){(Vector2){120, 40}, LEFT}, (struct ani_obj){(Vector2){100, 40}, RIGHTLEFT}, (struct ani_obj){(Vector2){80, 40}, RIGHTDOWN}, (struct ani_obj){(Vector2){80, 60}, UPDOWN}, (struct ani_obj){(Vector2){80, 80}, UPDOWN}, (struct ani_obj){(Vector2){80, 100}, UPDOWN}, (struct ani_obj){(Vector2){80, 120}, UPDOWN}, (struct ani_obj){(Vector2){80, 140}, UP},
        (struct ani_obj){(Vector2){-20, -20}, 0}, (struct ani_obj){(Vector2){140, 40}, LEFT}, (struct ani_obj){(Vector2){120, 40}, RIGHTLEFT}, (struct ani_obj){(Vector2){100, 40}, RIGHTLEFT}, (struct ani_obj){(Vector2){80, 40}, RIGHTDOWN}, (struct ani_obj){(Vector2){80, 60}, UPDOWN}, (struct ani_obj){(Vector2){80, 80}, UPDOWN}, (struct ani_obj){(Vector2){80, 100}, UPDOWN}, (struct ani_obj){(Vector2){80, 120}, UP},
        (struct ani_obj){(Vector2){-20, -20}, 0}, (struct ani_obj){(Vector2){160, 40}, LEFT}, (struct ani_obj){(Vector2){140, 40}, RIGHTLEFT}, (struct ani_obj){(Vector2){120, 40}, RIGHTLEFT}, (struct ani_obj){(Vector2){100, 40}, RIGHTLEFT}, (struct ani_obj){(Vector2){80, 40}, RIGHTDOWN}, (struct ani_obj){(Vector2){80, 60}, UPDOWN}, (struct ani_obj){(Vector2){80, 80}, UPDOWN}, (struct ani_obj){(Vector2){80, 100}, UP},
    };

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        //-----------------------------------------------------------------
        // opening screen
        if (state == MENU) {

            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                menuPos--;
                if (menuPos == -1) menuPos = 0;
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                menuPos++;
                if (menuPos == 3) menuPos = 2;
            }

            if (IsKeyPressed(KEY_ENTER)) {
                if (menuPos == 0) state = PREGAME;
                else if (menuPos == 1) state = SETTINGS;
                else if (menuPos == 2) state = HOWTO;
            }


            BeginDrawing();
                ClearBackground(BLACK);


                DrawText("Snake", (screenWidth - MeasureText("Snake", 70)) / 2, screenHeight / 6, 70, WHITE);


                if (menuPos == 0) DrawText("> Play <", (screenWidth - MeasureText("> Play <", 30)) / 2, screenHeight / 6 * 3, 30, WHITE);
                else DrawText("Play", (screenWidth - MeasureText("Play", 30)) / 2, screenHeight / 6 * 3, 30, WHITE);

                if (menuPos == 1) DrawText("> Settings <", (screenWidth - MeasureText("> Settings <", 30)) / 2, screenHeight / 6 * 4, 30, WHITE);
                else DrawText("Settings", (screenWidth - MeasureText("Settings", 30)) / 2, screenHeight / 6 * 4, 30, WHITE);

                if (menuPos == 2) DrawText("> How To Play <", (screenWidth - MeasureText("> How To Play <", 30)) / 2, screenHeight / 6 * 5, 30, WHITE);
                else DrawText("How To Play", (screenWidth - MeasureText("How To Play", 30)) / 2, screenHeight / 6 * 5, 30, WHITE);
            EndDrawing();
        }

        //-----------------------------------------------------------------
        // settings menu
        if (state == SETTINGS) {
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                settingsPos--;
                if (settingsPos == -1) settingsPos = 0;
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                settingsPos++;
                if (settingsPos == 5) settingsPos = 4;
            }
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
                switch (settingsPos) {
                    case 0:
                        newActiveRes--;
                        if (newActiveRes == -1) newActiveRes = 0;
                        break;
                    case 1:
                        newDifficulty--;
                        if (newDifficulty == -1) newDifficulty = 0;
                        break;
                    case 2:
                        newNumApples--;
                        if (newNumApples == -1) newNumApples = 0;
                        break;
                };
            }
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
                switch (settingsPos) {
                    case 0:
                        newActiveRes++;
                        if (newActiveRes == 4) newActiveRes = 3;
                        break;
                    case 1:
                        newDifficulty++;
                        if (newDifficulty == 3) newDifficulty = 2;
                        break;
                    case 2:
                        newNumApples++;
                        if (newNumApples == 5) newNumApples = 4;
                        break;
                };
            }


            if (IsKeyPressed(KEY_ENTER)) {
                if (settingsPos == 3) {
                    numApples = newNumApples;
                    difficulty = newDifficulty;
                    switch (difficulty) {
                        case 0:
                            skipFrames = 8;
                            break;
                        case 1:
                            skipFrames = 6;
                            break;
                        case 2:
                            skipFrames = 4;
                            break;
                    };
                    if (newActiveRes != activeRes) {
                        activeRes = newActiveRes;
                        switch (activeRes) {
                            case 0:
                                screenWidth = 800;
                                screenHeight = 500;
                                break;
                            case 1:
                                screenWidth = 1000;
                                screenHeight = 600;
                                break;
                            case 2:
                                screenWidth = 1920;
                                screenHeight = 1080;
                                break;
                            case 3:
                                screenWidth = 1920;
                                screenHeight = 1080;

                                break;
                        };
                        UnloadRenderTexture(canvas);
                        canvas = LoadRenderTexture(screenWidth, screenHeight);
                        SetWindowSize(screenWidth, screenHeight);
                    }
                    newActiveRes = activeRes;
                    state = MENU;
                } else if (settingsPos == 4) {
                    newActiveRes = activeRes;
                    newDifficulty = difficulty;
                    newNumApples = numApples;
                    state = MENU;
                }
            menuPos = 0;
            settingsPos = 0;
            }


            BeginDrawing();
                ClearBackground(BLACK);


                DrawText("Settings", (screenWidth - MeasureText("Settings", 70)) / 2, screenHeight / 10 * 1, 70, WHITE);


                if (settingsPos == 0) DrawText("> Resolution <", screenWidth / 4 - MeasureText("> Resolution <", 30) / 2, screenHeight / 10 * 4, 30, WHITE);
                else DrawText("Resolution", screenWidth / 4 - MeasureText("Resolution", 30) / 2, screenHeight / 10 * 4, 30, WHITE);
                DrawText(resolutions[newActiveRes], screenWidth / 4 * 3 - MeasureText(resolutions[newActiveRes], 30) / 2, screenHeight / 10 * 4, 30, WHITE);

                if (settingsPos == 1) DrawText("> Difficulty <", screenWidth / 4 - MeasureText("> Difficulty <", 30) / 2, screenHeight / 10 * 5, 30, WHITE);
                else DrawText("Difficulty", screenWidth / 4 - MeasureText("Difficulty", 30) / 2, screenHeight / 10 * 5, 30, WHITE);
                DrawText(difficultys[newDifficulty], screenWidth / 4 * 3 - MeasureText(difficultys[newDifficulty], 30) / 2, screenHeight / 10 * 5, 30, WHITE);

                if (settingsPos == 2) DrawText("> Apples <", screenWidth / 4 - MeasureText("> Apples <", 30) / 2, screenHeight / 10 * 6, 30, WHITE);
                else DrawText("Apples", screenWidth / 4 - MeasureText("Apples", 30) / 2, screenHeight / 10 * 6, 30, WHITE);
                DrawText(FormatText("%i", newNumApples), screenWidth / 4 * 3 - MeasureText(FormatText("%i", newNumApples), 30) / 2, screenHeight / 10 * 6, 30, WHITE);

                // maybe add high score reset (if do move all settings up by 1)

                if (settingsPos == 3) DrawText("> Save <", screenWidth / 2 - MeasureText("> Save <", 30) / 2, screenHeight / 10 * 8, 30, WHITE);
                else DrawText("Save", screenWidth / 2 - MeasureText("Save", 30) / 2, screenHeight / 10 * 8, 30, WHITE);

                if (settingsPos == 4) DrawText("> Back <", screenWidth / 2 - MeasureText("> Back <", 30) / 2, screenHeight / 10 * 9, 30, WHITE);
                else DrawText("Back", screenWidth / 2 - MeasureText("Back", 30) / 2, screenHeight / 10 * 9, 30, WHITE);
            EndDrawing();
        }

        //-----------------------------------------------------------------
        // How to play game
        if (state == HOWTO) {
            if (IsKeyPressed(KEY_ENTER)) {
                state = MENU;
            }

            BeginTextureMode(helpcnv);
            ClearBackground(BLACK);
                DrawTexture(appleTexture, help_ani[ani_frame % 9 * 9 + 0].pos.x, help_ani[ani_frame % 9 * 9 + 0].pos.y, WHITE);
                DrawTextureRec(snakeHeadTexture, snakeHeadTextureRecs[help_ani[ani_frame % 9 * 9 + 1].texture], help_ani[ani_frame % 9 * 9 + 1].pos, WHITE);
                for (int i = 2; i < 8; i++) {
                    DrawTextureRec(snakeTexture, snakeTextureRecs[help_ani[ani_frame % 9 * 9 + i].texture], help_ani[ani_frame % 9 * 9 + i].pos, WHITE);
                }
                DrawTextureRec(snakeTailTexture, snakeTailTextureRecs[help_ani[ani_frame % 9 * 9 + 8].texture], help_ani[ani_frame % 9 * 9 + 8].pos, WHITE);

            EndTextureMode();


            BeginDrawing();
                ClearBackground(BLACK);

                DrawTextureRec(helpcnv.texture, (Rectangle){0.0f, 0.0f, 300, -300}, (Vector2){screenWidth/4 * 3 - 150, screenHeight/2 - 150}, WHITE);

                DrawText("How To", (screenWidth - MeasureText("How To", 70)) / 2, screenHeight / 10 * 1, 70, WHITE);

                DrawText("Eat apples to make the snake\ngrow but dont hit the edge or\nyourself.\nControls:\nPause: [Space]\nMove: [WSAD],[P;L'],[arrowkeys]\nQuit: [Esc]", screenWidth / 4 - MeasureText("Eat apples to make the snake", 30) / 2, screenHeight / 10 * 3, 30, WHITE);

                //DrawText("Press [Enter] to start or [Backspace] to return", screenWidth / 2 - MeasureText("Press [Enter] to start or [Backspace] to return", 30) / 2, screenHeight / 10 * 9, 30, WHITE);
            EndDrawing();

            help_frame++;
            if (help_frame % 9 == 0) ani_frame++;
        }

        //-----------------------------------------------------------------
        // Game
        if (state == GAME || state == PREGAME || state == PAUSED) {

            // apple spawning
            for (int i = 0; i < numApples; i++) {
                if (!spawned[i]) {
                    bool valid = 0;
                    while (!valid) {
                        apple[i] = (Vector2){GetRandomValue(0, (screenWidth - 20) / 20) * 20, GetRandomValue(1, (screenHeight - 20) / 20) * 20};
                        valid = 1;
                        for (int j = 0; j < snakeLength; j++) if (snake[j].x == apple[i].x && snake[j].y == apple[i].y) valid = 0;
                        for (int j = 0; j < numApples; j++) if (i != j && (apple[j].x == apple[i].x && apple[j].y == apple[i].y)) valid = 0;
                    }
                spawned[i] = 1;
                }
            }


            // Input
            if (IsKeyPressed(KEY_SPACE)) {
                if (state == GAME) state = PAUSED;
                else if (state == PAUSED) state = GAME;
            }

            if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_APOSTROPHE)) && state != PAUSED) {
                queuePos++;
                if (queuePos == maxLen) queuePos--;
                else queue[queuePos] = (Vector2){20.0f, 0.0f};
                state = GAME;
            }
            if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_L)) && state != PAUSED) {
                queuePos++;
                if (queuePos == maxLen) queuePos--;
                else queue[queuePos] = (Vector2){-20.0f, 0.0f};
                state = GAME;
            }
            if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_P)) && state != PAUSED) {
                queuePos++;
                if (queuePos == maxLen) queuePos--;
                else queue[queuePos] = (Vector2){0.0f, -20.0f};
                state = GAME;
            }
            if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_SEMICOLON)) && state != PAUSED) {
                queuePos++;
                if (queuePos == maxLen) queuePos--;
                else queue[queuePos] = (Vector2){0.0f, 20.0f};
                state = GAME;
            }

            // movement
            if (frame % skipFrames == 0 && state != PAUSED) {

                if (queuePos > -1) {
                    if (!((speed.x == -queue[0].x && speed.x != 0) || (speed.y == -queue[0].y && speed.y != 0))) speed = queue[0];

                    for (int i = 0; i < queuePos; i++) queue[i] = queue[i+1];
                    queuePos--;
                }

                for (int i = snakeLength-1; i >= 0; i--) {
                    if (i == 0) {
                        snake[0].x += speed.x;
                        snake[0].y += speed.y;
                    } else snake[i] = snake[i-1];
                }
            }

            // Collision Logic
            if (state == GAME && frame > 10 && frame % skipFrames == 0) {
                for (int i = 1; i < snakeLength; i++) {
                    if (state != GAMEOVER) {
                        if (equals(&snake[0], &snake[i])) state = GAMEOVER;
                    }
                }

                if (snake[0].x < 0 || snake[0].x >= screenWidth || snake[0].y < 20 || snake[0].y >= screenHeight) state = GAMEOVER;

                for (int a = 0; a < numApples; a++) {
                    if (snake[0].x == apple[a].x && snake[0].y == apple[a].y) {
                        snakeLength += 4;
                        for(int i = snakeLength-5; i < snakeLength; i++) snake[i] = snake[snakeLength-5];
                        spawned[a] = 0;
                    }
                }
            }

            if (state == GAME) frame++;
            if (state == GAMEOVER) {
                if (snakeLength > snakeLongest) snakeLongest = snakeLength;
            }
            // Draw
            //----------------------------------------------------------------------------------
            //BeginDrawing();
            BeginTextureMode(canvas);
                ClearBackground(BLACK);

                // apple
                for (int i = 0; i < numApples; i++) DrawTexture(appleTexture, apple[i].x, apple[i].y, WHITE);

                enum bodyType b;
                Vector2 diff;

                //Body
                for (int i = snakeLength-2; i > 0; i--) {
                    bool up = 0, down = 0, left = 0, right = 0;
                    for (int a = -1; a < 2; a += 2) {
                        diff = (Vector2){snake[i+a].x-snake[i].x, snake[i+a].y-snake[i].y}; // might have to multiply by 1/-1
                        if (diff.x > 0) right = 1;
                        if (diff.x < 0) left = 1;
                        if (diff.y > 0) down = 1;
                        if (diff.y < 0) up = 1;
                    }
                    b = CENTER;
                    if (up && down) b = UPDOWN;
                    else if (right && left) b = RIGHTLEFT;
                    else if (up && right) b = UPRIGHT;
                    else if (right && down) b = RIGHTDOWN;
                    else if (down && left) b = DOWNLEFT;
                    else if (left && up) b = LEFTUP;
                    else if (up) b = ONLYUP;
                    else if (right) b = ONLYRIGHT;
                    else if (down) b = ONLYDOWN;
                    else if (left) b = ONLYLEFT;

                    DrawTextureRec(snakeTexture, snakeTextureRecs[b], snake[i], WHITE);
                }

                //Tail
                b = NONE;
                diff = (Vector2){snake[snakeLength-2].x-snake[snakeLength-1].x, snake[snakeLength-2].y-snake[snakeLength-1].y}; // might have to multiply by 1/-1
                for (int offset = 2; diff.x == diff.y; offset++) diff = (Vector2){snake[snakeLength-offset].x-snake[snakeLength-1].x, snake[snakeLength-offset].y-snake[snakeLength-1].y};
                if (diff.x > 0) b = RIGHT;
                if (diff.x < 0) b = LEFT;
                if (diff.y > 0) b = DOWN;
                if (diff.y < 0) b = UP;
                DrawTextureRec(snakeTailTexture, snakeTailTextureRecs[b], snake[snakeLength-1], WHITE);

                //Head
                b = NONE;
                diff = (Vector2){snake[1].x-snake[0].x, snake[1].y-snake[0].y}; // might have to multiply by 1/-1
                if (diff.x > 0) b = RIGHT;
                if (diff.x < 0) b = LEFT;
                if (diff.y > 0) b = DOWN;
                if (diff.y < 0) b = UP;
                DrawTextureRec(snakeHeadTexture, snakeHeadTextureRecs[b], snake[0], WHITE);

                // score
                DrawRectangle(0, 0, screenWidth, 20, GRAY);
                DrawText(FormatText("Length: %i", snakeLength), 0, 0, 20, WHITE);
                DrawText(FormatText("Longest: %i", snakeLongest), screenWidth / 4, 0, 20, WHITE);

                if(state == PAUSED) DrawText("[Paused]", screenWidth/2-MeasureText("[Paused]",30)/2, screenHeight/2, 30, WHITE);

            EndTextureMode();

            BeginDrawing();
                DrawTextureRec(canvas.texture, (Rectangle){0.0f, 0.0f, screenWidth, -screenHeight}, (Vector2){0, 0}, WHITE);
            EndDrawing();
        }

        //-----------------------------------------------------------------
        // Gameover screen
        if (state == GAMEOVER) {

            if (IsKeyPressed(KEY_ENTER)) {
                for (int i = 0; i < snakeLength; i++) snake[i] = (Vector2){500.0f, 300.0f};
                for (int i = 0; i < maxLen; i++) queue[i] = (Vector2){0.0f, 0.0f};
                for (int i = 0; i < 4; i++) spawned[i] = 0;
                speed = (Vector2){0.0f, 0.0f};

                snakeLength = 4;
                frame = 0;
                queuePos = -1;
                state = PREGAME;
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                for (int i = 0; i < snakeLength; i++) snake[i] = (Vector2){500.0f, 300.0f};
                for (int i = 0; i < maxLen; i++) queue[i] = (Vector2){0.0f, 0.0f};
                for (int i = 0; i < 4; i++) spawned[i] = 0;
                speed = (Vector2){0.0f, 0.0f};

                snakeLength = 4;
                frame = 0;
                queuePos = -1;

                state = MENU;
            }

            BeginDrawing();
                ClearBackground(BLACK);
                DrawTextureRec(canvas.texture, (Rectangle){0.0f, 0.0f, screenWidth, -screenHeight}, (Vector2){0, 0}, GRAY);
                DrawText("[Game Over]", (screenWidth - MeasureText("[Game Over]", 30))/2, screenHeight/2, 30, WHITE);

            EndDrawing();
        }
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    UnloadTexture(appleTexture);
    UnloadTexture(snakeTexture);
    UnloadTexture(snakeHeadTexture);
    UnloadTexture(snakeTailTexture);
    UnloadRenderTexture(canvas);
    //--------------------------------------------------------------------------------------

    return 0;
}
