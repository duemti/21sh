/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tty.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetrov <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/05 11:32:09 by dpetrov           #+#    #+#             */
/*   Updated: 2017/12/07 18:01:46 by dpetrov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"

void	find(void)
{
	char	*path;

	printf("{%d}", ttyslot());
	if (isatty(STDIN))
	{
		if ((path = ttyname(STDIN)) == NULL)
			fatal("tty:", "Could not find terminal device.\n");
	}
	else
		fatal("tty:", "Not on a terminal\n");
	ft_putstr(path);
}

void				tty_enable_raw()
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, &raw);
	raw.c_iflag &= ~(ICRNL | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void				tty_disable_raw()
{
	struct termios	cooked;

	tcgetattr(STDIN_FILENO, &cooked);
	cooked.c_iflag |= ICRNL | IXON;
	cooked.c_oflag |= OPOST;
	cooked.c_lflag |= ECHO | ICANON | IEXTEN | ISIG;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &cooked);
}