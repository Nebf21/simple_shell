#include "shell_header.h"

/**
 * _print - writes a array of chars in the standar output
 * @string: pointer to the array of chars
 * Return: the number of bytes writed or .
 * On error, -1 is returned, and errno is set appropriately.
 */
int _print(char *string)
{
	return (write(STDOUT_FILENO, string, str_length(string)));
}
/**
 * _printe - writes a array of chars in the standar error
 * @string: pointer to the array of chars
 * Return: the number of bytes writed or .
 * On error, -1 is returned, and errno is set appropriately.
 */
int _print_strerr(char *string)
{
	return (write(STDERR_FILENO, string, str_length(string)));
}

/**
 * _print_error - writes a array of chars in the standart error
 * @data: a pointer to the program's data'
 * @errorcode: error code to print
 * Return: the number of bytes writed or .
 * On error, -1 is returned, and errno is set appropriately.
 */
int _print_error(int errorcode, data_of_program *data)
{
	char n_as_string[10] = {'\0'};

	long_to_string((long) data->exec_counter, n_as_string, 10);

	if (errorcode == 2 || errorcode == 3)
	{
		_print_strerr(data->program_name);
		_print_strerr(": ");
		_print_strerr(n_as_string);
		_print_strerr(": ");
		_print_strerr(data->tokens[0]);
		if (errorcode == 2)
			_print_strerr(": Illegal number: ");
		else
			_print_strerr(": can't cd to ");
		_print_strerr(data->tokens[1]);
		_print_strerr("\n");
	}
	else if (errorcode == 127)
	{
		_print_strerr(data->program_name);
		_print_strerr(": ");
		_print_strerr(n_as_string);
		_print_strerr(": ");
		_print_strerr(data->command_name);
		_print_strerr(": not found\n");
	}
	else if (errorcode == 126)
	{
		_print_strerr(data->program_name);
		_print_strerr(": ");
		_print_strerr(n_as_string);
		_print_strerr(": ");
		_print_strerr(data->command_name);
		_print_strerr(": Permission denied\n");
	}
	return (0);
}
