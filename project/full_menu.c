#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

typedef struct
{
    char name[50];
    char pass[50];
    char email[50];
    int score;
    int money;
    int games_played;
    int exp;
} USER_DATA;

pthread_t thread1 , thread2;

typedef struct
{
    int damage;
    int type;
    int range;
    int count;
    int direction;
    int active;
    char symbol;
} WEAPON;

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
    int last_room;

    int hunger;
    int hunger_timer;

    int health_charm;
    int power_charm;
    int speed_charm;

    pthread_mutex_t lock;
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
    pthread_mutex_t lock;
} PASS_CHARNGER;

typedef struct
{
    int x_pos[1000];
    int y_pos[1000];
    int num;
} OBJECTS;

typedef struct 
{
    int type;
    int x_pos;
    int y_pos;
    int health;
    int damage;
    char symbol;
    int speed;
    int timer;
    int active;
    int room_number;
    int moves;
} MONSTER;

typedef struct 
{
    int x_pos;
    int y_pos;
    int range;
    int direction;
    int active;
    char symbol;
    int timer;
    int moves;
    int damage;
} DAGGER;

typedef struct
{
    int type;
    int timer;
    int x_pos;
    int y_pos;
    int active;
    int damage;
    int awe_timer;
    int awe_active;
    int mag_timer;
    int mag_active;
    char symbol;
} FOOD;

typedef struct 
{
    int x_pos;
    int y_pos;
    int used;
    int type;
    int timer;
    int active;
    char symbol;
} CHARM;

int make_menu();
int profile_page();
int score_board();

int compare_users();
void sort_users_by_score();

int make_menu_aval();
int sign_up_menu();
int user_name_exist(char name[50]);
void save_info(USER_DATA user);
int valid_pass(char password[50]);
int valid_mail(char email[50]);
void generate_password(char *password, int length);
int user_name_exist_2(char name[50] , char pass[50] , char email[50]);
int log_in_menu();

int generate_map(ROOM room , char floor[50][180]);
int show_map(ROOM room , char floor[50][180] , char shown_floor[50][180]);
int make_hallway(int doors_x[10] , int doors_y[10] , char floor[50][180]);
int move_player(PLAYER * player , char floor[50][180] , char shown_floor[50][180] , GOLD gold[6] , int * floor_level , ROOM rooms[6] , TRAP trap[3] , PASS_CHARNGER password , MONSTER * monster , DAGGER * dagger , FOOD foods[4] , CHARM charm[4]);
int gold_of_place(int x , int y , GOLD gold[6]);
int what_to_see(PLAYER * player , char floor[50][180] , char shown_floor[50][180] , GOLD gold[6] , int * floor_level , ROOM rooms[6] , MONSTER monster[6]);
int show_everything(char floor[50][180] , char shown_floor[50][180] , GOLD gold[6] , int * floor_level , ROOM rooms[6]);
int is_in_trap(PLAYER * player , TRAP trap[3]);
void* countdown(void* arg);
int pass_door_menu();
void * countdown_pass(void * arg);
int column_finder(int y , int x , ROOM room);
int is_empty(int x , int y , OBJECTS object);
int move_monster(MONSTER * monster , PLAYER * player , char floor[50][180] , char shown_floor[50][180] , int * floor_level);
int move_dagger(DAGGER * dagger , char floor[50][180] , char shown_floor[50][180] , MONSTER * monster , PLAYER * player);
int food_basics(FOOD * food);
int eat_food_menu(FOOD foods[4]);
int use_charm_menu(CHARM charm[4]);
int init_monsters(MONSTER monster[6] , ROOM rooms[6]);


USER_DATA user_main;

int main()
{
    initscr();
    clear();
    noecho();
    cbreak();
    srand(time(NULL));
    
    int num = make_menu_aval();
    if(num == 0)
    {
        sign_up_menu();
    }
    if(num == 1){
        log_in_menu();
    }
    
    strcpy(user_main.name , "mehrad1");
    strcpy(user_main.email , "");
    strcpy(user_main.pass , "");
    user_main.money = 0;
    user_main.exp = 0;
    user_main.games_played = 0;
    user_main.score = 0;

    clear();
    noecho();
    cbreak();
    // here we make our new menu
    int num_2  = make_menu(user_main);
    
    // print info : print : user.kossher
    
    // profile menu : users info : ranking - name - score - money - games_played - exp


    //here it ends
    getch();
    clear();
    noecho();
    cbreak();
    keypad(stdscr , TRUE);
    halfdelay(1);

    PLAYER player1 = {-1 , -1 , 0 , 100 ,  0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , PTHREAD_MUTEX_INITIALIZER};
    PASS_CHARNGER password = {"\0" , 0 , 0 , PTHREAD_MUTEX_INITIALIZER};
    srand(time(NULL));

    ROOM rooms[4][6];

    char floor[4][50][180] , shown_floor[4][50][180];

    int end_program = 0;
    while(end_program < 4)
    {
        MONSTER monster[6];
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
        init_monsters(monster , rooms[end_program]);
        // for(int i = 0 ; i < 6 ; i++)
        // {
        //     monster[i].x_pos = rooms[end_program][i].left_col + 1;
        //     monster[i].y_pos = rooms[end_program][i].up_row + 1;
        //     monster[i].symbol = 'S';
        //     monster[i].speed = 10;
        //     monster[i].timer = 0;
        //     monster[i].damage = 10;
        //     monster[i].health = 20;
        //     monster[i].active = 0;
        //     monster[i].room_number = i;
        // }
        // monster[0].active = 1;
        
        player1.y_pos = (rooms[end_program][0].up_row + rooms[end_program][0].down_row) / 2;
        player1.x_pos = (rooms[end_program][0].left_col + rooms[end_program][0].right_col) / 2;

        for(int i = 0 ; i < 6 ; i++)
        {
            generate_map(rooms[end_program][i] , floor[end_program]);
            rooms[end_program][i].been = 0;
        }
        //floor[end_program][monster.y_pos][monster.x_pos] = 'S';
        
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
            gold[i].amount = rand() % 20;
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
        FOOD foods[4];
        while(1)
        {
            int x = rand() % 180 , y = rand() % 50;
            if(floor[end_program][y][x] == '.' && is_empty(x , y , object) == 0)
            {
                foods[end_program].x_pos = x;
                foods[end_program].y_pos = y;
                object.x_pos[object.num] = x;
                object.y_pos[object.num] = y;
                object.num++;
                break;
            }
        }
        food_basics(&foods[end_program]);
        floor[end_program][foods[end_program].y_pos][foods[end_program].x_pos] = foods[end_program].symbol;

        // making charms:
        CHARM charm[4];
        while(1)
        {
            int x = rand() % 180 , y = rand() % 50;
            if(floor[end_program][y][x] == '.' && is_empty(x , y , object) == 0)
            {
                charm[end_program].x_pos = x;
                charm[end_program].y_pos = y;
                object.x_pos[object.num] = x;
                object.y_pos[object.num] = y;
                object.num++;
                break;
            }
        }
        if(end_program == 0)
        {
            charm[end_program].symbol = 's';
            charm[end_program].timer = 0;
            charm[end_program].used = 0;
            charm[end_program].type = 1;
            charm[end_program].active = 0;
        }
        else if(end_program == 1)
        {
            charm[end_program].symbol = 'p';
            charm[end_program].timer = 0;
            charm[end_program].used = 0;
            charm[end_program].type = 2;
            charm[end_program].active = 0;
        }
        else
        {
            charm[end_program].symbol = 'h';
            charm[end_program].timer = 0;
            charm[end_program].used = 0;
            charm[end_program].type = 3;
            charm[end_program].active = 0;
        }
        floor[end_program][charm[end_program].y_pos][charm[end_program].x_pos] = charm[end_program].symbol;
        // making columns
        for(int i = 0 ; i < 6 ; i++)
        {
            while(1)
            {
                rooms[end_program][i].cols_x[rooms[end_program][i].col_count] = rooms[end_program][i].left_col + 2 + rand() % (rooms[end_program][i].right_col - rooms[end_program][i].left_col - 4);
                rooms[end_program][i].cols_y[rooms[end_program][i].col_count] = rooms[end_program][i].up_row + 2 + rand() % (rooms[end_program][i].down_row - rooms[end_program][i].up_row - 4);
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
                rooms[end_program][i].cols_x[rooms[end_program][i].col_count] = rooms[end_program][i].left_col + 2 + rand() % (rooms[end_program][i].right_col - rooms[end_program][i].left_col - 4);
                rooms[end_program][i].cols_y[rooms[end_program][i].col_count] = rooms[end_program][i].up_row + 2 + rand() % (rooms[end_program][i].down_row - rooms[end_program][i].up_row - 4);
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
        mvprintw(monster[0].y_pos , monster[0].x_pos , "%c" , monster[0].symbol);
        shown_floor[end_program][monster[0].y_pos][monster[0].x_pos] = monster[0].symbol;
        refresh();

        mvprintw(player1.y_pos , player1.x_pos , "H");
        move(player1.y_pos , player1.x_pos);

        DAGGER dagger;
        dagger.active = 0;
        dagger.range = 5;
        dagger.symbol = '~';
        dagger.timer = 0;
        dagger.moves = 0;
        dagger.damage = 12;

        move_player(&player1 , floor[end_program] , shown_floor[end_program] , gold , &end_program , rooms[end_program] , trap , password , monster , &dagger , foods , charm);
//        end_program = 5;
    }
    getch();
    endwin();
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    endwin();
}

int make_menu(USER_DATA user)
{
    clear();
    mvprintw(0 , 0 , "use up and down button to move and press enter");
    refresh();
    keypad(stdscr , TRUE);
    char options[5][15] = {"NEW GAME"  , "LOAD GAME" , "SCORE TABLE" , "MY PROFILE" , "SETTINGS"};
    int x = 50 , y = 10;
    attron(A_REVERSE);
    mvprintw(y , x , "%s" , options[0]);
    attroff(A_REVERSE);
    mvprintw(y + 1 , x , "%s" , options[1]);
    mvprintw(y + 2 , x , "%s" , options[2]);
    mvprintw(y + 3 , x , "%s" , options[3]);
    mvprintw(y + 4 , x , "%s" , options[4]);
    int which = 0;
    while(1)
    {
        int mover = getch();
        if(mover == 10)
        {
            if(which == 2)
            {
                // make scoreboard
                // return make menu
                score_board(user);
                return make_menu(user);
            }
            if(which == 3)
            {
                // make profile page
                // return make menu
                profile_page(user);
                return make_menu(user);
            }
            if(which == 4)
            {
                // make settings page
                // return make menu
            }
            else
            {
                return which;
            }
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
            which += 5;
            which %= 5;
            for(int i = 0 ; i < 5 ; i++)
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

int profile_page(USER_DATA user)
{
    clear();
    mvprintw(4 , 40 , "your name is %s" , user.name);
    mvprintw(5 , 40 , "your email is %s" , user.email);
    mvprintw(6 , 40 , "your password is %s" , user.pass);
    mvprintw(7 , 40 , "your score is %d" , user.score);
    mvprintw(8 , 40 , "your exp is %d" , user.exp);
    mvprintw(9 , 40 , "your games played is %d" , user.games_played);
    mvprintw(10 , 40 , "your money is %d" , user.money);
    refresh();
    getch();
    return 1;

}

int score_board(USER_DATA user)
{
    clear();
    USER_DATA all_users[1000];
    FILE *file = fopen("users_info2.txt" , "r");
    if(file == NULL)
    {
        return 0;
    }
    int ind = 0;
    while(fscanf(file , "%s %s %s %d %d %d %d\n" , all_users[ind].name , all_users[ind].pass , all_users[ind].email , &all_users[ind].score , &all_users[ind].exp , &all_users[ind].games_played , &all_users[ind].money) == 7)
    {
        ind++;
    }
    sort_users_by_score(all_users, ind);
    mvprintw(1 , 40 , "THIS IS THE RANKING");
    for(int i = 1 ; i <= ind ; i++)
    {
        if(strcmp(all_users[i - 1].name , user.name) == 0)
        {
            attron(A_REVERSE);
            mvprintw(1 + i , 1 , "%d." , i);
            mvprintw(1 + i , 5 , "%s" , all_users[i - 1].name);
            mvprintw(1 + i , 25 , "%d" , all_users[i - 1].score);
            mvprintw(1 + i , 30 , "%d" , all_users[i - 1].exp);
            mvprintw(1 + i , 35 , "%d" , all_users[i - 1].games_played);
            mvprintw(1 + i , 40 , "%d" , all_users[i - 1].money);
            attroff(A_REVERSE);
        }
        else
        {
            mvprintw(1 + i , 1 , "%d." , i);
            mvprintw(1 + i , 5 , "%s" , all_users[i - 1].name);
            mvprintw(1 + i , 25 , "%d" , all_users[i - 1].score);
            mvprintw(1 + i , 30 , "%d" , all_users[i - 1].exp);
            mvprintw(1 + i , 35 , "%d" , all_users[i - 1].games_played);
            mvprintw(1 + i , 40 , "%d" , all_users[i - 1].money);
        }
    }
    getch();
    return 1;
}

int compare_users(const void *a, const void *b)
{
    USER_DATA *user_a = (USER_DATA *)a;
    USER_DATA *user_b = (USER_DATA *)b;
    return user_b->score - user_a->score; // برای مرتب‌سازی نزولی
}

void sort_users_by_score(USER_DATA users[], int num_users)
{
    qsort(users, num_users, sizeof(USER_DATA), compare_users);
}





int make_menu_aval()
{
    clear();
    mvprintw(0 , 0 , "use up and down button to move and press enter");
    refresh();
    keypad(stdscr , TRUE);
    char options[3][15] = {"SIGN UP"  , "LOG IN" , "GUEST MODE"};
    int x = 50 , y = 10;
    attron(A_REVERSE);
    mvprintw(y , x , "%s" , options[0]);
    attroff(A_REVERSE);
    mvprintw(y + 1 , x , "%s" , options[1]);
    mvprintw(y + 2 , x , "%s" , options[2]);
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
            which += 3;
            which %= 3;
            for(int i =  0 ; i < 3 ; i++)
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

int sign_up_menu()
{
    clear();
    mvprintw(6 , 50 , "%s" , "NAME : ");
    mvprintw(10 , 50 , "%s" , "PASSWORD : ");
    mvprintw(14 , 50 , "%s" , "EMAIL : ");
    refresh();
    echo();
    while(1)
    {    
        mvgetnstr(6 , 57  , user_main.name , 50);
        if(user_name_exist(user_main.name) == 1)
        {
            mvprintw(16 , 50 , "%s" , "User Name Already Exists!");
            move(6 , 57);
            clrtoeol();
            refresh();
        }
        else
        {
            break;
        }
    }
    
    while(1)
    {
        mvprintw(18 , 50 , "%s" , "Enter G For Random Generated Password");
        mvgetnstr(10 , 61 , user_main.pass , 50);
        if(valid_pass(user_main.pass))
        {
            move(18 , 50);
            clrtoeol();
            move(16 , 50);
            clrtoeol();
            refresh();
            break;
        }
        else if(strcmp(user_main.pass , "G") == 0)
        {
            move(16 , 50);
            clrtoeol();
            char rand_pass[13]; 
            generate_password(rand_pass , 12);
            mvprintw(16 , 50 , "Your Random Password : %s" , rand_pass);
            move(10 , 61);
            clrtoeol();
            refresh();
        }
        else
        {
            move(16 , 50);
            clrtoeol();
            mvprintw(16 , 50 , "%s" , "Password Format Is Not Correct!");
            move(10 , 61);
            clrtoeol();
            refresh();
        }
    }

    while (1)
    {
        mvgetnstr(14 , 58  , user_main.email , 50);
        if(valid_mail(user_main.email))
        {
            break;
        }
        else
        {
            move(16 , 50);
            clrtoeol();
            mvprintw(16 , 50 , "%s" , "Email Format Is Not Correct!");
            move(14 , 58);
            clrtoeol();
            refresh();
        }
    }

    save_info(user_main);
    endwin();

}

int user_name_exist(char name[50])
{
    char file_name[50] , file_password[50] , file_email[50] , n[10] , p[20] , e[10];
    FILE *file = fopen("users_info.txt" , "r");
    if(file == NULL)
    {
        return 0;
    }
    else
    {
        while(fscanf(file, "%s %s %s %s %s %s\n", n , file_name , p , file_password , e , file_email) == 6) 
        {
            if(strcmp(name , file_name) == 0)
            {
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
    return 0;
}

void save_info(USER_DATA user)
{
    FILE *file = fopen("users_info.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "Name: %s Password: %s  Email: %s\n", user.name, user.pass, user.email);
        fclose(file);
    }
}

int valid_pass(char password[50])
{
    int upper = 0 , lower = 0 , number = 0;
    for(int i = 0 ; i < strlen(password) ; i++)
    {
        if(password[i] > 47 && password[i] < 58)
        {
            number = 1;
        }
        if(password[i] > 64 && password[i] < 91)
        {
            upper = 1;
        }
        if(password[i] > 96 && password[i] < 123)
        {
            lower = 1;
        }
    }
    if(upper && lower && number && strlen(password) > 6)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int valid_mail(char email[50])
{
    int ad = 0;
    for(int i = 1 ; i < strlen(email) ; i++)
    {
        if(ad)
        {
            if(i < strlen(email) - 1 && email[i] == '.')
            {
                return 1;
            }
        }
        if(email[i] == '@')
        {
            ad = 1;
            i++;
        }
    }
    return 0;
}

void generate_password(char *password, int length)
{
    const char uppercase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char lowercase[] = "abcdefghijklmnopqrstuvwxyz";
    const char digits[] = "0123456789";
    const char all_characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    
    int i;

    // Initialize random number generator
    srand((unsigned int)time(NULL));

    // Ensure the password contains at least one uppercase letter, one lowercase letter, and one digit
    password[0] = uppercase[rand() % (sizeof(uppercase) - 1)];
    password[1] = lowercase[rand() % (sizeof(lowercase) - 1)];
    password[2] = digits[rand() % (sizeof(digits) - 1)];

    // Fill the rest of the password with random characters from all_characters
    for (i = 3; i < length; i++) {
        password[i] = all_characters[rand() % (sizeof(all_characters) - 1)];
    }

    // Null-terminate the password
    password[length] = '\0';

    // Shuffle the password to randomize the placement of the initial characters
    for (i = 0; i < length; i++) {
        int j = rand() % length;
        char temp = password[i];
        password[i] = password[j];
        password[j] = temp;
    }
}

int user_name_exist_2(char name[50] , char pass[50] , char email[50])
{
    char file_name[50] , file_password[50] , file_email[50] , n[10] , p[20] , e[10];
    FILE *file = fopen("users_info.txt" , "r");
    if(file == NULL)
    {
        return 0;
    }
    else
    {
        while(fscanf(file, "%s %s %s %s %s %s\n", n , file_name , p , file_password , e , file_email) == 6) 
        {
            if(strcmp(name , file_name) == 0)
            {
                strcpy(pass , file_password);
                strcpy(email , file_email);
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
    return 0;
}

int log_in_menu()
{
    clear();
    char name[50] , pass[50] , correct_pass[50] , email[50] , correct_email[50];
    mvprintw(6 , 50 , "%s" , "ENTER USER NAME : ");
    mvprintw(10 , 50 , "%s" , "ENTER USER PASSWORD : ");
    refresh();
    echo();
    while(1)
    {
        mvgetnstr(6 , 68  , name , 50);
        if(user_name_exist_2(name , correct_pass , correct_email) == 1)
        {
            move(16 , 50);
            clrtoeol();
            strcpy(user_main.name , name);
            break;
        }
        else
        {
            mvprintw(16 , 50 , "%s" , "User Name Doesn't Exist!");
            move(6 , 68);
            clrtoeol();
            refresh();
        }
    }
    while(1)
    {
        mvprintw(18 , 50 , "%s" , "Enter F For Forget Password");
        mvgetnstr(10 , 72 , pass , 50);
        refresh();
        if(strcmp(pass , correct_pass) == 0)
        {
            move(18 , 50);
            clrtoeol();
            move(16 , 50);
            clrtoeol();
            move(14 , 50);
            clrtoeol();
            break;
        }
        else if(strcmp(pass , "F") == 0)
        {
            while(1)
            {
                move(14 , 50);
                clrtoeol();
                mvprintw(14 , 50 , "%s" , "Enter Your Given Email : ");
                mvgetnstr(14 , 75 , email , 50);
                if(strcmp(email , correct_email) == 0)
                {
                    move(14 , 50);
                    clrtoeol();
                    move(20 , 50);
                    clrtoeol();
                    move(10 , 72);
                    clrtoeol();
                    mvprintw(14 , 50 , "Your Password Is : %s" , correct_pass);
                    break;
                }
                else
                {
                    mvprintw(20 , 50 , "%s" , "Wrong Email");
                }
                refresh();
            }
        }
        else
        {
            mvprintw(16 , 50 , "%s" , "Password Is Not Correct");
            move(10 , 72);
            clrtoeol();
            refresh();
        }
    }


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
                if(floor[j][i] == 'f' || floor[j][i] =='m' || floor[j][i] == 'a')
                {
                    mvprintw(j , i , "%c" , floor[j][i]);
                    shown_floor[j][i] = floor[j][i];
                }
                else if(floor[j][i] == 's' || floor[j][i] =='h' || floor[j][i] == 'p')
                {
                    mvprintw(j , i , "%c" , floor[j][i]);
                    shown_floor[j][i] = floor[j][i];
                }
                else if(floor[j][i] == '@')
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

int move_player(PLAYER * player , char floor[50][180] , char shown_floor[50][180] , GOLD gold[6] , int * floor_level , ROOM rooms[6] , TRAP trap[3] , PASS_CHARNGER password , MONSTER * monsters , DAGGER * dagger , FOOD foods[4] , CHARM charm[4])
{
    MONSTER * monster = (MONSTER *)malloc(6 * sizeof(MONSTER));
    monster = monsters;
    while(floor_level[0] < 4)
    {
        if(player->health <= 0)
        {
            clear();
            mvprintw(0 , 0 , "YOU HAVE LOST");
            usleep(10000000);
            floor_level[0] = 4;
            break;
            
        }
        move(2 , 0);
        clrtoeol();
        mvprintw(2 , 0 , "HEALTH : %d" , player->health);
        if(password.count == 0 && password.flag == 0)
        {
            pthread_mutex_lock(&password.lock);
            password.count = 1;
            pthread_mutex_unlock(&password.lock);
            pthread_create(&thread2, NULL, countdown_pass, &password);
        }
        else if(password.count == 1 && password.flag == 0)
        {
            pthread_mutex_lock(&password.lock);
            password.count = 1;
            pthread_mutex_unlock(&password.lock);
        }

        if(floor[player->y_pos][player->x_pos] == '@')
        {
            cbreak();
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
        else
        {
            halfdelay(1);
        }
        if(is_in_trap(player , trap) != 0)//function
        {
            floor[player->y_pos][player->x_pos] = 'T';
        }
        what_to_see(player , floor , shown_floor , gold , floor_level , rooms , monster);
        mvprintw(player->y_pos , player->x_pos , "H");
        shown_floor[player->y_pos][player->x_pos] = 'H';
        move(player->y_pos , player->x_pos);

        int mover = getch();
        if(mover == 32)
        {
            if(abs(monster[player->last_room].x_pos - player->x_pos) + abs(monster[player->last_room].y_pos - player->y_pos) == 1)
            {
                monster[player->last_room].health -= 5 * player->power_charm;
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "YOU HAVE HIT THE MONSTER");
                if(monster[player->last_room].health <= 0)
                {
                    move(0 , 0);
                    clrtoeol();
                    mvprintw(0 , 0 , "YOU HAVE KILLED THE MONSTER");
                    mvprintw(monster[player->last_room].y_pos , monster[player->last_room].x_pos , ".");
                    shown_floor[monster[player->last_room].y_pos][monster[player->last_room].x_pos] = '.';
                    move(player->y_pos , player->x_pos);
                }
                refresh();
            }
            else
            {
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "PICK THE DIRECTION");
                while(1)
                {
                    int dir = getch();
                    if(dir == KEY_LEFT)
                    {
                        dagger->active = 1;
                        dagger->direction = 1;
                        dagger->x_pos = player->x_pos;
                        dagger->y_pos = player->y_pos;
                        break;
                    }
                    else if(dir == KEY_RIGHT)
                    {
                        dagger->active = 1;
                        dagger->direction = 2;
                        dagger->x_pos = player->x_pos;
                        dagger->y_pos = player->y_pos;
                        break;
                    }
                    if(dir == KEY_UP)
                    {
                        dagger->active = 1;
                        dagger->direction = 3;
                        dagger->x_pos = player->x_pos;
                        dagger->y_pos = player->y_pos;
                        break;
                    }
                    if(dir == KEY_DOWN)
                    {
                        dagger->active = 1;
                        dagger->direction = 4;
                        dagger->x_pos = player->x_pos;
                        dagger->y_pos = player->y_pos;
                        break;
                    }
                }
                refresh();
            }
        }
        else if(mover == 97)
        {
            dagger->active = 1;
            dagger->x_pos = player->x_pos;
            dagger->y_pos = player->y_pos;
        }
        else if(mover == 113)
        {
            floor_level[0]++;
            if(floor_level[0] == 4)
            {
                endwin();
                pthread_join(thread1, NULL);
                pthread_join(thread2, NULL);
                pthread_mutex_destroy(&player->lock);
                pthread_mutex_destroy(&password.lock);
            }
            break;
        }
        else if(mover == 109)
        {
            show_everything(floor , shown_floor , gold , floor_level , rooms);
            //int show_everything(char floor[50][180] , GOLD gold[6] , int * floor_level , rooms);
        }
        else if(mover == 101)
        {
            clear();
            refresh();
            int ind = eat_food_menu(foods);
            if(ind != -1)
            {
                player->hunger -= 30;
                player->health += foods[ind].damage * player->health_charm;
                if(player->health > 100)
                {
                    player->health = 100;
                }
                foods[ind].active = 0;
                if(foods[ind].type == 3)
                {
                    foods[ind].awe_active = 1;
                    player->power_charm = 2;
                }
                else if(foods[ind].type == 2)
                {
                    foods[ind].mag_active = 1;
                    player->speed_charm = 1;
                }
            }
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
        else if(mover == 99)
        {
            clear();
            refresh();
            int ind = use_charm_menu(charm);
            if(ind == 0)
            {
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "SPEED CHARM ACTIVATED");
                charm[ind].used = 1;
                player->speed_charm = 1;
            }
            else if(ind == 1)
            {
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "POWER CHARM ACTIVATED");
                charm[ind].used = 1;
                player->power_charm = 2;
            }
            else if(ind == 2 || ind == 3)
            {
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "HEALTH CHARM ACTIVATED");
                charm[ind].used = 1;
                player->health_charm = 2;
            }
            for(int j = 1 ; j < 50 ; j++)
            {
                for(int i = 0 ; i < 180 ; i++)
                {
                    mvprintw(j , i , "%c" , shown_floor[j][i]);
                }
            }
            refresh();
        }
        else if(mover == KEY_UP)
        {
            for(int i = 0 ; i <= player->speed_charm ; i++)
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
                    if(money_found >= 5)
                    {
                        player->money += money_found;
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "YOU'VE FOUND %d AMOUNT OF MONEY" , money_found);
                    }
                    else
                    {
                        player->money += money_found + 30;
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "CONGRATS FOR FINDING DARK GOLD! YOU'VE FOUND %d AMOUNT OF MONEY" , money_found + 30);
                    }
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
                else if(mvinch(player->y_pos - 1 , player->x_pos) == 'f' || mvinch(player->y_pos - 1 , player->x_pos) == 'a' || mvinch(player->y_pos - 1 , player->x_pos) == 'm')
                {
                    mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                    shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                    player->y_pos--;
                    mvprintw(player->y_pos , player->x_pos , "H");
                    shown_floor[player->y_pos][player->x_pos] = 'H';
                    move(player->y_pos , player->x_pos);
                    while(1)
                    {
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "PRESS u TO TAKE THE FOOD. PRESS d TO LEAVE IT.");
                        int pick_up = getch();
                        if(pick_up == 117)
                        {
                            move(0 , 0);
                            clrtoeol();
                            mvprintw(0 , 0 , "FOOD ADDED TO INVENTORY");
                            floor[player->y_pos][player->x_pos] = '.';
                            foods[floor_level[0]].active = 1;
                            break;
                        }
                        else if(pick_up == 100)
                        {
                            break;
                        }
                    }
                }
                else if(mvinch(player->y_pos - 1 , player->x_pos) == 's' || mvinch(player->y_pos - 1 , player->x_pos) == 'h' || mvinch(player->y_pos - 1 , player->x_pos) == 'p')            
                {
                    mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                    shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                    player->y_pos--;
                    mvprintw(player->y_pos , player->x_pos , "H");
                    shown_floor[player->y_pos][player->x_pos] = 'H';
                    move(player->y_pos , player->x_pos);
                    while(1)
                    {
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "PRESS u TO TAKE THE CHARM. PRESS d TO LEAVE IT.");
                        int pick_up = getch();
                        if(pick_up == 117)
                        {
                            move(0 , 0);
                            clrtoeol();
                            mvprintw(0 , 0 , "CHARM ADDED TO INVENTORY");
                            floor[player->y_pos][player->x_pos] = '.';
                            charm[floor_level[0]].active = 1;
                            break;
                        }
                        else if(pick_up == 100)
                        {
                            break;
                        }
                    }
                }
                else
                {
                    move(player->y_pos , player->x_pos);
                }
            }
        }
        else if(mover == KEY_DOWN)
        {
            for(int i = 0 ; i <= player->speed_charm ; i++)
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
                    if(money_found >= 5)
                    {
                        player->money += money_found;
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "YOU'VE FOUND %d AMOUNT OF MONEY" , money_found);
                    }
                    else
                    {
                        player->money += money_found + 30;
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "CONGRATS FOR FINDING DARK GOLD! YOU'VE FOUND %d AMOUNT OF MONEY" , money_found + 30);
                    }
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
                else if(mvinch(player->y_pos + 1 , player->x_pos) == 'f' || mvinch(player->y_pos + 1 , player->x_pos) == 'a' || mvinch(player->y_pos + 1 , player->x_pos) == 'm')
                {
                    mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                    shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                    player->y_pos++;
                    mvprintw(player->y_pos , player->x_pos , "H");
                    shown_floor[player->y_pos][player->x_pos] = 'H';
                    move(player->y_pos , player->x_pos);
                    while(1)
                    {
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "PRESS u TO TAKE THE FOOD. PRESS d TO LEAVE IT.");
                        int pick_up = getch();
                        if(pick_up == 117)
                        {
                            move(0 , 0);
                            clrtoeol();
                            mvprintw(0 , 0 , "FOOD ADDED TO INVENTORY");
                            floor[player->y_pos][player->x_pos] = '.';
                            foods[floor_level[0]].active = 1;
                            break;
                        }
                        else if(pick_up == 100)
                        {
                            break;
                        }
                    }
                }
                else if(mvinch(player->y_pos + 1 , player->x_pos) == 's' || mvinch(player->y_pos + 1 , player->x_pos) == 'h' || mvinch(player->y_pos + 1 , player->x_pos) == 'p')            
                {
                    mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                    shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                    player->y_pos++;
                    mvprintw(player->y_pos , player->x_pos , "H");
                    shown_floor[player->y_pos][player->x_pos] = 'H';
                    move(player->y_pos , player->x_pos);
                    while(1)
                    {
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "PRESS u TO TAKE THE CHARM. PRESS d TO LEAVE IT.");
                        int pick_up = getch();
                        if(pick_up == 117)
                        {
                            move(0 , 0);
                            clrtoeol();
                            mvprintw(0 , 0 , "CHARM ADDED TO INVENTORY");
                            floor[player->y_pos][player->x_pos] = '.';
                            charm[floor_level[0]].active = 1;
                            break;
                        }
                        else if(pick_up == 100)
                        {
                            break;
                        }
                    }
                }
                else
            {
                move(player->y_pos , player->x_pos);
            }
            }
        }
        else if(mover == KEY_RIGHT)
        {
            for(int i = 0 ; i <= player->speed_charm ; i++)
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
                    if(money_found >= 5)
                    {
                        player->money += money_found;
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "YOU'VE FOUND %d AMOUNT OF MONEY" , money_found);
                    }
                    else
                    {
                        player->money += money_found + 30;
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "CONGRATS FOR FINDING DARK GOLD! YOU'VE FOUND %d AMOUNT OF MONEY" , money_found + 30);
                    }
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
                else if(mvinch(player->y_pos , player->x_pos + 1) == 'f' || mvinch(player->y_pos , player->x_pos + 1) == 'a' || mvinch(player->y_pos , player->x_pos + 1) == 'm')
                {
                    mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                    shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                    player->x_pos++;
                    mvprintw(player->y_pos , player->x_pos , "H");
                    shown_floor[player->y_pos][player->x_pos] = 'H';
                    move(player->y_pos , player->x_pos);
                    while(1)
                    {
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "PRESS u TO TAKE THE FOOD. PRESS d TO LEAVE IT.");
                        int pick_up = getch();
                        if(pick_up == 117)
                        {
                            move(0 , 0);
                            clrtoeol();
                            mvprintw(0 , 0 , "FOOD ADDED TO INVENTORY");
                            floor[player->y_pos][player->x_pos] = '.';
                            foods[floor_level[0]].active = 1;
                            break;
                        }
                        else if(pick_up == 100)
                        {
                            break;
                        }
                    }
                }
                else if(mvinch(player->y_pos , player->x_pos + 1) == 's' || mvinch(player->y_pos , player->x_pos + 1) == 'h' || mvinch(player->y_pos , player->x_pos + 1) == 'p')
                {
                    mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                    shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                    player->x_pos++;
                    mvprintw(player->y_pos , player->x_pos , "H");
                    shown_floor[player->y_pos][player->x_pos] = 'H';
                    move(player->y_pos , player->x_pos);
                    while(1)
                    {
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "PRESS u TO TAKE THE CHARM. PRESS d TO LEAVE IT.");
                        int pick_up = getch();
                        if(pick_up == 117)
                        {
                            move(0 , 0);
                            clrtoeol();
                            mvprintw(0 , 0 , "CHARM ADDED TO INVENTORY");
                            floor[player->y_pos][player->x_pos] = '.';
                            charm[floor_level[0]].active = 1;
                            break;
                        }
                        else if(pick_up == 100)
                        {
                            break;
                        }
                    }
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
        }
        else if(mover == KEY_LEFT)
        {
            for(int i = 0 ; i <= player->speed_charm ; i++)
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
                    if(money_found >= 5)
                    {
                        player->money += money_found;
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "YOU'VE FOUND %d AMOUNT OF MONEY" , money_found);
                    }
                    else
                    {
                        player->money += money_found + 30;
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "CONGRATS FOR FINDING DARK GOLD! YOU'VE FOUND %d AMOUNT OF MONEY" , money_found + 30);
                    }
                    mvprintw(player->y_pos , player->x_pos , "H");
                    shown_floor[player->y_pos][player->x_pos] = 'H';
                    move(player->y_pos , player->x_pos);
                }
                else if(mvinch(player->y_pos , player->x_pos - 1) == '<')
                {
                    floor_level[0]++;
                    break;
                }
                else if(mvinch(player->y_pos , player->x_pos - 1) == 'f' || mvinch(player->y_pos , player->x_pos - 1) == 'a' || mvinch(player->y_pos , player->x_pos - 1) == 'm')
                {
                    mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                    shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                    player->x_pos--;
                    mvprintw(player->y_pos , player->x_pos , "H");
                    shown_floor[player->y_pos][player->x_pos] = 'H';
                    move(player->y_pos , player->x_pos);
                    while(1)
                    {
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "PRESS u TO TAKE THE FOOD. PRESS d TO LEAVE IT.");
                        int pick_up = getch();
                        if(pick_up == 117)
                        {
                            move(0 , 0);
                            clrtoeol();
                            mvprintw(0 , 0 , "FOOD ADDED TO INVENTORY");
                            floor[player->y_pos][player->x_pos] = '.';
                            foods[floor_level[0]].active = 1;
                            break;
                        }
                        else if(pick_up == 100)
                        {
                            break;
                        }
                    }
                }
                else if(mvinch(player->y_pos , player->x_pos - 1) == 's' || mvinch(player->y_pos , player->x_pos - 1) == 'h' || mvinch(player->y_pos , player->x_pos - 1) == 'p')
                {
                    mvprintw(player->y_pos , player->x_pos , "%c" , floor[player->y_pos][player->x_pos]);
                    shown_floor[player->y_pos][player->x_pos] = floor[player->y_pos][player->x_pos];
                    player->x_pos--;
                    mvprintw(player->y_pos , player->x_pos , "H");
                    shown_floor[player->y_pos][player->x_pos] = 'H';
                    move(player->y_pos , player->x_pos);
                    while(1)
                    {
                        move(0 , 0);
                        clrtoeol();
                        mvprintw(0 , 0 , "PRESS u TO TAKE THE CHARM. PRESS d TO LEAVE IT.");
                        int pick_up = getch();
                        if(pick_up == 117)
                        {
                            move(0 , 0);
                            clrtoeol();
                            mvprintw(0 , 0 , "CHARM ADDED TO INVENTORY");
                            floor[player->y_pos][player->x_pos] = '.';
                            charm[floor_level[0]].active = 1;
                            break;
                        }
                        else if(pick_up == 100)
                        {
                            break;
                        }
                    }
                }
                else
            {
                move(player->y_pos , player->x_pos);
            }
            }
        }
        refresh();

        int ind = is_in_trap(player , trap);
        if(ind == 0)
        {
            pthread_mutex_lock(&player->lock);
            player->in_trap = 0;
            pthread_mutex_unlock(&player->lock);
        }
        else if(!player->in_trap)
        {
            move(0 , 0);
            clrtoeol();
            mvprintw(0 , 0 , "YOU HAVE 4 SECONDS TO ESCAPE THE TRAP");
            pthread_mutex_lock(&player->lock);
            player->in_trap = 1;
            player->trap_damage = trap[ind - 1].damage;
            pthread_mutex_unlock(&player->lock);
            pthread_create(&thread1, NULL, countdown, player);
        }
        for(int i = 0 ; i < 6 ; i++)
        {
            if(monster[i].health > 0 && monster[i].active == 1)
            {
                if(monster[i].timer >= monster[i].speed)
                {
                    if(i == 0 || i == 5)
                    {
                        move_monster(monster + i , player , floor , shown_floor , floor_level);
                        monster[i].timer = 0;
                    }
                    else
                    {
                        if(monster[i].moves > 0)
                        {
                            move_monster(monster + i , player , floor , shown_floor , floor_level);
                            monster[i].timer = 0;
                            monster[i].moves--;
                        }
                        else
                        {
                            move_monster(monster + i , player , floor , shown_floor , floor_level);
                            monster[i].timer = 0;
                        }
                    }
                }
                else
                {
                    monster[i].timer++;
                }
            }
            if(player->last_room == monster[i].room_number)
            {
                monster[i].active = 1;
            }
            else
            {
                monster[i].active = 0;
            }
        }
        if(dagger->active == 1)
        {
            if(dagger->moves < 5)
            {
                if(dagger->timer >= 5)
                {
                    move_dagger(dagger , floor , shown_floor , monster , player);
                    dagger->timer = 0;
                    dagger->moves++;
                }
                else
                {
                    dagger->timer++;
                }
            }
            else
            {
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "YOU HAVE MISSED");
                dagger->active = 0;
                dagger->range = 5;
                dagger->symbol = '~';
                dagger->timer = 0;
                dagger->moves = 0;
            }
        }
        if(player->hunger_timer > 50)
        {
            if(player->hunger > 30)
            {
                player->health--;
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "YOU ARE HUNGRY. EAT FOOD.");
            }
            else
            {
                player->hunger++;
                if(player->health < 100 && player->hunger < 10) 
                {   
                    player->health += player->health_charm;
                }
            }
            player->hunger_timer = 0;
            refresh();
        }
        else
        {
            player->hunger_timer++;
        }

        for(int i = 0 ; i < 4 ; i++)
        {
            if(foods[i].active == 1)
            {
                if((foods[i]).timer > 600)
                {
                    if(foods[i].type == 2 || foods[i].type == 3)
                    {
                        foods[i].type = 1;
                        foods[i].damage = 5;
                        foods[i].symbol = 'f';
                    }
                    else
                    {
                        foods[i].type = 0;
                        foods[i].damage = -5;
                    }
                    foods[i].timer = 0;
                }
                else
                {
                    foods[i].timer++;
                }
            }
        }
        for(int i = 0 ; i < 4 ; i++)
        {
            if(charm[i].active && charm[i].used)
            {
                if(charm[i].timer > 100)
                {
                    charm[i].active = 0;
                    if(i == 1)
                    {
                        player->power_charm = 1;
                    }
                    else if(i == 0)
                    {
                        player->speed_charm = 0;
                    }
                    else
                    {
                        player->health_charm = 1;
                    }
                    charm[i].timer = 0;
                    move(0 , 0);
                    clrtoeol();
                    mvprintw(0 , 0 , "CHARM IS NOW OFF");
                }
                else
                {
                    charm->timer++;
                }
            }
        }
        for(int i = 0 ; i < 4 ; i++)
        {
            if(foods[i].awe_active)
            {
                if(foods[i].awe_timer > 100)
                {
                    player->power_charm = 1;
                    foods[i].awe_active = 0;
                    foods[i].awe_timer = 0;
                }
                else
                {
                    foods[i].awe_timer++;
                }
            }
            else if(foods[i].mag_active)
            {
                if(foods[i].mag_timer > 100)
                {
                    player->speed_charm = 0;
                    foods[i].mag_active = 0;
                    foods[i].mag_timer = 0;
                }
                else
                {
                    foods[i].mag_timer++;
                }
            }
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

int what_to_see(PLAYER * player , char floor[50][180] , char shown_floor[50][180] , GOLD gold[6] , int * floor_level , ROOM rooms[6] , MONSTER monster[6])
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
            player->last_room = i;
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
                mvprintw(monster[i].y_pos , monster[i].x_pos , "%c" , monster[i].symbol);
                shown_floor[monster[i].y_pos][monster[i].x_pos] = monster[i].symbol;
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
    PLAYER * player = (PLAYER*)arg;
    for(int i = 0 ; i < 5 ; i++)
    {
        sleep(1);
        pthread_mutex_lock(&player->lock);
        if(!player->in_trap)
        {
            move(0 , 0);
            clrtoeol();
            mvprintw(0 , 0 , "YOU ARE SAFE");
            move(player->y_pos , player->x_pos);
            refresh();
//            getch();
            pthread_mutex_unlock(&player->lock);
            return NULL;
        }
        pthread_mutex_unlock(&player->lock);
    }
    pthread_mutex_lock(&player->lock);
    player->health -= player->trap_damage;
    move(0 , 0);
    clrtoeol();
    mvprintw(0 , 0 , "YOU'VE LOST HEALTH");
    pthread_mutex_unlock(&player->lock);
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
    PASS_CHARNGER * password = (PASS_CHARNGER *) arg;
    for(int i = 0 ; i < 20 ; i++)
    {
        sleep(1);
        pthread_mutex_lock(&password->lock);
        if(password->flag == 1)
        {
            pthread_mutex_unlock(&password->lock);
            return NULL;
        }
        pthread_mutex_unlock(&password->lock);
    }
    pthread_mutex_lock(&password->lock);
    if(password->flag == 0)
    {
        password->count = 0;
        for(int i = 0 ; i < 4 ; i++)
        {
            password->password[i] = (rand() % 10) + 48;
        }
        password->password[4] = '\0';
    }
    pthread_mutex_unlock(&password->lock);
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

int move_monster(MONSTER * monster , PLAYER * player , char floor[50][180] , char shown_floor[50][180] , int * floor_level)
{
    if(abs(monster->x_pos - player->x_pos) + abs(monster->y_pos - player->y_pos) > 1 && (monster->moves > 0 || monster->type == 1))
    {
        if(rand() % 2 == 0)
        {
            if(monster->x_pos < player->x_pos && mvinch(monster->y_pos , monster->x_pos + 1) == '.')
            {
                mvprintw(monster->y_pos , monster->x_pos , "%c" , floor[monster->y_pos][monster->x_pos]);
                shown_floor[monster->y_pos][monster->x_pos] = floor[monster->y_pos][monster->x_pos];
                monster->x_pos++;
                mvprintw(monster->y_pos , monster->x_pos , "%c" , monster->symbol);
                shown_floor[monster->y_pos][monster->x_pos] = monster->symbol;
            }
            else if(monster->x_pos > player->x_pos && mvinch(monster->y_pos , monster->x_pos - 1) == '.')
            {
                mvprintw(monster->y_pos , monster->x_pos , "%c" , floor[monster->y_pos][monster->x_pos]);
                shown_floor[monster->y_pos][monster->x_pos] = floor[monster->y_pos][monster->x_pos];
                monster->x_pos--;
                mvprintw(monster->y_pos , monster->x_pos , "%c" , monster->symbol);
                shown_floor[monster->y_pos][monster->x_pos] = monster->symbol;
            }
            else if(monster->y_pos < player->y_pos && mvinch(monster->y_pos + 1 , monster->x_pos) == '.')
            {
                mvprintw(monster->y_pos , monster->x_pos , "%c" , floor[monster->y_pos][monster->x_pos]);
                shown_floor[monster->y_pos][monster->x_pos] = floor[monster->y_pos][monster->x_pos];
                monster->y_pos++;
                mvprintw(monster->y_pos , monster->x_pos , "%c" , monster->symbol);
                shown_floor[monster->y_pos][monster->x_pos] = monster->symbol;
            }
            else if(monster->y_pos > player->y_pos && mvinch(monster->y_pos - 1 , monster->x_pos) == '.')
            {
                mvprintw(monster->y_pos , monster->x_pos , "%c" , floor[monster->y_pos][monster->x_pos]);
                shown_floor[monster->y_pos][monster->x_pos] = floor[monster->y_pos][monster->x_pos];
                monster->y_pos--;
                mvprintw(monster->y_pos , monster->x_pos , "%c" , monster->symbol);
                shown_floor[monster->y_pos][monster->x_pos] = monster->symbol;
            }
        }
        else
        {
            if(monster->y_pos < player->y_pos && mvinch(monster->y_pos + 1 , monster->x_pos) == '.')
            {
                mvprintw(monster->y_pos , monster->x_pos , "%c" , floor[monster->y_pos][monster->x_pos]);
                shown_floor[monster->y_pos][monster->x_pos] = floor[monster->y_pos][monster->x_pos];
                monster->y_pos++;
                mvprintw(monster->y_pos , monster->x_pos , "%c" , monster->symbol);
                shown_floor[monster->y_pos][monster->x_pos] = monster->symbol;
            }
            else if(monster->y_pos > player->y_pos && mvinch(monster->y_pos - 1 , monster->x_pos) == '.')
            {
                mvprintw(monster->y_pos , monster->x_pos , "%c" , floor[monster->y_pos][monster->x_pos]);
                shown_floor[monster->y_pos][monster->x_pos] = floor[monster->y_pos][monster->x_pos];
                monster->y_pos--;
                mvprintw(monster->y_pos , monster->x_pos , "%c" , monster->symbol);
                shown_floor[monster->y_pos][monster->x_pos] = monster->symbol;
            }
            else if(monster->x_pos < player->x_pos && mvinch(monster->y_pos , monster->x_pos + 1) == '.')
            {
                mvprintw(monster->y_pos , monster->x_pos , "%c" , floor[monster->y_pos][monster->x_pos]);
                shown_floor[monster->y_pos][monster->x_pos] = floor[monster->y_pos][monster->x_pos];
                monster->x_pos++;
                mvprintw(monster->y_pos , monster->x_pos , "%c" , monster->symbol);
                shown_floor[monster->y_pos][monster->x_pos] = monster->symbol;
            }
            else if(monster->x_pos > player->x_pos && mvinch(monster->y_pos , monster->x_pos - 1) == '.')
            {
                mvprintw(monster->y_pos , monster->x_pos , "%c" , floor[monster->y_pos][monster->x_pos]);
                shown_floor[monster->y_pos][monster->x_pos] = floor[monster->y_pos][monster->x_pos];
                monster->x_pos--;
                mvprintw(monster->y_pos , monster->x_pos , "%c" , monster->symbol);
                shown_floor[monster->y_pos][monster->x_pos] = monster->symbol;
            }
        }
    }
    else if(monster->type == 1)
    {
        move(0 , 0);
        clrtoeol();
        mvprintw(0 , 0 , "YOU GOT HIT BY SNAKE");
        move(player->y_pos , player->x_pos);;
        player->health -= monster->damage;
        if(player->health <= 0)
        {
            clear();
            mvprintw(0 , 0 , "YOU HAVE LOST");
            usleep(10000000);
            floor_level[0] = 4;
        }
    }
    else if(abs(monster->x_pos - player->x_pos) + abs(monster->y_pos - player->y_pos) == 1)
    {
        move(0 , 0);
        clrtoeol();
        if(monster->type == 2)
        {
            mvprintw(0 , 0 , "YOU GOT HIT BY DEMON");
        }
        else if(monster->type == 3)
        {
            mvprintw(0 , 0 , "YOU GOT HIT BY FIRE BREATHING MONSTER");
        }
        else if(monster->type == 4)
        {
            mvprintw(0 , 0 , "YOU GOT HIT BY GIANT");
        }
        else if(monster->type == 5)
        {
            mvprintw(0 , 0 , "YOU GOT HIT BY UNDEED");
            monster->moves = 5;
        }
        move(player->y_pos , player->x_pos);
        player->health -= monster->damage;
        if(player->health <= 0)
        {
            clear();
            mvprintw(0 , 0 , "YOU HAVE LOST");
            usleep(10000000);
            floor_level[0] = 4;
        }
    }
    refresh();
}

int move_dagger(DAGGER * dagger , char floor[50][180] , char shown_floor[50][180] , MONSTER * monster , PLAYER * player)
{
    if(dagger->direction == 1)
    {
        if(mvinch(dagger->y_pos , dagger->x_pos - 1) == '.')
        {
            mvprintw(dagger->y_pos , dagger->x_pos , ".");
            shown_floor[dagger->y_pos][dagger->x_pos] = '.';
            dagger->x_pos--;
            mvprintw(dagger->y_pos , dagger->x_pos , "%c" , dagger->symbol);
            shown_floor[dagger->y_pos][dagger->x_pos] = dagger->symbol;
        }
        else if(dagger->x_pos - 1 == monster->x_pos && dagger->y_pos == monster->y_pos)
        {
            monster->health -= dagger->damage * player->power_charm;
            mvprintw(dagger->y_pos , dagger->x_pos , ".");
            shown_floor[dagger->y_pos][dagger->x_pos] = '.';
            dagger->active = 0;
            dagger->range = 5;
            dagger->symbol = '~';
            dagger->timer = 0;
            dagger->moves = 0;
            move(0 , 0);
            clrtoeol();
            mvprintw(0 , 0 , "YOU HAVE HIT THE MONSTER");
            if(monster->health <= 0)
            {
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "YOU HAVE KILLED THE MONSTER");
                monster->active = 0;
                mvprintw(monster->y_pos , monster->x_pos , ".");
                shown_floor[monster->y_pos][monster->x_pos] = '.';
            }
        }
        refresh();
    }
    else if(dagger->direction == 2)
    {
        if(mvinch(dagger->y_pos , dagger->x_pos + 1) == '.')
        {
            mvprintw(dagger->y_pos , dagger->x_pos , ".");
            shown_floor[dagger->y_pos][dagger->x_pos] = '.';
            dagger->x_pos++;
            mvprintw(dagger->y_pos , dagger->x_pos , "%c" , dagger->symbol);
            shown_floor[dagger->y_pos][dagger->x_pos] = dagger->symbol;
        }
        else if(dagger->x_pos + 1 == monster->x_pos && dagger->y_pos == monster->y_pos)
        {
            monster->health -= dagger->damage * player->power_charm;
            mvprintw(dagger->y_pos , dagger->x_pos , ".");
            shown_floor[dagger->y_pos][dagger->x_pos] = '.';
            dagger->active = 0;
            dagger->range = 5;
            dagger->symbol = '~';
            dagger->timer = 0;
            dagger->moves = 0;
            move(0 , 0);
            clrtoeol();
            mvprintw(0 , 0 , "YOU HAVE HIT THE MONSTER");
            if(monster->health <= 0)
            {
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "YOU HAVE KILLED THE MONSTER");
                monster->active = 0;
                mvprintw(monster->y_pos , monster->x_pos , ".");
                shown_floor[monster->y_pos][monster->x_pos] = '.';
            }
        }
        refresh();
    }
    else if(dagger->direction == 3)
    {
        if(mvinch(dagger->y_pos - 1 , dagger->x_pos) == '.')
        {
            mvprintw(dagger->y_pos , dagger->x_pos , ".");
            shown_floor[dagger->y_pos][dagger->x_pos] = '.';
            dagger->y_pos--;
            mvprintw(dagger->y_pos , dagger->x_pos , "%c" , dagger->symbol);
            shown_floor[dagger->y_pos][dagger->x_pos] = dagger->symbol;
        }
        else if(dagger->x_pos == monster->x_pos && dagger->y_pos - 1 == monster->y_pos)
        {
            monster->health -= dagger->damage * player->power_charm;
            mvprintw(dagger->y_pos , dagger->x_pos , ".");
            shown_floor[dagger->y_pos][dagger->x_pos] = '.';
            dagger->active = 0;
            dagger->range = 5;
            dagger->symbol = '~';
            dagger->timer = 0;
            dagger->moves = 0;
            move(0 , 0);
            clrtoeol();
            mvprintw(0 , 0 , "YOU HAVE HIT THE MONSTER");
            if(monster->health <= 0)
            {
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "YOU HAVE KILLED THE MONSTER");
                monster->active = 0;
                mvprintw(monster->y_pos , monster->x_pos , ".");
                shown_floor[monster->y_pos][monster->x_pos] = '.';
            }
        }
        refresh();
    }
    else if(dagger->direction == 4)
    {
        if(mvinch(dagger->y_pos + 1 , dagger->x_pos) == '.')
        {
            mvprintw(dagger->y_pos , dagger->x_pos , ".");
            shown_floor[dagger->y_pos][dagger->x_pos] = '.';
            dagger->y_pos++;
            mvprintw(dagger->y_pos , dagger->x_pos , "%c" , dagger->symbol);
            shown_floor[dagger->y_pos][dagger->x_pos] = dagger->symbol;
        }
        else if(dagger->x_pos == monster->x_pos && dagger->y_pos + 1 == monster->y_pos)
        {
            monster->health -= dagger->damage * player->power_charm;
            mvprintw(dagger->y_pos , dagger->x_pos , ".");
            shown_floor[dagger->y_pos][dagger->x_pos] = '.';
            dagger->active = 0;
            dagger->range = 5;
            dagger->symbol = '~';
            dagger->timer = 0;
            dagger->moves = 0;
            move(0 , 0);
            clrtoeol();
            mvprintw(0 , 0 , "YOU HAVE HIT THE MONSTER");
            if(monster->health <= 0)
            {
                move(0 , 0);
                clrtoeol();
                mvprintw(0 , 0 , "YOU HAVE KILLED THE MONSTER");
                monster->active = 0;
                mvprintw(monster->y_pos , monster->x_pos , ".");
                shown_floor[monster->y_pos][monster->x_pos] = '.';
            }
        }
        refresh();
    }
}

int food_basics(FOOD * food)
{
    int r = rand();
    if(r % 5 == 0)
    {
        food->type = 2;
        food->damage = 10;
        food->symbol = 'm';
        food->awe_active = 0;
        food->awe_timer = 0;
        food->mag_active = 0;
        food->mag_timer = 0;
    }
    else if(r % 5 == 1)
    {
        food->type = 3;
        food->damage = 10;
        food->symbol = 'a';
        food->awe_active = 0;
        food->awe_timer = 0;
        food->mag_active = 0;
        food->mag_timer = 0;
    }
    else
    {
        food->type = 1;
        food->damage = 5;
        food->symbol = 'f';
        food->awe_active = 0;
        food->awe_timer = 0;
        food->mag_active = 0;
        food->mag_timer = 0;
    }
}

int eat_food_menu(FOOD foods[4])
{
    int act_inds[4] , num = 0;
    char options[4][20];
    for(int i = 0 ; i < 4 ; i++)
    {
        if(foods[i].active)
        {
            act_inds[num] = i;
            if(foods[i].type == 0)
            {
                strcpy(options[num] , "BAD FOOD");
            }
            if(foods[i].type == 1)
            {
                strcpy(options[num] , "COMMON FOOD");
            }
            if(foods[i].type == 2)
            {
                strcpy(options[num] , "MAGIC FOOD");
            }
            if(foods[i].type == 3)
            {
                strcpy(options[num] , "AWESOME FOOD");
            }
            num++;
        }
    }
    int x = 50 , y = 15;
    for(int i = 0 ; i < num ; i++)
    {
        if(i == 0)
        {
            attron(A_REVERSE);
            mvprintw(y , x , "%s" , options[0]);
            attroff(A_REVERSE);
        }
        else
        {
            mvprintw(y + 2 * i , x , "%s" , options[0]);
        }
    }
    int which = 0;
    if(num == 0)
    {
        clear();
        mvprintw(0 , 0 , "NO FOOD IN INVENTORY");
        refresh();
        usleep(3000000);
        return -1;
    }
    while(1)
    {
        int mover = getch();
        if(mover == 10)
        {
            return act_inds[which];
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
            which += num;
            which %= num;
            for(int i = 0 ; i < num ; i++)
            {
                if(which == i)
                {
                    attron(A_REVERSE);
                    mvprintw(y + 2 * i , x , "%s" , options[i]);
                    attroff(A_REVERSE);
                }
                else
                {
                    mvprintw(y + 2 * i , x , "%s" , options[i]);
                }
            }
        }
        refresh();
    }
}

int use_charm_menu(CHARM charm[4])
{
    int act_inds[4] , num = 0;
    char options[4][20];
    for(int i = 0 ; i < 4 ; i++)
    {
        if(charm[i].active)
        {
            act_inds[num] = i;
            if(charm[i].type == 1)
            {
                strcpy(options[num] , "SPEED CHARM");
            }
            if(charm[i].type == 2)
            {
                strcpy(options[num] , "POWER CHARM");
            }
            if(charm[i].type == 3)
            {
                strcpy(options[num] , "HEALTH CHARM");
            }
            num++;
        }
    }
    int x = 50 , y = 15;
    for(int i = 0 ; i < num ; i++)
    {
        if(i == 0)
        {
            attron(A_REVERSE);
            mvprintw(y , x , "%s" , options[0]);
            attroff(A_REVERSE);
        }
        else
        {
            mvprintw(y + 2 * i , x , "%s" , options[0]);
        }
    }
    int which = 0;
    if(num == 0)
    {
        clear();
        mvprintw(0 , 0 , "NO CHARM IN INVENTORY");
        refresh();
        usleep(3000000);
        return -1;
    }
    while(1)
    {
        int mover = getch();
        if(mover == 10)
        {
            return act_inds[which];
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
            which += num;
            which %= num;
            for(int i = 0 ; i < num ; i++)
            {
                if(which == i)
                {
                    attron(A_REVERSE);
                    mvprintw(y + 2 * i , x , "%s" , options[i]);
                    attroff(A_REVERSE);
                }
                else
                {
                    mvprintw(y + 2 * i , x , "%s" , options[i]);
                }
            }
        }
        refresh();
    }
}

int init_monsters(MONSTER monster[6] , ROOM rooms[6])
{
    for(int i = 0 ; i < 6 ; i++)
    {
        monster[i].x_pos = rooms[i].left_col + 1;
        monster[i].y_pos = rooms[i].up_row + 1;
        monster[i].room_number = i;
    }
    monster[0].symbol = 'S';
    monster[0].speed = 10;
    monster[0].timer = 0;
    monster[0].damage = 10;
    monster[0].health = 20;
    monster[0].active = 1;
    monster[0].type = 1;

    monster[1].symbol = 'D';
    monster[1].speed = 10;
    monster[1].timer = 0;
    monster[1].damage = 2;
    monster[1].health = 5;
    monster[1].active = 0;
    monster[1].type = 2;
    monster[1].moves = 0;

    monster[2].symbol = 'F';
    monster[2].speed = 10;
    monster[2].timer = 0;
    monster[2].damage = 5;
    monster[2].health = 15;
    monster[2].active = 0;
    monster[2].type = 3;
    monster[2].moves = 0;

    monster[3].symbol = 'G';
    monster[3].speed = 10;
    monster[3].timer = 0;
    monster[3].damage = 10;
    monster[3].health = 15;
    monster[3].active = 0;
    monster[3].type = 4;
    monster[3].moves = 5;

    monster[4].symbol = 'U';
    monster[4].speed = 10;
    monster[4].timer = 0;
    monster[4].damage = 15;
    monster[4].health = 30;
    monster[4].active = 0;
    monster[4].type = 5;
    monster[4].moves = 5;

    monster[5].symbol = 'S';
    monster[5].speed = 10;
    monster[5].timer = 0;
    monster[5].damage = 10;
    monster[5].health = 20;
    monster[5].active = 0;
    monster[5].type = 1;
}