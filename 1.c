#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>

int son_pid = -1;

void skip_spaces(char *ch) {
	while(*ch == ' ')
		*ch = getchar();
}

char ***free_list(char ***text)
{
	for (int i = 0; text[i]; i++){
		for(int j = 0; text[i][j]; j++)
			free(text[i][j]);
		free(text[i]);
	}
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

int change_directory(char ***cmd, char *home)
{
	char s[100];
	if(strcmp(cmd[0][0], "cd") == 0) {
		if (cmd[0][1] == NULL || strcmp(cmd[0][1], "~") == 0) {
			chdir(home);
		} else {
			chdir(cmd[0][1]);
		}
		printf("%s\n", getcwd(s, 100));
		return 1;
	}
	return 0;
}


char *get_word(char *end, char *sign, int *flag, int *phone, int *conv)
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
		if(ch == '|' && *flag) {
			*conv == 2;
			break;
		}
		if(ch == '|') {
			*flag = 1;
			break;
		}
		if(ch == '&' && *phone) {
			*conv == 1;
			break;
		}
		if(ch == '&') {
			*phone == 1;
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


char **get_list(char *sign, char *in_out[], char *end_of_w, int *phone, int *conv)
{
	size_t len_tx = 0;
	char **text = NULL;
	int flag = 0;
	char *word = NULL;
	while(1) {
		word = get_word(end_of_w, sign, &flag, phone, conv);
		if(*sign) {
			if(*sign == '<')
				in_out[0] = word;
			else if(*sign == '>')
				in_out[1] = word;
			*sign = 0;
			if(*end_of_w == '\n')
				break;
			continue;
		}
		if(flag || *phone || *conv)
			break;
		text = realloc(text, (len_tx + 1) * sizeof(char*));
		text[len_tx++] = word;
		if(*end_of_w == '\n')
			break;
	}
	text = realloc(text, (len_tx + 1) * sizeof(char*));
	text[len_tx] = NULL;
	return text;
};

char ***get_commands(char *in_out[], int *num_com, int *phone, int *conv)
{
		char ***all_com = NULL;
		char **text = NULL;
		*num_com = 0;
		char sign = 0, end = 0;
		text = get_list(&sign, in_out, &end, phone, conv);
		while(1) {
        	all_com = realloc(all_com, (*num_com + 1) * sizeof(char**));
        	all_com[(*num_com)++] = text;
			if(end == '\n')
				break;
        	sign = 0;
			text = get_list(&sign, in_out, &end, phone, conv);

    }
    all_com = realloc(all_com, (*num_com + 1) * sizeof(char**));
    all_com[*num_com] = NULL;
    return all_com;
};


int exec_single(char **cmd, int input_fd, int output_fd) {
	pid_t pid;
	pid = fork();
	if(pid == 0) {
		if(input_fd != 0) {
			dup2(input_fd, 0);
			close(input_fd);
		}
		if(output_fd != 1) {
			dup2(output_fd, 1);
			close(output_fd);
		}
		if(execvp(cmd[0], cmd) < 0) {
			perror("Exec failed");
			exit(1);
		}
	}
	return pid;
}

int *exec_all(char *in_out[], char ***cmd, int n, int phone, int conv) {
	int *pid = malloc((n) * sizeof(int));
	int pipefd[n + 1][2];
	pipefd[0][0] = redir_in(in_out[0]);
	pipefd[n][1] = redir_out(in_out[1]);

	for(int i = 1; i <= n; i++) {
		if(i < n) {
			pipe(pipefd[i]);
		}
		pid[i - 1] = exec_single(cmd[i - 1], pipefd[i - 1][0], pipefd[i - 0][1]);
		if(pipefd[i - 1][0] != 0) {
			close(pipefd[i - 1][0]);
		}
		if(pipefd[i - 0][1] != 1) {
			close(pipefd[i - 0][1]);
		}
	}
	son_pid = pid[0];
	if(phone)
		return pid;
	for(int j = 0; j < n; j++)
			waitpid(pid[j], NULL, 0);
	free(pid);
	return NULL;
}

/*
void kill_pid(int *pid, int n) {
	for(int i = 0; i < n; i++) {
		if((kill(pid[i], 0) != 0))
			kill(pid[i], SIGKILL);
	}
}
*/
void handler(int signo) {
	puts("received SIGINT");
	kill(son_pid, SIGKILL);
}

int conv_and(char ***list, char *in_out[], int n)
{
	int fd[2];
	fd[0] = redir_in(in_out[0]);
	fd[1] = redir_out(in_out[1]);
	for(int i = 0; i < n; i++) {
		if(fork() == 0) {
			dup2(fd[0], 0);
			dup2(fd[1], 1);
			if(execvp(list[0][i], list[i]) < 0) {
				perror("Exec failed");
				exit(1);
			}
		}
		int wstatus;
		wait(&wstatus);
		printf("%d\n", WEXITSTATUS(wstatus)); 
		if(wstatus != 0)
			break;
		}
	return 0;
}

int conv_or(char ***list, char *in_out[], int n)
{
	int fd[2];
	fd[0] = redir_in(in_out[0]);
	fd[1] = redir_out(in_out[1]);
	for(int i = 0; i < n; i++) {
		if(fork() == 0) {
			dup2(fd[0], 0);
			dup2(fd[1], 1);
			if(execvp(list[0][i], list[i]) < 0) {
				perror("Exec failed");
				exit(1);
			}
		}
		int wstatus;
		wait(&wstatus);
		printf("%d\n", WEXITSTATUS(wstatus)); 
		if(wstatus == 0)
			break;
		}
	return 0;
}


int main(int argc, char **argv)
{
	int n = 0;
	char *in_out[] = {NULL, NULL};
	char ***all_commands = NULL;
	char *home = getenv("HOME");
	int phone = 0, conv = 0;
//	signal(SIGINT, handler);
	while(1) {
		all_commands = get_commands(in_out, &n, &phone, &conv);
		if(exit_proc(all_commands))
			break;
		int *pid = NULL;
		if(!change_directory(all_commands, home)) {
			if(!conv) {
			 pid = exec_all(in_out, all_commands, n, phone, conv);
		//	if(pid != NULL)
		//		kill_pid(pid, n);
			}
			else if(conv == 1) {
				conv_and(all_commands, in_out, n);
			}
			else if(conv == 2) {
				conv_or(all_commands, in_out, n);
			}
		}
		all_commands = free_list(all_commands);
		phone = 0, conv = 0;
//		all_commands = get_commands(in_out, &n);
	}
	all_commands = free_list(all_commands);
	return  0;
}
