#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#define WORD_LEN 50
#define MAX_WORDS 10000

char **lines;
int total_lines = 0;
int current_line = 0;

char words[MAX_WORDS][WORD_LEN];
int counts[MAX_WORDS];
int total_words = 0;

pthread_mutex_t line_mutex;
pthread_mutex_t word_mutex;

void add_word(char *word)
{
    pthread_mutex_lock(&word_mutex);

    for (int i = 0; i < total_words; i++) {
        if (strcmp(words[i], word) == 0) {
            counts[i]++;
            pthread_mutex_unlock(&word_mutex);
            return;
        }
    }

    strcpy(words[total_words], word);
    counts[total_words] = 1;
    total_words++;

    pthread_mutex_unlock(&word_mutex);
}

void *count_words(void *arg)
{
    while (1) {
        pthread_mutex_lock(&line_mutex);
        int line_number = current_line++;
        pthread_mutex_unlock(&line_mutex);

        if (line_number >= total_lines)
            break;

        char *line = lines[line_number];
        char word[WORD_LEN];
        int idx = 0;

        for (int i = 0; line[i]; i++) {
            if (isalpha(line[i])) {
                word[idx++] = tolower(line[i]);
            } else if (idx > 0) {
                word[idx] = '\0';
                add_word(word);
                idx = 0;
            }
        }

        if (idx > 0) {
            word[idx] = '\0';
            add_word(word);
        }
    }
    return NULL;
}

void sort_by_frequency()
{
    for (int i = 0; i < total_words - 1; i++) {
        for (int j = i + 1; j < total_words; j++) {
            if (counts[j] > counts[i]) {
                int tmp_count = counts[i];
                counts[i] = counts[j];
                counts[j] = tmp_count;

                char tmp_word[WORD_LEN];
                strcpy(tmp_word, words[i]);
                strcpy(words[i], words[j]);
                strcpy(words[j], tmp_word);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <file> <threads>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        printf("Cannot open file\n");
        return 1;
    }

    int num_threads = atoi(argv[2]);
    pthread_t threads[num_threads];

    printf("Word count program starting...\n");
    printf("Input file: %s\n", argv[1]);
    printf("Number of threads: %d\n", num_threads);
    printf("Output file: result.txt\n\n");

    pthread_mutex_init(&line_mutex, NULL);
    pthread_mutex_init(&word_mutex, NULL);

    int capacity = 100;
    lines = malloc(capacity * sizeof(char *));
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (total_lines == capacity) {
            capacity *= 2;
            lines = realloc(lines, capacity * sizeof(char *));
        }
        lines[total_lines] = strdup(buffer);
        total_lines++;
    }

    fclose(fp);

    printf("Total lines read: %d\n", total_lines);
    printf("Starting word counting...\n");

    for (int i = 0; i < num_threads; i++)
        pthread_create(&threads[i], NULL, count_words, NULL);

    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    printf("Word counting finished\n");
    printf("Sorting results...\n");

    sort_by_frequency();

    FILE *out = fopen("result.txt", "w");
    for (int i = 0; i < total_words; i++)
        fprintf(out, "%s : %d\n", words[i], counts[i]);
    fclose(out);

    printf("Processing completed\n");
    printf("Total unique words: %d\n", total_words);
    printf("Results written to result.txt\n");

    return 0;
}
