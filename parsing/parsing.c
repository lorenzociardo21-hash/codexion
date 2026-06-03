/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 18:06:13 by lciardo           #+#    #+#             */
/*   Updated: 2026/06/03 18:30:58 by lciardo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_errorr(void)
{
	write (2, "Error\n", 6);
	exit(1);
}

static void	parsnumber(char **av)
{
	int	x;
	int	y;

	x = 1;
	while (x < 8)
	{
		y = 0;
		while (av[x][y])
		{
			if (av[x][y] < '0' || av[x][y] > '9')
				ft_errorr();
			y++;
		}
		if (y > 10)
			ft_errorr();
		else if (y == 10 && strcmp(av[x], "2147483647") > 0)
			ft_errorr();
		x++;
	}
}

t_config	*parsing(char **av)
{
	t_config	*node;

	parsnumber(av);
	node = malloc (sizeof(t_config));
	if (!node)
		ft_errorr();
	node->number_of_coders = atoi(av[1]);
	node->time_to_burnout = atoi(av[2]);
	node->time_to_compile = atoi(av[3]);
	node->time_to_debug = atoi(av[4]);
	node->time_to_refactor = atoi(av[5]);
	node->number_of_compiles_required = atoi(av[6]);
	node->dongle_cooldown = atoi(av[7]);
	if (strcmp(av[8], "fifo") == 0)
		node->scheduler = 0;
	else if (strcmp(av[8], "edf") == 0)
		node->scheduler = 1;
	else
	{
		free(node);
		ft_errorr();
	}
	return (node);
}
