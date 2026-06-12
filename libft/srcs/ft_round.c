#include "libft.h"

double	ft_round(double nb)
{
	double decimal = (nb - (int)nb);
	if ((decimal > 0 && decimal >= 0.5) || (decimal < 0 && decimal > -0.5))
		return (int)nb + 1;
	return (int)nb;
}