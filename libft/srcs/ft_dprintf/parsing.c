#include "libft.h"

size_t parse_flag_type(const char *str, t_ft_dprintf *data, va_list args)
{
	char	*types = "csvpiduxX%";
	char	*flags = ".-*";
	size_t	count = 0;

	data->flags = 0;
	data->padding = 0;
	data->precision = 0;

	for (size_t i = 0; str[i]; i++)
	{
		if (ft_is_in_set(str[i], types))
		{
			switch (str[i])
			{
				case 'c':
					data->type = CHAR;
					break;
				
				case 's':
					if (str[i + 1] && str[i + 1] == 'v')
						data->type = STRING_VIEW;
					else
						data->type = STRING;
					break;
				
				case 'p':
					data->type = POINTER;
					break;
				
				case 'd':
					data->type = INT;
					break;
				
				case 'i':
					data->type = INT;
					break;

				case 'u':
					data->type = UNSIGNED_INT;
					break;
				
				case 'x':
					data->type = HEX_MIN;
					break;
				
				case 'X':
					data->type = HEX_MAJ;
					break;
				
				case '%':
					data->type = PERCENT;
					break;
			}
			count += (data->type == STRING_VIEW) ? 2 : 1;
			return count;
		}
		else if (ft_is_in_set(str[i], flags))
		{
			switch (str[i])
			{
				case '.':
					data->flags |= DOT_FLAG;
					if (str[i + 1] && ft_isdigit(str[i + 1]))
					{
						i++;
						data->precision = ft_atoi(&str[i]);
						while (str[i] && ft_isdigit(str[i]))
						{
							count++;
							i++;
						}
						i--;
					}
					break;
				
				case '-':
					data->flags |= MINUS_FLAG;
					break;
				
				case '*':
					if (data->padding)
						return (0);
					data->padding = (int)va_arg(args, int);
					break;
			}
			count++;
		}
		else if (ft_isdigit(str[i]))
		{
			if (data->padding)
				return (0);
			data->padding = ft_atoi(&str[i]);
			while (str[i] && ft_isdigit(str[i]))
			{
				count++;
				i++;
			}
			i--;
		}
	}
	return 0;
}