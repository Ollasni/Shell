#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_word(char *end)
{
    size_t len_w = 0;
    char *word = NULL, ch;
	if(!word) {
        word = malloc((len_w + 1) * sizeof(char));
    }
	while(1) {
    	ch = getchar();
    	if (ch == ' ' || ch == '\t' || ch == '\n')
    		break;
        word = realloc(word, (len_w + 1) * sizeof(char));
        word[len_w++] = ch;
    }
    word = realloc(word, (len_w + 1) * sizeof(char));
    word[len_w + 1] = '\0';
    *end = ch;
    return word;
};

char **get_list()
{
    size_t len_tx = 0;
    char **text = NULL, *word, end_of_w = 0;
    if(!text) {
        text = malloc((len_tx + 1) * sizeof(char*));
    }
    while(1)
    {
		if(end_of_w == '\n')
			break;
        text = realloc(text, (len_tx + 1) * sizeof(char*));
        word = get_word(&end_of_w);
        text[len_tx++] = word;
    }
    text = realloc(text, (len_tx + 1) * sizeof(char*));
    text[len_tx] = '\0';
    return text;
};

char **free_list(char **text)
{
    int ind = 0;
    while(text[ind] != NULL) {
        free(text[ind++]);
    }
    free(text);
}


int main()
{
    char **elem = get_list();
    int ind = 0;
    do {
    	printf("%s ", elem[ind++]);
	} while(elem[ind] != NULL);
    putchar('\n');
	free_list(elem);
    return 0;
}
