/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:33:17 by aklein            #+#    #+#             */
/*   Updated: 2024/04/21 16:56:13 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minitalk.h>

void	handle_sigusr(int sig, siginfo_t *siginfo, void *context)
{
	static char	c = 0;
	static int	i = 0;

	(void)(context);
	if (sig == SIGUSR1)
		c = c | (1 << (7 - i));
	i++;
	if (i > 7)
	{
		if (c == 0)
		{
			ft_printf("\n");
			i = 0;
			c = 0;
			kill(siginfo->si_pid, SIGUSR2);
			return ;
		}
		else
			ft_printf("%c", c);
		i = 0;
		c = 0;
	}
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
		pause();
	}
	return (0);
}
