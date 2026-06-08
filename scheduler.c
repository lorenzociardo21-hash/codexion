/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lciardo <lciardo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 17:05:08 by lciardo           #+#    #+#             */
/*   Updated: 2026/06/08 19:28:56 by lciardo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void queuee(t_queue *q)
{
	t_queue	*new;

	new = malloc(sizeof(t_queue));
	new->id_order = q->id_order + 1;
	new->time = get_time();
	q->next = new;
	new->next = NULL;
}