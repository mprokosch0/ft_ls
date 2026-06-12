#include "libft.h"

// verifie if a char is in a set of chars, presume the char set is null-terminated
int ft_is_in_set(char c, char *set)
{
	for (size_t i = 0; set[i]; i++)
	{
		if (c == set[i])
			return 1;
	}
	return 0;
}