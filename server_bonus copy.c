/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahouass <ahouass@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 21:45:24 by ahouass           #+#    #+#             */
/*   Updated: 2025/02/10 22:39:45 by ahouass          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

pid_t	g_pid = 0;

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	static int		bit = 0;
	static char		c = 0;
	static int		len = 0;
	static int		byte_count = 0;
	static char		*buffer = NULL;
	static int		index = 0;

	(void)context;
	if (g_pid != 0 && g_pid != info->si_pid)
	{
    	bit = 0;
    	c = 0;
    	len = 0;
    	byte_count = 0;
    	index = 0;
    	if (buffer)
    	{
        	free(buffer);
        	buffer = NULL;
    	}
    	g_pid = info->si_pid;
	}
	if (g_pid == 0)
		g_pid = info->si_pid;
	if (signum == SIGUSR1)
		c = (c << 1);
	else if (signum == SIGUSR2)
		c = (c << 1) | 1;
	bit++;
	if (bit == 8)
	{
		bit = 0;
		if (byte_count < 4)
		{
			((char *)&len)[byte_count] = c;
			byte_count++;
			if (byte_count == 4)
			{
				if (len <= 0 || len > 100000)
				{
					g_pid = 0;
					return;
				}
				buffer = malloc(len + 1);
				if (!buffer)
					exit(EXIT_FAILURE);
				buffer[len] = '\0';
			}
		}
		else if (buffer)
		{
			buffer[index++] = c;
			if (index == len)
			{
				ft_putstr_fd(buffer, 1);
				write(1, "\n", 1);
				free(buffer);
				buffer = NULL;
				kill(info->si_pid, SIGUSR1);
    			g_pid = 0;
    			bit = 0;
    			c = 0;
    			len = 0;
    			byte_count = 0;
    			index = 0;
			}
		}
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
