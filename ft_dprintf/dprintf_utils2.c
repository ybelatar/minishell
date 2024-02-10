/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dprintf_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 03:36:51 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/10 06:41:45 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

void	ft_putunbr(unsigned int nb, t_print *print)
{
	if (nb >= 10)
		ft_putnbr(nb / 10, print);
	ft_putchar('0' + nb % 10, print);
}

void	ft_putnbrbase(size_t nb, t_print *print, const char *base,
		size_t base_len)
{
	if (nb >= base_len)
		ft_putnbrbase(nb / base_len, print, base, base_len);
	ft_putchar(base[nb % base_len], print);
}

void	ft_putaddr(void *addr, t_print *print)
{
	if (!addr)
		ft_putstr("(nil)", print);
	ft_putstr("0x", print);
	ft_putnbrbase((size_t)addr, print, "0123456789abcdef", 16);
}
