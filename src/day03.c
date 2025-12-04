#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers/input_helper.h"

#define DIGIT_MASK 15

static int get_joltage_digit_val(char digitChar) {
    if (digitChar >= '1' && digitChar <= '9') return digitChar & DIGIT_MASK;
    return -1;
}

void solve_day03_a(char** instructions, int count) {
    long long res = 0;

    for (int bank = 0; bank < count; ++bank) {
        char* bankStr = instructions[bank];
        size_t len = strlen(bankStr);
        int maxJoltage = 0;

        for (size_t i = 0; i < len; ++i) {
            int digit1Val = get_joltage_digit_val(bankStr[i]);
            if (digit1Val == -1) continue;

            for (size_t j = i + 1; j < len; ++j) {
                int digit2Val = get_joltage_digit_val(bankStr[j]);
                if (digit2Val == -1) continue;

                int currJoltage = (digit1Val * 10) + digit2Val;
                if (currJoltage > maxJoltage) {
                    maxJoltage = currJoltage;
                    if (maxJoltage == 99) break;
                }
            }
            if (maxJoltage == 99) break;
        }
        res += maxJoltage;
    }

    printf("Res: %lld\n", res);
}

#define JOLTAGE_LEN 12

void solve_day03_b(char** instructions, int count) {
    long long res = 0;

    for (int bank = 0; bank < count; ++bank) {
        char* bankStr = instructions[bank];
        size_t len = strlen(bankStr);
        long long currJoltage = 0;
        int currIdx = 0;
        int digitsLeft = JOLTAGE_LEN;

        while (digitsLeft) {
            size_t searchEnd = len - digitsLeft + 1;
            char maxDigitChar = 0;
            int maxIdx = -1;

            for (size_t i = currIdx; i < searchEnd; ++i) {
                char currDigitChar = bankStr[i];

                if (currDigitChar >= '1' && currDigitChar <= '9') {
                    if (currDigitChar > maxDigitChar) {
                        maxDigitChar = currDigitChar;
                        maxIdx = i;
                        if (maxDigitChar == '9') break;
                    }
                }
            }
            int digitVal = maxDigitChar & DIGIT_MASK;
            currJoltage = currJoltage * 10 + digitVal;
            currIdx = maxIdx + 1;
            digitsLeft--;
        }
        res += currJoltage;
    }

    printf("Res: %lld\n", res);
}


int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    int task = 1;
    if (argc >= 3) task = atoi(argv[2]);

    int count = 0;
    char** instructions = read_input_file(argv[1], &count);
    if (!instructions) return 1;

    switch (task) {
        case 1:
            solve_day03_a(instructions, count);
            break;
        case 2:
            solve_day03_b(instructions, count);
            break;
        default:
            fprintf(stderr, "Unknown TASK: %d\n", task);
            for (int i = 0; i < count; ++i) free(instructions[i]);
            free(instructions);
            return 1;
    }
    for (int i = 0; i < count; ++i) free(instructions[i]);
    free(instructions);
    return 0;
}
