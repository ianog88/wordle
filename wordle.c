#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WORD_LENGTH 5
#define MAX_WORDS 12972  

char *possible_words[MAX_WORDS];
int total_words = 0;

void load_words(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }

    char word[WORD_LENGTH + 2];  
    while (fgets(word, sizeof(word), file)) {
        word[strcspn(word, "\n")] = '\0';  
        possible_words[total_words] = strdup(word);  
        total_words++;
    }
    fclose(file);
}

void get_feedback(const char *guess, const char *actual_word, char *feedback) {
    int used_in_actual[WORD_LENGTH] = {0};  

    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == actual_word[i]) {
            feedback[i] = 'G';  
            used_in_actual[i] = 1;
        } else {
            feedback[i] = 'X';  
        }
    }

    for (int i = 0; i < WORD_LENGTH; i++) {
        if (feedback[i] == 'G') continue;  
        for (int j = 0; j < WORD_LENGTH; j++) {
            if (i != j && guess[i] == actual_word[j] && !used_in_actual[j]) {
                feedback[i] = 'Y';  
                used_in_actual[j] = 1;
                break;
            }
        }
    }
    feedback[WORD_LENGTH] = '\0';  
}

double calculate_entropy(int *pattern_counts, int num_patterns, int total_outcomes) {
    double entropy = 0.0;
    for (int i = 0; i < num_patterns; i++) {
        if (pattern_counts[i] > 0) {
            double probability = (double)pattern_counts[i] / total_outcomes;
            entropy -= probability * log2(probability);
        }
    }
    return entropy;
}

int feedback_to_index(const char *feedback) {
    int index = 0;
    for (int i = 0; i < WORD_LENGTH; i++) {
        index *= 3;
        if (feedback[i] == 'G') index += 2;  
        else if (feedback[i] == 'Y') index += 1;  
    }
    return index;
}

double expected_entropy(const char *guess, char **remaining_words, int num_remaining) {
    int pattern_counts[243] = {0};  

    char feedback[WORD_LENGTH + 1];
    for (int i = 0; i < num_remaining; i++) {
        get_feedback(guess, remaining_words[i], feedback);
        int index = feedback_to_index(feedback);
        pattern_counts[index]++;
    }

    return calculate_entropy(pattern_counts, 243, num_remaining);
}

char* find_best_guess(char **remaining_words, int num_remaining) {
    double max_entropy = -1;
    char *best_guess = NULL;

    for (int i = 0; i < num_remaining; i++) {
        double entropy = expected_entropy(remaining_words[i], remaining_words, num_remaining);
        if (entropy > max_entropy) {
            max_entropy = entropy;
            best_guess = remaining_words[i];
        }
    }

    return best_guess;
}

int main() {
    load_words("wordle.txt");

    int num_remaining = total_words;
    char **remaining_words = possible_words;

    for (int round = 1; round <= 6; round++) {
        char *best_guess = find_best_guess(remaining_words, num_remaining);

        printf("Best guess for round %d: %s\n", round, best_guess);

        char feedback[WORD_LENGTH + 1];
        printf("Enter feedback (G = green, Y = yellow, X = gray): ");
        scanf("%s", feedback);

        int new_remaining = 0;
        for (int i = 0; i < num_remaining; i++) {
            char temp_feedback[WORD_LENGTH + 1];
            get_feedback(best_guess, remaining_words[i], temp_feedback);
            if (strcmp(temp_feedback, feedback) == 0) {
                remaining_words[new_remaining++] = remaining_words[i];
            }
        }
        num_remaining = new_remaining;

        if (num_remaining == 1) {
            printf("The solution is: %s\n", remaining_words[0]);
            break;
        }
    }

    return 0;
}
