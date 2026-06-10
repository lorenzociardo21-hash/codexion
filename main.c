/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 17:05:30 by lciardo           #+#    #+#             */
/*   Updated: 2026/06/10 19:10:21 by lciardo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char *argv[])
{
	t_config	*config;
	t_coder		*coders;

	if (argc != 9)
		ft_errorr(NULL, NULL);
	config = parsing(argv);
	coders = codercreate(config);
	docodexion(coders);
	ft_cleanup(config, coders);
	return (0);
}
