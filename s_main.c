/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:33:17 by aklein            #+#    #+#             */
/*   Updated: 2024/04/19 02:14:49 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minitalk.h>

volatile sig_atomic_t	is_busy = 0;

static int	current_pid(int reset, int new_pid)
{
	static	int expected_pid = 0;
	if (reset)
		expected_pid = 0;
	if (new_pid)
		expected_pid = new_pid;
	return (expected_pid);
}

void	handle_sigusr(int sig, siginfo_t *siginfo, void *context)
{
	static char	c = 0;
	static int	i = 0;
	int			current_expected_pid;

	(void)context;
	if (current_pid(0, 0) == 0)
	{
		current_pid(0, siginfo->si_pid);
		c = 0;
		i = 0;
	}
	current_expected_pid = current_pid(0, 0);
	if (current_expected_pid != siginfo->si_pid)
		return ;
	is_busy++;
	if (sig == SIGUSR1)
		c = c | (1 << (7 - i));
	i++;
	if (i > 7)
	{
		ft_printf("%c", c);
		i = 0;
		c = 0;
	}
	kill(siginfo->si_pid, SIGUSR1);
}

#include <stdio.h>
#include <stdlib.h>
void save_pid(int pid)
{
	char *spid = ft_itoa(pid);
	unlink("spid");
	int fd = open("spid", O_WRONLY | O_CREAT, 0777);
	write(fd, spid, ft_strlen(spid));
	close(fd);
	free(spid);
	spid = NULL;
}

int	main()
{
	struct sigaction	action;
	sigset_t			set;
	int					pid;
	int					last_busy;

	pid = (int)getpid();
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	ft_bzero(&action, sizeof(action));
	action.sa_flags = SA_SIGINFO;
	action.sa_mask = set;
	action.sa_sigaction = &handle_sigusr;
	sigaction(SIGUSR1, &action, NULL);
	sigaction(SIGUSR2, &action, NULL);
	ft_printf("%d\n", pid);
	/*remove me*/
	save_pid(pid);
	/*---------*/
	last_busy = 0;
	while (42)
	{
		usleep(10000);
		if (is_busy == last_busy && last_busy != 0)
		{
			is_busy = 0;
			current_pid(1, 0);
			ft_printf("\n");
		}
		last_busy = is_busy;
	}
	return(0);
}
