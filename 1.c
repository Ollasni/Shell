#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


char *get_word(char *end)
{
	char ch;
	size_t newsize = 0;
	char *words = NULL;
	if(!words) {
		words = malloc((newsize + 1) * sizeof(char));
	}
	while(1) {
	ch = getchar();
	if (ch == '\t' || ch == '\n' || ch == ' ')
		break;
	words = realloc(words, (newsize + 1) * sizeof(char));
	words[newsize] = ch;
	newsize++;
	}
	*end = ch;
	words = realloc(words, ((newsize + 1) * sizeof(char)));
	words[newsize] = '\0';
	return words;
}

char **get_list()
{
	char **text = NULL;
	int size = 0;
	char *word;
	char end_of_w = '0';
	if(!text) {
		text = malloc((size + 1) * sizeof(char*));
	}
	while(1) {
		if(end_of_w == '\n')
			break;
		text = realloc(text, (size + 1) * sizeof(char*));
		word = get_word(&end_of_w);
		text[size] = word;
		size++;
	}
	text = realloc(*text, ((size + 1) * sizeof(char*)));
	text[size] = '\0';
	return text;
}



int main() {
	char **word = get_list();
	int i = 0;
	while(word[i] != NULL)
		printf("%s", word[i++]);
	return 0;
}
