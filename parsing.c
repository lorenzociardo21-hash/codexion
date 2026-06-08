/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 17:05:25 by lciardo           #+#    #+#             */
/*   Updated: 2026/06/08 19:25:15 by lciardo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_dongles	*createdongles(int num, t_config *node)
{
	t_dongles	*dongles;
	int				i;

	(void)node;
	dongles = malloc(sizeof(t_dongles) * num);
	if (!dongles)
	{
		free(node);
		ft_errorr(NULL, NULL);
	}
	i = 0;
	while (i < num)
	{
		dongles[i].queue = NULL;
		dongles[i].queue->id_order = 0;
		dongles[i].flag = 0;
		dongles[i].last_release_time = 0;
		pthread_mutex_init(&dongles[i].lock, NULL);
		pthread_cond_init(&dongles[i].wait_list, NULL);
		i++;
	}
	return (dongles);
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
				ft_errorr(NULL, NULL);
			y++;
		}
		if (y > 10)
			ft_errorr(NULL, NULL);
		else if (y == 10 && strcmp(av[x], "2147483647") > 0)
			ft_errorr(NULL, NULL);
		x++;
	}
}

t_config	*parsing(char **av)
{
	t_config	*node;

	parsnumber(av);
	node = malloc(sizeof(t_config));
	if (!node)
		ft_errorr(NULL, NULL);
	node->number_of_coders = atoi(av[1]);
	node->time_to_burnout = (size_t)atoi(av[2]);
	node->time_to_compile = atoi(av[3]);
	node->time_to_debug = atoi(av[4]);
	node->time_to_refactor = atoi(av[5]);
	node->number_of_compiles_required = atoi(av[6]);
	node->dongle_cooldown = (size_t)atoi(av[7]);
	if (strcmp(av[8], "fifo") == 0)
		node->scheduler = 0;
	else if (strcmp(av[8], "edf") == 0)
		node->scheduler = 1;
	else
	{
		free(node);
		ft_errorr(NULL, NULL);
	}
	node->dongles = createdongles(atoi(av[1]), node);
	node->start_time = get_time();
	node->stop_sim = 0;
	pthread_mutex_init(&node->log_mutex, NULL);
	pthread_mutex_init(&node->stop_mutex, NULL);
	return (node);
}
