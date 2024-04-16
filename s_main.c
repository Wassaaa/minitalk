/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:33:17 by aklein            #+#    #+#             */
/*   Updated: 2024/04/16 18:50:00 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minitalk.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("Caught signal %d\n", sig);
		// exit(1);
	}
}

void	handle_sigusr(int sig)
{

}

int	main()
{
	struct sigaction	action;
	sigset_t			signal_set;

	sigemptyset(&signal_set);
	sigaddset(&signal_set, SIGINT);
	ft_bzero(&action, sizeof(action));
	action.sa_handler = &handle_sigint;
	action.sa_mask = signal_set;
	sigaction(SIGINT, &action, NULL);
	while (42)
	{
		ft_printf("%d\n", getpid());
		sleep(1);
	}
	return(0);
}