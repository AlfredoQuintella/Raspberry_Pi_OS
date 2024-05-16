#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SHELL_VERSION "1.0"

/********************************************************************/
/* The following code is a basic shell. As we'll deal with multiple */
/* commands we need a way to find the command fast or see if the    */
/* command is wrong fast. To solve this problem we'll make a        */
/* hashtable and every command have to be put there.                */
/********************************************************************/
/* Be aware of memory leaks or using memory not alocated as we'll   */
/* use the arguments of the main function. Most of the problems can */
/* be solved (and this method have to be implemented) just by       */
/* checking the number of arguments passed (argc).                  */
/********************************************************************/
/* We'll be implmeneted a fork mode. The fork generates a son shell */
/* to execute the command. With this we dont have to wait in the    */
/* father shell. Basic commands like version, ping and others dont  */
/* need a fork, but more complex function need one.                 */
/********************************************************************/

// defining the type of function for the command
typedef void (*CommandFunction)(char *);

// structuring the command
typedef struct {
    const char *name;
    CommandFunction function;
} CommandEntry;

/********************************************************************/

/* int execute_command (char *args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}
 */

/********************************************************************/

void command_version() {
    printf("Shell version %s\n", SHELL_VERSION);
}

void command_ping(char *args) {
    printf("Pong!\n");
}

void command_mkdir(char *args) {
    printf("Implementar comando mkdir com argumentos: %s\n", args);
}

void command_cd(char *args) {
    printf("Implementar comando cd com argumentos: %s\n", args);
}

void command_ls(char *args) {
    printf("Implementar comando ls com argumentos: %s\n", args);
}

/********************************************************************/

// hashtable of the commands
CommandEntry commands[] = {
    {"version", command_version},
    {"ping", command_ping},
    {"mkdir", command_mkdir},
    {"cd", command_cd},
    {"ls", command_ls},
    {NULL, NULL} // Marcação de fim da lista
};

/********************************************************************/

int main(int argc, char **argv) {
    char command[1024];
    char *token;

    while (1) {
        printf("Shell > ");
        fflush(stdout);

        // Reading the command from the client
        if (fgets(command, sizeof(command), stdin) == NULL) {
            printf("\n"); // Trata o caso de Ctrl+D (EOF)
            break;
        }

        // Remove the new line char
        command[strcspn(command, "\n")] = '\0';

        // if nothing is written then we cant make the check and should continue the initial loop
        if (strcmp(command, "") == 0) {
            continue; 
        }

        // extract the command and arguments
        token = strtok(command, " ");
        char *command_name = token;
        char *command_args = strtok(NULL, "");

        if (strcmp(command_name, "exit") == 0) {
            printf("Killing the shell...\n");
            return EXIT_SUCCESS;
        }

        // finding the command in hashtable and execute it
        int i = 0;
        while (commands[i].name != NULL) {
            if (strcmp(command_name, commands[i].name) == 0) {
                commands[i].function(command_args);
                break;
            }
            i++;
        }

        // if the command is not found a message is shown
        if (commands[i].name == NULL) {
            printf("Unknown command: %s\n", command_name);
        }
    }

    return EXIT_SUCCESS;
}