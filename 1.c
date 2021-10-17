#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

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


int check_exit(char **list)
{
	if (list == NULL)
		return 0;
	if (list[0] == NULL)
		return 0;
	if (!strcmp(list[0], "exit") || !strcmp(list[0], "quit"))
		return 1;
	return 0;
}


int exec_process(char **list)
{
	if (fork() > 0)
		wait(NULL);
	else {
		if (execvp(list[0], list) < 0) {
			perror("Exec failed");
			exit(1);
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	while(1) {
		char **command = get_list();
		if(check_exit(command)) {
			free_list(command);
			break;
		}
		if (command && command[0]) {
			exec_process(command);
    		command = free_list(command);
		}
	}
    return 0;
}
