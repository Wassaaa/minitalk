/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:33:17 by aklein            #+#    #+#             */
/*   Updated: 2024/04/21 18:09:47 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minitalk.h>

volatile sig_atomic_t	g_is_busy = 0;

void	handle_sigusr(int sig, siginfo_t *siginfo, void *context)
{
	static char	c = 0;
	static int	i = 0;
	static int	current_expected_pid = 0;

	(void)context;
	if (!g_is_busy)
	{
		current_expected_pid = siginfo->si_pid;
		c = 0;
		i = 0;
	}
	if (current_expected_pid != siginfo->si_pid)
		return ;
	// ft_printf("(%d)", g_is_busy);
	g_is_busy++;
	if (sig == SIGUSR1)
		c = c | (1 << (7 - i));
	i++;
	if (i > 7)
	{
		if (c == 0)
		{
			kill(current_expected_pid, SIGUSR2);
			ft_printf("\n");
			g_is_busy = 0;
			return ;
		}
		else
			ft_printf("%c", c);
		i = 0;
		c = 0;
	}
	usleep(20);
	kill(current_expected_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	action;
	int					pid;
	int					last_busy;

	pid = (int)getpid();
	ft_bzero(&action, sizeof(action));
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = &handle_sigusr;
	sigaction(SIGUSR1, &action, NULL);
	sigaction(SIGUSR2, &action, NULL);
	ft_printf("%d\n", pid);
	last_busy = 0;
	while (42)
	{
		usleep(10000);
		if (g_is_busy == last_busy && g_is_busy != 0)
			g_is_busy = 0;
		last_busy = g_is_busy;
	}
	return (0);
}
