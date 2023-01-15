/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_die.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradice <aradice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 22:37:28 by aradice           #+#    #+#             */
/*   Updated: 2023/01/15 22:38:03 by aradice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	ft_philo_die(t_data *data, t_table *table)
{
	t_table	*temp;
	int		i;

	temp = table;
	while (!(data->nb_each_ate))
	{
		i = 0;
		while (temp && !(data->philo_dead))
		{
			if (temp->type == PHILO)
			{
				pthread_mutex_lock(&(data->meal));
				if (ft_philo_die_2(data, temp, i))
					return ;
				pthread_mutex_unlock(&(data->meal));
				usleep(100);
				if (temp == table)
					break ;
			}
		}
		ft_philo_die_3(data, table, temp, i);
	}
}

int	ft_philo_die_2(t_data *data, t_table *temp, int i)
{
	if ((ft_convert_time() - temp->last_meal)
		> data->args->time_die)
	{
		ft_write_text(data, i, "died");
		data->philo_dead = 1;
		pthread_mutex_unlock(&data->meal);
		return (1);
	}
	return (0);
}

void	ft_philo_die_3(t_data *data, t_table *table, t_table *temp, int i)
{
	temp = table;
	i = 0;
	while (temp && data->args->nb_times_philos_must_eat != -1
		&& temp->finished_meal >= data->args->nb_times_philos_must_eat - 1)
	{
		temp = temp->next;
		i++;
	}
	if (i == data->args->nb_philos)
		data->nb_each_ate = 1;
}
