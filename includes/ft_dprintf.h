/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 18:45:33 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/09 01:43:50 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DPRINTF_H

# define FT_DPRINTF_H

# include <stdarg.h>
# include <stdint.h>
# include <unistd.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef struct s_print
{
	char	buff[BUFFER_SIZE];
	int		fd;
	int		len;
	int		size;
}			t_print;

int			ft_printf(const char *str, ...);
int			ft_dprintf(int fd, const char *str, ...);

void		ft_flushbuff(t_print *print);
void		ft_putchar(const char c, t_print *print);
void		ft_putstr(const char *str, t_print *print);
void		ft_putnbr(int nb, t_print *print);
void		ft_putunbr(unsigned int nb, t_print *print);
void		ft_putnbrbase(size_t nb, t_print *print, const char *base,
				size_t base_len);
void		ft_putaddr(void *addr, t_print *print);

#endif