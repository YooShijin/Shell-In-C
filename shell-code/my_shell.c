#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_WORK_DIR 256

/* Splits the string by space and returns the array of tokens
 *
 */
char **tokenize(char *line)
{
	char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
	int i, tokenIndex = 0, tokenNo = 0;

	for (i = 0; i < strlen(line); i++)
	{

		char readChar = line[i];

		if (readChar == ' ' || readChar == '\n' || readChar == '\t')
		{
			token[tokenIndex] = '\0';
			if (tokenIndex != 0)
			{
				tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0;
			}
		}
		else
		{
			token[tokenIndex++] = readChar;
		}
	}

	free(token);
	tokens[tokenNo] = NULL;
	return tokens;
}

int main(int argc, char *argv[])
{
	char line[MAX_INPUT_SIZE];
	char **tokens;
	int i;

	while (1)
	{
		/* BEGIN: TAKING INPUT */
		memset(line, 0, sizeof(line));
		char buf[MAX_WORK_DIR];
		getcwd(buf, MAX_WORK_DIR);
		printf("%s ", buf);
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();

		/* END: TAKING INPUT */

		line[strlen(line)] = '\n';
		tokens = tokenize(line);

		if (strcmp(tokens[0], "cd") == 0)
		{
			if (chdir(tokens[1]))
			{
				perror("Failure: ");
			}
			continue;
		}
		pid_t pid = fork();
		if (pid < 0)
		{
			perror("Fork failed");
			exit(1);
		}
		else if (pid == 0)
		{
			execvp(tokens[0], tokens);
			fflush(stdout);
			perror("execvp failed");
			exit(1);
		}
		{
			int status;
			wait(&status);
			printf("EXITSTATUS: %d\n", WEXITSTATUS(status));
		}

		// Freeing the allocated memory
		for (i = 0; tokens[i] != NULL; i++)
		{
			free(tokens[i]);
		}
		free(tokens);
	}
	return 0;
}
