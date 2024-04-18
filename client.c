/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:33:25 by aklein            #+#    #+#             */
/*   Updated: 2024/04/19 02:49:15 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minitalk.h>

volatile sig_atomic_t	g_signal_recieved = 0;

void	send_char(char c, int pid)
{
	int	i;
	int	timeout;

	timeout = 0;
	i = 8;
	while (i--)
	{
		g_signal_recieved = 0;
		if (c >> i & 1)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		while (!g_signal_recieved)
		{
			usleep(100);
			timeout += 100;
			if (timeout > 1000000)
			{
				ft_printf("\nserver not responding, try again!\n");
				exit(1);
			}
		}
	}
}

void	acknowledge(int sig)
{
	if (sig == SIGUSR1)
		g_signal_recieved = 1;
}

#include <stdio.h>
#include <stdlib.h>

int	server_pid(void)
{
	int		fd;
	char	*spid;
	int		pid;

	fd = open("spid", O_RDONLY);
	spid = get_next_line(fd);
	pid = ft_atoi(spid);
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
		ft_bzero(&action, sizeof(action));
		action.sa_handler = acknowledge;
		sigaction(SIGUSR1, &action, NULL);
		sigaction(SIGUSR2, &action, NULL);
		pid = ft_atoi(argv[1]);
		/*remove me*/
		pid = server_pid();
		/*---------*/
		str = argv[2];
		while (*str)
			send_char(*str++, pid);
	}
	return (0);
}
