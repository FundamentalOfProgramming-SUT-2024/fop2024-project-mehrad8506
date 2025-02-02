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
int sign_up_menu();
int user_name_exist(char name[50]);
void save_info(USER_DATA user);
int valid_pass(char password[50]);
int valid_mail(char email[50]);
void generate_password(char *password, int length);

int main()
{
    initscr();
    clear();
    noecho();
    cbreak();
    
    int num = make_menu();
    if(num == 0)
    {
        sign_up_menu();
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

int sign_up_menu()
{
    clear();
    USER_DATA user;
    mvprintw(6 , 50 , "%s" , "NAME : ");
    mvprintw(10 , 50 , "%s" , "PASSWORD : ");
    mvprintw(14 , 50 , "%s" , "EMAIL : ");
    refresh();
    echo();
    while(1)
    {    
        mvgetnstr(6 , 57  , user.name , 50);
        if(user_name_exist(user.name) == 1)
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
        mvgetnstr(10 , 61 , user.password , 50);
        if(valid_pass(user.password))
        {
            move(18 , 50);
            clrtoeol();
            move(16 , 50);
            clrtoeol();
            refresh();
            break;
        }
        else if(strcmp(user.password , "G") == 0)
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
        mvgetnstr(14 , 58  , user.email , 50);
        if(valid_mail(user.email))
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

    save_info(user);
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
        fprintf(file, "Name: %s Password: %s  Email: %s\n", user.name, user.password, user.email);
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