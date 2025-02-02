#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    char name[50];
    char password[50];
    char email[50];
} USER_DATA;

int make_menu();
int user_name_exist(char name[50] , char pass[50] , char email[50]);
int log_in_menu();

int main()
{
    initscr();
    clear();
    noecho();
    cbreak();
    
    int num = make_menu();
    // if(num == 0)
    // {
    //     sign_up_menu();
    // }
    if(num == 1)
    {
        log_in_menu();
    }
    
    getch();
    endwin();
    return 0;
    

    
}

int make_menu()
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

int user_name_exist(char name[50] , char pass[50] , char email[50])
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
        if(user_name_exist(name , correct_pass , correct_email) == 1)
        {
            move(16 , 50);
            clrtoeol();
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