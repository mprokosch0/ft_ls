/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprokosc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:20:32 by mprokosc          #+#    #+#             */
/*   Updated: 2024/11/18 12:27:26 by mprokosc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdarg.h>
# include <stdint.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

float	ft_minf(float a, float b);
void	ft_freee(void **ptr, size_t size);
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isspace(int c);
int		ft_isprint(int c);
int		ft_len_int(int nb);
int		ft_strcmp(char *s1, char *s2);
size_t	ft_strlen(char const *str);
void	*ft_memset(void *s, int c, size_t n);
void	ft_bzero(void *s, size_t n);
void	*ft_realloc(void *ptr, size_t size, size_t old_size);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dest, char *src, size_t size);
int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
int		ft_atoi(const char *nptr);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *src);

char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoin_free(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);
char	*ft_ftoa(float n, int precis);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
ssize_t	ft_putchar_fd(char c, int fd);
ssize_t	ft_putstr_fd(char *s, int fd);
ssize_t	ft_putendl_fd(char *s, int fd);
ssize_t	ft_putnbr_fd(int n, int fd);

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}			t_list;

char	*get_next_line(int fd);
t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

int		ft_is_in_set(char c, char *set);
double	ft_round(double nb);

// ft_dprintf

# define PRINTF_BUFFER_SIZE 4096

typedef struct ft_dprintf
{
	char	buff[PRINTF_BUFFER_SIZE];
	size_t	size;
	int		fd;
	int		len;
	uint8_t	flags;
	uint8_t	type;
	int		padding;
	int		precision;
}	t_ft_dprintf;

typedef enum printf_flags
{
	DOT_FLAG	= 0b00000001,
	MINUS_FLAG	= 0b00000010
}	e_printf_flags;

# define IS_DOT_FLAG(val) ((val) & DOT_FLAG)
# define IS_MINUS_FLAG(val) ((val) & MINUS_FLAG)

typedef enum printf_types
{
	CHAR,
	STRING,
	STRING_VIEW,
	POINTER,
	INT,
	UNSIGNED_INT,
	HEX_MIN,
	HEX_MAJ,
	PERCENT
}	e_printf_types;


int		ft_dprintf(int fd, const char *s, ...);
size_t	parse_flag_type(const char *str, t_ft_dprintf *data, va_list args);
void	flush_buffer(t_ft_dprintf *data);
void	write_str_buffer(t_ft_dprintf *data, char *str);
void	write_char_buffer(t_ft_dprintf *data, char c);
void	ft_putnbr_fd_printf(int n, t_ft_dprintf *data);
void	ft_putnbr_unsigned_fd(unsigned int n, t_ft_dprintf *data);
void	convert_hex(int maj, unsigned int nb, t_ft_dprintf *data);
void	add_padding(t_ft_dprintf *data, size_t len_str);

// string views

# define NPOS ((size_t)-1)

typedef struct String_view
{
	char	*str;
	char	*str2;
	size_t	size;
	size_t	size2;
	int		with_space;
}	String_view;


String_view	sv_create(char *str);
String_view	sv_create_len(char *str, size_t len);
void		sv_cut_left(String_view *sv, size_t n);
void		sv_cut_right(String_view *sv, size_t n);
void		sv_trim_left(String_view *sv);
void		sv_trim_right(String_view *sv);
void		sv_trim(String_view *sv);
void		sv_trim_left_set(String_view *sv, char *set);
void		sv_trim_right_set(String_view *sv, char *set);
void		sv_trim_set(String_view *sv, char *set);
String_view	sv_cut_delim_left(String_view sv, int delim, int which_part, int with_delim);
String_view	sv_cut_delim_right(String_view sv, int delim, int which_part, int with_delim);
String_view	sv_cut_pos(String_view sv, size_t pos, int which_part);
int			sv_strcmp(String_view s1, String_view s2);
String_view	sv_join(String_view s1, String_view s2, int with_space);
char		*sv_to_str(String_view sv);

__attribute__((unused))
static size_t sv_find1(String_view sv, const char *str)
{
	size_t str_len = ft_strlen(str);
	if (!str_len)
		return (0);

	size_t sv_total = sv.size + sv.size2 + (sv.with_space ? 1 : 0);

	for (size_t i = 0; i < sv_total; i++)
	{
		size_t j = 0;
		while (str[j] && i + j < sv_total)
		{
			char c;
			size_t pos = i + j;
			if (pos < sv.size)
				c = sv.str[pos];
			else if (sv.with_space && pos == sv.size)
				c = ' ';
			else
				c = sv.str2[pos - sv.size - (sv.with_space ? 1 : 0)];
			if (c != str[j])
				break ;
			j++;
		}
		if (j == str_len)
			return (i);
	}
	return (NPOS);
}

__attribute__((unused))
static size_t sv_find2(String_view sv, String_view sv2)
{
	size_t sv2_len = sv2.size + sv2.size2 + (sv2.with_space ? 1 : 0);
	if (!sv2_len)
		return (0);

	size_t sv_total = sv.size + sv.size2 + (sv.with_space ? 1 : 0);
	for (size_t i = 0; i < sv_total; i++)
	{
		size_t j = 0;
		while (j < sv2_len && i + j < sv_total)
		{
			char c, c2;
			size_t pos = i + j;
			if (pos < sv.size)
				c = sv.str[pos];
			else if (sv.with_space && pos == sv.size)
				c = ' ';
			else
				c = sv.str2[pos - sv.size - (sv.with_space ? 1 : 0)];

			if (j < sv2.size)
				c2 = sv2.str[j];
			else if (sv2.with_space && j == sv2.size)
				c2 = ' ';
			else
				c2 = sv2.str2[j - sv2.size - (sv2.with_space ? 1 : 0)];
			
			if (c != c2)
				break ;
			j++;
		}
		if (j == sv2_len)
			return (i);
	}
	return NPOS;
}

// search for a string in the string_view, if the string is found, it returns the pos of the first char, else it returns NPOS
# define sv_find(sv, str) _Generic((str),	\
	char *: sv_find1,						\
	const char *:  sv_find1,              	\
	String_view : sv_find2					\
	)(sv, str)



#endif

