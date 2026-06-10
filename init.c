/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 17:05:34 by lciardo           #+#    #+#             */
/*   Updated: 2026/06/10 16:43:07 by lciardo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	docodexion(t_coder *coders)
{
	int	numcode;
	int	i;
	int	j;

	numcode = coders[0].config->number_of_coders;
	i = 0;
	while (i < numcode)
	{
		coders[i].last_compile_start = get_time();
		if (pthread_create(&coders[i].thread_id, NULL, &routine, &coders[i]) != 0)
		{
			pthread_mutex_lock(&coders[0].config->stop_mutex);
			coders[0].config->stop_sim = 1;
			pthread_mutex_unlock(&coders[0].config->stop_mutex);
			j = 0;
			while (j < i)
			{
				pthread_join(coders[j].thread_id, NULL);
				j++;
			}
			ft_errorr(coders[0].config, coders);
		}
		i++;
	}
	i = 0;
	controller(coders);
	while (i < numcode)
	{
		pthread_join(coders[i].thread_id, NULL);
		i++;
	}
}

static void	codercreatee(int numcode, t_config *config, t_coder *coders)
{
	int	i;

	i = 0;
	while (i < numcode)
	{
		coders[i].n_compile = 0;
		coders[i].config = config;
		coders[i].id = i + 1;
		if (i == 0)
		{
			coders[i].left_dongle = &config->dongles[i];
			coders[i].right_dongle = &config->dongles[numcode - 1];
		}
		else
		{
			coders[i].left_dongle = &config->dongles[i - 1];
			coders[i].right_dongle = &config->dongles[i];
		}
		i++;
	}
}

t_coder	*codercreate(t_config *config)
{
	int		numcode;
	t_coder	*coders;

	numcode = config->number_of_coders;
	coders = malloc(sizeof(t_coder) * numcode);
	if (!coders)
		ft_errorr(config, NULL);
	codercreatee(numcode, config, coders);
	return (coders);
}
