#include "shell_header.h"

/**
 * main - Entry point for the shell program
 * @argc: Number of arguments passed to the program
 * @argv: Array of strings containing arguments passed to the program
 * @env: Array of strings containing the environment variables
 * 
 * This function initializes the program's data structure and signals handlers, 
 * then calls the shell loop to read input and execute commands until the user exits.
 * 
 * Return: Always returns zero
 */

int main(int argc, char *argv[], char *env[])
{
    data_of_program data_struct = {NULL}, *data = &data_struct;
    char *prompt = "";

    setup_program_data(data, argc, argv, env);

    signal(SIGINT, handle_ctrl_c);

    if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO) && argc == 1)
    {
        errno = 2;
        prompt = PROMPT_MSG;
    }
    errno = 0;
    sisifo(prompt, data);
    return (0);
}



/**
 * handle_signal_interrupt - Prints the prompt in a new line when the SIGINT
 * (ctrl + c) signal is sent to the program
 * @unused: Unused parameter to match the function signature
 */
void handle_signal_interrupt(int unused)
{
    _print("\n");
    _print(PROMPT_MSG);
}

/**
 * setup_program_data - inicialize the struct with the info of the program
 * @data: pointer to the structure of data
 * @argv: array of arguments pased to the program execution
 * @env: environ pased to the program execution
 * @argc: number of values received from the command line
 */
void setup_program_data(data_of_program *data, int argc, char *argv[], char **env)
{
	int i = 0;

	data->program_name = argv[0];
	data->input_line = NULL;
	data->command_name = NULL;
	data->exec_counter = 0;
	
	if (argc == 1)
		data->file_descriptor = STDIN_FILENO;
	else
	{
		data->file_descriptor = open(argv[1], O_RDONLY);
		if (data->file_descriptor == -1)
		{
			_print_strerr(data->program_name);
			_print_strerr(": 0: Can't open ");
			_print_strerr(argv[1]);
			_print_strerr("\n");
			exit(127);
		}
	}
	data->tokens = NULL;
	data->env = malloc(sizeof(char *) * 50);
	if (env)
	{
		for (; env[i]; i++)
		{
			data->env[i] = str_duplicate(env[i]);
		}
	}
	data->env[i] = NULL;
	env = data->env;

	data->alias_list = malloc(sizeof(char *) * 20);
	for (i = 0; i < 20; i++)
	{
		data->alias_list[i] = NULL;
	}
}
/**
 * run_shell - its a infinite loop that shows the prompt
 * @prompt: prompt to be printed
 * @data: its a infinite loop that shows the prompt
 * Return: Just Void
 */
void run_shell(char *prompt, data_of_program *data)
{
	int error_code = 0, string_len = 0;

	while (++(data->exec_counter))
	{
		_print(prompt);
		error_code = string_len = _getline(data);

		if (error_code == EOF)
		{
			free_all_data(data);
			exit(errno);
		}
		if (string_len >= 1)
		{
			expand_alias(data);
			expand_variables(data);
			tokenize(data);
			if (data->tokens[0])
			{ 
				error_code = execute(data);
				if (error_code != 0)
					_print_error(error_code, data);
			}
			free_recurrent_data(data);
		}
	}
}
