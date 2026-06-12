/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:49:49 by mprokosc          #+#    #+#             */
/*   Updated: 2026/06/07 16:38:47 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

ssize_t	ft_putnbr_fd(int n, int fd)
{
	int		a;
	ssize_t	val;

	if (n == -2147483648)
		return write(fd, "-2147483648", 11);
	if (n < 0)
	{
		val = write(fd, "-", 1);
		n = -n;
	}
	if (n >= 10)
		ft_putnbr_fd(n / 10, fd);
	a = (n % 10) + '0';
	val = write(fd, &a, 1);
	return val;
}
