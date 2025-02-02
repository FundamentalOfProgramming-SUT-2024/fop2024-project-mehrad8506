#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

typedef struct 
{
    int left_col;
    int right_col;
    int up_row;
    int down_row;

    // doors:

} ROOM;

typedef struct 
{
    int x_pos;
    int y_pos;

    int money;
} PLAYER;

typedef struct 
{
    int x_pos;
    int y_pos;
    int amount;
    char symbol;
} GOLD;

int generate_map(int left_x , int right_x , int up_y , int down_y , char floor[50][180]);
int make_hallway(int doors_x[10] , int doors_y[10] , char floor[50][180]);
int move_player(PLAYER * player , char floor[50][180] , GOLD gold[6] , int * floor_level);
int gold_of_place(int x , int y , GOLD gold[6]);

int main()
{
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr , TRUE);

    PLAYER player1;
    player1.money = 0;

    srand(time(NULL));

    ROOM rooms[4][6];

    char floor[4][50][180];

    int end_program = 0;
    while(end_program < 4)
    {
        clear();
        for(int i = 0 ; i < 3 ; i++)
        {
            rooms[end_program][i].up_row = 5 + rand() % 10;
            rooms[end_program][i].down_row = rooms[end_program][i].up_row + 8 + rand() % (15 - rooms[end_program][i].up_row);
        }
        for(int i = 3 ; i < 6 ; i++)
        {
            rooms[end_program][i].up_row = 25 + rand() % 10;
            rooms[end_program][i].down_row = rooms[end_program][i].up_row + 8 + rand() % (35 - rooms[end_program][i].up_row);
        }
        for(int i = 0 ; i < 6 ; i += 3)
        {
            rooms[end_program][i].left_col = 20 + rand() % 20;
            rooms[end_program][i].right_col = rooms[end_program][i].left_col + 10 + rand() % 15;
        }
        for(int i = 1 ; i < 6 ; i += 3)
        {
            rooms[end_program][i].left_col = 70 + rand() % 20;
            rooms[end_program][i].right_col = rooms[end_program][i].left_col + 10 + rand() % 15;
        }
        for(int i = 2 ; i < 6 ; i += 3)
        {
            rooms[end_program][i].left_col = 120 + rand() % 20;
            rooms[end_program][i].right_col = rooms[end_program][i].left_col + 10 + rand() % 15;
        }
        
        
        player1.y_pos = (rooms[end_program][0].up_row + rooms[end_program][0].down_row) / 2;
        player1.x_pos = (rooms[end_program][0].left_col + rooms[end_program][0].right_col) / 2;
        
        int doors_x[10] , doors_y[10];


        // draw rooms
        for(int i = 0 ; i < 6 ; i++)
        {
            generate_map(rooms[end_program][i].up_row , rooms[end_program][i].down_row , rooms[end_program][i].left_col , rooms[end_program][i].right_col , floor[end_program]);
        }
        // make doors
        for(int i = 0 ; i < 10 ; i++)
        {
            if(i % 4 == 0)
            {
                doors_x[i] = rooms[end_program][i / 4].left_col + 1 + rand() % (rooms[end_program][i / 4].right_col - rooms[end_program][i / 4].left_col - 2);
                doors_y[i] = rooms[end_program][i / 4].down_row;
            }
            else if(i % 4 == 1)
            {
                doors_x[i] = rooms[end_program][3 + i / 4].left_col + 1 + rand() % (rooms[end_program][3 + i / 4].right_col - rooms[end_program][3 + i / 4].left_col - 2);
                doors_y[i] = rooms[end_program][3 + i / 4].up_row;
            }
            else if(i % 4 == 2)
            {
                doors_x[i] = rooms[end_program][3 + i / 4].right_col;
                doors_y[i] = rooms[end_program][3 + i / 4].up_row + 1 + rand() % (rooms[end_program][3 + i / 4].down_row - rooms[end_program][3 + i / 4].up_row - 2);
            }
            else
            {
                doors_x[i] = rooms[end_program][1 + i / 4].left_col;
                doors_y[i] = rooms[end_program][1 + i / 4].up_row + 1 + rand() % (rooms[end_program][1 + i / 4].down_row - rooms[end_program][1 + i / 4].up_row - 2);
            }
            mvprintw(doors_y[i] , doors_x[i] , "+");
            floor[end_program][doors_y[i]][doors_x[i]] = '+';
            refresh();
        }

        make_hallway(doors_x , doors_y , floor[end_program]);

        // giving every room a money:
        GOLD gold[6];
        for(int i = 0 ; i < 6 ; i++)
        {
            gold[i].symbol = '*';
            gold[i].x_pos = rooms[end_program][i].left_col + 1 + rand() % (rooms[end_program][i].right_col - rooms[end_program][i].left_col - 2);
            gold[i].y_pos = rooms[end_program][i].up_row + 1 + rand() % (rooms[end_program][i].down_row - rooms[end_program][i].up_row - 2);
            gold[i].amount = rand() % 40;
            mvprintw(gold[i].y_pos , gold[i].x_pos , "%c" , gold[i].symbol);
        }
        // make stairs
        refresh();
        int stair_x , stair_y;
        while(1)
        {
            stair_x = rooms[end_program][5].left_col + 1 + rand() % (rooms[end_program][5].right_col - rooms[end_program][5].left_col - 2);
            stair_y = rooms[end_program][5].up_row + 1 + rand() % (rooms[end_program][5].down_row - rooms[end_program][5].up_row - 2);
            if(mvinch(stair_y , stair_x) == '.')
            {
                break;
            }
        }
        mvprintw(stair_y , stair_x , "<");
        refresh();

        // 

        mvprintw(player1.y_pos , player1.x_pos , "@");
        move(player1.y_pos , player1.x_pos);

        move_player(&player1 , floor[end_program] , gold , &end_program);
//        end_program = 5;
    }
    getch();
    endwin();
    // make rooms

    // make halls

    // save the floor in sth
    
    // place and move player


}

int generate_map(int left_x , int right_x , int up_y , int down_y , char floor[50][180])
{
    for(int i = up_y ; i <= down_y ; i++)
    {
        for(int j = left_x ; j <= right_x ; j++)
        {
            if(i == up_y || i == down_y)
            {
                mvprintw(j , i , "|");
                floor[j][i] = '|';
            }
            else if(j == left_x || j == right_x)
            {
                mvprintw(j , i , "-");
                floor[j][i] = '-';
            }
            else
            {
                mvprintw(j , i , ".");
                floor[j][i] = '.';
            }
        }
        refresh();
    }
    return 1;
}

int make_hallway(int doors_x[10] , int doors_y[10] , char floor[50][180])
{
    for(int i = 0 ; i < 10 ; i += 2)
    {
        int x1 = doors_x[i];
        int x2 = doors_x[i + 1];
        int y1 = doors_y[i];
        int y2 = doors_y[i + 1];
        int count = 1000;
        while((abs(x1 - x2) > 0 || abs(y1 - y2) > 0) && (abs(x1 - x2) + abs(y1 - y2) >= 2))
        {
            if((rand() % 2 == 0 && abs(x1 - x2) > 1) || abs(y1 - y2) == 1)
            {
                if(mvinch(y1 , x1 - 1) == ' ' && abs(x1 - x2) > abs(x1 - 1 - x2))
                {
                    x1--;
                }
                else if(mvinch(y1 , x1 + 1) == ' ' && abs(x1 - x2) > abs(x1 + 1 - x2))
                {
                    x1++;
                }
                else if(mvinch(y1 - 1 , x1) == ' ' && abs(y1 - y2) > abs(y1 - 1 - y2))
                {
                    y1--;
                }
                else if(mvinch(y1 + 1 , x1) == ' ' && abs(y1 - y2) > abs(y1 + 1 - y2))
                {
                    y1++;
                }
            }
            else
            {
                if(mvinch(y1 - 1 , x1) == ' ' && abs(y1 - y2) > abs(y1 - 1 - y2))
                {
                    y1--;
                }
                else if(mvinch(y1 + 1 , x1) == ' ' && abs(y1 - y2) > abs(y1 + 1 - y2))
                {
                    y1++;
                }
                else if(mvinch(y1 , x1 - 1) == ' ' && abs(x1 - x2) > abs(x1 - 1 - x2))
                {
                    x1--;
                }
                else if(mvinch(y1 , x1 + 1) == ' ' && abs(x1 - x2) > abs(x1 + 1 - x2))
                {
                    x1++;
                }
            }
            mvprintw(y1 , x1 , "#");
            floor[y1][x1] = '#';
            refresh();
        }
    }
}

int move_player(PLAYER * player , char floor[50][180] , GOLD gold[6] , int * floor_level)
{
    while(1)
    {
        int mover = getch();
        if(mover == 113)
        {
            floor_level[0]++;
            break;
        }
        if(mover == KEY_UP)
        {
            if(mvinch(player->y_pos - 1 , player->x_pos) == '.' || mvinch(player->y_pos - 1 , player->x_pos) == '+' || mvinch(player->y_pos - 1 , player->x_pos) == '#')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                player->y_pos--;
                mvprintw(player->y_pos , player->x_pos , "@");
                move(player->y_pos , player->x_pos);
            }
            else if(mvinch(player->y_pos - 1 , player->x_pos) == '*')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                player->y_pos--;
                int money_found = gold_of_place(player->x_pos , player->y_pos , gold);
                player->money += money_found;
                mvprintw(0 , 0 , "YOU'VE FOUND %d AMOUNT OF MONEY" , money_found);
                mvprintw(player->y_pos , player->x_pos , "@");
                move(player->y_pos , player->x_pos);
            }
            else if(mvinch(player->y_pos - 1 , player->x_pos) == '<')
            {
                floor_level[0]++;
                break;
            }
            else
            {
                move(player->y_pos , player->x_pos);
            }
        }
        if(mover == KEY_DOWN)
        {
            if(mvinch(player->y_pos + 1 , player->x_pos) == '.' || mvinch(player->y_pos + 1 , player->x_pos) == '+' || mvinch(player->y_pos + 1 , player->x_pos) == '#')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                player->y_pos++;
                mvprintw(player->y_pos , player->x_pos , "@");
                move(player->y_pos , player->x_pos);
            }
            else if(mvinch(player->y_pos + 1 , player->x_pos) == '*')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                player->y_pos++;
                int money_found = gold_of_place(player->x_pos , player->y_pos , gold);
                player->money += money_found;
                mvprintw(0 , 0 , "YOU'VE FOUND %d AMOUNT OF MONEY" , money_found);
                mvprintw(player->y_pos , player->x_pos , "@");
                move(player->y_pos , player->x_pos);
            }
            else if(mvinch(player->y_pos + 1 , player->x_pos) == '<')
            {
                floor_level[0]++;
                break;
            }
            else
            {
                move(player->y_pos , player->x_pos);
            }
        }
        if(mover == KEY_RIGHT)
        {
            if(mvinch(player->y_pos , player->x_pos + 1) == '.' || mvinch(player->y_pos , player->x_pos + 1) == '+' || mvinch(player->y_pos , player->x_pos + 1) == '#')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                player->x_pos++;
                mvprintw(player->y_pos , player->x_pos , "@");
                move(player->y_pos , player->x_pos);
            }
            else if(mvinch(player->y_pos , player->x_pos + 1) == '*')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                player->x_pos++;
                int money_found = gold_of_place(player->x_pos , player->y_pos , gold);
                player->money += money_found;
                mvprintw(0 , 0 , "YOU'VE FOUND %d AMOUNT OF MONEY" , money_found);
                mvprintw(player->y_pos , player->x_pos , "@");
                move(player->y_pos , player->x_pos);
            }
            else if(mvinch(player->y_pos , player->x_pos + 1) == '<')
            {
                floor_level[0]++;
                break;
            }
            else
            {
                move(player->y_pos , player->x_pos);
            }
        }
        if(mover == KEY_LEFT)
        {
            if(mvinch(player->y_pos , player->x_pos - 1) == '.' || mvinch(player->y_pos , player->x_pos - 1) == '+' || mvinch(player->y_pos , player->x_pos - 1) == '#')
            {
            mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
            player->x_pos--;
            mvprintw(player->y_pos , player->x_pos , "@");
            move(player->y_pos , player->x_pos);
            }
            else if(mvinch(player->y_pos , player->x_pos - 1) == '*')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                player->x_pos--;
                int money_found = gold_of_place(player->x_pos , player->y_pos , gold);
                player->money += money_found;
                mvprintw(0 , 0 , "YOU'VE FOUND %d AMOUNT OF MONEY" , money_found);
                mvprintw(player->y_pos , player->x_pos , "@");
                move(player->y_pos , player->x_pos);
            }
            else if(mvinch(player->y_pos , player->x_pos - 1) == '<')
            {
                floor_level[0]++;
                break;
            }
            else
            {
                move(player->y_pos , player->x_pos);
            }
        }
        refresh();
    }
}

int gold_of_place(int x , int y , GOLD gold[6])
{
    for(int i = 0 ; i < 6 ; i++)
    {
        if(x == gold[i].x_pos && y == gold[i].y_pos)
        {
            return gold[i].amount;
        }
    }
    return 0;
}