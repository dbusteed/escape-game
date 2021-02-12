#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// grid dimensions
#define X 10
#define Y 10

// printable characters
#define PLAYER '@'
#define ENEMY '*'
#define BLANK ' '
#define V_BORDER '|'
#define H_BORDER '-'
#define C_BORDER '+'

// generalized struct for the 
// player and the AI enemies
typedef struct Army {
    int x;
    int y;
} Army;

// struct for controlling game status
typedef struct Game {
    bool playing;
    bool won;
} Game;

// function definitions
void print_menu();
bool check_enemy_position(Army es[], int posX, int posY, int level);
void print_grid(char grid[X][Y], int level, char border[]);
Game update_grid(Game game, char grid[X][Y], Army p, Army es[], int level);
Army place_enemy(Army p, char inp, Army es[], int i);

int main()
{
    // init random seed
    srand(time(NULL));

    // used for loop control
    Game game;
    game.playing = true;
    game.won = true;

    // grid, input, other vars
    char grid[X][Y];
    char inp;
    int i, j, posX, posY;
    bool valid_pos, enemy_pos;

    // create the top border
    char border[(2*Y)+2];
    for(i=0; i<(2*Y)+2; i++)
    {
        if(i==0 || i==(2*Y)+1)
        {
            // corner border
            border[i] = C_BORDER;
        }
        else
        {
            border[i] = H_BORDER;
        }
    }

    int enemies = 100;

    // the player
    Army p;

    // the (e)nemie(s)
    Army *es = malloc(enemies * sizeof(Army));

    print_menu();

    // continue as player advances
    // thru the levels
    while(game.won)
    {
        game.playing = true;

        // set up player location
        p.x = X / 2;
        p.y = Y / 2;

        es = realloc(es, enemies * sizeof(Army)); 

        // setup positions for the enemies
        for(i=0; i<enemies; i++)
        {
            es[i].x = -1;
            es[i].y = -1;
        }

        es[0] = place_enemy(p, "w", es, i);

        // build grid and print
        update_grid(game, grid, p, es, enemies);
        print_grid(grid, enemies, border);

        int moves = 1;

        // game loop for the current level
        while(game.playing)
        {
            // handle the input
            printf("?: ");
            scanf("%c", &inp);
            getchar();
            printf("\n");

            // player movement
            if(inp == 'w')
            {
                p.x -= 1;
            }
            else if(inp == 's')
            {
                p.x += 1;
            }
            else if(inp == 'a')
            {
                p.y -= 1;
            }
            else if(inp == 'd')
            {
                p.y += 1;
            }

            es[moves] = place_enemy(p, inp, es, moves);
            moves += 1;

            game = update_grid(game, grid, p, es, enemies);
            print_grid(grid, enemies, border);
        }

        if(game.won)
        {
            printf("you won!\n");
            printf("press ENTER to start next level...");
            getchar();

            enemies += 1;
        }
        else
        {
            printf("you lost!\n");
        }
    }

    free(es);
    return 0;
}

void print_menu()
{
    system("clear");

    // TODO be fancy and add some colors
    printf("+----------+\n");
    printf("|  ESCAPE  |\n");
    printf("+----------+\n");
    printf("\n");
    printf("* escape from your enemies by\n");
    printf("   reaching the border of the map\n");
    printf("* use WASD + ENTER to move\n\n");
    printf("press ENTER to begin...");
    getchar();
}

bool check_enemy_position(Army es[], int posX, int posY, int level)
{
    int i;
    for(i=0; i<level; i++)
    {
        // return false if the new position
        // conflicts with another enemy position
        if(es[i].x == posX && es[i].y == posY)
        {
            return false;
        }
        // or if the new position is off the board
        else if(posX < 0 || posX >= X || posY < 0 || posY >= Y)
        {
            return false;
        }
    }

    // otherwise return true!
    return true;
}

Game update_grid(Game game, char grid[X][Y], Army p, Army es[], int level)
{
    bool collision = false;

    // create blank grid
    int i, j;
    for(i=0; i<X; i++)
    {
        for(j=0; j<Y; j++)
        {
            grid[i][j] = BLANK;
        }
    }

    // draw enemies
    for(i=0; i<level; i++)
    {
        if(es[i].x > -1 && es[i].y > -1)
        {
            grid[es[i].x][es[i].y] = ENEMY;
        }

        // check if enemy collided with player
        if(es[i].x == p.x && es[i].y == p.y)
        {
            collision = true;
        }
    }

    // this needs to be separated from the above loop
    // so that all enemies are drawn regardless
    if(collision)
    {
        game.playing = false;
        game.won = false;
        return game;
    }

    // check if player has won, if not draw player
    if(p.x < 0 || p.x >= X || p.y < 0 || p.y >= Y)
    {
        game.playing = false;
        game.won = true;
        return game;
    }

    grid[p.x][p.y] = PLAYER;

    game.playing = true;
    return game;
}

void print_grid(char grid[X][Y], int level, char border[])
{
    int i, j;

    system("clear");

    printf("%s\n", border);

    // side border, grid characters, side border
    for(i=0; i<X; i++)
    {
        printf("%c", V_BORDER);
        for(j=0; j<Y; j++)
        {
            printf("%c ", grid[i][j]);
        }
        printf("%c\n", V_BORDER);
    }

    printf("%s\n", border);
}

Army place_enemy(Army p, char inp, Army es[], int i)
{
    bool valid_pos, enemy_pos;
    int j, posX, posY;
    
    

    // change X, Y to be a certain bounds

    valid_pos = false;
    while(!valid_pos)
    {
        enemy_pos = false;
        posX = (rand() % X);
        posY = (rand() % Y);

        // check if near Player
        if((posX >= ((p.x) - 1) && posX <= ((p.x) + 1)) && ((posY >= ((p.y) - 1) && posY <= ((p.y) + 1))))
        {
            // invalid position
        }
        else
        {
            // check previous enemy positions
            for(j=0; j<i; j++)
            {
                if(posX == es[j].x && posY == es[j].y)
                {
                    enemy_pos = true;
                }
            }

            if(enemy_pos)
            {
                valid_pos = false;
            }
            else
            {
                valid_pos = true;
            }
        }
    }

    Army tmp;
    tmp.x = posX;
    tmp.y = posY;

    return tmp;
}