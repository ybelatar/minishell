/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dprintf_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 18:53:09 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/09 04:46:56 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

void	ft_flushbuff(t_print *print)
{
	if (!print->size)
		return ;
	print->len += write(print->fd, print->buff, print->size);
	print->size = 0;
}

void	ft_putchar(const char c, t_print *print)
{
	if (print->size + 1 == BUFFER_SIZE)
		ft_flushbuff(print);
	print->buff[print->size++] = c;
}

void	ft_putstr(const char *str, t_print *print)
{
	const char	*s;

	s = str;
	while (*s)
	{
		ft_putchar(*s, print);
		++s;
	}
}

void	ft_putnbr(int nb, t_print *print)
{
	unsigned int	n;
	unsigned int	sign;

	n = nb;
	sign = 1;
	if (nb < 0)
	{
		n = -nb;
		ft_putchar('-', print);
	}
	while (n >= 10)
	{
		ft_putchar('0' + n % 10, print);
		n = n / 10;
	}
	ft_putchar('0' + n % 10, print);
}
