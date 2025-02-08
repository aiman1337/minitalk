/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahouass <ahouass@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:30:52 by ahouass           #+#    #+#             */
/*   Updated: 2025/02/08 16:37:11 by ahouass          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

pid_t	g_pid = 0;

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	static int	bit;
	static char	c;

	(void)context;
	if (g_pid != info->si_pid)
	{
		c = 0;
		bit = 0;
		g_pid = info->si_pid;
	}
	if (signum == SIGUSR1)
		c = (c << 1);
	else if (signum == SIGUSR2)
		c = (c << 1) | 1;
	bit++;
	if (bit == 8)
	{
		write(1, &c, 1);
		if (c == 0)
			kill(g_pid, SIGUSR1);
		bit = 0;
		c = 0;
	}
}

int	main(void)
{
	pid_t				pid;
	struct sigaction	sa;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_handler;
	pid = getpid();
	ft_putnbr_fd(pid, 1);
	write(1, "\n", 1);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
		pause();
	return (0);
}
