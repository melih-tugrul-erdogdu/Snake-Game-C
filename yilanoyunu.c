#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 20
#define HEIGHT 20
#define INITIAL_LENGTH 3

typedef struct
{
    int x, y;
} Point;

Point snake[100];
int snakeLength = INITIAL_LENGTH;
Point apple;
int gameOver = 0;
int direction = 'd'; // Başlangıç yönü: sağ

HANDLE console;
COORD CursorPosition;

void gotoTop()
{
    CursorPosition.X = 0;
    CursorPosition.Y = 0;
    SetConsoleCursorPosition(console, CursorPosition);
}

void initializeGame()
{
    snake[0].x = WIDTH / 2;
    snake[0].y = HEIGHT / 2;

    for (int i = 1; i < snakeLength; i++)
    {
        snake[i].x = snake[0].x - i;
        snake[i].y = snake[0].y;
    }

    apple.x = (rand() % (WIDTH - 2)) + 1; // Sınırları ihlal etmemesi için
    apple.y = (rand() % (HEIGHT - 2)) + 1;
}

void draw()
{
    gotoTop();

    for (int y = 0; y <= HEIGHT; y++)
    {
        for (int x = 0; x <= WIDTH; x++)
        {
            if (y == 0 || y == HEIGHT || x == 0 || x == WIDTH)
            {
                printf("#"); // Sınır çizgileri
            }
            else if (x == snake[0].x && y == snake[0].y)
            {
                printf("O"); // Yılanın kafası
            }
            else if (x == apple.x && y == apple.y)
            {
                printf("A"); // Elma
            }
            else
            {
                int isBody = 0;
                for (int k = 1; k < snakeLength; k++)
                {
                    if (x == snake[k].x && y == snake[k].y)
                    {
                        printf("o"); // Yılanın gövdesi
                        isBody = 1;
                        break;
                    }
                }
                if (!isBody)
                    printf(" ");
            }
        }
        printf("\n");
    }
    printf("Puan: %d\n", snakeLength - INITIAL_LENGTH);
}

void input()
{
    if (_kbhit())
    {
        char key = _getch();
        // Tam tersi yönlere dönmeyi engellemek için kontrol
        if ((key == 'w' && direction != 's') ||
            (key == 's' && direction != 'w') ||
            (key == 'a' && direction != 'd') ||
            (key == 'd' && direction != 'a'))
        {
            direction = key;
        }
    }
}

void logic()
{
    Point prev = snake[0];
    Point prev2;

    // Yılanın kafasını yönlendirme
    switch (direction)
    {
    case 'w':
        snake[0].y--;
        break;
    case 'a':
        snake[0].x--;
        break;
    case 's':
        snake[0].y++;
        break;
    case 'd':
        snake[0].x++;
        break;
    }

    // Yılanın gövdesini takip ettirme
    for (int i = 1; i < snakeLength; i++)
    {
        prev2 = snake[i];
        snake[i] = prev;
        prev = prev2;
    }

    // Elmayı yerse yılanı uzatma
    if (snake[0].x == apple.x && snake[0].y == apple.y)
    {
        snakeLength++;
        apple.x = (rand() % (WIDTH - 2)) + 1;
        apple.y = (rand() % (HEIGHT - 2)) + 1;
    }

    // Sınır kontrolü (oyun bitişi)
    if (snake[0].x <= 0 || snake[0].x >= WIDTH || snake[0].y <= 0 || snake[0].y >= HEIGHT)
    {
        gameOver = 1;
    }

    // Yılanın kendine çarpması durumu (oyun bitişi)
    for (int i = 1; i < snakeLength; i++)
    {
        if (snake[i].x == snake[0].x && snake[i].y == snake[0].y)
        {
            gameOver = 1;
        }
    }
}

int main()
{
    console = GetStdHandle(STD_OUTPUT_HANDLE);
    initializeGame();

    while (!gameOver)
    {
        draw();
        input();
        logic();
        Sleep(100); // Yavaşlatmak için bekletme
    }

    printf("Oyun Bitti! Skorunuz: %d\n", snakeLength - INITIAL_LENGTH);

    return 0;
}
