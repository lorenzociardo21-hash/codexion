/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 18:32:08 by lciardo           #+#    #+#             */
/*   Updated: 2026/06/10 19:02:50 by lciardo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// static void onecoder(t_coder *coder)
// {
//     while (1)
//     {
//         if (stop_simu(coder) == 1)
//             break;
//         pthread_mutex_lock(&coder->left_dongle->lock);
//         print_action(coder, "has taken a dongle");
//         if (coder->config->number_of_coders == 1)
//         {
//             ft_usleep(coder->config->time_to_burnout, coder);
//             pthread_mutex_unlock(&coder->left_dongle->lock);
//             break ;
//         }
//     }
// }

// static void routinee(t_coder *coder)
// {
//     size_t  time;

//     while (1)
//     {
//         if (stop_simu(coder) == 1)
//             break;
//         pthread_mutex_lock(&coder->left_dongle->lock);
//         time = get_time() - coder->left_dongle->last_release_time;
//         if (time < coder->config->dongle_cooldown)
//             ft_usleep((coder->config->dongle_cooldown - time), coder);
//         print_action(coder, "has taken a dongle");
//         pthread_mutex_lock(&coder->right_dongle->lock);
//         time = get_time() - coder->right_dongle->last_release_time;
//         if (time < coder->config->dongle_cooldown)
//             ft_usleep((coder->config->dongle_cooldown - time), coder);
//         print_action(coder, "has taken a dongle");
//         pthread_mutex_lock(&coder->config->stop_mutex);
//         coder->last_compile_start = get_time();
//         pthread_mutex_unlock(&coder->config->stop_mutex);
//         print_action(coder, "is compiling");
//         pthread_mutex_lock(&coder->config->stop_mutex);
//         coder->n_compile += 1;
//         pthread_mutex_unlock(&coder->config->stop_mutex);
//         ft_usleep(coder->config->time_to_compile, coder);
//         coder->left_dongle->last_release_time = get_time();
//         coder->right_dongle->last_release_time = get_time();
//         pthread_mutex_unlock(&coder->left_dongle->lock);
//         pthread_mutex_unlock(&coder->right_dongle->lock);
//         if (stop_simu(coder) == 1)
//             break;
//         print_action(coder, "is debugging");
//         ft_usleep(coder->config->time_to_debug, coder);
//         print_action(coder, "is refactoring");
//         ft_usleep(coder->config->time_to_refactor, coder);
//     }
// }

static void	onecoder(t_coder *coder)
{
	while (1)
	{
		if (stop_simu(coder) == 1)
			break ;
		take_dongle(coder, coder->left_dongle);
		print_action(coder, "has taken a dongle");
		if (coder->config->number_of_coders == 1)
		{
			ft_usleep(coder->config->time_to_burnout, coder);
			release_dongle(coder, coder->left_dongle);
			break ;
		}
	}
}

static void	grab_dongles(t_coder *coder)
{
	size_t	time;

	take_dongle(coder, coder->left_dongle);
	time = get_time() - coder->left_dongle->last_release_time;
	if (time < coder->config->dongle_cooldown)
		ft_usleep((coder->config->dongle_cooldown - time), coder);
	print_action(coder, "has taken a dongle");
	take_dongle(coder, coder->right_dongle);
	time = get_time() - coder->right_dongle->last_release_time;
	if (time < coder->config->dongle_cooldown)
		ft_usleep((coder->config->dongle_cooldown - time), coder);
	print_action(coder, "has taken a dongle");
}

static void	perform_compilation(t_coder *coder)
{
	pthread_mutex_lock(&coder->config->stop_mutex);
	coder->last_compile_start = get_time();
	pthread_mutex_unlock(&coder->config->stop_mutex);
	print_action(coder, "is compiling");
	pthread_mutex_lock(&coder->config->stop_mutex);
	coder->n_compile += 1;
	pthread_mutex_unlock(&coder->config->stop_mutex);
	ft_usleep(coder->config->time_to_compile, coder);
	coder->left_dongle->last_release_time = get_time();
	coder->right_dongle->last_release_time = get_time();
	release_dongle(coder, coder->left_dongle);
	release_dongle(coder, coder->right_dongle);
}

static void	routinee(t_coder *coder)
{
	while (1)
	{
		if (coder->id % 2 != 0)
			usleep(50);
		if (stop_simu(coder) == 1)
			break ;
		grab_dongles(coder);
		perform_compilation(coder);
		if (stop_simu(coder) == 1)
			break ;
		print_action(coder, "is debugging");
		ft_usleep(coder->config->time_to_debug, coder);
		print_action(coder, "is refactoring");
		ft_usleep(coder->config->time_to_refactor, coder);
	}
}

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->config->number_of_coders == 1)
	{
		onecoder(coder);
		return (NULL);
	}
	routinee(coder);
	return (NULL);
}
