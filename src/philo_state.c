/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aradice <aradice@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 22:40:35 by aradice           #+#    #+#             */
/*   Updated: 2023/01/18 20:26:41 by aradice          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*ft_sleep_and_think(void *table)
{
	t_table	*temp;
	t_data	*data;
	int		bool_philo_dead;
	int		bool_all_ate;

	temp = (t_table *)table;
	data = temp->data;
	if (data->args->nb_philos == 1)
	{
		ft_one_philo_eat(temp);
		return (NULL);
	}
	if (temp->id % 2)
		usleep(15000);
	bool_philo_dead = ft_lock_dead(data, bool_philo_dead);
	while (!(bool_philo_dead))
	{
		ft_left_or_right(temp);
		bool_all_ate = ft_lock_all_ate(data, bool_all_ate);
		if (data->bool_all_ate)
			break ;
		ft_write_sleep_think(data, temp);
		bool_philo_dead = ft_lock_dead(data, bool_philo_dead);
	}
	return (NULL);
}

void	ft_left_or_right(t_table *temp)
{
	if (temp->id % 2)
		ft_philo_eat_left(temp);
	else
		ft_philo_eat_right(temp);
}

void	ft_philo_eat_left(t_table *table)
{
	t_data	*data;
	t_table	*left_fork;
	t_table	*right_fork;

	data = table->data;
	left_fork = table->prev;
	right_fork = table->next;
	pthread_mutex_lock(&left_fork->mutex);
	ft_write_text(data, table->id, "has taken a fork");
	pthread_mutex_lock(&right_fork->mutex);
	ft_write_text(data, table->id, "has taken a fork");
	pthread_mutex_lock(&data->meal);
	ft_write_text(data, table->id, "is eating");
	table->time_last_meal = ft_convert_time();
	pthread_mutex_unlock(&data->meal);
	ft_wait_time(data, data->args->time_eat);
	pthread_mutex_lock(&data->meal);
	table->nb_finished_meal = table->nb_finished_meal + 1;
	pthread_mutex_unlock(&data->meal);
	pthread_mutex_unlock(&left_fork->mutex);
	pthread_mutex_unlock(&right_fork->mutex);
}

void	ft_philo_eat_right(t_table *table)
{
	t_data	*data;
	t_table	*left_fork;
	t_table	*right_fork;

	data = table->data;
	left_fork = table->prev;
	right_fork = table->next;
	pthread_mutex_lock(&right_fork->mutex);
	ft_write_text(data, table->id, "has taken a fork");
	pthread_mutex_lock(&left_fork->mutex);
	ft_write_text(data, table->id, "has taken a fork");
	pthread_mutex_lock(&data->meal);
	ft_write_text(data, table->id, "is eating");
	table->time_last_meal = ft_convert_time();
	pthread_mutex_unlock(&data->meal);
	ft_wait_time(data, data->args->time_eat);
	pthread_mutex_lock(&data->meal);
	table->nb_finished_meal = table->nb_finished_meal + 1;
	pthread_mutex_unlock(&data->meal);
	pthread_mutex_unlock(&right_fork->mutex);
	pthread_mutex_unlock(&left_fork->mutex);
}

void	ft_one_philo_eat(t_table *table)
{
	t_data	*data;
	t_table	*left_fork;

	data = table->data;
	left_fork = table->prev;
	pthread_mutex_lock(&left_fork->mutex);
	ft_write_text(data, table->id, "has taken a fork");
	pthread_mutex_unlock(&left_fork->mutex);
}
