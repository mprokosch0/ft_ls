#include "libft.h"

static size_t count_digits(int nb)
{
	size_t count = 0;
	if (!nb)
		return 1;
	while (nb)
	{
		nb /= 10;
		count++;
	}
	return count;
}

static size_t count_digits_hex(unsigned int nb)
{
	size_t count = 0;
	if (!nb)
		return 1;
	while (nb)
	{
		nb /= 16;
		count++;
	}
	return count;
}

static void	ft_str(va_list args, t_ft_dprintf *data)
{
	char	*d;
	size_t	d_len;

	d = (char *)va_arg(args, char *);
	if (!d)
	{
		write_str_buffer(data, "(null)");
		return ;
	}
	d_len = ft_strlen(d);
	if (!IS_MINUS_FLAG(data->flags))
		add_padding(data, d_len);
	write_str_buffer(data, d);
	if (IS_MINUS_FLAG(data->flags))
		add_padding(data, d_len);
}

static void	ft_ptr(va_list args, t_ft_dprintf *data)
{
	void	*d;

	d = (void *)va_arg(args, void *);
	if (!(unsigned long)d)
	{
		write_str_buffer(data, "(nil)");
		return ;
	}
	size_t num_len = count_digits_hex((unsigned long)d);
	if (!IS_MINUS_FLAG(data->flags))
		add_padding(data, num_len + 2);
	write_str_buffer(data, "0x");
	convert_hex(0, (unsigned long)d, data);
	if (IS_MINUS_FLAG(data->flags))
		add_padding(data, num_len + 2);
}

static void	ft_hex(va_list args, t_ft_dprintf *data)
{
	int	nb;
	nb = (int)va_arg(args, int);
	size_t num_len = count_digits_hex(nb);

	if (!IS_MINUS_FLAG(data->flags))
		add_padding(data, num_len);
	if (data->type == HEX_MIN)
		convert_hex(0, nb, data);
	else
		convert_hex(1, nb, data);
	if (IS_MINUS_FLAG(data->flags))
		add_padding(data, num_len);
}

static void	ft_string_view(va_list args, t_ft_dprintf *data)
{
	String_view sv = (String_view)va_arg(args, String_view);
	size_t size = sv.size + sv.size2 + (sv.with_space) ? 1 : 0;

	if (IS_DOT_FLAG(data->flags) && data->type == STRING)
		size = ((int)sv.size + (int)sv.size2 > data->precision) ? (size_t)data->precision : sv.size + sv.size2;

	size_t	padding_diff = (data->padding > (int)size) ? data->padding - size : 0;
	
	if (data->size + size + padding_diff >= PRINTF_BUFFER_SIZE)
		flush_buffer(data);

	if (!IS_MINUS_FLAG(data->flags) && padding_diff)
		add_padding(data, padding_diff);

	ft_memcpy(&data->buff[data->size], sv.str, sv.size);
	data->size += sv.size;

	if (sv.with_space)
		data->buff[data->size++] = ' ';

	ft_memcpy(&data->buff[data->size], sv.str2, sv.size2);
	data->size += sv.size2;

	if (IS_MINUS_FLAG(data->flags) && padding_diff)
		add_padding(data, padding_diff);
}

static void	ft_int(int num, t_ft_dprintf *data)
{
	size_t num_len = count_digits(num);
	if (!IS_MINUS_FLAG(data->flags))
		add_padding(data, num_len);
	ft_putnbr_fd_printf(num, data);
	if (IS_MINUS_FLAG(data->flags))
		add_padding(data, num_len);
}

static void ft_uint(unsigned int num, t_ft_dprintf *data)
{
	size_t num_len = count_digits(num);
	if (!IS_MINUS_FLAG(data->flags))
		add_padding(data, num_len);
	ft_putnbr_unsigned_fd(num, data);
	if (IS_MINUS_FLAG(data->flags))
		add_padding(data, num_len);
}

static void	ft_char(char c, t_ft_dprintf *data)
{
	if (!IS_MINUS_FLAG(data->flags))
		add_padding(data, 1);
	write_char_buffer(data, c);
	if (IS_MINUS_FLAG(data->flags))
		add_padding(data, 1);
}

static void	m_args(va_list args, t_ft_dprintf *data)
{
	if (data->type == CHAR)
		ft_char((int)va_arg(args, int), data);
	else if (data->type == STRING)
		ft_str(args, data);
	else if (data->type == STRING_VIEW)
		ft_string_view(args, data);
	else if (data->type == POINTER)
		ft_ptr(args, data);
	else if (data->type == INT)
		ft_int((int)va_arg(args, int), data);
	else if (data->type == UNSIGNED_INT)
		ft_uint((unsigned int)va_arg(args, unsigned int), data);
	else if (data->type == HEX_MIN || data->type == HEX_MAJ)
		ft_hex(args, data);
	else if (data->type == PERCENT)
		write_char_buffer(data, '%');
}

int	ft_dprintf(int fd, const char *s, ...)
{
	va_list			args;
	t_ft_dprintf	data = {.buff = {0}, .size = 0, .fd = fd, .len = 0, .flags = 0, .type = 0, .padding = 0, .precision = 0};

	if (!s)
		return (-1);
	va_start(args, s);
	while (*s)
	{
		while (*s && *s != '%')
		{
			write_char_buffer(&data, *s);
			s++;
		}
		if (*s == '%')
		{
			s++;
			s += parse_flag_type(s, &data, args);
			if (data.padding < 0 || data.precision < 0)
				return (-1);
			m_args(args, &data);
		}
	}
	va_end(args);
	flush_buffer(&data);
	return (data.len);
}
