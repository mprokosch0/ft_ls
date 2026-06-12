#include "libft.h"

void	flush_buffer(t_ft_dprintf *data)
{
	int val = write(data->fd, data->buff, data->size);
	if (val >= 0)
		data->len += val;
	ft_bzero(data->buff, data->size);
	data->size = 0;
}

void	add_padding(t_ft_dprintf *data, size_t len)
{
	size_t padding = (data->padding > (int)len) ? data->padding - len : 0;
	for (size_t i = 0; i < padding; i++)
		data->buff[data->size++] = ' ';
}

void	write_str_buffer(t_ft_dprintf *data, char *str)
{
	size_t	len = ft_strlen(str);

	if (IS_DOT_FLAG(data->flags) && data->type == STRING)
		len = ((int)len > data->precision) ? (size_t)data->precision : len;

	if (data->size + len >= PRINTF_BUFFER_SIZE)
		flush_buffer(data);
	
	ft_memcpy(&data->buff[data->size], str, len);

	data->size += len;
}

void	write_char_buffer(t_ft_dprintf *data, char c)
{
	if (data->size >= PRINTF_BUFFER_SIZE)
		flush_buffer(data);
	
	data->buff[data->size++] = c;
}

void	ft_putnbr_fd_printf(int n, t_ft_dprintf *data)
{
	int	a;

	a = 0;
	if (n == -2147483648)
	{
		write_str_buffer(data, "-2147483648");
		return ;
	}
	if (n < 0)
	{
		write_char_buffer(data, '-');
		n = -n;
	}
	if (n >= 10)
		ft_putnbr_fd_printf(n / 10, data);
	a = (n % 10) + '0';
	write_char_buffer(data, a);
}

void	ft_putnbr_unsigned_fd(unsigned int n, t_ft_dprintf *data)
{
	unsigned int	a;

	a = 0;
	if (n >= 10)
		ft_putnbr_unsigned_fd(n / 10, data);
	a = (n % 10) + '0';
	write_char_buffer(data, a);
}

void	convert_hex(int maj, unsigned int nb, t_ft_dprintf *data)
{
	char			*set1;
	char			*set2;
	unsigned long	nbr;

	nbr = nb;
	set1 = "0123456789abcdef";
	set2 = "0123456789ABCDEF";
	if (nbr >= 16)
		convert_hex(maj, nbr / 16, data);
	if (maj)
		write_char_buffer(data, set2[nbr % 16]);
	else
		write_char_buffer(data, set1[nbr % 16]);
}
