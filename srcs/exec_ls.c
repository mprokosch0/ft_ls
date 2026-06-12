#include "ft_ls.h"

size_t partition(t_data *data, t_entry_info ***dir_tab, size_t low, size_t high)
{
	char	*pivot = (*dir_tab)[high]->entry_name;
	size_t	i = low - 1;

	for (size_t j = low; j <= high - 1; j++)
	{
		if (IS_OPT_r(data->opts))
		{
			if (ft_strcmp((*dir_tab)[j]->entry_name, pivot) > 0)
				swap_addr(&(*dir_tab)[++i], &(*dir_tab)[j]);
		}
		else if (ft_strcmp((*dir_tab)[j]->entry_name, pivot) < 0)
			swap_addr(&(*dir_tab)[++i], &(*dir_tab)[j]);
	}
	swap_addr(&(*dir_tab)[i + 1], &(*dir_tab)[high]);
	return i + 1;
}

size_t last_modif_partition(t_data *data, t_entry_info ***dir_tab, size_t low, size_t high)
{
	long pivot_sec = (IS_OPT_u(data->opts)) ? (*dir_tab)[high]->entry_stats.st_atim.tv_sec : (*dir_tab)[high]->entry_stats.st_mtim.tv_sec;
	long pivot_nsec = (IS_OPT_u(data->opts)) ? (*dir_tab)[high]->entry_stats.st_atim.tv_nsec : (*dir_tab)[high]->entry_stats.st_mtim.tv_nsec;
	size_t	i = low - 1;

	for (size_t j = low; j <= high - 1; j++)
	{
		long s1_sec = (IS_OPT_u(data->opts)) ? (*dir_tab)[j]->entry_stats.st_atim.tv_sec : (*dir_tab)[j]->entry_stats.st_mtim.tv_sec;
		long s1_nsec = (IS_OPT_u(data->opts)) ? (*dir_tab)[j]->entry_stats.st_atim.tv_nsec : (*dir_tab)[j]->entry_stats.st_mtim.tv_nsec;
		if (IS_OPT_r(data->opts))
		{
			if (s1_sec < pivot_sec)
				swap_addr(&(*dir_tab)[++i], &(*dir_tab)[j]);
			else if (s1_sec == pivot_sec)
			{
				if (s1_nsec < pivot_nsec)
					swap_addr(&(*dir_tab)[++i], &(*dir_tab)[j]);
				else if (s1_nsec == pivot_nsec)
				{
					if (ft_strcmp((*dir_tab)[j]->entry_name, (*dir_tab)[high]->entry_name) > 0)
						swap_addr(&(*dir_tab)[++i], &(*dir_tab)[j]);
				}
			}
		}
		else if (s1_sec > pivot_sec)
			swap_addr(&(*dir_tab)[++i], &(*dir_tab)[j]);
		else if (s1_sec == pivot_sec)
		{
			if (s1_nsec > pivot_nsec)
				swap_addr(&(*dir_tab)[++i], &(*dir_tab)[j]);
			else if (s1_nsec == pivot_nsec)
			{
				if (ft_strcmp((*dir_tab)[j]->entry_name, (*dir_tab)[high]->entry_name) < 0)
					swap_addr(&(*dir_tab)[++i], &(*dir_tab)[j]);
			}
		}
	}
	swap_addr(&(*dir_tab)[i + 1], &(*dir_tab)[high]);
	return i + 1;
}

static int	ascii_iter_quick_sort(t_data *data, t_entry_info **dir_tab, size_t low, size_t high)
{
	if (high < 1)
    	return false;

	size_t *stack = ft_calloc(high - low + 1, 2 * sizeof(size_t));
	if (!stack)
	{
		ft_dprintf(1, "ft_ls: Error: malloc failed\n");
		data->return_value = 3;
		return true;
	}
	int top = -1;
	stack[++top] = low;
	stack[++top] = high;
	while (top >= 0)
	{
		high = stack[top--];
		low = stack[top--];

		size_t p_index = 0;
		if (IS_OPT_t(data->opts) || (IS_OPT_u(data->opts) && IS_OPT_t(data->opts)) || (IS_OPT_u(data->opts) && !IS_OPT_l(data->opts)))
			p_index = last_modif_partition(data, &dir_tab, low, high);
		else
			p_index = partition(data, &dir_tab, low, high);

		if (p_index > 0 && p_index - 1 > low)
		{
			stack[++top] = low;
			stack[++top] = p_index - 1;
		}

		if (p_index + 1 < high)
		{
			stack[++top] = p_index + 1;
			stack[++top] = high;
		}
	}
	free(stack);
	return false;
}

static int first_checks(t_data *data, char *entry)
{
	struct stat st;
	bool	is_dir = false;
	
	if (lstat(entry, &st) < 0)
		data->return_value = 2;
	else
		is_dir = ((st.st_mode & S_IFMT) == S_IFDIR);
	if (!is_dir || IS_OPT_d(data->opts))
	{
		if (data->cur_dir_files)
		{
			t_entry_info **entry_infs = ft_calloc(data->cur_dir_files_size, sizeof(t_entry_info *));
			if (!entry_infs)
			{
				ft_dprintf(2, "ft_ls: Error: malloc failed\n");
				data->return_value = 3;
				return 1;
			}
			size_t j = 0;
			for (size_t i = 0; i < data->cur_dir_files_size; i++)
			{
				entry_infs[j] = ft_calloc(1, sizeof(t_entry_info));
				if (!entry_infs[j])
				{
					ft_dprintf(2, "ft_ls: Error: malloc failed\n");
					data->return_value = 3;
					free_struct(entry_infs, j);
					return 1;
				}
				if (lstat(data->cur_dir_files[i], &entry_infs[j]->entry_stats) < 0)
				{
					ft_dprintf(2, "ft_ls: cannot acess '%s': No such file or directory", data->cur_dir_files[i]);
					if (i + 1 != data->cur_dir_files_size)
						ft_dprintf(1, "\n");
					data->return_value = 2;
					free(entry_infs[j]);
					entry_infs[j] = NULL;
					continue ;
				}
				entry_infs[j]->entry_name = ft_strdup(data->cur_dir_files[i]);
				if (!entry_infs[j]->entry_name)
				{
					ft_dprintf(2, "ft_ls: Error: malloc failed\n");
					data->return_value = 3;
					free_struct(entry_infs, j);
					return 1;
				}
				j++;
			}
			print_dir(data, entry_infs, j, "");
			free_struct(entry_infs, data->cur_dir_files_size);
		}
		else
		{
			t_entry_info entry_inf = {.entry_name = entry, .entry_stats = st};
			t_entry_info *ptr = &entry_inf;
			print_dir(data, &ptr, 1, "");
			ft_dprintf(1, "\n");
		}
		return 1;
	}
	return 0;
}

void	extract_entry(t_data *data, char *entry);

static void	recursive_ls(t_data *data, t_entry_info **dir_tab, char *entry_path, size_t nb_entry)
{
	ft_dprintf(1, "\n");
	for (size_t i = 0; i < nb_entry; i++)
	{
		if ((dir_tab[i]->entry_stats.st_mode & S_IFMT) != S_IFDIR)
			continue ;
		if (ft_strcmp(dir_tab[i]->entry_name, ".")
			&& ft_strcmp(dir_tab[i]->entry_name, ".."))
		{
			char new_path[PATH_MAX] = {0};
			ft_memcpy(new_path, entry_path, ft_strlen(entry_path));
			ft_memcpy(new_path + ft_strlen(entry_path), dir_tab[i]->entry_name, ft_strlen(dir_tab[i]->entry_name));
			ft_dprintf(1, "\n%s:\n", new_path);
			extract_entry(data, new_path);
			if (data->return_value > 2)
				return ;
		}
	}
}


int	fill_dir_tab(t_data *data, t_entry_info ***dir_tab, char *entry_path, size_t *nb_entry)
{
	int		fd = open(entry_path, O_RDONLY | __O_DIRECTORY);
	char	buffer[GETDENTS_BUFFER_SIZE];
	if (fd < 0)
	{
		ft_dprintf(2, "ft_ls: cannot access '%s': No such file or directory\n", entry_path);
		data->return_value = 2;
		return true;
	}
	(*dir_tab) = ft_calloc((*nb_entry), sizeof(t_entry_info *));
	if (!(*dir_tab))
	{
		ft_dprintf(2, "ft_ls: Error: malloc failed\n");
		data->return_value = 3;
		close(fd);
		return true;
	}
	size_t i = 0;
	for (;;)
	{
		long nread = getdents64(fd, buffer, GETDENTS_BUFFER_SIZE);
		if (nread < 0)
		{
			ft_dprintf(2, "ft_ls: cannot open directory '%s': Permission denied\n", entry_path);
			close(fd);
			(*nb_entry) = i;
			data->return_value = 2;
			return true;
		}
		if (nread == 0)
			break ;
		
		for (size_t pos = 0; pos < (size_t)nread;)
		{
			struct linux_dirent *d = (struct linux_dirent *)(buffer + pos);
			if (d->d_name[0] == '.' && !IS_OPT_a(data->opts))
			{
				pos += d->d_reclen;
				continue;
			}
			if (i >= (*nb_entry))
			{
				t_entry_info **tmp = ft_realloc((*dir_tab), (*nb_entry) * 2 * sizeof(t_entry_info *), (*nb_entry) * sizeof(t_entry_info *));
				if (!tmp)
				{
					ft_dprintf(2, "ft_ls: Error: malloc failed\n");
					data->return_value = 3;
					close(fd);
					(*nb_entry) = i;
					return true;
				}
				(*dir_tab) = tmp;
				ft_bzero((*dir_tab) + (*nb_entry), (*nb_entry) * sizeof(t_entry_info *));
				(*nb_entry) *= 2;
			}
			(*dir_tab)[i] = ft_calloc(1, sizeof(t_entry_info));
			if (!(*dir_tab)[i])
			{
				ft_dprintf(2, "ft_ls: Error: malloc failed\n");
				data->return_value = 3;
				close(fd);
				(*nb_entry) = i;
				return true;
			}
			(*dir_tab)[i]->entry_name = ft_strdup(d->d_name);
			if (!(*dir_tab)[i]->entry_name)
			{
				ft_dprintf(2, "ft_ls: Error: malloc failed\n");
				data->return_value = 3;
				close(fd);
				(*nb_entry) = i;
				return true;
			}
			char member_path[PATH_MAX] = {0};
			ft_memcpy(member_path, entry_path, ft_strlen(entry_path));
			ft_memcpy(member_path + ft_strlen(entry_path), (*dir_tab)[i]->entry_name, ft_strlen((*dir_tab)[i]->entry_name));
			if (lstat(member_path, &(*dir_tab)[i]->entry_stats) < 0)
			{
				free((*dir_tab)[i]->entry_name);
				i--;
			}
			pos += d->d_reclen;
			i++;
		}
	}
	close(fd);
	if (i < (*nb_entry))
		(*nb_entry) = i;
	if (!i)
		return true;
	return false;
}

void	extract_entry(t_data *data, char *entry)
{
	if (first_checks(data, entry))
		return ;
	
	char			entry_path[PATH_MAX] = {0};
	size_t			nb_entry = 10;
	t_entry_info	**dir_tab = NULL;
	int				rt_val = 0;

	if (entry[ft_strlen(entry) - 1] != '/')
	{
		ft_memcpy(entry_path, entry, ft_strlen(entry));
		ft_memcpy(entry_path + ft_strlen(entry), "/", 1);
	}
	else
		ft_memcpy(entry_path, entry, ft_strlen(entry));
	
	DIR *dir = opendir(entry);
	if (dir)
	{
		rt_val = fill_dir_tab(data, &dir_tab, entry_path, &nb_entry);
		closedir(dir);
		if (data->return_value > 2 || rt_val)
			return (void)free_struct(dir_tab, nb_entry);

		
		if (!IS_OPT_U(data->opts))
			rt_val = ascii_iter_quick_sort(data, dir_tab, 0, nb_entry - 1);
	
		if (data->return_value > 2 || rt_val)
			return (void)free_struct(dir_tab, nb_entry);

		rt_val = print_dir(data, dir_tab, nb_entry, entry_path);
	
		if (data->return_value > 2 || rt_val)
			return (void)free_struct(dir_tab, nb_entry);

		if (IS_OPT_R(data->opts))
			recursive_ls(data, dir_tab, entry_path, nb_entry);
		else
			ft_dprintf(1, "\n");

		free_struct(dir_tab, nb_entry);
	}
	else
		ft_dprintf(2, "ft_ls: cannot open directory \'%s\', Permission denied\n", entry);
}

void	exec_ls(t_data *data)
{
	if (!(data->num_entries + data->cur_dir_files_size))
	{
		if (IS_OPT_d(data->opts))
			data->is_arg = true;
		extract_entry(data, ".");
		return ;
	}
	if (data->cur_dir_files_size)
	{
		data->is_arg = true;
		extract_entry(data, data->cur_dir_files[0]);
		data->is_arg = false;
		ft_dprintf(1, "\n");
		if (data->num_entries)
			ft_dprintf(1, "\n");
	}
	for (size_t i = 1; i < data->num_entries + 1; i++)
	{
		if (data->num_entries + data->cur_dir_files_size > 1)
			ft_dprintf(1, "%s:\n", data->files[i]);
		extract_entry(data, data->files[i]);
		if (data->return_value > 2)
			return ;
		if (data->num_entries > 1 && i < data->num_entries)
			ft_dprintf(1, "\n");
	}
}