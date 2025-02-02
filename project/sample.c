#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PASSWORD_LENGTH 12 // Define the length of the password

// Function to generate a random password
void generate_password(char *password, int length) {
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

int main() {
    char password[PASSWORD_LENGTH + 1]; // +1 for the null terminator

    generate_password(password, PASSWORD_LENGTH);

    printf("Generated Password: %s\n", password);

    return 0;
}
