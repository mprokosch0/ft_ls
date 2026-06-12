#include "ft_ls.h"

static int	does_fit_with_nb_cols(t_entry_info **dir_tab, size_t nb_rows, size_t nb_cols, size_t length, long terminal_width)
{
	int	*pad_tab = ft_calloc(nb_cols, sizeof(int));
	if (!pad_tab)
		return 2;
	for (size_t col = 0; col < nb_cols; col++)
	{
		for (size_t row = 0; row < nb_rows; row++)
		{
			size_t idx = row + col * nb_rows;
			if (idx >= length)
				continue;
			int val = ft_strlen(dir_tab[idx]->entry_name);
			if (pad_tab[col] < val)
				pad_tab[col] = val;
		}
	}
	for (size_t row = 0; row < nb_rows; row++)
	{
		long total_width = 0;
		for (size_t col = 0; col < nb_cols; col++)
		{
			size_t idx = row + col * nb_rows;
			if (idx >= length)
				continue;

			total_width += pad_tab[col] + 2;
		}
		if (total_width > terminal_width)
			return free(pad_tab), 0;
	}
	free(pad_tab);
	return 1;
}

static long	get_term_width(void)
{
	struct winsize ws;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	return ws.ws_col;
}

static long	get_longest_name(t_entry_info **dir_tab, size_t length)
{
	long res = 0;
	for (size_t i = 0; i < length; i++)
	{
		long val = ft_strlen(dir_tab[i]->entry_name);
		if (val > res)
			res = val;
	}
	return res;
}

static bool	get_tab_dim(t_entry_info **dir_tab, size_t *nb_cols, size_t *nb_rows, size_t length, long terminal_width)
{
	if (*nb_cols < 1)
	{
	    *nb_cols = 1;
		*nb_rows = length;
	}
	else
		*nb_rows = (length + *nb_cols - 1) / *nb_cols;
	while (1)
	{
		int val = does_fit_with_nb_cols(dir_tab, *nb_rows, *nb_cols, length, terminal_width);
		if (val == 2)
			return true;
		else if (val == 0)
			break ;
		(*nb_cols)++;
		*nb_rows = (length + *nb_cols - 1) / *nb_cols;
		if (*nb_cols >= length)
			break ;
	}
	(*nb_cols)--;
	if (*nb_cols < 1)
	{
	    *nb_cols = 1;
		*nb_rows = length;
	}
	else
		*nb_rows = (length + *nb_cols - 1) / *nb_cols;
	return false;
}

static int	print_normal(t_data *data, t_entry_info **dir_tab, size_t length, char *path)
{
	long terminal_width = get_term_width();
	long longuest_name = get_longest_name(dir_tab, length);
	
	size_t nb_cols = terminal_width / (longuest_name + 2);
	size_t nb_rows = 0;
	
	if (get_tab_dim(dir_tab, &nb_cols, &nb_rows, length, terminal_width))
	{
		ft_dprintf(2, "ft_ls: Error: malloc failed\n");
		data->return_value = 3;
		return true;
	}

	int	*pad_tab = ft_calloc(nb_cols, sizeof(int)); // a proteger
	if (!pad_tab)
	{
		ft_dprintf(2, "ft_ls: Error: malloc failed\n");
		data->return_value = 3;
		return true;
	}
	for (size_t col = 0; col < nb_cols; col++)
	{
		for (size_t row = 0; row < nb_rows; row++)
		{
			size_t idx = row + col * nb_rows;
			if (idx >= length)
				continue;
			int val = ft_strlen(dir_tab[idx]->entry_name);
			if (pad_tab[col] < val)
				pad_tab[col] = val;
		}
	}
	for (size_t row = 0; row < nb_rows; row++)
	{
		for (size_t col = 0; col < nb_cols; col++)
		{
			size_t idx = row + col * nb_rows;
			if (idx >= length)
				continue;
			String_view color;
			if (S_ISLNK(dir_tab[idx]->entry_stats.st_mode))
			{
				char full_path[PATH_MAX] = {0};
				ft_memmove(full_path, path, ft_strlen(path));
				ft_memmove(full_path + ft_strlen(path), dir_tab[idx]->entry_name, ft_strlen(dir_tab[idx]->entry_name));
				char buffer[256] = {0};
				if (readlink(full_path, buffer, 256) < 0)
				{
					data->return_value = 1;
					continue ;
				}
				struct stat st;
				if (stat(full_path, &st) < 0)
					color = sv_create("40;31;01");
				else
					color = get_color(data, dir_tab[idx]->entry_name,
										dir_tab[idx]->entry_stats.st_mode, data->opts);
			}
			else
				color = get_color(data, dir_tab[idx]->entry_name,
										dir_tab[idx]->entry_stats.st_mode, data->opts);
			if (col + 1 == nb_cols)
				ft_dprintf(1, "\e[%svm%-s\e[0m", color, dir_tab[idx]->entry_name);
			else
				ft_dprintf(1, "\e[%svm%-*s\e[0m  ", color, pad_tab[col], dir_tab[idx]->entry_name);
		}
		if (row + 1 != nb_rows)
			ft_dprintf(1, "\n");
	}
	free(pad_tab);
	return false;
}	

int	print_dir(t_data *data, t_entry_info **dir_tab, size_t length, char *path)
{
	if (IS_OPT_l(data->opts) || IS_OPT_g(data->opts))
	{
		print_dir_long_format(data, dir_tab, length, path);
		return false;
	}
	if (print_normal(data, dir_tab, length, path))
		return true;
	return false;
}