/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 18:14:49 by doleksiu          #+#    #+#             */
/*   Updated: 2026/06/30 16:01:40 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	set_stop_flag(t_data *data)
{
	pthread_mutex_lock(&data->mutex_stop_sim);
	data->stop_simulation = 1;
	pthread_mutex_unlock(&data->mutex_stop_sim);
}

static int	check_if_dead(t_philo *philo_array, t_data *data, int i)
{
	long long		time;

	pthread_mutex_lock(&philo_array[i].mutex_deathtime);
	time = get_current_time(data);
	if (philo_array[i].deathtime <= time)
	{
		pthread_mutex_unlock(&philo_array[i].mutex_deathtime);
		set_stop_flag(data);
		return (1);
	}
	pthread_mutex_unlock(&philo_array[i].mutex_deathtime);
	return (0);
}

static int	check_if_full(t_philo *philo_array, t_data *data, int i)
{
	pthread_mutex_lock(&data->mutex_stop_sim);
	if (philo_array[i].eat_count >= data->num_of_times_to_eat)
	{
		data->finished_eating_count++;
		if (data->finished_eating_count == data->num_of_philos)
		{
			pthread_mutex_unlock(&data->mutex_stop_sim);
			set_stop_flag(data);
			return (1);
		}
	}
	pthread_mutex_unlock(&data->mutex_stop_sim);
	return (0);
}

/* check if someone is dead or everyone ate as many times as it's needed
 if the above conditions are met, it returns 1*/

static int	check_stop_conditions(t_data *data, t_philo *philo_array, int i)
{
	if (check_if_dead(philo_array, data, i) != 0)
	{
		print_state(data, philo_array[i].philo_id, "died");
		return (1);
	}
	if (data->num_of_times_to_eat > 0)
	{
		if (check_if_full(philo_array, data, i) != 0)
			return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	int		i;
	t_philo	*philo_array;
	t_data	*data;

	i = 0;
	philo_array = (t_philo *)arg;
	data = philo_array[0].data;
	while (1)
	{
		if (i == data->num_of_philos)
		{
			usleep(500);
			i = 0;
			pthread_mutex_lock(&data->mutex_stop_sim);
			data->finished_eating_count = 0;
			pthread_mutex_unlock(&data->mutex_stop_sim);
		}
		if (check_stop_conditions(data, philo_array, i))
			break ;
		i++;
	}
	return (NULL);
}
