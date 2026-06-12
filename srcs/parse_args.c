#include "ft_ls.h"

static void	print_help(void)
{
	ft_dprintf(1, "Usage: ft_ls [OPTION]... [FILE]...\nList information about the FILEs (the current directory by default).\n"
				"Sort entries alphabetically if none of -cftuvSUX nor --sort is specified.\n\n"
				"Mandatory arguments to long options are mandatory for short options too.\n"
				"  -a, --all\t\tdo not ignore entries starting with .\n"
				"      --color\t\tcolor the output\n"
				"  -d, --directory\tlist directories themselves, not their contents\n"
				"  -f\t\t\tsame as -a -U\n"
				"  -g\t\t\tlike -l, but do not list owner\n"
				"  -G, --no-group\tin a long listing, don't print group names\n"
				"  -l\t\t\tuse a long listing format\n"
				"  -r, --reverse\t\treverse order while sorting\n"
				"  -R, --recursive\tlist subdirectories recursively\n"
				"  -t\t\t\tsort by time, newest first\n"
				"  -u\t\t\twith -lt: sort by, and show, access time;\n"
				"    \t\t\twith -l: show access time and sort by name;\n"
				"    \t\t\totherwise: sort by access time, newest first\n"
				"  -U\t\t\tdo not sort directory entries\n\n"
				"Exit status:\n"
				" 0  if OK,\n"
				" 1  if minor problems (e.g., cannot access subdirectory),\n"
				" 2  if serious trouble (e.g., cannot access command-line argument),\n"
				" 3  if memory allocation error.\n");

}

static void	manage_opts_extended(t_data *data, char *opt)
{
	if (!ft_strcmp(opt, "--help"))
	{
		print_help();
		data->help = true;
	}
	else if (!ft_strcmp(opt, "--all"))
		data->opts |= OPT_a;
	else if (!ft_strcmp(opt, "--color"))
		data->opts |= OPT_color;
	else if (!ft_strcmp(opt, "--directory"))
		data->opts |= OPT_d;
	else if (!ft_strcmp(opt, "--no-group"))
		data->opts |= OPT_G;
	else if (!ft_strcmp(opt, "--reverse"))
		data->opts |= OPT_r;
	else if (!ft_strcmp(opt, "--recursive"))
		data->opts |= OPT_R;
	else
	{
		ft_dprintf(2, "ft_ls: unrecognized option -- \'%c\'\nTry 'ft_ls --help for more information.\n", opt);
		data->return_value = 2;
	}
}

static void	manage_opts(t_data *data, char opt)
{
	switch (opt)
	{
		case 'a':
			data->opts |= OPT_a;
			break;
		
		case 'd':
			data->opts |= OPT_d;
			break;

		case 'f':
			data->opts |= OPT_a | OPT_U;
			break;

		case 'g':
			data->opts |= OPT_g;
			break;

		case 'G':
			data->opts |= OPT_G;
			break;

		case 'l':
			data->opts |= OPT_l;
			break;
			
		case 'r':
			data->opts |= OPT_r;
			break;

		case 'R':
			data->opts |= OPT_R;
			break;
		
		case 't':
			data->opts |= OPT_t;
			break;
		
		case 'u':
			data->opts |= OPT_u;
			break;

		case 'U':
			data->opts |= OPT_U;
			break;

		default:
			ft_dprintf(2, "ft_ls: invalid option -- \'%c\'\nTry 'ft_ls --help for more information.\n", opt);
			data->return_value = 2;
			break;
	}
}

static bool is_sorted(t_data *data, size_t num_opts)
{
	for (size_t i = 1; i < data->num_entries + num_opts; i++)
	{
		if (!data->files[i] && data->files[i + 1])
			return false;
		if (IS_OPT_r(data->opts) && ft_strcmp(data->files[i], data->files[i + 1]) < 0)
			return false;
		else if (ft_strcmp(data->files[i], data->files[i + 1]) > 0)
			return false;
	}
	return true;
}

static void	swap_ascii(t_data *data, size_t j)
{
	if (IS_OPT_r(data->opts))
	{
		if (ft_strcmp(data->files[j], data->files[j + 1]) < 0)
		{
			char *tmp = data->files[j];
			data->files[j] = data->files[j + 1];
			data->files[j + 1] = tmp;
		}
	}
	else
	{
		if (ft_strcmp(data->files[j], data->files[j + 1]) > 0)
		{
			char *tmp = data->files[j];
			data->files[j] = data->files[j + 1];
			data->files[j + 1] = tmp;
		}
	}
}

static void	swap_last_modif(t_data *data, size_t j, struct stat st1, struct stat st2)
{
	long s1_sec = (IS_OPT_u(data->opts)) ? st1.st_atim.tv_sec : st1.st_mtim.tv_sec;
	long s2_sec = (IS_OPT_u(data->opts)) ? st2.st_atim.tv_sec : st2.st_mtim.tv_sec;
	long s1_nsec = (IS_OPT_u(data->opts)) ? st1.st_atim.tv_nsec : st1.st_mtim.tv_nsec;
	long s2_nsec = (IS_OPT_u(data->opts)) ? st2.st_atim.tv_nsec : st2.st_mtim.tv_nsec;
	if (IS_OPT_r(data->opts))
	{
		if (s1_sec >s2_sec)
		{
			char *tmp = data->files[j];
			data->files[j] = data->files[j + 1];
			data->files[j + 1] = tmp;
		}
		else if (s1_sec == s2_sec)
		{
			if (s1_nsec > s2_nsec)
			{
				char *tmp = data->files[j];
				data->files[j] = data->files[j + 1];
				data->files[j + 1] = tmp;
			}
			else if (s1_nsec == s2_nsec)
				swap_ascii(data, j);
		}
	}
	else
	{
		if (s1_sec <s2_sec)
		{
			char *tmp = data->files[j];
			data->files[j] = data->files[j + 1];
			data->files[j + 1] = tmp;
		}
		else if (s1_sec == s2_sec)
		{
			if (s1_nsec < s2_nsec)
			{
				char *tmp = data->files[j];
				data->files[j] = data->files[j + 1];
				data->files[j + 1] = tmp;
			}
			else if (s1_nsec == s2_nsec)
				swap_ascii(data, j);
		}
	}
}

static void	sort_args(t_data *data, size_t num_opts)
{
	for (size_t i = 1; i < data->num_entries + 1; i++)
	{
		for (size_t j = 1; j < data->num_entries; j++)
		{
			struct stat st1, st2;
			bool init1 = true, init2 = true;

			if (lstat(data->files[j], &st1) < 0)
				init1 = false;
			if (lstat(data->files[j + 1], &st2) < 0)
				init2 = false;
			if ((!init1 && !init2) || (!init1 && init2))
				continue ;
			else if (init1 && !init2)
			{
				char *tmp = data->files[j];
				data->files[j] = data->files[j + 1];
				data->files[j + 1] = tmp;
			}
			else if (IS_OPT_t(data->opts) || (IS_OPT_u(data->opts) && IS_OPT_t(data->opts)) || (IS_OPT_u(data->opts) && !IS_OPT_l(data->opts)))
				swap_last_modif(data, j, st1, st2);
			else
				swap_ascii(data, j);
		}
		if (is_sorted(data, num_opts))
			break ;
	}
}

static void	check_options(t_data *data, size_t *num_opts)
{
	for (size_t i = 1; data->files[i]; i++)
	{
		if (data->files[i][0] == '-' && data->files[i][1])
		{
			(*num_opts)++;
			if (data->files[i][1] && data->files[i][1] == '-')
				manage_opts_extended(data, data->files[i]);
			else
			{
				for (size_t j = 1; data->files[i][j]; j++)
				{
					manage_opts(data, data->files[i][j]);
					if (data->return_value == 2)
						return ;
				}
			}
			if (data->return_value == 2 || data->help)
				return ;
			data->files[i] = NULL;
		}
		else
			data->num_entries++;
	}
}

void	parse_env(t_data *data)
{
	char *env = getenv("LS_COLORS");
	if (env)
	{
		String_view	sv = sv_create(env), sv2 = sv_create("");
		size_t count = 0;
		while (sv_strcmp(sv2, sv))
		{
			sv2 = sv;
			sv = sv_cut_delim_left(sv, ':', 1, 0);
			count++;
		}
		if (count)
		{
			sv = sv_create(env);
			for (size_t i = 0; i < count - 1; i++)
			{
				sv2 = sv_cut_delim_left(sv, ':', 0, 0);
				sv = sv_cut_delim_left(sv, ':', 1, 0);
				String_view key = sv_cut_delim_left(sv2, '=', 0, 0);
				char *s_key = NULL;
				char * s_value = NULL;
				if (key.str[0] == '*')
					sv_cut_left(&key, 1);
				s_key = sv_to_str(key);
				s_value = sv_to_str(sv_cut_delim_left(sv2, '=', 1, 0));
				if (!s_key || !s_value)
				{
					ft_dprintf(2, "ft_ls: Error: malloc failed\n");
					data->return_value = 3;
					free_map(&data->env_colors);
					return ;
				}
				color_map_insert(&data->env_colors, s_key, s_value);
			}
		}
		data->env_loaded = 1;
	}
}

static void	push_args_to_end(t_data *data, size_t num_opts)
{
	for (size_t i = 1; i < data->num_entries + num_opts + 1; i++)
	{
		for (size_t j = 1; j < data->num_entries + num_opts; j++)
		{
			bool init1 = !data->files[j], init2 = !data->files[j + 1];

			if (init1 && !init2)
			{
				char *tmp = data->files[j];
				data->files[j] = data->files[j + 1];
				data->files[j + 1] = tmp;
			}
		}
	}
}

void	push_to_curr_dir_files(t_data *data)
{
	size_t count = 0;
	for (size_t i = 1; i < data->num_entries + 1; i++)
	{
		struct stat st;
		if (lstat(data->files[i], &st) >= 0 && (st.st_mode & S_IFMT) == S_IFDIR && !IS_OPT_d(data->opts))
			continue ;
		count++;
	}
	if (!count)
		return ;
	data->cur_dir_files_size = count;
	data->cur_dir_files = ft_calloc(count, sizeof(char *));
	if (!data->cur_dir_files)
	{
		ft_dprintf(2, "ft_ls: Error: malloc failed\n");
		data->return_value = 3;
		free_map(&data->env_colors);
		return ;
	}
	size_t j = 0;
	for (size_t i = 1; i < data->num_entries + 1 && j < count; i++)
	{
		struct stat st;
		if (lstat(data->files[i], &st) >= 0 && (st.st_mode & S_IFMT) == S_IFDIR && !IS_OPT_d(data->opts))
			continue ;

		data->cur_dir_files[j] = ft_strdup(data->files[i]);
		if (!data->cur_dir_files[j])
		{
			ft_dprintf(2, "ft_ls: Error: malloc failed\n");
			data->return_value = 3;
			free_map(&data->env_colors);
			ft_freee((void **)data->cur_dir_files, j);
		}
		j++;
		data->files[i] = NULL;
	}
	push_args_to_end(data, data->num_entries);
	data->num_entries -= count;
}

void	parse_args(t_data *data)
{
	size_t num_opts = 0;
	
	check_options(data, &num_opts);
	if (data->return_value || data->help)
		return ;

	push_args_to_end(data, num_opts);
	if (!IS_OPT_U(data->opts))
		sort_args(data, num_opts);
	push_to_curr_dir_files(data);
	if (data->return_value)
		return ;
	if (IS_OPT_color(data->opts))
		parse_env(data);
}