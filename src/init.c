/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradice <aradice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 22:36:33 by aradice           #+#    #+#             */
/*   Updated: 2023/01/17 01:11:47 by aradice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_table	*ft_table_init(t_data *data, t_arguments *args, int i)
{
	t_table	*first;
	t_table	*prev;
	t_table	*fork;
	t_table	*philosopher;

	first = ft_philo_or_fork_init(PHILO, data, *args, 0);
	fork = ft_philo_or_fork_init(FORK, data, *args, 0);
	first->next = fork;
	fork->prev = first;
	prev = fork;
	i = 1;
	while (i < args->nb_philos)
	{
		philosopher = ft_philo_or_fork_init(PHILO, data, *args, i);
		prev->next = philosopher;
		philosopher->prev = prev;
		fork = ft_philo_or_fork_init(FORK, data, *args, i);
		philosopher->next = fork;
		fork->prev = philosopher;
		prev = fork;
		i++;
	}
	prev->next = first;
	first->prev = prev;
	return (first);
}

t_table	*ft_philo_or_fork_init(t_type type, t_data *data,
	t_arguments args, int nb)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (NULL);
	if (type == FORK)
		pthread_mutex_init(&table->mutex, NULL);
	table->args = args;
	table->type = type;
	table->id = nb;
	table->time_last_meal = 0;
	table->nb_finished_meal = 0;
	table->data = data;
	table->next = NULL;
	table->prev = NULL;
	return (table);
}
