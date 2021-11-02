#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
/*int exec_proc(char **list, char *in_out[])
{
	int fd[2];
	fd[0] = redir_in(in_out[0]);
	fd[1] = redir_out(in_out[1]);
	if(fork() > 0)
		wait(NULL);
	else {
		dup2(fd[0], 0);
		dup2(fd[1], 1);
		if(execvp(list[0], list) < 0) {
			perror("Exec failed");
			exit(1);
		}
	}
	return 0;
}
*/


void skip_spaces(char *ch) {
	while(*ch == ' ')
		*ch = getchar();
}

char *get_word(char *end, char *sign, int *flag)
{
    size_t len_w = 0;
    char *word = NULL, ch;
	ch = getchar();
	skip_spaces(&ch);
	while(1) {
		if(ch == ' ' || ch == '\t' || ch == '\n')
			break;
		if(ch == '<' || ch == '>') {
			*sign = ch;
			ch = getchar();
			skip_spaces(&ch);
		}
		if(ch == '|') {
			*flag = 1;
			break;
		}
		word = realloc(word, (len_w + 1) * sizeof(char));
		word[len_w++] = ch;
		ch = getchar();
	}
	*end = ch;
	if (word == NULL)
		return 0;
	word = realloc(word, (len_w + 1) * sizeof(char));
	word[len_w] = '\0';
	return word;
};

char **get_list(char *sign, char *in_out[], char *end_of_w)
{
	size_t len_tx = 0;
	char **text = NULL;
	int flag = 0;
	char *word = NULL;
	while(1) {
		word = get_word(end_of_w, sign, &flag);
//		exit(1);
		for(int i = 0; word[i]; i++)
			putchar(word[i]);
		exit(1);
		if(*sign && word) {
//			printf("%d", *sign);

			if(*sign == '<') {
				in_out[0] = word;
			}
			else if(*sign == '>') {
				in_out[1] = word;
			}
			*sign = 0;
			continue;
		}
		if(flag) {
			break;
		}
		text = realloc(text, (len_tx + 1) * sizeof(char*));
		text[len_tx++] = word;
		if(*end_of_w == '\n')
			break;
	}
	text = realloc(text, (len_tx + 1) * sizeof(char*));
	text[len_tx] = NULL;
	return text;
};

char ***get_commands(char *in_out[], char ***mas_in_out)
{
		char ***all_com = NULL;
		//char ***mas_in_out = NULL;
		char **text = NULL;
		int num_com = 0;
		char *sign = NULL, end = 0;
		text = get_list(sign, in_out, &end);
		while(1) {
        	all_com = realloc(all_com, (num_com + 1) * sizeof(char**));
			exit(1);
        	mas_in_out = realloc(mas_in_out, (num_com + 1) * sizeof(char**));
        	mas_in_out[num_com] = in_out;
        	all_com[num_com++] = text;
        	sign = NULL;
			if(end == '\n')
				break;
			text = get_list(sign, in_out, &end);

    }
    if(mas_in_out != NULL) {
        mas_in_out = realloc(mas_in_out, (num_com + 1) * sizeof(char**));
        mas_in_out[num_com] = NULL;
    }
    all_com = realloc(all_com, (num_com + 1) * sizeof(char**));
    all_com[num_com] = NULL;
    return all_com;
};

char ***free_list(char ***text)
{
	for (int i = 0; text[i]; i++)
		for(int j = 0; text[i][j]; j++)
			free(text[i][j]);
	free(text);
}

int exit_proc(char ***list)
{
	if(*list == NULL || *list[0] == NULL || list == NULL)
		return 0;
	if(strcmp(*list[0], "exit") == 0 || strcmp(*list[0], "quit") == 0)
		return 1;
	return 0;
}

int redir_in(char *in_out) {
	int x = 0;
	if(in_out) {
		x = open(in_out, O_RDONLY, S_IRUSR|S_IWUSR);
	if(x < 0) {
		perror("open input error");
		return -1;
		}
	}
	return x;
}

int redir_out(char *in_out) {
	int x = 1;
	if(in_out) {
		x = open(in_out, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	if(x < 0) {
		perror("open output error");
		return -1;
		}
	}
	return x;
}

int exec_proc_per_two_command(char *A[], char *B[], char **in_out_A, char **in_out_B)
{
//	char *A[] = **list[0];
//	char *B[] = **list[1];
	int pipefd[2];
	pipe(pipefd);
	int fd[2];
	fd[0] = redir_in(in_out_A[0]); //for A
	fd[1] = redir_out(in_out_B[1]); //for B
	if(fork() == 0) {
		dup2(fd[0], 0);
		dup2(pipefd[1], 1);
		close(pipefd[0]);
		close(pipefd[1]);
		if(execvp(A[0], A) < 0) {
			perror("Exec failed");
			exit(1);
		}
	}
	if(fork == 0) {
		dup2(fd[1], 1);
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		close(pipefd[1]);
		if(execvp(B[0], B) < 0) {
			perror("Exec failed");
			exit(1);
		}
	}
	close(fd[1]);
	close(fd[0]);
	wait(NULL);
	wait(NULL);
	return 0;
}

void putline(char ***line)
{
	if (line == NULL)
		return;
	for (int i = 0; line[i]; i++){
		for(int j = 0; line[i][j]; j++)
			printf("%s ", line[i][j]);
		printf("\n");
	}
//	putchar('\n');
}


int main(int argc, char **argv)
{
	char *in_out[] = {NULL, NULL};
	char ***mas_in_out = NULL;
	char ***all_commands = get_commands(in_out, mas_in_out);
	while(!exit_proc(all_commands)) {
		//exec_proc(command, in_out);
//		exec_proc_per_two_command(all_commands[0], all_commands[1], mas_in_out[0], mas_in_out[1]);
		putline(all_commands);
		all_commands = free_list(all_commands);
		all_commands = get_commands(in_out, mas_in_out);
//		putline(all_commands);
//		all_commands = get_commands(char *in_out[], char ***mas_in_out, 0, 0);
	}
	//all_commands = free_list(all_commands);
	return  0;
}
