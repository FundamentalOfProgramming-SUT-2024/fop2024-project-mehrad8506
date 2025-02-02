#include <stdio.h>
#include <ncurses.h>
#include <time.h>
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

int make_menu_aval();
int sign_up_menu();
int user_name_exist(char name[50]);
void save_info(USER_DATA user);
int valid_pass(char password[50]);
int valid_mail(char email[50]);
void generate_password(char *password, int length);
int user_name_exist_2(char name[50] , char pass[50] , char email[50]);
int log_in_menu();


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