/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:56:34 by doleksiu          #+#    #+#             */
/*   Updated: 2026/04/11 11:34:46 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	main(int argc, char *argv[])
{
	t_data	data;
	t_philo	*philo_array;

	philo_array = NULL;
	if (init_program_data(argc, argv, &data, &philo_array) != 0)
	{
		if (philo_array)
			clean_exit(&data, philo_array);
		return (1);
	}
	run_simulation(&data, philo_array);
	clean_exit(&data, philo_array);
	return (0);
}

/* creates threads for each of the philos and monitoring, 
 and joins them when finished */

int	run_simulation(t_data *data, t_philo *philo_array)
{
	int		i;
	t_philo	*p;

	i = 0;
	while (i < data->num_of_philos)
	{
		p = &philo_array[i];
		if (pthread_create(&p->thread_id, NULL, &philo_routine, p) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&data->death_th, NULL,
			&monitor_routine, philo_array) != 0)
		return (1);
	i = 0;
	while (i < data->num_of_philos)
	{
		p = &philo_array[i];
		if (pthread_join(p->thread_id, NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_join(data->death_th, NULL) != 0)
		return (1);
	return (0);
}
