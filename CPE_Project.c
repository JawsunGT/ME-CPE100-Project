#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 1000
#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define FILENAME_LEN 100

typedef struct {
    char word[50];
    char part_of_speech[30];
    char sentence[200];
    char meaning[100];
} Vocabulary;

Vocabulary vocabs[MAX_WORDS];
int vocab_count = 0;

void clear_screen();
void print_welcome();
int signup();
int login(char *username);
void main_menu(char *username);
void load_vocabulary(char *username);
void save_vocabulary(char *username);
void add_new_word(char *username);
void review_vocabulary(char *username);
void export_to_csv(char *username);
void start_quiz(char *username);

int main() {
    char username[MAX_USERNAME];
    int choice;

    while (1) {
        clear_screen();
        print_welcome();
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            if (signup()) {
                printf("Sign Up successful. Please Login.\n");
            }
        } else if (choice == 2) {
            if (login(username)) {
                main_menu(username);
            }
        } else if (choice == 3) {
            printf("Goodbye.\n");
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
        getchar();
    }

    return 0;
}

void clear_screen() {
    system("clear || cls");
}

void print_welcome() {
    printf("========================================\n");
    printf("         Vocabulary Trainer Program     \n");
    printf("========================================\n");
}

int signup() {
    char username[MAX_USERNAME], password[MAX_PASSWORD];
    FILE *fp = fopen("users.csv", "a+");
    if (!fp) {
        printf("Error opening user file.\n");
        return 0;
    }

    printf("Enter new username: ");
    fgets(username, MAX_USERNAME, stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Enter password: ");
    fgets(password, MAX_PASSWORD, stdin);
    password[strcspn(password, "\n")] = 0;

    fprintf(fp, "%s,%s\n", username, password);
    fclose(fp);
    return 1;
}

int login(char *username) {
    char input_user[MAX_USERNAME], input_pass[MAX_PASSWORD];
    char file_user[MAX_USERNAME], file_pass[MAX_PASSWORD];
    int found = 0;

    FILE *fp = fopen("users.csv", "r");
    if (!fp) {
        printf("Error opening user file.\n");
        return 0;
    }

    printf("Enter username: ");
    fgets(input_user, MAX_USERNAME, stdin);
    input_user[strcspn(input_user, "\n")] = 0;

    printf("Enter password: ");
    fgets(input_pass, MAX_PASSWORD, stdin);
    input_pass[strcspn(input_pass, "\n")] = 0;

    while (fscanf(fp, "%[^,],%[^\n]\n", file_user, file_pass) != EOF) {
        if (strcmp(input_user, file_user) == 0 && strcmp(input_pass, file_pass) == 0) {
            found = 1;
            strcpy(username, input_user);
            break;
        }
    }

    fclose(fp);

    if (found) {
        printf("Login successful. Welcome %s!\n", username);
        return 1;
    } else {
        printf("Login failed. Try again.\n");
        return 0;
    }
}

void main_menu(char *username) {
    int choice;
    load_vocabulary(username);

    while (1) {
        clear_screen();
        printf("=== Main Menu (%s) ===\n", username);
        printf("Words learned: %d\n", vocab_count);
        printf("1. Add New Vocabulary\n");
        printf("2. Start Quiz\n");
        printf("3. Review Vocabulary\n");
        printf("4. Export Vocabulary to CSV\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            add_new_word(username);
        } else if (choice == 2) {
            start_quiz(username);
        } else if (choice == 3) {
            review_vocabulary(username);
        } else if (choice == 4) {
            export_to_csv(username);
        } else if (choice == 5) {
            save_vocabulary(username);
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
        getchar();
    }
}

void load_vocabulary(char *username) {
    char filename[FILENAME_LEN];
    sprintf(filename, "user_vocabs_%s.csv", username);

    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    vocab_count = 0;
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^\n]\n",
            vocabs[vocab_count].word,
            vocabs[vocab_count].part_of_speech,
            vocabs[vocab_count].sentence,
            vocabs[vocab_count].meaning) != EOF) {
        vocab_count++;
    }
    fclose(fp);
}

void save_vocabulary(char *username) {
    char filename[FILENAME_LEN];
    sprintf(filename, "user_vocabs_%s.csv", username);

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error saving vocabularies.\n");
        return;
    }

    for (int i = 0; i < vocab_count; i++) {
        fprintf(fp, "%s,%s,%s,%s\n",
            vocabs[i].word,
            vocabs[i].part_of_speech,
            vocabs[i].sentence,
            vocabs[i].meaning);
    }

    fclose(fp);
}

void add_new_word(char *username) {
    printf("Enter word: ");
    fgets(vocabs[vocab_count].word, sizeof(vocabs[vocab_count].word), stdin);
    vocabs[vocab_count].word[strcspn(vocabs[vocab_count].word, "\n")] = 0;

    printf("Enter part of speech: ");
    fgets(vocabs[vocab_count].part_of_speech, sizeof(vocabs[vocab_count].part_of_speech), stdin);
    vocabs[vocab_count].part_of_speech[strcspn(vocabs[vocab_count].part_of_speech, "\n")] = 0;

    printf("Enter example sentence: ");
    fgets(vocabs[vocab_count].sentence, sizeof(vocabs[vocab_count].sentence), stdin);
    vocabs[vocab_count].sentence[strcspn(vocabs[vocab_count].sentence, "\n")] = 0;

    printf("Enter meaning: ");
    fgets(vocabs[vocab_count].meaning, sizeof(vocabs[vocab_count].meaning), stdin);
    vocabs[vocab_count].meaning[strcspn(vocabs[vocab_count].meaning, "\n")] = 0;

    vocab_count++;
    save_vocabulary(username);

}

void review_vocabulary(char *username) {
    if (vocab_count == 0) {
        printf("No vocabulary to review.\n");
        return;
    }

    for (int i = 0; i < vocab_count; i++) {
        printf("Word: %s\n", vocabs[i].word);
        printf("Part of Speech: %s\n", vocabs[i].part_of_speech);
        printf("Sentence: %s\n", vocabs[i].sentence);
        printf("Meaning: %s\n", vocabs[i].meaning);
        printf("--------------------------\n");
    }
}

void export_to_csv(char *username) {
    save_vocabulary(username);
    printf("Exported vocabulary to CSV.\n");
}

void start_quiz(char *username) {
    if (vocab_count < 2) {
        printf("Not enough vocabulary for a multiple-choice quiz. Minimum 4 words needed.\n");
        return;
    }

    int asked_indices[MAX_WORDS] = {0}; // ติดตามคำถามที่ถูกถามแล้ว
    int total_asked = 0;
    int score = 0;
    int continue_quiz = 1;

    srand(time(NULL)); // seed random

    while (continue_quiz == 1) {
        int questions_in_this_set = 0;

        while (questions_in_this_set < 2 && total_asked < vocab_count) {
            int correct_index;

            // หา index ที่ยังไม่ถูกถาม
            do {
                correct_index = rand() % vocab_count;
            } while (asked_indices[correct_index]);

            asked_indices[correct_index] = 1;
            total_asked++;
            questions_in_this_set++;

            // เตรียมตัวเลือก
            char *correct_meaning = vocabs[correct_index].meaning;
            char *choices[2];
            int correct_pos = rand() % 2;
            choices[correct_pos] = correct_meaning;

            int filled = 1;
            for (int i = 0; i < 2; i++) {
                if (i == correct_pos) continue;
                int rand_index;
                int is_duplicate;
                do {
                    rand_index = rand() % vocab_count;
                    is_duplicate = (rand_index == correct_index);
                    for (int j = 0; j < i; j++) {
                        if (choices[j] && strcmp(choices[j], vocabs[rand_index].meaning) == 0) {
                            is_duplicate = 1;
                            break;
                        }
                    }
                } while (is_duplicate);

                choices[i] = vocabs[rand_index].meaning;
                filled++;
            }

            // ถามคำถาม
            printf("Q%d: What is the meaning of '%s'?\n", total_asked, vocabs[correct_index].word);
            for (int i = 0; i < 2; i++) {
                printf("%d. %s\n", i + 1, choices[i]);
            }

            int answer;
            printf("Your answer (1-2): ");
            scanf("%d", &answer);
            getchar(); // clear buffer

            if (answer - 1 == correct_pos) {
                printf(" Correct!!!\n");
                score++;
            } else {
                printf("Wrong!!!! Don't worry. Just practice more!\n");
                printf("Correct answer: %s\n", correct_meaning);
            }

            printf("-----------------------------\n");
        }

        if (total_asked >= vocab_count) {
            printf("You've completed all available questions.\n");
            break;
        }

        // ถามว่าจะทำต่อไหม
        printf("Want to continue? (1 = Yes, 2 = No): ");
        scanf("%d", &continue_quiz);
        getchar(); // clear buffer

        if (continue_quiz != 1) {
            break; // ถ้ากด 2 หรืออื่น ๆ ให้จบ
        }
    }

    // แสดงคะแนนสะสมทั้งหมดเมื่อจบ
    printf("Quiz ended. Your total score: %d/%d\n ", score, total_asked);
}


/*

#define MAX_WORDS 1000
#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define FILENAME_LEN 100

typedef struct {
    char word[50];
    char part_of_speech[30];
    char sentence[200];
    char meaning[100];
} Vocabulary;

Vocabulary vocabs[MAX_WORDS];
int word_addded = 0;
int vocab_count = 0;

void clear_screen();
void print_welcome();
int signup();
int login(char *username);
void main_menu(char *username);
void load_vocabulary(char *username);
void save_vocabulary(char *username);
void add_new_word(char *username);
void review_vocabulary(char *username);
void export_to_csv(char *username);
void start_quiz(char *username);

int main() {
    char username[MAX_USERNAME];
    int choice;

    while (1) {
        clear_screen();
        print_welcome();
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            if (signup()) {
                printf("Sign Up successful. Please Login.\n");
            }
        } else if (choice == 2) {
            if (login(username)) {
                main_menu(username);
            }
        } else if (choice == 3) {
            printf("Goodbye.\n");
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
        getchar();
    }

    return 0;
}

void clear_screen() {
    system("clear || cls");
}

void print_welcome() {
    printf("========================================\n");
    printf("         Vocabulary Trainer Program     \n");
    printf("========================================\n");
}

int signup() {
    char username[MAX_USERNAME], password[MAX_PASSWORD];
    FILE *fp = fopen("users.csv", "a+");
    if (!fp) {
        printf("Error opening user file.\n");
        return 0;
    }

    printf("Enter new username: ");
    fgets(username, MAX_USERNAME, stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Enter password: ");
    fgets(password, MAX_PASSWORD, stdin);
    password[strcspn(password, "\n")] = 0;

    fprintf(fp, "%s,%s\n", username, password);
    fclose(fp);
    return 1;
}

int login(char *username) {
    char input_user[MAX_USERNAME], input_pass[MAX_PASSWORD];
    char file_user[MAX_USERNAME], file_pass[MAX_PASSWORD];
    int found = 0;

    FILE *fp = fopen("users.csv", "r");
    if (!fp) {
        printf("Error opening user file.\n");
        return 0;
    }

    printf("Enter username: ");
    fgets(input_user, MAX_USERNAME, stdin);
    input_user[strcspn(input_user, "\n")] = 0;

    printf("Enter password: ");
    fgets(input_pass, MAX_PASSWORD, stdin);
    input_pass[strcspn(input_pass, "\n")] = 0;

    while (fscanf(fp, "%[^,],%[^\n]\n", file_user, file_pass) != EOF) {
        if (strcmp(input_user, file_user) == 0 && strcmp(input_pass, file_pass) == 0) {
            found = 1;
            strcpy(username, input_user);
            break;
        }
    }

    fclose(fp);

    if (found) {
        printf("Login successful. Welcome %s!\n", username);
        return 1;
    } else {
        printf("Login failed. Try again.\n");
        return 0;
    }
}

void main_menu(char *username) {
    int choice;
    load_vocabulary(username);

    while (1) {
        clear_screen();
        printf("=== Main Menu (%s) ===\n", username);
        printf("Words learned: %d\n", vocab_count);
        printf("1. Add New Vocabulary\n");
        printf("2. Start Quiz\n");
        printf("3. Review Vocabulary\n");
        printf("4. Export Vocabulary to CSV\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            add_new_word(username);
        } else if (choice == 2) {
            start_quiz(username);
        } else if (choice == 3) {
            review_vocabulary(username);
        } else if (choice == 4) {
            export_to_csv(username);
        } else if (choice == 5) {
            save_vocabulary(username);
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
        printf("Word successfully added\n");
        printf("Go to menu please Enter : ");
        getchar();
    }
}

void load_vocabulary(char *username) {
    char filename[FILENAME_LEN];
    sprintf(filename, "user_vocabs_%s.csv", username);

    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    vocab_count = 0;
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^\n]\n",
            vocabs[vocab_count].word,
            vocabs[vocab_count].part_of_speech,
            vocabs[vocab_count].sentence,
            vocabs[vocab_count].meaning) != EOF) {
        vocab_count++;
    }
    fclose(fp);
}

void save_vocabulary(char *username) {
    char filename[FILENAME_LEN];
    sprintf(filename, "user_vocabs_%s.csv", username);

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error saving vocabularies.\n");
        return;
    }

    for (int i = 0; i < vocab_count; i++) {
        fprintf(fp, "%s,%s,%s,%s\n",
            vocabs[i].word,
            vocabs[i].part_of_speech,
            vocabs[i].sentence,
            vocabs[i].meaning);
    }

    fclose(fp);
}

void add_new_word(char *username) {
    printf("Enter word: ");
    fgets(vocabs[vocab_count].word, sizeof(vocabs[vocab_count].word), stdin);
    vocabs[vocab_count].word[strcspn(vocabs[vocab_count].word, "\n")] = 0;

    printf("Enter part of speech: ");
    fgets(vocabs[vocab_count].part_of_speech, sizeof(vocabs[vocab_count].part_of_speech), stdin);
    vocabs[vocab_count].part_of_speech[strcspn(vocabs[vocab_count].part_of_speech, "\n")] = 0;

    printf("Enter example sentence: ");
    fgets(vocabs[vocab_count].sentence, sizeof(vocabs[vocab_count].sentence), stdin);
    vocabs[vocab_count].sentence[strcspn(vocabs[vocab_count].sentence, "\n")] = 0;

    printf("Enter meaning: ");
    fgets(vocabs[vocab_count].meaning, sizeof(vocabs[vocab_count].meaning), stdin);
    vocabs[vocab_count].meaning[strcspn(vocabs[vocab_count].meaning, "\n")] = 0;

    vocab_count++;
    save_vocabulary(username);
    word_addded = 1 ;

}

void review_vocabulary(char *username) {
    if (vocab_count == 0) {
        printf("No vocabulary to review.\n");
        return;
    }

    for (int i = 0; i < vocab_count; i++) {
        printf("Word: %s\n", vocabs[i].word);
        printf("Part of Speech: %s\n", vocabs[i].part_of_speech);
        printf("Sentence: %s\n", vocabs[i].sentence);
        printf("Meaning: %s\n", vocabs[i].meaning);
        printf("--------------------------\n");
    }
}

void export_to_csv(char *username) {
    save_vocabulary(username);
    printf("Exported vocabulary to CSV.\n");
}

void start_quiz(char *username) {
    if (vocab_count < 2) {
        printf("Not enough vocabulary for a multiple-choice quiz. Minimum 4 words needed.\n");
        return;
    }

    int asked_indices[MAX_WORDS] = {0}; // ติดตามคำถามที่ถูกถามแล้ว
    int total_asked = 0;
    int score = 0;
    int continue_quiz = 1;

    srand(time(NULL)); // seed random

    while (continue_quiz == 1) {
        int questions_in_this_set = 0;

        while (questions_in_this_set < 2 && total_asked < vocab_count) {
            int correct_index;

            // หา index ที่ยังไม่ถูกถาม
            do {
                correct_index = rand() % vocab_count;
            } while (asked_indices[correct_index]);

            asked_indices[correct_index] = 1;
            total_asked++;
            questions_in_this_set++;

            // เตรียมตัวเลือก
            char *correct_meaning = vocabs[correct_index].meaning;
            char *choices[2];
            int correct_pos = rand() % 2;
            choices[correct_pos] = correct_meaning;

            int filled = 1;
            for (int i = 0; i < 2; i++) {
                if (i == correct_pos) continue;
                int rand_index;
                int is_duplicate;
                do {
                    rand_index = rand() % vocab_count;
                    is_duplicate = (rand_index == correct_index);
                    for (int j = 0; j < i; j++) {
                        if (choices[j] && strcmp(choices[j], vocabs[rand_index].meaning) == 0) {
                            is_duplicate = 1;
                            break;
                        }
                    }
                } while (is_duplicate);

                choices[i] = vocabs[rand_index].meaning;
                filled++;
            }

            // ถามคำถาม
            printf("Q%d: What is the meaning of '%s'?\n", total_asked, vocabs[correct_index].word);
            for (int i = 0; i < 2; i++) {
                printf("%d. %s\n", i + 1, choices[i]);
            }

            int answer;
            printf("Your answer (1-2): ");
            scanf("%d", &answer);
            getchar(); // clear buffer

            if (answer - 1 == correct_pos) {
                printf(" Correct!!!\n");
                score++;
            } else {
                printf("Wrong!!!! Don't worry. Just practice more!\n");
                printf("Correct answer: %s\n", correct_meaning);
            }

            printf("-----------------------------\n");
        }

        if (total_asked >= vocab_count) {
            printf("You've completed all available questions.\n");
            break;
        }

        // ถามว่าจะทำต่อไหม
        printf("Want to continue? (1 = Yes, 2 = No): ");
        scanf("%d", &continue_quiz);
        getchar(); // clear buffer

        if (continue_quiz != 1) {
            break; // ถ้ากด 2 หรืออื่น ๆ ให้จบ
        }
    }

    // แสดงคะแนนสะสมทั้งหมดเมื่อจบ
    printf("Quiz ended. Your total score: %d/%d\n ", score, total_asked);
}

*/
