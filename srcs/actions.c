/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 18:27:46 by doleksiu          #+#    #+#             */
/*   Updated: 2026/04/07 14:14:57 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	philo_sleep(t_data *data, int philo_id)
{
	if (!check_stop(data))
		print_state(data, philo_id, "is sleeping");
	ft_sleep_ms(data->time_to_sleep);
	return (0);
}

int	lock_forks(t_data *data, t_philo *philo_array, int index)
{
	pthread_mutex_lock(philo_array[index].left_fork);
	if (!check_stop(data))
		print_state(data, index + 1, "has taken a fork");
	if (data->num_of_philos == 1)
	{
		ft_sleep_ms(data->time_to_die);
		pthread_mutex_unlock(philo_array[index].left_fork);
		return (1);
	}
	pthread_mutex_lock(philo_array[index].right_fork);
	if (!check_stop(data))
		print_state(data, index + 1, "has taken a fork");
	return (0);
}

int	philo_eat(t_data *data, t_philo *philo_array, int index)
{
	long long		time;

	if (lock_forks(data, philo_array, index))
		return (1);
	if (!check_stop(data))
	{
		pthread_mutex_lock(&philo_array[index].mutex_deathtime);
		time = get_current_time(data);
		philo_array[index].death_time = time + data->time_to_die;
		pthread_mutex_unlock(&philo_array[index].mutex_deathtime);
		print_state(data, index + 1, "is eating");
		ft_sleep_ms(data->time_to_eat);
	}
	pthread_mutex_unlock(philo_array[index].right_fork);
	pthread_mutex_unlock(philo_array[index].left_fork);
	if (check_stop(data))
		return (1);
	if (data->num_of_times_to_eat > 0)
	{
		pthread_mutex_lock(&data->mutex_stop);
		philo_array[index].eat_count++;
		pthread_mutex_unlock(&data->mutex_stop);
	}
	return (0);
}

int	philo_think(t_data *data, int philo_id)
{
	if (!check_stop(data))
		print_state(data, philo_id, "is thinking");
	if (data->num_of_philos % 2 != 0)
		ft_sleep_ms(1);
	return (0);
}
