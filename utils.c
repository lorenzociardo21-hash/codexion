/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 18:31:57 by lciardo           #+#    #+#             */
/*   Updated: 2026/06/10 18:53:41 by lciardo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_cleanup(t_config *config, t_coder *coders)
{
	int	i;

	if (config)
	{
		if (config->dongles)
		{
			i = 0;
			while (i < config->number_of_coders)
			{
				pthread_mutex_destroy(&config->dongles[i].lock);
				i++;
			}
			free(config->dongles);
		}
		pthread_mutex_destroy(&config->log_mutex);
		pthread_mutex_destroy(&config->stop_mutex);
		free(config);
	}
	if (coders)
		free(coders);
}

void	ft_errorr(t_config *config, t_coder *coders)
{
	ft_cleanup(config, coders);
	write(2, "Error\n", 6);
	exit(1);
}

void	ft_usleep(size_t milliseconds, t_coder *coder)
{
	size_t	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
	{
		pthread_mutex_lock(&coder->config->stop_mutex);
		if (coder->config->stop_sim == 1)
		{
			pthread_mutex_unlock(&coder->config->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&coder->config->stop_mutex);
		usleep(500);
	}
}

size_t	get_time(void)
{
	struct timeval	time;
	size_t			start;

	gettimeofday(&time, NULL);
	start = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (start);
}

void	print_action(t_coder *coder, char *action)
{
	size_t	time;

	time = get_time() - coder->config->start_time;
	if (stop_simu(coder) == 1)
		return ;
	pthread_mutex_lock(&coder->config->log_mutex);
	if (strcmp(action, "is compiling") == 0)
		printf("\033[31m%zu %d %s\033[0m\n", time, coder->id, action);
	else
		printf("%zu %d %s\n", time, coder->id, action);
	pthread_mutex_unlock(&coder->config->log_mutex);
}
