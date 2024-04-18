/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:33:17 by aklein            #+#    #+#             */
/*   Updated: 2024/04/18 21:15:05 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minitalk.h>

static int	current_pid(int reset, int new_pid)
{
	static	int expected_pid = 0;
	if (reset)
		expected_pid = 0;
	if (new_pid != 0)
		expected_pid = new_pid;
	return (expected_pid);
}

static int	signal_flag(int reset, int updated)
{
	static int	last_sig = 1;
	if (updated)
		last_sig++;
	if (reset)
		last_sig = 1;
	return (last_sig);
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
	// ft_printf("signal '%d' from PID: %d\n", sig, siginfo->si_pid);
	signal_flag(0, 1);
	if (sig == SIGUSR1)
		c = c | (1 << (7 - i));
	i++;
	if (i > 7)
	{
		ft_printf("%c", c);
		i = 0;
		c = 0;
	}
	if (sig == SIGUSR1)
		kill(siginfo->si_pid, SIGUSR1);
	else
		kill(siginfo->si_pid, SIGUSR2);
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
	int					flag;

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
	flag = 1;
	while (42)
	{
		usleep(100000);
		// sleep(2);
		if (signal_flag(0, 0) == flag)
		{
			signal_flag(1, 0);
			current_pid(1, 0);
		}
		flag = signal_flag(0, 0);
	}
	return(0);
}
