/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 20:07:41 by doleksiu          #+#    #+#             */
/*   Updated: 2026/06/29 17:37:27 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	lock_forks(t_data *data, t_philo *philo_array, int index)
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

static int	philo_eat(t_data *data, t_philo *philo_array, int index)
{
	long long		time;

	if (lock_forks(data, philo_array, index) != 0)
		return (1);
	if (!check_stop(data))
	{
		pthread_mutex_lock(&philo_array[index].mutex_deathtime);
		time = get_current_time(data);
		philo_array[index].deathtime = time + data->time_to_die;
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
		pthread_mutex_lock(&philo_array[index].mutex_deathtime);
		philo_array[index].eat_count++;
		pthread_mutex_unlock(&philo_array[index].mutex_deathtime);
	}
	return (0);
}

static int	philo_sleep(t_data *data, int philo_id)
{
	if (!check_stop(data))
		print_state(data, philo_id, "is sleeping");
	ft_sleep_ms(data->time_to_sleep);
	return (0);
}

/* if uneven num of philos, sleep for 1ms 
to avoid philosophers taking knives in the same second */

static int	philo_think(t_data *data, int philo_id)
{
	if (!check_stop(data))
		print_state(data, philo_id, "is thinking");
	if (data->num_of_philos % 2 != 0)
		ft_sleep_ms(1);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->philo_id % 2 == 0)
		ft_sleep_ms(data->time_to_eat / 2);
	while (1)
	{
		if (philo_eat(data, data->philo_array, philo->philo_id - 1) != 0)
			break ;
		if (philo_sleep(data, philo->philo_id) != 0)
			break ;
		if (philo_think(data, philo->philo_id) != 0)
			break ;
	}
	return (NULL);
}
