/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:33:25 by aklein            #+#    #+#             */
/*   Updated: 2024/04/20 05:41:24 by aklein           ###   ########.fr       */
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

void	sending_finished(int sig)
{
	if (sig == SIGUSR2)
		ft_printf("Sending finished, recieved confirmation from server\n");
	exit(0);
}

int	validate_pid(char *pid)
{
	while (*pid)
	{
		if (!ft_isdigit(*pid))
			return (0);
		pid++;
	}
	return (1);
}

void	acknowledge(int sig)
{
	if (sig == SIGUSR1)
		g_signal_recieved = 1;
}

int	main(int argc, char **argv)
{
	int					pid;
	char				*str;
	struct sigaction	action;
	struct sigaction	action_end;

	if (argc == 3)
	{
		if (!validate_pid(argv[1]))
		{
			ft_printf("invalid PID\n");
			return (1);
		}
		ft_bzero(&action, sizeof(action));
		ft_bzero(&action_end, sizeof(action_end));
		action.sa_handler = acknowledge;
		action_end.sa_handler = sending_finished;
		sigaction(SIGUSR1, &action, NULL);
		sigaction(SIGUSR2, &action_end, NULL);
		pid = ft_atoi(argv[1]);
		str = argv[2];
		while (*str)
			send_char(*str++, pid);
		send_char(0, pid);
		sleep(1);
		return (1);
	}
}
