#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    int x_pos;
    int y_pos;
} PLAYER;

int generate_map();
int move_player(PLAYER *player);

int main()
{
    initscr();
    clear();
    noecho();
    cbreak();

    //generate_random_map()
    /*mvprintw(4 , 20 , "--------");
    mvprintw(5 , 20 , "|......|");
    mvprintw(6 , 20 , "|......|");
    mvprintw(7 , 20 , "|......|");
    mvprintw(8 , 20 , "--------");
    mvprintw(6 , 20 , "+");
    refresh();*/

    // every room has these
    keypad(stdscr , TRUE);
    generate_map(10 , 20 , 5 , 22);
    refresh();
    PLAYER player1;
    player1.y_pos = 14;
    player1.x_pos = 14;
    mvprintw(player1.y_pos , player1.x_pos , "@");
    move(player1.y_pos , player1.x_pos);
    move_player(&player1);
    move(40,150);
    
    getch();
    endwin();
}

int generate_map(int left_x , int right_x , int up_y , int down_y)
{
    for(int i = up_y ; i <= down_y ; i++)
    {
        for(int j = left_x ; j <= right_x ; j++)
        {
            if(i == up_y || i == down_y)
            {
                mvprintw(j , i , "|");
            }
            else if(j == left_x || j == right_x)
            {
                mvprintw(j , i , "-");
            }
            else
            {
                mvprintw(j , i , ".");
            }
        }
        refresh();
    }
    return 1;
}

int move_player(PLAYER *player)
{
    while(1)
    {
        int mover = getch();
        if(mover == 113)
        {
            break;
        }
        if(mover == KEY_UP)
        {
            if(mvinch(player->y_pos - 1 , player->x_pos) == '.')
            {
                mvprintw(player->y_pos , player->x_pos , ".");
                player->y_pos--;
                mvprintw(player->y_pos , player->x_pos , "@");
                move(player->y_pos , player->x_pos);
            }
            else
            {
                move(player->y_pos , player->x_pos);
            }
        }
        if(mover == KEY_DOWN)
        {
            if(mvinch(player->y_pos + 1 , player->x_pos) == '.')
            {
                mvprintw(player->y_pos , player->x_pos , ".");
                player->y_pos++;
                mvprintw(player->y_pos , player->x_pos , "@");
                move(player->y_pos , player->x_pos);
            }
            else
            {
                move(player->y_pos , player->x_pos);
            }
        }
        if(mover == KEY_RIGHT)
        {
            if(mvinch(player->y_pos , player->x_pos + 1) == '.')
            {
                mvprintw(player->y_pos , player->x_pos , ".");
                player->x_pos++;
                mvprintw(player->y_pos , player->x_pos , "@");
                move(player->y_pos , player->x_pos);
            }
            else
            {
                move(player->y_pos , player->x_pos);
            }
        }
        if(mover == KEY_LEFT)
        {
            if(mvinch(player->y_pos , player->x_pos - 1) == '.')
            {
            mvprintw(player->y_pos , player->x_pos , ".");
            player->x_pos--;
            mvprintw(player->y_pos , player->x_pos , "@");
            move(player->y_pos , player->x_pos);
            }
            else
            {
                move(player->y_pos , player->x_pos);
            }
        }
        refresh();
    }
}