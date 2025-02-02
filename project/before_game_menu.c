#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
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

int make_menu();
int profile_page();
int score_board();

int compare_users();
void sort_users_by_score();

int main()
{
    USER_DATA user;
    strcpy(user.name , "mehrad1");
    strcpy(user.email , "mehrad@hesari.com1");
    strcpy(user.pass , "hesari1");
    user.money = 41;
    user.exp = 21;
    user.games_played = 31;
    user.score = 11;


    initscr();
    clear();
    noecho();
    cbreak();
    // here we make our new menu
    int num  = make_menu(user);
    
    // print info : print : user.kossher
    
    // profile menu : users info : ranking - name - score - money - games_played - exp


    //here it ends
    getch();
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