#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 4 // 定義數字的長度
#define NAME_LENGTH 50 // 定義玩家名稱的最大長度

// 檢查數字是否有重複
// 輸入：整數 number
// 輸出：返回 1（無重複數字）或 0（有重複數字）
int is_unique_number(int number) {
    int digits[SIZE];
    // 將數字轉換為陣列，每位數分別存入
    for (int i = SIZE - 1; i >= 0; i--) {
        digits[i] = number % 10;
        number /= 10;
    }
    // 檢查陣列中是否有重複數字
    for (int i = 0; i < SIZE; i++) {
        for (int j = i + 1; j < SIZE; j++) {
            if (digits[i] == digits[j]) {
                return 0; // 有重複數字
            }
        }
    }
    return 1; // 無重複數字
}

// 將數字轉換為陣列
// 輸入：整數 number，陣列 array
// 輸出：將數字的每位存入陣列中
void number_to_array(int number, int array[]) {
    for (int i = SIZE - 1; i >= 0; i--) {
        array[i] = number % 10;
        number /= 10;
    }
}

// 計算幾 A 幾 B
// 輸入：目標數字 question，猜測數字 guess，指向 A 和 B 的指標
// 輸出：更新 A 和 B 值
void calculate_AB(const int question[], const int guess[], int *A, int *B) {
    *A = 0;
    *B = 0;
    // 比較每位數字的位置和內容
    for (int i = 0; i < SIZE; i++) {
        if (guess[i] == question[i]) {
            (*A)++; // 數字和位置都正確
        } else {
            for (int j = 0; j < SIZE; j++) {
                if (guess[i] == question[j]) {
                    (*B)++; // 數字正確但位置不對
                    break;
                }
            }
        }
    }
}

// 猜題者模式：電腦猜玩家的數字
// 輸入：玩家名稱
void questioner_mode(const char *name) {
    int possible_numbers[5040]; // 儲存所有可能的數字（10P4 = 5040）
    int count = 0;

    // 生成所有不重複的 4 位數字
    for (int i = 0; i < 10000; i++) {
        if (is_unique_number(i)) {
            possible_numbers[count++] = i;
        }
    }

    int attempt = 0;
    // 猜測迴圈
    while (count > 0) {
        attempt++;
        int guess = possible_numbers[rand() % count]; // 隨機選擇候選數字
        int guess_array[SIZE];
        number_to_array(guess, guess_array);

        printf("Attempt %d: My guess is %04d\n", attempt, guess);

        int A, B;
        printf("Enter A and B (e.g., 2 1 for 2A1B): ");
        scanf("%d %d", &A, &B);

        if (A == 4 && B == 0) { // 電腦猜中
            printf("PC WIN! I guessed your number in %d attempts.\n", attempt);
            return;
        }

        if (A + B > 4 || A + B < 0) { // 非法輸入
            printf("%s, YOU CHEAT! Invalid A and B values.\n", name);
            return;
        }

        // 過濾符合 A 和 B 的數字
        int new_count = 0; // 用於計算並存儲符合條件的候選數字數量
        for (int i = 0; i < count; i++) {// 遍歷當前所有可能的候選數字
            int candidate_array[SIZE];// 用於存儲候選數字的數字陣列形式
            number_to_array(possible_numbers[i], candidate_array);// 將候選數字轉換為陣列，方便每個數字比較

            int tempA = 0, tempB = 0; // 暫存當前候選數字的 A 和 B 值
            calculate_AB(guess_array, candidate_array, &tempA, &tempB);// 計算猜測數字與當前候選數字的 A 和 B 值

            if (tempA == A && tempB == B) { // 如果計算得到的 A 和 B 值與玩家輸入的結果匹配
                possible_numbers[new_count++] = possible_numbers[i];// 將該候選數字保留到新的候選清單中，並增加計數
            }
        }

        count = new_count;

        if (count == 0) { // 無匹配數字，視為使用者作弊
            printf("%s, YOU CHEAT! No possible numbers match your inputs.\n", name);
            return;
        }
    }

    printf("%s, YOU CHEAT! No valid numbers left to guess.\n", name);
}

// 出題者模式：玩家猜電腦的數字
// 輸入：玩家名稱
void player_mode(const char *name) {
    srand((unsigned int)time(NULL));
    int question[SIZE];

    // 生成隨機不重複的 4 位數字
    for (int i = 0; i < SIZE; i++) {
        question[i] = rand() % 10;
        for (int j = 0; j < i; j++) {
            if (question[i] == question[j]) {
                i--; // 發現重複則重新生成
                break;
            }
        }
    }

    printf("Please input your answer.%s!\n(Input four different number between 0 and 9)\n", name);

    int A = 0, B = 0;
    char input[10];

    while (A != SIZE) { // 玩家未完全猜中時
        printf("Enter your guess: ");
        scanf("%s", input);

        if (strlen(input) != SIZE) { // 檢查輸入長度是否為4
            printf("Please input four different number between 0 and 9 again.\n");
            continue;
        }

        int guess[SIZE];
        int has_duplicates = 0;
        // 檢查數字是否有重複
        for (int i = 0; i < SIZE; i++) {
            guess[i] = input[i] - '0';
            if (guess[i] < 0 || guess[i] > 9) { // 檢查數字範圍
                printf("Please input four different number between 0 and 9 again.\n");
                continue;
            }
            // 檢查是否有重複的數字
            for (int j = 0; j < i; j++) {
                if (guess[i] == guess[j]) {
                    has_duplicates = 1;
                    break;
                }
            }
        }

        if (has_duplicates) {
            printf("Please input four different number between 0 and 9 again.\n");
            continue; // 如果有重複數字，要求重新輸入
        }

        calculate_AB(question, guess, &A, &B);
        printf("Result: %dA%dB\n", A, B);
    }

    printf("Congratulations, %s! You win!\n", name);
}

int main() {
    srand((unsigned int)time(NULL)); // 初始化隨機數生成器
    char play_again = 'y';
    char name[NAME_LENGTH];

    // 讓使用者輸入名稱
    printf("Welcome to the 1A2B game!\n");
    printf("Please enter your name: ");
    scanf("%s", name);

    while (play_again == 'y' || play_again == 'Y') {
        int mode;

        printf("Hello, %s! Choose a mode:\n", name);
        printf("1. Player Mode (You guess the number)\n");
        printf("2. Question Mode (PC guesses your number)\n");

        // 輸入遊玩模式
        scanf("%d", &mode);
        while (mode != 1 && mode != 2) { // 檢查輸入是否合法
            printf("Invalid choice. Please input '1' or '2' again: ");
            scanf("%d", &mode);
        }

        // 根據使用者輸入選擇猜題或出題
        if (mode == 1) {
            player_mode(name);
        } 
        else if (mode == 2) {
            questioner_mode(name);
        }

        // 是否重新遊戲
        printf("Do you want to play again? Yes or No (Please input 'y' or 'n'): ");
        scanf(" %c", &play_again);

        while (play_again != 'y' && play_again != 'Y' && play_again != 'n' && play_again != 'N') {
            printf("Invalid choice. Please input 'y' or 'n': ");
            scanf(" %c", &play_again);
        }
    }

    printf("Thank you for playing, %s! Goodbye!\n", name);
    return 0;
}
