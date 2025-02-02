#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

pthread_t thread1 , thread2;

typedef struct 
{
    int left_col;
    int right_col;
    int up_row;
    int down_row;

    int been;
    
    int stair_y;
    int stair_x;

    int room_type;

    int cols_x[10];
    int cols_y[10];
    int col_count;

} ROOM;

typedef struct 
{
    int x_pos;
    int y_pos;

    int money;
    int health;

    int in_trap;
    int trap_damage;

    int last_room_type;
} PLAYER;

typedef struct 
{
    int x_pos;
    int y_pos;
    int amount;
    char symbol;
    int used;
} GOLD;

typedef struct 
{
    int x_pos;
    int y_pos;

    int second;
    int damage;
} TRAP;

typedef struct
{
    char password[5];
    int count;
    int flag;
} PASS_CHARNGER;

typedef struct
{
    int x_pos[1000];
    int y_pos[1000];
    int num;
} OBJECTS;

typedef struct 
{
    PLAYER * player;
    PASS_CHARNGER * password;
    pthread_mutex_t lock;
} LOCK;


int generate_map(ROOM room , char floor[50][180]);
int show_map(ROOM room , char floor[50][180] , char shown_floor[50][180]);
int make_hallway(int doors_x[10] , int doors_y[10] , char floor[50][180]);
int move_player(PLAYER * player , char floor[50][180] , char shown_floor[50][180] , GOLD gold[6] , int * floor_level , ROOM rooms[6] , TRAP trap[3] , PASS_CHARNGER password , LOCK lock1);
int gold_of_place(int x , int y , GOLD gold[6]);
int what_to_see(PLAYER * player , char floor[50][180] , char shown_floor[50][180] , GOLD gold[6] , int * floor_level , ROOM rooms[6]);
int show_everything(char floor[50][180] , char shown_floor[50][180] , GOLD gold[6] , int * floor_level , ROOM rooms[6]);
int is_in_trap(PLAYER * player , TRAP trap[3]);
void* countdown(void* arg);
int pass_door_menu();
void * countdown_pass(void * arg);
int column_finder(int y , int x , ROOM room);
int is_empty(int x , int y , OBJECTS object);

int main()
{
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr , TRUE);

    PLAYER player1 = {-1 , -1 , 0 , 30 ,  0 , 0 , 0};
    PASS_CHARNGER password = {"\0" , 0 , 0};
    LOCK lock1 = {&player1 , &password , PTHREAD_MUTEX_INITIALIZER};
    srand(time(NULL));

    ROOM rooms[4][6];

    char floor[4][50][180] , shown_floor[4][50][180];

    int end_program = 0;
    while(end_program < 4)
    {
        OBJECTS object;
        object.num = 0;
        // making null floor
        for(int i = 0 ; i < 50 ; i++)
        {
            for(int j = 0 ; j < 180 ; j++)
            {
                floor[end_program][i][j] = ' ';
                shown_floor[end_program][i][j] = ' ';
            }
        }
        clear();
        // room making
        for(int i = 0 ; i < 3 ; i++)
        {
            rooms[end_program][i].up_row = 5 + rand() % 10;
            rooms[end_program][i].down_row = rooms[end_program][i].up_row + 8 + rand() % (15 - rooms[end_program][i].up_row);
            rooms[end_program][i].room_type = 0;
            rooms[end_program][i].col_count = 0;
        }
        for(int i = 3 ; i < 6 ; i++)
        {
            rooms[end_program][i].up_row = 25 + rand() % 10;
            rooms[end_program][i].down_row = rooms[end_program][i].up_row + 8 + rand() % (35 - rooms[end_program][i].up_row);
            rooms[end_program][i].room_type = 0;
            rooms[end_program][i].col_count = 0;
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

        for(int i = 0 ; i < 6 ; i++)
        {
            generate_map(rooms[end_program][i] , floor[end_program]);
            rooms[end_program][i].been = 0;
        }
        
        int doors_x[10] , doors_y[10];

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
            //mvprintw(doors_y[i] , doors_x[i] , "+");
            floor[end_program][doors_y[i]][doors_x[i]] = '+';
            refresh();
        }
        //   print : & instead of + for down door of room 3
        //   make an object for the pass_key
        //   if in the door : two options : pay 10 money or use the key
        //   fix moving to &

        //
        floor[end_program][doors_y[4]][doors_x[4]] = '@';
        floor[end_program][rooms[end_program][1].up_row + 4][rooms[end_program][1].right_col] = '&';
        rooms[end_program][2].room_type = 1;

        rooms[end_program][3].room_type = 2;

        make_hallway(doors_x , doors_y , floor[end_program]);

        // giving every room a money:
        GOLD gold[6];
        for(int i = 0 ; i < 6 ; i++)
        {
            gold[i].symbol = '*';
            gold[i].x_pos = rooms[end_program][i].left_col + 1 + rand() % (rooms[end_program][i].right_col - rooms[end_program][i].left_col - 2);
            gold[i].y_pos = rooms[end_program][i].up_row + 1 + rand() % (rooms[end_program][i].down_row - rooms[end_program][i].up_row - 2);
            gold[i].amount = rand() % 40;
            gold[i].used = 0;
            object.x_pos[object.num] = gold[i].x_pos;
            object.y_pos[object.num] = gold[i].y_pos;
            object.num++;
        }
        
        // make stairs
        int stair_x , stair_y;
        while(1)
        {
            stair_x = rooms[end_program][5].left_col + 1 + rand() % (rooms[end_program][5].right_col - rooms[end_program][5].left_col - 2);
            stair_y = rooms[end_program][5].up_row + 1 + rand() % (rooms[end_program][5].down_row - rooms[end_program][5].up_row - 2);
            if(floor[end_program][stair_y][stair_x] == '.' && (stair_x != gold[5].x_pos || stair_y != gold[5].y_pos))
            {
                object.x_pos[object.num] = stair_x;
                object.y_pos[object.num] = stair_y;
                object.num++;
                break;
            }
        }

        rooms[end_program][5].stair_x = stair_x;
        rooms[end_program][5].stair_y = stair_y;
        refresh();

        // trap

        TRAP trap[3];
        for(int i = 0 ; i < 3 ; i++)
        {
            while(1)
            {
                int y_pos = rand() % 50;
                int x_pos = rand() % 180;
                if(floor[end_program][y_pos][x_pos] != ' ' && floor[end_program][y_pos][x_pos] != '+' && floor[end_program][y_pos][x_pos] != '|' && floor[end_program][y_pos][x_pos] != '-' && (x_pos != player1.x_pos || y_pos != player1.y_pos))
                {
                    trap[i].x_pos = x_pos;
                    trap[i].y_pos = y_pos;
                    object.x_pos[object.num] = x_pos;
                    object.y_pos[object.num] = y_pos;
                    object.num++;
                    trap[i].second = 4 + rand() % 3;
                    trap[i].damage = 10;
                    break;
                }
            }
        }
        // floor[end_program][trap[0].y_pos][trap[0].x_pos] = 'T';
        // floor[end_program][trap[1].y_pos][trap[1].x_pos] = 'T';
        // floor[end_program][trap[2].y_pos][trap[2].x_pos] = 'T';
        // 
        
        
        // make food



        // making columns
        for(int i = 0 ; i < 6 ; i++)
        {
            while(1)
            {
                rooms[end_program][i].cols_x[rooms[end_program][i].col_count] = rooms[end_program][i].left_col + 1 + rand() % (rooms[end_program][i].right_col - rooms[end_program][i].left_col - 2);
                rooms[end_program][i].cols_y[rooms[end_program][i].col_count] = rooms[end_program][i].up_row + 1 + rand() % (rooms[end_program][i].down_row - rooms[end_program][i].up_row - 2);
                if(is_empty(rooms[end_program][i].cols_x[rooms[end_program][i].col_count] , rooms[end_program][i].cols_y[rooms[end_program][i].col_count] , object) == 0)
                {
                    rooms[end_program][i].col_count++;
                    object.x_pos[object.num] = rooms[end_program][i].cols_x[rooms[end_program][i].col_count];
                    object.y_pos[object.num] = rooms[end_program][i].cols_y[rooms[end_program][i].col_count];
                    floor[end_program][object.y_pos[object.num]][object.x_pos[object.num]] = 'O';
                    object.num++;
                    break;
                }
            }
            while(1)
            {
                rooms[end_program][i].cols_x[rooms[end_program][i].col_count] = rooms[end_program][i].left_col + 1 + rand() % (rooms[end_program][i].right_col - rooms[end_program][i].left_col - 2);
                rooms[end_program][i].cols_y[rooms[end_program][i].col_count] = rooms[end_program][i].up_row + 1 + rand() % (rooms[end_program][i].down_row - rooms[end_program][i].up_row - 2);
                if(is_empty(rooms[end_program][i].cols_x[rooms[end_program][i].col_count] , rooms[end_program][i].cols_y[rooms[end_program][i].col_count] , object) == 0)
                {
                    rooms[end_program][i].col_count++;
                    object.x_pos[object.num] = rooms[end_program][i].cols_x[rooms[end_program][i].col_count];
                    object.y_pos[object.num] = rooms[end_program][i].cols_y[rooms[end_program][i].col_count];
                    floor[end_program][object.y_pos[object.num]][object.x_pos[object.num]] = 'O';
                    object.num++;
                    break;
                }
            }
        }

        show_map(rooms[end_program][0] , floor[end_program] , shown_floor[end_program]);
        mvprintw(doors_y[0] , doors_x[0] , "+");
        shown_floor[end_program][doors_y[0]][doors_x[0]] = '+';
        rooms[end_program][0].been = 1;
        mvprintw(gold[0].y_pos , gold[0].x_pos , "*");
        shown_floor[end_program][gold[0].y_pos][gold[0].x_pos] = '*';
        refresh();

        mvprintw(player1.y_pos , player1.x_pos , "H");
        move(player1.y_pos , player1.x_pos);

        move_player(&player1 , floor[end_program] , shown_floor[end_program] , gold , &end_program , rooms[end_program] , trap , password , lock1);
//        end_program = 5;
    }
    getch();
    endwin();
    // make rooms

    // make halls

    // save the floor in sth
    
    // place and move player

    printf("%d\n" , player1.health);
}

int generate_map(ROOM room , char floor[50][180])
{
    for(int i = room.left_col ; i <= room.right_col ; i++)
    {
        for(int j = room.up_row ; j <= room.down_row ; j++)
        {
            if(i == room.left_col || i == room.right_col)
            {
                //mvprintw(j , i , "|");
                floor[j][i] = '|';
            }
            else if(j == room.up_row || j == room.down_row)
            {
                //mvprintw(j , i , "-");
                floor[j][i] = '-';
            }
            else if(column_finder(j , i , room))
            {
                floor[j][i] = 'O';
            }
            else
            {
                //mvprintw(j , i , ".");
                floor[j][i] = '.';
            }
        }
        refresh();
    }
    return 1;
}

int show_map(ROOM room , char floor[50][180] , char shown_floor[50][180])
{
    for(int i = room.left_col ; i <= room.right_col ; i++)
    {
        for(int j = room.up_row ; j <= room.down_row ; j++)
        {
            if(floor[j][i] != '+')
            {
                if(floor[j][i] == '@')
                {
                    mvprintw(j , i , "@");
                    shown_floor[j][i] = '@';
                }
                else if(floor[j][i] == '&')
                {
                    mvprintw(j , i , "&");
                    shown_floor[j][i] = '&';
                }
                else if(i == room.left_col || i == room.right_col)
                {
                    mvprintw(j , i , "|");
                    shown_floor[j][i] = '|';
                }
                else if(j == room.up_row || j == room.down_row)
                {
                    mvprintw(j , i , "-");
                    shown_floor[j][i] = '-';
                }
                else if(column_finder(j , i , room))
                {
                    mvprintw(j , i , "O");
                    shown_floor[j][i] = 'O';
                }
                else
                {
                    mvprintw(j , i , ".");
                    shown_floor[j][i] = '.';
                }
            }
            else if(room.room_type == 2)
            {
                mvprintw(j , i , "|");
                shown_floor[j][i] = '|';
            }
            else
            {
                mvprintw(j , i , "+");
                shown_floor[j][i] = '+';
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
                if(floor[y1][x1 - 1] == ' ' && abs(x1 - x2) > abs(x1 - 1 - x2))
                {
                    x1--;
                }
                else if(floor[y1][x1 + 1] == ' ' && abs(x1 - x2) > abs(x1 + 1 - x2))
                {
                    x1++;
                }
                else if(floor[y1 - 1][x1] == ' ' && abs(y1 - y2) > abs(y1 - 1 - y2))
                {
                    y1--;
                }
                else if(floor[y1 + 1][x1] == ' ' && abs(y1 - y2) > abs(y1 + 1 - y2))
                {
                    y1++;
                }
            }
            else
            {
                if(floor[y1 - 1][x1] == ' ' && abs(y1 - y2) > abs(y1 - 1 - y2))
                {
                    y1--;
                }
                else if(floor[y1 + 1][x1] == ' ' && abs(y1 - y2) > abs(y1 + 1 - y2))
                {
                    y1++;
                }
                else if(floor[y1][x1 - 1] == ' ' && abs(x1 - x2) > abs(x1 - 1 - x2))
                {
                    x1--;
                }
                else if(floor[y1][x1 + 1] == ' ' && abs(x1 - x2) > abs(x1 + 1 - x2))
                {
                    x1++;
                }
            }
            //mvprintw(y1 , x1 , "#");
            floor[y1][x1] = '#';
            refresh();
        }
    }
}

int move_player(PLAYER * player , char floor[50][180] , char shown_floor[50][180] , GOLD gold[6] , int * floor_level , ROOM rooms[6] , TRAP trap[3] , PASS_CHARNGER password , LOCK lock1)
{
    while(1)
    {
        if(password.count == 0 && password.flag == 0)
        {
            pthread_mutex_lock(&lock1.lock);
            password.count = 1;
            pthread_mutex_unlock(&lock1.lock);
            pthread_create(&thread2, NULL, countdown_pass, &lock1);
        }
        else if(password.count == 1 && password.flag == 0)
        {
            pthread_mutex_lock(&lock1.lock);
            password.count = 1;
            pthread_mutex_unlock(&lock1.lock);
        }

        if(floor[player->y_pos][player->x_pos] == '@')
        {
            while(1)
            {
                int k = pass_door_menu();
                if(k == 0)
                {
                    clear();
                    refresh();
                    mvprintw(15 , 60 , "ENTER THE PASSWORD");
                    echo();
                    while(1)
                    {
                        char given[50];
                        mvgetnstr(18 , 60 , given , 50);
                        refresh();
                        if(strcmp(given , password.password) == 0)
                        {
                            getch();
                            break;
                        }
                        else
                        {
                            mvprintw(21 , 60 , "WRONG PASSWORD. ENTER AGAIN");
                            move(18 , 60);
                            clrtoeol();
                        }
                        refresh();
                    }
                    break;
                }
                else if(k == 1)
                {
                    clear();
                    refresh();
                    if(player->money >= 15)
                    {
                        mvprintw(15 , 60 , "YOU PAID 15 BUCKS");
                        player->money -= 15;
                        getch();
                        break;
                    }
                    else
                    {
                        mvprintw(15 , 60 , "NOT ENOUGH MONEY");
                        getch();
                    }
                }
            }
            refresh();
            clear();
            for(int j = 0 ; j < 50 ; j++)
            {
                for(int i = 0 ; i < 180 ; i++)
                {
                    mvprintw(j , i , "%c" , shown_floor[j][i]);
                }
            }
            refresh();
        }
        if(is_in_trap(player , trap) != 0)//function
        {
            floor[player->y_pos][player->x_pos] = 'T';
        }
        what_to_see(player , floor , shown_floor , gold , floor_level , rooms);
        mvprintw(player->y_pos , player->x_pos , "H");
        shown_floor[player->y_pos][player->x_pos] = 'H';
        move(player->y_pos , player->x_pos);



        int mover = getch();
        if(mover == 113)
        {
            floor_level[0]++;
            if(floor_level[0] == 4)
            {
                endwin();
                pthread_join(thread1, NULL);
                pthread_join(thread2, NULL);
                pthread_mutex_destroy(&lock1.lock);
            }
            break;
        }
        else if(mover == 109)
        {
            show_everything(floor , shown_floor , gold , floor_level , rooms);
            //int show_everything(char floor[50][180] , GOLD gold[6] , int * floor_level , rooms);
        }
        if(mover == KEY_UP)
        {
            if(mvinch(player->y_pos - 1 , player->x_pos) == '.' || mvinch(player->y_pos - 1 , player->x_pos) == '+' || mvinch(player->y_pos - 1 , player->x_pos) == '#')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                player->y_pos--;
                mvprintw(player->y_pos , player->x_pos , "H");
                shown_floor[player->y_pos][player->x_pos] = 'H';
                move(player->y_pos , player->x_pos);
            }
            else if(mvinch(player->y_pos - 1 , player->x_pos) == 'T')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                player->y_pos--;
                mvprintw(player->y_pos , player->x_pos , "H");
                shown_floor[player->y_pos][player->x_pos] = 'H';
                move(player->y_pos , player->x_pos);

                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "YOU ARE IN TRAP CELL");
            }
            else if(mvinch(player->y_pos - 1 , player->x_pos) == '*')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                player->y_pos--;
                int money_found = gold_of_place(player->x_pos , player->y_pos , gold);
                player->money += money_found;
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "YOU'VE FOUND %d AMOUNT OF MONEY" , money_found);
                mvprintw(player->y_pos , player->x_pos , "H");
                shown_floor[player->y_pos][player->x_pos] = 'H';
                move(player->y_pos , player->x_pos);
            }
            else if(mvinch(player->y_pos - 1 , player->x_pos) == '<')
            {
                floor_level[0]++;
                break;
            }
            else if(mvinch(player->y_pos - 1 , player->x_pos) == '@')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                player->y_pos--;
                mvprintw(player->y_pos , player->x_pos , "H");
                shown_floor[player->y_pos][player->x_pos] = 'H';
                move(player->y_pos , player->x_pos);
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
                shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                player->y_pos++;
                mvprintw(player->y_pos , player->x_pos , "H");
                shown_floor[player->y_pos][player->x_pos] = 'H';
                move(player->y_pos , player->x_pos);
            }
            else if(mvinch(player->y_pos + 1 , player->x_pos) == 'T')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                player->y_pos++;
                mvprintw(player->y_pos , player->x_pos , "H");
                shown_floor[player->y_pos][player->x_pos] = 'H';
                move(player->y_pos , player->x_pos);

                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "YOU ARE IN TRAP CELL");
            }
            else if(mvinch(player->y_pos + 1 , player->x_pos) == '*')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                player->y_pos++;
                int money_found = gold_of_place(player->x_pos , player->y_pos , gold);
                player->money += money_found;
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "YOU'VE FOUND %d AMOUNT OF MONEY" , money_found);
                mvprintw(player->y_pos , player->x_pos , "H");
                shown_floor[player->y_pos][player->x_pos] = 'H';
                move(player->y_pos , player->x_pos);
            }
            else if(mvinch(player->y_pos + 1 , player->x_pos) == '<')
            {
                floor_level[0]++;
                break;
            }
            else if(mvinch(player->y_pos + 1 , player->x_pos) == '@')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                player->y_pos++;
                mvprintw(player->y_pos , player->x_pos , "H");
                shown_floor[player->y_pos][player->x_pos] = 'H';
                move(player->y_pos , player->x_pos);
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
                shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                player->x_pos++;
                mvprintw(player->y_pos , player->x_pos , "H");
                shown_floor[player->y_pos][player->x_pos] = 'H';
                move(player->y_pos , player->x_pos);
            }
            else if(mvinch(player->y_pos , player->x_pos + 1) == 'T')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                player->x_pos++;
                mvprintw(player->y_pos , player->x_pos , "H");
                shown_floor[player->y_pos][player->x_pos] = 'H';
                move(player->y_pos , player->x_pos);

                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "YOU ARE IN TRAP CELL");
            }
            else if(mvinch(player->y_pos , player->x_pos + 1) == '*')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                player->x_pos++;
                int money_found = gold_of_place(player->x_pos , player->y_pos , gold);
                player->money += money_found;
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "YOU'VE FOUND %d AMOUNT OF MONEY" , money_found);
                mvprintw(player->y_pos , player->x_pos , "H");
                shown_floor[player->y_pos][player->x_pos] = 'H';
                move(player->y_pos , player->x_pos);
            }
            else if(mvinch(player->y_pos , player->x_pos + 1) == '<')
            {
                floor_level[0]++;
                break;
            }
            else if(mvinch(player->y_pos , player->x_pos + 1) == '&')
            {
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "THIS IS DOORS PASS : %s" , password.password);
                move(player->y_pos , player->x_pos);
            }
            else if(player->last_room_type == 2)
            {
                if(mvinch(player->y_pos , player->x_pos + 1) == '|' && floor[player->y_pos][player->x_pos + 1] == '+')
                {
                    mvprintw(player->y_pos , player->x_pos + 1 , "+");
                    shown_floor[player->y_pos][player->x_pos + 1] = '+';

                    mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                    shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                    player->x_pos++;

                    mvprintw(player->y_pos , player->x_pos , "H");
                    shown_floor[player->y_pos][player->x_pos] = 'H';
                    move(player->y_pos , player->x_pos);

                }
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
                shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                player->x_pos--;
                mvprintw(player->y_pos , player->x_pos , "H");
                shown_floor[player->y_pos][player->x_pos] = 'H';
                move(player->y_pos , player->x_pos);
            }
            else if(mvinch(player->y_pos , player->x_pos - 1) == 'T')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                player->x_pos--;
                mvprintw(player->y_pos , player->x_pos , "H");
                shown_floor[player->y_pos][player->x_pos] = 'H';
                move(player->y_pos , player->x_pos);

                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "YOU ARE IN TRAP CELL");
            }
            else if(mvinch(player->y_pos , player->x_pos - 1) == '*')
            {
                mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                player->x_pos--;
                int money_found = gold_of_place(player->x_pos , player->y_pos , gold);
                player->money += money_found;
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "YOU'VE FOUND %d AMOUNT OF MONEY" , money_found);
                mvprintw(player->y_pos , player->x_pos , "H");
                shown_floor[player->y_pos][player->x_pos] = 'H';
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

        int ind = is_in_trap(player , trap);
        if(ind == 0)
        {
            pthread_mutex_lock(&lock1.lock);
            player->in_trap = 0;
            pthread_mutex_unlock(&lock1.lock);
        }
        else if(!player->in_trap)
        {
            move(0 , 0);
            clrtoeol();
            mvprintw(0 , 0 , "YOU HAVE 4 SECONDS TO ESCAPE THE TRAP");
            pthread_mutex_lock(&lock1.lock);
            player->in_trap = 1;
            player->trap_damage = trap[ind - 1].damage;
            pthread_mutex_unlock(&lock1.lock);
            pthread_create(&thread1, NULL, countdown, &lock1);
        }
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

int what_to_see(PLAYER * player , char floor[50][180] , char shown_floor[50][180] , GOLD gold[6] , int * floor_level , ROOM rooms[6])
{
    // hall
    if(mvinch(player->y_pos - 1 , player->x_pos) == ' ' && floor[player->y_pos - 1][player->x_pos] == '#')
    {
        mvprintw(player->y_pos - 1 , player->x_pos , "#");
        shown_floor[player->y_pos - 1][player->x_pos] = '#';
    }
    if(mvinch(player->y_pos , player->x_pos - 1) == ' ' && floor[player->y_pos][player->x_pos - 1] == '#')
    {
        mvprintw(player->y_pos , player->x_pos - 1 , "#");
        shown_floor[player->y_pos][player->x_pos - 1] = '#';
    }
    if(mvinch(player->y_pos , player->x_pos + 1) == ' ' && floor[player->y_pos][player->x_pos + 1] == '#')
    {
        mvprintw(player->y_pos , player->x_pos + 1 , "#");
        shown_floor[player->y_pos][player->x_pos + 1] = '#';
    }
    if(mvinch(player->y_pos + 1 , player->x_pos) == ' ' && floor[player->y_pos + 1][player->x_pos] == '#')
    {
        mvprintw(player->y_pos + 1 , player->x_pos , "#");
        shown_floor[player->y_pos + 1][player->x_pos] = '#';
    }
    // door
    if(mvinch(player->y_pos - 1 , player->x_pos) == ' ' && floor[player->y_pos - 1][player->x_pos] == '+')
    {
        mvprintw(player->y_pos - 1 , player->x_pos , "+");
        shown_floor[player->y_pos - 1][player->x_pos] = '+';
    }
    if(mvinch(player->y_pos , player->x_pos - 1) == ' ' && floor[player->y_pos][player->x_pos - 1] == '+')
    {
        mvprintw(player->y_pos , player->x_pos - 1 , "+");
        shown_floor[player->y_pos][player->x_pos - 1] = '+';
    }
    if(mvinch(player->y_pos , player->x_pos + 1) == ' ' && floor[player->y_pos][player->x_pos + 1] == '+')
    {
        mvprintw(player->y_pos , player->x_pos + 1 , "+");
        shown_floor[player->y_pos][player->x_pos + 1] = '+';
    }
    if(mvinch(player->y_pos + 1 , player->x_pos) == ' ' && floor[player->y_pos + 1][player->x_pos] == '+')
    {
        mvprintw(player->y_pos + 1 , player->x_pos , "+");
        shown_floor[player->y_pos + 1][player->x_pos] = '+';
    }
    // rooms
    for(int i = 0 ; i < 6 ; i++)
    {
        if(player->y_pos >= rooms[i].up_row && player->y_pos <= rooms[i].down_row &&
        player->x_pos >= rooms[i].left_col && player->x_pos <= rooms[i].right_col)
        {
            player->last_room_type = rooms[i].room_type;
            if(rooms[i].been == 0)
            {
                rooms[i].been = 1;
                show_map(rooms[i] , floor , shown_floor);
                mvprintw(gold[i].y_pos , gold[i].x_pos , "*");
                shown_floor[gold[i].y_pos][gold[i].x_pos] = '*';
                if(i == 5)
                {
                    mvprintw(rooms[i].stair_y , rooms[i].stair_x , "<");
                    shown_floor[rooms[i].stair_y][rooms[i].stair_x] = '<';
                }
            }
        }
    }
}

int show_everything(char floor[50][180] , char shown_floor[50][180] , GOLD gold[6] , int * floor_level , ROOM rooms[6])
{
    clear();
    for(int j = 0 ; j < 50 ; j++)
    {
        for(int i = 0 ; i < 180 ; i++)
        {
            mvprintw(j , i , "%c" , floor[j][i]);
        }
    }
    refresh();
    usleep(1000000);
    while(1)
    {
        char c = getch();
        if(c == 'm')
        {
            clear();
            for(int j = 0 ; j < 50 ; j++)
            {
                for(int i = 0 ; i < 180 ; i++)
                {
                    mvprintw(j , i , "%c" , shown_floor[j][i]);
                }
            }
            break;
        }
    }
    return 1;
    // floor
}

int is_in_trap(PLAYER * player , TRAP trap[3])
{
    for(int i = 0 ; i < 3 ; i++)
    {
        if(player->x_pos ==  trap[i].x_pos && player->y_pos == trap[i].y_pos)
        {
            return i + 1;
        }
    }
    return 0;
}

void* countdown(void* arg)
{
    LOCK * lock1 = (LOCK*)arg;
    PLAYER * player = lock1->player;
    for(int i = 0 ; i < 5 ; i++)
    {
        sleep(1);
        pthread_mutex_lock(&lock1->lock);
        if(!player->in_trap)
        {
            move(0 , 0);
            clrtoeol();
            mvprintw(0 , 0 , "YOU ARE SAFE");
            move(player->y_pos , player->x_pos);
            refresh();
//            getch();
            pthread_mutex_unlock(&lock1->lock);
            return NULL;
        }
        pthread_mutex_unlock(&lock1->lock);
    }
    pthread_mutex_lock(&lock1->lock);
    player->health -= player->trap_damage;
    move(0 , 0);
    clrtoeol();
    mvprintw(0 , 0 , "YOU'VE LOST HEALTH , %d" , player->health);
    pthread_mutex_unlock(&lock1->lock);
    return NULL;
}

int pass_door_menu()
{
    clear();
    mvprintw(0 , 0 , "use up and down button to move and press enter");
    refresh();
    keypad(stdscr , TRUE);
    char options[2][15] = {"ENTER THE PASS"  , "PAY MONEY"};
    int x = 50 , y = 10;
    attron(A_REVERSE);
    mvprintw(y , x , "%s" , options[0]);
    attroff(A_REVERSE);
    mvprintw(y + 1 , x , "%s" , options[1]);
    int which = 0;
    while(1)
    {
        int mover = getch();
        if(mover == 10)
        {
            return which;
        }
        else
        {
            if(mover == KEY_UP)
            {
                which--;
            }
            if(mover == KEY_DOWN)
            {
                which++;
            }
            which += 2;
            which %= 2;
            for(int i = 0 ; i < 2 ; i++)
            {
                if(which == i)
                {
                    attron(A_REVERSE);
                    mvprintw(y + i , x , "%s" , options[i]);
                    attroff(A_REVERSE);
                }
                else
                {
                    mvprintw(y + i , x , "%s" , options[i]);
                }
            }
        }
        refresh();
    }
}

void * countdown_pass(void * arg)
{
    LOCK * lock1 = (LOCK *)arg;
    PASS_CHARNGER * password = lock1->password;
    for(int i = 0 ; i < 20 ; i++)
    {
        sleep(1);
        pthread_mutex_lock(&lock1->lock);
        if(password->flag == 1)
        {
            pthread_mutex_unlock(&lock1->lock);
            return NULL;
        }
        pthread_mutex_unlock(&lock1->lock);
    }
    pthread_mutex_lock(&lock1->lock);
    if(password->flag == 0)
    {
        password->count = 0;
        for(int i = 0 ; i < 4 ; i++)
        {
            password->password[i] = (rand() % 10) + 48;
        }
        password->password[4] = '\0';
    }
    pthread_mutex_unlock(&lock1->lock);
    return NULL;
}

int column_finder(int y , int x , ROOM room)
{
    for(int i = 0 ; i < room.col_count ; i++)
    {
        if(y == room.cols_y[i] && x == room.cols_x[i])
        {
            return 1;
        }
    }
    return 0;
}

int is_empty(int x , int y , OBJECTS object)
{
    for(int i = 0 ; i < object.num ; i++)
    {
        if(object.x_pos[i] == x && object.y_pos[i] == y)
        {
            return 1;
        }
    }
    return 0;
}