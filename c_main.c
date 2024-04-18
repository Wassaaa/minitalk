/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:33:25 by aklein            #+#    #+#             */
/*   Updated: 2024/04/18 21:52:46 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minitalk.h>

void	send_char(char c, int pid)
{
	int	i;

	i = 8;
	while (i--)
	{
		if (c >> i & 1)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		pause();
	}
}

void	print_bits(char c)
{
	int	i;

	i = 8;
	while (i--)
	{
		ft_printf("%d", c >> i & 1);
	}
	ft_printf(" = %d\n", c);
}

void	send_str(char *str, int pid)
{
	while (*str)
	{
		print_bits(*str);
		send_char(*str++, pid);
	}
	send_char('\n', pid);
}

void	acknowledge(int sig)
{
	(void)sig;
}

// void	acknowledge(int sig)
// {
// 	static char	c = 0;
// 	static int	i = 0;

// 	if (sig == SIGUSR1)
// 		c = c | (1 << (7 - i));
// 	i++;
// 	if (i > 7)
// 	{
// 		ft_printf("%c", c);
// 		i = 0;
// 		c = 0;
// 	}
// }

#include <stdlib.h>
#include <stdio.h>

int	server_pid()
{
	int fd = open("spid", O_RDONLY);
	char *spid = get_next_line(fd);
	int pid = ft_atoi(spid);
	close(fd);
	free(spid);
	spid = NULL;
	return (pid);
}

int	main(int argc, char **argv)
{
	int					pid;
	char				*str;
	struct sigaction	action;

	if (argc == 3)
	{
		int	my_pid;

		my_pid = (int)getpid();
		ft_bzero(&action, sizeof(action));
		action.sa_handler = acknowledge;
		sigaction(SIGUSR1, &action, NULL);
		sigaction(SIGUSR2, &action, NULL);
		pid = ft_atoi(argv[1]);
		/*remove me*/
		pid = server_pid();
		/*---------*/
		str = argv[2];
		send_str(str, pid);
	}
	return(0);
}