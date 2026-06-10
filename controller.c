/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 17:05:39 by lciardo           #+#    #+#             */
/*   Updated: 2026/06/10 19:24:57 by lciardo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	controlncompile(t_coder *coder)
{
	int		i;
	int		f;
	int		ncompile;

	f = 0;
	i = 0;
	ncompile = 0;
	while (i < coder->config->number_of_coders)
	{
		pthread_mutex_lock(&coder->config->stop_mutex);
		if (coder[i].n_compile >= \
			coder->config->number_of_compiles_required)
			ncompile += 1;
		pthread_mutex_unlock(&coder->config->stop_mutex);
		i++;
	}
	if (ncompile == coder->config->number_of_coders)
	{
		f = 1;
		pthread_mutex_lock(&coder->config->stop_mutex);
		coder[0].config->stop_sim = 1;
		pthread_mutex_unlock(&coder->config->stop_mutex);
	}
	return (f);
}

static int	controlburnout(t_coder *coder)
{
	int		i;
	int		f;
	size_t	diff;

	f = 0;
	i = 0;
	while (i < coder->config->number_of_coders)
	{
		pthread_mutex_lock(&coder->config->stop_mutex);
		diff = get_time() - coder[i].last_compile_start;
		if (diff >= coder[i].config->time_to_burnout)
		{
			pthread_mutex_unlock(&coder->config->stop_mutex);
			print_action(&coder[i], "burned out");
			pthread_mutex_lock(&coder->config->stop_mutex);
			coder[i].config->stop_sim = 1;
			pthread_mutex_unlock(&coder->config->stop_mutex);
			f = 1;
			break ;
		}
		pthread_mutex_unlock(&coder->config->stop_mutex);
		i++;
	}
	return (f);
}

static void	*controlcoders(void *arg)
{
	int		f;
	int		a;
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (1)
	{
		f = controlburnout(coder);
		a = controlncompile(coder);
		if (f == 1 || a == 1)
			break ;
		usleep(500);
	}
	return (NULL);
}

void	controller(t_coder *coder)
{
	int	i;

	if (pthread_create(&coder[0].config->controller, \
		NULL, &controlcoders, coder) != 0)
	{
		pthread_mutex_lock(&coder[0].config->stop_mutex);
		coder[0].config->stop_sim = 1;
		pthread_mutex_unlock(&coder[0].config->stop_mutex);
		i = 0;
		while (i < coder[0].config->number_of_coders)
		{
			pthread_join(coder[i].thread_id, NULL);
			i++;
		}
		ft_errorr(coder[0].config, coder);
	}
	pthread_join(coder[0].config->controller, NULL);
}
