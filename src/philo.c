/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradice <aradice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 02:16:50 by aradice           #+#    #+#             */
/*   Updated: 2023/01/18 20:07:02 by aradice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	main(int argc, char **argv)
{
	t_data		data;
	t_arguments	*args;
	t_table		*table;
	int			i;

	i = 0;
	ft_args_check(argc, argv);
	args = ft_args_parsing(&data, argc, argv);
	if (!args)
		return (1);
	table = ft_table_init(&data, args, i);
	if (!table)
	{
		free(args);
		return (1);
	}
	ft_process(&data, table);
	free(args);
	return (0);
}

void	ft_process(t_data *data, t_table *table)
{
	t_table	*temp;

	temp = table;
	data->start_time = ft_convert_time();
	while (temp)
	{
		if (temp->type == PHILO)
		{
			pthread_create(&temp->thread, NULL, ft_sleep_and_think, temp);
			pthread_mutex_lock(&data->meal);
			temp->time_last_meal = ft_convert_time();
			pthread_mutex_unlock(&data->meal);
		}
		temp = temp->next;
		if (temp == table)
			break ;
	}
	ft_process_2(data, table, temp);
}

void	ft_process_2(t_data *data, t_table *table, t_table *temp)
{
	ft_philo_die(data, table);
	temp = table;
	while (temp)
	{
		if (temp->type == PHILO)
			pthread_join(temp->thread, NULL);
		temp = temp->next;
		if (temp == table)
			break ;
	}
	ft_exit_program(data, table);
}
