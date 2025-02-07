/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahouass <ahouass@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:51:36 by ahouass           #+#    #+#             */
/*   Updated: 2025/02/07 11:26:24 by ahouass          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	ft_send_char(pid_t pid, char c)
{
	int		i;

	i = 7;
	while (i >= 0)
	{
		if ((c >> i) & 1)
		{
			if (kill(pid, SIGUSR2) == -1)
			{
				ft_putstr_fd("can not send signal\n", 1);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if (kill(pid, SIGUSR1) == -1)
			{
				ft_putstr_fd("can not send signal\n", 1);
				exit(EXIT_FAILURE);
			}
		}
		i--;
		usleep(100);
	}
}

int	main(int argc, char **argv)
{
	int		i;
	pid_t	pid;

	i = 0;
	if (argc != 3)
		exit(EXIT_FAILURE);
	pid = ft_atoi(argv[1]);
	if (pid <= 0)
		exit(EXIT_FAILURE);
	while (argv[2][i])
		ft_send_char(pid, argv[2][i++]);
	ft_send_char(pid, '\n');
	return (0);
}
