#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TEXT_SIZE 1024
#define WORD_SIZE 50
#define DELIM   " .?-_'"

void text_replace(const char *text, const char *old, const char *new, char *replaced_text);
int char_count(const char *text, const char ch);
int unique_words_count(const char *text);
void extract_unique_words(const char *text, char words[][WORD_SIZE]);

int main(void)
{
    const char text[] = "This is a simple test. This text is for testing.";

    char old_word[] = "test";
    char new_word[] = "example";

    char replaced_text[TEXT_SIZE] = {0};

    char unique_words[TEXT_SIZE][WORD_SIZE] = {0};

    int unique_words_cnt = 0;

    printf("original_text: %s\n", text);
    text_replace(text, old_word, new_word, replaced_text);
    printf("replaced_text: %s\n", replaced_text);

    printf("totally character \'%c\' comes %d times in \"%s\"\n", 's', char_count(text, 's'), text);
    printf("totally character \'%c\' comes %d times in \"%s\"\n", 's', char_count(replaced_text, 's'), replaced_text);

    printf("\"%s\" have %d words\n", text, unique_words_count(text));

    extract_unique_words(text, unique_words);
    for (int i = 0; unique_words[i][0]; i++) {
        printf("%s\n", unique_words[i]);
    }

    return 0;
}

void text_replace(const char *text, const char *old, const char *new, char *replaced_text) {
    char *pos = NULL;
    char *temp = replaced_text;

    while ((pos = strstr(text, old)) != NULL) {// find old word in text
        if (strncpy(temp, text, pos - text) == NULL) {// copy previous substring at the position old word
            printf("[strncpy]: Error copy from text to replaced_text.\n");
        }

        if (strncat(temp, new, strlen(new)) == NULL) {// then concatenate substring and new word
            printf("Error cat\n");
        }

        // update finded pos and copied pos
        temp += pos - text + strlen(new);
        text = pos + strlen(old);
    }

    if (strcpy(temp, text) == NULL) {// finally copy remained substring in original text
        printf("[strcpy]: Error copy from text to replaced_text.\n");
    }
}


int char_count(const char *text, const char ch) {
    int cnt = 0;

    for (int i = 0; text[i]; ++i) {
        if (text[i] == ch) cnt += 1;
    }

    return cnt;
}

int unique_words_count(const char *text) {
    int cnt = 0;
    char temp[TEXT_SIZE] = {0};
    char *token = NULL;

    if (strncpy(temp, text, strlen(text)) == NULL) {
        printf("[strncpy]: Error copy from text to temp.\n");
    }

    token = strtok(temp, DELIM);
    while (token != NULL) {
        cnt += 1;
        token = strtok(NULL, DELIM);
    }
    
    return cnt;
}

void extract_unique_words(const char *text, char words[][WORD_SIZE]) {
    char temp[TEXT_SIZE] = {0};
    char *token = NULL;
    int row = 0;

    if (strncpy(temp, text, strlen(text)) == NULL) {
        printf("[strncpy]: Error copy from text to temp.\n");
    }
    printf("temp: %s\n", temp);

    token = strtok(temp, DELIM);
    while (token != NULL && row < TEXT_SIZE) {
        sprintf(words[row], "%s", token);
        token = strtok(NULL, DELIM);
        row += 1;
    }
}