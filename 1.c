#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

char *get_word(char *end, char *sign)
{
    size_t len_w = 0;
    char *word = NULL, ch;
	while(1) {
		ch = getchar();
		if(ch == ' ' || ch == '\t' || ch == '\n')
			break;
		if(ch == '<' || ch == '>') {
			sign = ch;
			ch = getchar();
		skip_spaces(end);
		}
		if(ch == '|')
		word = realloc(word, (len_w + 1) * sizeof(char));
		word[len_w++] = ch;
		}
	if (word == NULL)
		return 0;
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
	while(1) {
		if(end_of_w == '\n')
			break;
		word = get_word(&end_of_w, &sign);
		if(sign && word) {
			if(sign == '<')
				in_out[0] = word;
			else {
				in_out[1] = word;
			}
		sign = 0;
		continue;
		}
		text = realloc(text, (len_tx + 1) * sizeof(char*));
		text[len_tx++] = word;
	}
	if(text == NULL)
		return 0;
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
	free(text);
}

int exit_proc(char **list)
{
	if(list == NULL || list[0] == NULL)
		return 0;
	if(strcmp(list[0], "exit") == 0 || strcmp(list[0], "quit") == 0)
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

int exec_proc(char **list, char *in_out[])
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

int is_delimeter(char ch) {
	if(ch == '|')
	

}

void exec_single(char **command, int input, int output) {
	pid = fork();
	
}

int exec_proc_per_few_command(char **list[], char *in_out[])
{
	char *A[] = **list[0];
	char *B[] = **list[1];
	int pipefd[2];
	pipe(pipefd[2]);
	int fd[2];
	fd[0] = redir_in(in_out[0]);
	fd[1] = redir_out(in_out[1]);
	if(fork() == 0) {
		dup2(fd[0], 0);
		dup2(pipefd[1], 1);
		close(pipefd[0]);
		close(pipefd[1]);
		if(execve(A[0], A) < 0) {
			perror("Exec failed");
			exit(1);
		}
	}
	if(fork == 0) {
		dup2(fd[1], 1);
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		close(pipefd[1]);
		if(execve(B[0], B) < 0) {
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

void func_few_comm(char **x, int num_of_com, char **command)
	if(command) {
		x = realloc(x, (num_of_com + 1) * sizeof(char**));
		x[num_of_com++] = command;
	}
	x = realloc(x, (num_of_com + 1) * sizeof(char**));
    x[num_of_com] = NULL;
	return;
}

char ***get_commands(char sign, char *in_out[])
{
    size_t num_com = 0;
    char ***all_com = NULL;
    char **command = NULL;
    while(1) {
        command = get_list(0, char *in_out[]);
        all_com = realloc(all_com, (num_com + 1) * sizeof(char**));
        all_com[num_com++] = command;
    }
    if(all_com == NULL)
        return 0;
    all_com = realloc(all_com, (num_com + 1) * sizeof(char**));
    all_com[num_com] = NULL;
    return all_com;
};


int main(int argc, char **argv)
{
	char ***all_commands = NULL;
	char *in_out[] = {NULL, NULL};
	char **mass_of_viv[] =
	int num_of_com = 0;
	char **command = get_list(0, in_out);
	func_few_comm(all_commands[num_of_com], num_of_com, command);
	while(!exit_proc(command)) {
		exec_proc(command, in_out);
		command = free_list(command);
		command = get_list(0, in_out);
		func_few_comm(all_commands[num_of_com], num_of_com, command);
	}
	command = free_list(command);
	return  0;
}
