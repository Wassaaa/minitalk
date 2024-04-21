/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:33:17 by aklein            #+#    #+#             */
/*   Updated: 2024/04/21 22:10:39 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minitalk.h>

volatile sig_atomic_t	g_is_busy = 0;

void	print_char(char c, int current_pid)
	{
	if (c == 0)
	{
		kill(current_pid, SIGUSR2);
		ft_printf("\n");
		g_is_busy = 0;
		return ;
	}
	else
		ft_printf("%c", c);
}

void	handle_sigusr(int sig, siginfo_t *siginfo, void *context)
{
	static char	c = 0;
	static int	i = 0;

	(void)context;
	if (!g_is_busy)
	{
		c = 0;
		i = 0;
	}
	g_is_busy++;
	if (sig == SIGUSR1)
		c = c | (1 << (7 - i));
	i++;
	if (i > 7)
	{
		print_char(c, siginfo->si_pid);
		i = 0;
		c = 0;
	}
	usleep(20);
	kill(siginfo->si_pid, SIGUSR1);
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
		if (g_is_busy != 0 && g_is_busy == last_busy)
			g_is_busy = 0;
		last_busy = g_is_busy;
	}
	return (0);
}
