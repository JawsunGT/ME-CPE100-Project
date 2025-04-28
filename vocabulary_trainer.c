#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORDS 1000
#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define FILENAME_LEN 100

typedef struct {
    char word[50];
    char part_of_speech[30];
    char sentence[200];
    char meaning[100];
    int day_added;
} Vocabulary;

Vocabulary vocabs[MAX_WORDS];
int vocab_count = 0;

// ฟังก์ชันประกาศล่วงหน้า
void clear_screen();
void print_welcome();
int signup();
int login(char *username);
void main_menu(char *username);
void load_vocabulary(char *username);
void save_vocabulary(char *username);
void add_new_word(char *username);
void start_quiz(char *username);
int check_quiz_day(int days_since_added);
int calculate_days_since(int day_added);
int today_day();
void review_vocabulary(char *username); // ฟังก์ชันทวนคำศัพท์
void export_to_csv(char *username); // ฟังก์ชัน Export เป็น CSV

// ฟังก์ชันหลัก
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
        getchar(); // clear buffer

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
        getchar(); // wait for enter
    }

    return 0;
}

// ล้างหน้าจอ
void clear_screen() {
    system("clear || cls");
}

// พิมพ์หน้าจอหลัก
void print_welcome() {
    printf("========================================\n");
    printf("         Vocabulary Trainer Program     \n");
    printf("========================================\n");
}

// สมัครสมาชิก
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

// เข้าสู่ระบบ
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

// เมนูหลัก
void main_menu(char *username) {
    int choice;
    load_vocabulary(username);

    while (1) {
        clear_screen();
        printf("=== Main Menu (%s) ===\n", username);
        printf("Words learned: %d\n", vocab_count);
        printf("1. Add New Vocabulary\n");
        printf("2. Start Quiz\n");
        printf("3. Review Vocabulary\n"); // ฟังก์ชันใหม่
        printf("4. Export Vocabulary to CSV\n"); // ฟังก์ชันใหม่
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // clear buffer

        if (choice == 1) {
            add_new_word(username);
        } else if (choice == 2) {
            start_quiz(username);
        } else if (choice == 3) {
            review_vocabulary(username); // เรียกฟังก์ชันทวนคำศัพท์
        } else if (choice == 4) {
            export_to_csv(username); // เรียกฟังก์ชัน export เป็น CSV
        } else if (choice == 5) {
            save_vocabulary(username);
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
        getchar(); // wait for enter
    }
}

// โหลดคำศัพท์
void load_vocabulary(char *username) {
    char filename[FILENAME_LEN];
    sprintf(filename, "Vocabreview/user_vocabs_%s.csv", username);

    FILE *fp = fopen(filename, "r");
    if (!fp) return; // file อาจยังไม่สร้าง

    vocab_count = 0;
    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%d\n",
            vocabs[vocab_count].word,
            vocabs[vocab_count].part_of_speech,
            vocabs[vocab_count].sentence,
            vocabs[vocab_count].meaning,
            &vocabs[vocab_count].day_added) != EOF) {
        vocab_count++;
    }
    fclose(fp);
}

// บันทึกคำศัพท์
void save_vocabulary(char *username) {
    char filename[FILENAME_LEN];
    sprintf(filename, "Vocabreview/user_vocabs_%s.csv", username);

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error saving vocabularies.\n");
        return;
    }

    for (int i = 0; i < vocab_count; i++) {
        fprintf(fp, "%s,%s,%s,%s,%d\n",
            vocabs[i].word,
            vocabs[i].part_of_speech,
            vocabs[i].sentence,
            vocabs[i].meaning,
            vocabs[i].day_added);
    }

    fclose(fp);
}

// เพิ่มคำศัพท์ใหม่
void add_new_word(char *username) {
    int added_today = 0;
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    int today = tm_now->tm_yday;

    do {
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

        vocabs[vocab_count].day_added = today;
        vocab_count++;
        added_today++;

    } while (added_today == 0);

    save_vocabulary(username);
}

// ฟังก์ชันทวนคำศัพท์
void review_vocabulary(char *username) {
    if (vocab_count == 0) {
        printf("No vocabulary available.\n");
        return;
    }

    printf("=== Review Vocabulary ===\n");
    for (int i = 0; i < vocab_count; i++) {
        printf("Word: %s\n", vocabs[i].word);
        printf("Part of Speech: %s\n", vocabs[i].part_of_speech);
        printf("Sentence: %s\n", vocabs[i].sentence);
        printf("Meaning: %s\n", vocabs[i].meaning);
        printf("Day Added: %d\n", vocabs[i].day_added);
        printf("==========================\n\n");
    }
}

// ฟังก์ชัน Export คำศัพท์เป็น CSV
void export_to_csv(char *username) {
    if (vocab_count == 0) {
        printf("No vocabulary available to export.\n");
        return;
    }

    char filename[FILENAME_LEN];
    sprintf(filename, "Vocabreview/user_vocabs_%s.csv", username);

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error exporting to CSV.\n");
        return;
    }

    for (int i = 0; i < vocab_count; i++) {
        fprintf(fp, "%s,%s,%s,%s,%d\n",
            vocabs[i].word,
            vocabs[i].part_of_speech,
            vocabs[i].sentence,
            vocabs[i].meaning,
            vocabs[i].day_added);
    }

    fclose(fp);
    printf("Vocabulary data exported to %s\n", filename);
}
void start_quiz(char *username) {
    // ในฟังก์ชันนี้ คุณสามารถสร้างการทดสอบคำศัพท์หรือเกมทดสอบที่เกี่ยวข้องกับคำศัพท์
    printf("Starting quiz for %s...\n", username);
    // ตัวอย่างการทดสอบง่ายๆ
    for (int i = 0; i < vocab_count; i++) {
        printf("What is the meaning of '%s'?\n", vocabs[i].word);
        // สามารถเพิ่มการให้ผู้ใช้ตอบคำถามและเช็คคำตอบที่นี่
    }
}
