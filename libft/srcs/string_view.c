#include "libft.h"

// create a string_viewer
String_view	sv_create(char *str)
{
	return (String_view){.str = str, .str2 = NULL, .size = ft_strlen(str), .size2 = 0, .with_space = 0};
}

// create a string_viewer with len
String_view	sv_create_len(char *str, size_t len)
{
	size_t len2 = ft_strlen(str);
	if (len > len2)
		return (String_view){.str = str, .str2 = NULL, .size = len2, .size2 = 0, .with_space = 0};
	return (String_view){.str = str, .str2 = NULL, .size = len, .size2 = 0, .with_space = 0};
}

// cut n chars at the left of the string
void	sv_cut_left(String_view *sv, size_t n)
{
	if (!(sv->size + sv->size2))
		return ;
	
	while (n && sv->size)
	{
		sv->str++;
		sv->size--;
		n--;
	}
	while (n && sv->size2)
	{
		sv->str2++;
		sv->size2--;
		n--;
	}
}

// cut n chars at the right of the string
void	sv_cut_right(String_view *sv, size_t n)
{
	if (!(sv->size + sv->size2))
		return ;

	while (n && sv->size)
	{
		sv->size--;
		n--;
	}
	while (n && sv->size2)
	{
		sv->size2--;
		n--;
	}
}

// trim all the spaces at the left of the string_view
void	sv_trim_left(String_view *sv)
{
	while ((sv->size && ft_isspace(sv->str[0])) || (sv->size2 && ft_isspace(sv->str2[0])))
		sv_cut_left(sv, 1);
}

// trim all the spaces at the right of the string_view
void	sv_trim_right(String_view *sv)
{
	while ((sv->size && ft_isspace(sv->str[sv->size - 1])) || (sv->size2 && ft_isspace(sv->str2[sv->size2 - 1])))
		sv_cut_right(sv, 1);
}

// trim all the set chars at the left of the string_view
void	sv_trim_left_set(String_view *sv, char *set)
{
	while ((sv->size && ft_is_in_set(sv->str[0], set)) || (sv->size2 && ft_is_in_set(sv->str2[0], set)))
		sv_cut_left(sv, 1);
}

// trim all the set chars at the right of the string_view
void	sv_trim_right_set(String_view *sv, char *set)
{
	while ((sv->size && ft_is_in_set(sv->str[sv->size - 1], set)) || (sv->size2 && ft_is_in_set(sv->str2[sv->size2 - 1], set)))
		sv_cut_right(sv, 1);
}

// trim all the spaces at the left and right of the string_view
void	sv_trim(String_view *sv)
{
	sv_trim_left(sv);
	sv_trim_right(sv);
}

// trim all the chars of the set at the left and right of the string_view
void	sv_trim_set(String_view *sv, char *set)
{
	sv_trim_left_set(sv, set);
	sv_trim_right_set(sv, set);
}

// split the string_view with the delim from left to right, and return (0) the left part and (1) the right part
String_view	sv_cut_delim_left(String_view sv, int delim, int which_part, int with_delim)
{
	size_t i = 0;
	int	which_str = 0;

	for (i = 0; i < sv.size; i++)
	{
		if (sv.str[i] == delim)
		{
			which_str = 1;
			break ;
		}
	}
	
	if (!which_str)
	{
		if (delim == ' ' && sv.with_space)
		{
			if (!which_part)
				return (String_view){.str = sv.str, .size = sv.size, .str2 = NULL, .size2 = 0, .with_space = (!with_delim) ? 0 : 1};
			return (String_view){.str = NULL, .size = 0, .str2 = sv.str2, .size2 = sv.size2, .with_space = (!with_delim) ? 0 : 1};	
		}
		for (i = 0; i < sv.size2; i++)
		{
			if (sv.str2[i] == delim)
			{
				which_str = 2;
				break ;
			}
		}
	}

	if (!which_str)
		return sv;
	
	if (which_str == 1)
	{
		if (!which_part && !with_delim)
			return (String_view){.str = sv.str, .size = i, .str2 = NULL, .size2 = 0, .with_space = 0};
		else if (!which_part)
			return (String_view){.str = sv.str, .size = i + 1, .str2 = NULL, .size2 = 0, .with_space = 0};
		else if (!with_delim)
			return (String_view){.str = sv.str + i + 1, .size = sv.size - i - 1, .str2 = sv.str2, .size2 = sv.size2, .with_space = sv.with_space};
		return (String_view){.str = sv.str + i, .size = sv.size - i, .str2 = sv.str2, .size2 = sv.size2, .with_space = sv.with_space};
	}
	if (!which_part && !with_delim)
		return (String_view){.str = sv.str, .size = sv.size, .str2 = sv.str2, .size2 = i, .with_space = sv.with_space};
	else if (!which_part)
		return (String_view){.str = sv.str, .size = sv.size, .str2 = sv.str2, .size2 = i + 1, .with_space = sv.with_space};
	else if (!with_delim)
		return (String_view){.str = NULL, .size = 0, .str2 = sv.str2 + i + 1, .size2 = sv.size2 - i - 1, .with_space = 0};
	return (String_view){.str = NULL, .size = 0, .str2 = sv.str2 + i, .size2 = sv.size2 - i, .with_space = 0};
}

// split the string_view with the delim from right to left, and return (0) the left part and (1) the right part
String_view	sv_cut_delim_right(String_view sv, int delim, int which_part, int with_delim)
{
	size_t i = 0;
	int	which_str = 0;

	for (i = sv.size2; i-- > 0;)
	{
		if (sv.str2[i] == delim)
		{
			which_str = 2;
			break ;
		}
	}

	if (!which_str)
	{
		if (delim == ' ' && sv.with_space)
		{
			if (!which_part)
				return (String_view){.str = sv.str, .size = sv.size, .str2 = NULL, .size2 = 0, .with_space = (!with_delim) ? 0 : 1};
			return (String_view){.str = NULL, .size = 0, .str2 = sv.str2, .size2 = sv.size2, .with_space = (!with_delim) ? 0 : 1};	
		}
		for (i = sv.size; i-- > 0;)
		{
			if (sv.str[i] == delim)
			{
				which_str = 1;
				break ;
			}
		}
	}

	if (!which_str)
		return sv;
	
	if (which_str == 1)
	{
		if (!which_part && !with_delim)
			return (String_view){.str = sv.str, .size = i, .str2 = NULL, .size2 = 0, .with_space = 0};
		else if (!which_part)
			return (String_view){.str = sv.str, .size = i + 1, .str2 = NULL, .size2 = 0, .with_space = 0};
		else if (!with_delim)
			return (String_view){.str = sv.str + i + 1, .size = sv.size - i - 1, .str2 = sv.str2, .size2 = sv.size2, .with_space = sv.with_space};
		return (String_view){.str = sv.str + i, .size = sv.size - i, .str2 = sv.str2, .size2 = sv.size2, .with_space = sv.with_space};
	}
	if (!which_part && !with_delim)
		return (String_view){.str = sv.str, .size = sv.size, .str2 = sv.str2, .size2 = i, .with_space = sv.with_space};
	else if (!which_part)
		return (String_view){.str = sv.str, .size = sv.size, .str2 = sv.str2, .size2 = i + 1, .with_space = sv.with_space};
	else if (!with_delim)
		return (String_view){.str = NULL, .size = 0, .str2 = sv.str2 + i + 1, .size2 = sv.size2 - i - 1, .with_space = 0};
	return (String_view){.str = NULL, .size = 0, .str2 = sv.str2 + i, .size2 = sv.size2 - i, .with_space = 0};
}

// split the string_view with the pos, and return (0) the left part and (1) the right part
String_view	sv_cut_pos(String_view sv, size_t pos, int which_part)
{
	if (pos >= sv.size + sv.size2)
		return sv;
	
	char *left_str = (!which_part) ? sv.str : (pos < sv.size) ? sv.str : NULL;
	size_t		left_size = (!which_part) ? ((pos < sv.size) ? pos : sv.size) : (pos < sv.size) ? pos : 0;
	char *right_str = (!which_part) ? ((pos < sv.size) ? NULL : sv.str2) : sv.str2;
	size_t		right_size = (!which_part) ? ((pos < sv.size) ? 0 : pos - sv.size) : (pos < sv.size) ? sv.size2 : sv.size2 - (pos - sv.size);
	int			is_space = (!which_part) ? (pos < sv.size) ? 0 : sv.with_space : (pos < sv.size) ? sv.with_space : 0;

	return (String_view){.str = left_str, .size = left_size, .str2 = right_str, .size2 = right_size, .with_space = is_space};
}

int sv_strcmp(String_view s1, String_view s2)
{
	size_t n = 0, ns1 = 0, ns2 = 0;
	if (!s1.str || !s2.str)
		return (-1);
	char  *s1_str = s1.str;
	char  *s2_str = s2.str;
	int s1_space = 0, s2_space = 0; // 1 = on est sur l'espace virtuel
	size_t s1_total = s1.size + s1.size2 + (s1.with_space ? 1 : 0);
	size_t s2_total = s2.size + s2.size2 + (s2.with_space ? 1 : 0);

	while (n < s1_total && n < s2_total)
	{
		char c1 = s1_space ? ' ' : s1_str[ns1];
		char c2 = s2_space ? ' ' : s2_str[ns2];
		if (c1 != c2)
			return ((unsigned char)c1 - (unsigned char)c2);
		n++; ns1++; ns2++;
		if (!s1_space && ns1 == s1.size && n < s1_total)
		{
			if (s1.with_space)
				s1_space = 1;
			else
			{
				s1_str = s1.str2;
				ns1 = 0;
			}
		}
		else if (s1_space)
		{
			s1_space = 0;
			s1_str = s1.str2;
			ns1 = 0;
		}
		if (!s2_space && ns2 == s2.size && n < s2_total)
		{
			if (s2.with_space)
				s2_space = 1;
			else
			{
				s2_str = s2.str2;
				ns2 = 0;
			}
		}
		else if (s2_space)
		{
			s2_space = 0;
			s2_str = s2.str2;
			ns2 = 0;
		}
	}
	if (n == s1_total && n == s2_total)
		return (0);
	if (n == s1_total)
		return (-(unsigned char)(s2_space ? ' ' : s2_str[ns2]));
	return ((unsigned char)(s1_space ? ' ' : s1_str[ns1]));
}

// join two strings views (only takes their str1 to join them), with_space to add a virtual space between the two strings
String_view	sv_join(String_view s1, String_view s2, int with_space)
{
	return (String_view){.str = s1.str, .size = s1.size, .str2 = s2.str, .size2 = s2.size, .with_space = with_space};
}

// converts a string view into a allocated string, which has to be freed, returns null if alloc failed or string view empty
char	*sv_to_str(String_view sv)
{
	size_t	size = sv.size + sv.size2 + 1;
	char	*res = NULL;

	if (size == 1)
		return res;
	
	res = ft_calloc(size, sizeof(char));
	if (!res)
		return res;

	ft_memcpy(res, sv.str, sv.size);
	ft_memcpy(res + sv.size, sv.str2, sv.size2);
	return res;
}