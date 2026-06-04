/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 16:01:46 by lciardo           #+#    #+#             */
/*   Updated: 2026/06/04 19:01:33 by lciardo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	pthread_mutex_t *a = coder->left_dongle;
	printf("ciao io sono il coder numero %d\n", coder->id);
	printf("ho finito ciaooooo\n");
	return NULL;
}

void docodexion(t_coder *coders)
{
	int	numcode;
	int i;

	numcode = coders[0].config->number_of_coders;
	i = 0;
	while (i < numcode)
	{
		if (pthread_create(&coders[i].thread_id, NULL, &routine, &coders[i]) != 0)
				ft_errorr();
		i++;
	}
	i = 0;
	while (i < numcode)
	{
		pthread_join(coders[i].thread_id, NULL);
		i++;
	}
}

void	codercreatee(int numcode, t_config *config, t_coder *coders)
{
	int i;

	i = 0;
	while (i < numcode)
	{
		coders[i].config = config;
		coders[i].id = i + 1;
		if (i == 0)
		{
			coders[i].left_dongle =  &config->dongles[i];
			coders[i].right_dongle= &config->dongles[numcode - 1];
		}
		else
		{
			coders[i].left_dongle =  &config->dongles[i - 1];
			coders[i].right_dongle= &config->dongles[i];
		}
		i++;
	}
}

t_coder	*codercreate(t_config *config)
{
	int	numcode;
	t_coder *coders;
	
	numcode = config->number_of_coders;
	coders = malloc(sizeof(t_coder) * numcode);
	if (!coders)
	{
		free(config->dongles);
		free(config);
		ft_errorr();
	}
	codercreatee(numcode, config, coders);
	return (coders);
}

int	main(int argc, char *argv[])
{

	t_config	*config;
	t_coder		*coders;

	if (argc != 9)
		ft_errorr();
	config = parsing(argv);
	coders = codercreate(config);
	docodexion(coders);
	return (0);
}
