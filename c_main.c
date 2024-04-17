/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:33:25 by aklein            #+#    #+#             */
/*   Updated: 2024/04/17 06:09:10 by aklein           ###   ########.fr       */
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

void	send_str(char *str, int pid)
{

	while (*str)
		send_char(*str++, pid);
	ft_printf("\n");
	
}

void	acknowledge(int sig)
{
	(void)sig;
}

#include <sys/time.h>

int	main(int argc, char **argv)
{
	int					pid;
	char				*str;
	struct sigaction	action;

	long				elapsed;
	struct timeval		start;
	struct timeval 		end;
	gettimeofday(&start, NULL);

	if (argc == 3)
	{
		ft_bzero(&action, sizeof(action));
		action.sa_handler = acknowledge;
		sigaction(SIGUSR1, &action, NULL);
		sigaction(SIGUSR2, &action, NULL);
		pid = ft_atoi(argv[1]);
		str = argv[2];
		send_str(str, pid);
	}
	gettimeofday(&end, NULL);
	elapsed = (end.tv_sec - start.tv_sec) * 1000L;
	elapsed += (end.tv_usec - start.tv_usec) / 1000;
	ft_printf("%dms\n", elapsed);
	return(0);
}