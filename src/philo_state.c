/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradice <aradice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 22:40:35 by aradice           #+#    #+#             */
/*   Updated: 2023/01/16 00:43:33 by aradice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*ft_sleep_and_think(void *philosopher)
{
	t_table	*philo;
	t_data	*data;
	int		i;

	i = 0;
	philo = (t_table *)philosopher;
	data = philo->data;
	if (philo->id % 2)
		usleep(15000);
	while (!(data->philo_dead))
	{
		ft_philo_eat(philo);
		if (data->nb_each_ate)
			break ;
		ft_write_text(data, philo->id, "is sleeping");
		ft_check_time(data, data->args->time_sleep);
		ft_write_text(data, philo->id, "is thinking");
		i++;
	}
	return (NULL);
}

void	ft_philo_eat(t_table *philo)
{
	t_data	*data;
	t_table	*left_fork;
	t_table	*right_fork;

	data = philo->data;
	left_fork = philo->prev;
	right_fork = philo->next;
	pthread_mutex_lock(&left_fork->mutex);
	ft_write_text(data, philo->id, "has taken a fork");
	pthread_mutex_lock(&right_fork->mutex);
	ft_write_text(data, philo->id, "has taken a fork");
	pthread_mutex_lock(&data->meal);
	ft_write_text(data, philo->id, "is eating");
	philo->last_meal = ft_convert_time();
	ft_check_time(data, data->args->time_eat);
	philo->finished_meal = philo->finished_meal + 1;
	pthread_mutex_unlock(&data->meal);
	pthread_mutex_unlock(&right_fork->mutex);
	pthread_mutex_unlock(&left_fork->mutex);
}
