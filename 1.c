#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

char *get_word(char *end, char sign)
{
    size_t len_w = 0;
    char *word = NULL, ch;
	if(!word) {
        word = malloc((len_w) * sizeof(char));
    }
	while(1) {
    	ch = getchar();
    	if (ch == ' ' || ch == '\t' || ch == '\n')
    		break;
		if(ch == '<' || ch == '>') {
			sign = ch;
			ch = getchar();
			while(1) {
				if(ch != ' ')
					break;
				ch = getchar();
			}
		}
        word = realloc(word, (len_w + 1) * sizeof(char));
        word[len_w++] = ch;
    }
    word = realloc(word, (len_w + 1) * sizeof(char));
    word[len_w] = '\0';
    *end = ch;
    return word;
};

char **get_list(char sign, char *in_out[])
{
    size_t len_tx = 0;
    char **text = NULL, end_of_w = 0;
    char *word = NULL;
	if(!text) {
        text = malloc((len_tx) * sizeof(char*));
    }
    while(1)
    {
		if(end_of_w == '\n')
			break;
		word = get_word(&end_of_w, &sign);
		if(sign)
		{
			char *file = NULL;
			size_t len_f = strlen(word);
			file = malloc(len_f * sizeof(char*));
			strcpy(file, word + 1);
			if(sign == '<')
				in_out[0] = file;
			else {
				 in_out[1] = file;
			 }
			sign = 0;
			free(word);
			continue;
		}
        text = realloc(text, (len_tx + 1) * sizeof(char*));
        text[len_tx++] = word;
    }
    text = realloc(text, (len_tx + 1) * sizeof(char*));
    text[len_tx] = NULL;
    return text;
};

char **free_list(char **text)
{
    int ind = 0;
    while(text[ind] != NULL) {
        free(text[ind++]);
    }
    free(text[ind]);
    free(text);
}


int exit_proc(char **list)
{
	if (list == NULL || list[0] == NULL)
		return 0;
	if (strcmp(list[0], "exit") == 0 || strcmp(list[0], "quit") == 0)
		return 1;
	return 0;
}

int redir_in(char *in_out[]) {
	int x = 0;
  	if(in_out[0]) {
        x = open(in_out[0], O_RDONLY, S_IRUSR|S_IWUSR);
        if(x < 0) {
            perror("open input error");
                return -1;
        }
    }
    return x;
}

int redir_out(char *in_out[]) {
	int x = 1;
    if(in_out[1]) {
        x = open(in_out[1], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
        if(x < 0) {
            perror("open output error");
                return -1;
        }
    }
	return x;
}

int exec_proc(char **list, char *in_out[])
{
	int fd[2];
	fd[0] = redir_in(in_out);
	fd[1] = redir_out(in_out);
	if (fork() > 0)
		wait(NULL);
	else  {
			dup2(fd[0], 0);
			dup2(fd[1], 1);
			if (execvp(list[0], list) < 0) {
				perror("Exec failed");
				exit(1);
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	char *in_out[] = {NULL, NULL};
	char **command = get_list(0, in_out);
	while(!exit_proc(command)) {
		exec_proc(command, in_out);
		command = free_list(command);
		command = get_list(0, in_out);
	}
	command = free_list(command);
    return 0;
}
