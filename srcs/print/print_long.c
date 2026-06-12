#include "ft_ls.h"

static size_t count_digits(int nb)
{
	size_t count = 0;
	if (!nb)
		return 1;
	while (nb)
	{
		nb /= 10;
		count++;
	}
	return count;
}

static void get_auth(unsigned int mode, char *buff)
{
	buff[0] = S_ISDIR(mode) ? 'd' : S_ISLNK(mode) ? 'l' : S_ISFIFO(mode) ? 'p': '-';
	buff[1] = (mode & S_IRUSR) ? 'r' : '-';
	buff[2] = (mode & S_IWUSR) ? 'w' : '-';
	buff[3] = (mode & S_IXUSR) ? 'x' : '-';
	buff[4] = (mode & S_IRGRP) ? 'r' : '-';
	buff[5] = (mode & S_IWGRP) ? 'w' : '-';
	buff[6] = (mode & S_IXGRP) ? 'x' : '-';
	buff[7] = (mode & S_IROTH) ? 'r' : '-';
	buff[8] = (mode & S_IWOTH) ? 'w' : '-';
	buff[9] = (mode & S_IXOTH) ? 'x' : '-';
}

static char get_ext_attr(char *path_name)
{
	char	buf[256];
	ssize_t len = lgetxattr(path_name, "security.selinux", buf, sizeof(buf));
	if (len < 0)
		return 0;
	if (ft_strncmp(buf, "system_u", 8) == 0)
		return 0;
	len = lgetxattr(path_name, "system.posix_acl_access", NULL, 0);
	if (len < 0)
		return '.';
	return '+';
}

static char *get_uid(t_data *data, unsigned int uid)
{
	char *res = id_cache_get_str_by_id(data->id_cache, uid);
	if (!res)
	{
		struct passwd *psw = getpwuid(uid);
		if (!psw)
		{
			// ft_dprintf(2, "ft_ls: Error: Function getpwuid failed\n");
			data->return_value = 1;
			return NULL;
		}
		id_cache_add(&data->id_cache, id_cache_create(uid, psw->pw_name));
		return id_cache_get_str_by_id(data->id_cache, uid);
	}
	return (res);
}

static char *get_gid(t_data *data, unsigned int gid)
{
	char *res = id_cache_get_str_by_id(data->gid_cache, gid);
	if (!res)
	{
		struct group *grp = getgrgid(gid);
		if (!grp)
		{
			// ft_dprintf(2, "ft_ls: Error: Function getgrgid failed\n");
			data->return_value = 1;
			return NULL;
		}
		id_cache_add(&data->gid_cache, id_cache_create(gid, grp->gr_name));
		return id_cache_get_str_by_id(data->gid_cache, gid);
	}
	return res;
}

static String_view get_date(time_t file_time)
{
	time_t	tnow = time(NULL);
	struct tm *tm_file = localtime(&file_time);
	static char	date[13] = {0};
	bool	same_year = ((1970 + (tnow / 31557600)) == (1970 + (file_time / 31557600)));
	if (same_year)
	{
		strftime(date, sizeof(date), "%b %e %H:%M", tm_file);
		return sv_create(date);
	}
	strftime(date, sizeof(date), "%b %e  %Y", tm_file);
	return sv_create(date);
}

static void	fill_stats(t_data *data, t_entry_info **dir_tab, t_print_long *stats, size_t i, char *path, size_t *pad_uid, size_t *pad_gid, bool *is_acl)
{
	stats->nlinks = dir_tab[i]->entry_stats.st_nlink;
	if (IS_OPT_g(data->opts))
	{
		stats->uid = "";
		*pad_uid = 0;
	}
	else
	{
		stats->uid = get_uid(data, dir_tab[i]->entry_stats.st_uid);
		*pad_uid = ft_strlen(stats->uid) + 1;
	}
	if (IS_OPT_G(data->opts))
	{
		stats->gid = "";
		*pad_gid = 0;
	}
	else
	{
		stats->gid = get_gid(data, dir_tab[i]->entry_stats.st_gid);
		*pad_gid = ft_strlen(stats->gid) + 1;
	}
	stats->size = dir_tab[i]->entry_stats.st_size;
	stats->entry_name = dir_tab[i]->entry_name;
	get_auth(dir_tab[i]->entry_stats.st_mode, stats->auth);
	time_t date = (IS_OPT_u(data->opts)) ? dir_tab[i]->entry_stats.st_atim.tv_sec : dir_tab[i]->entry_stats.st_mtim.tv_sec;
	stats->date = get_date(date);
	stats->name_color = get_color(data, dir_tab[i]->entry_name, dir_tab[i]->entry_stats.st_mode, data->opts);
	stats->ext_attr = get_ext_attr(path);
	if (stats->ext_attr)
		*is_acl = true;
	else if (*is_acl)
		stats->ext_attr = ' ';
}

void	print_dir_long_format(t_data *data, t_entry_info **dir_tab, size_t length, char *path)
{
	size_t pad_nlink = 0, pad_size = 0, total_blocs = 0, pad_uid = 0, pad_gid = 0;
	bool	is_acl = false;
	for (size_t i = 0; i < length; i++)
	{
		if (dir_tab[i]->entry_name[0] == '.' && !IS_OPT_a(data->opts))
			continue;
		size_t count = count_digits(dir_tab[i]->entry_stats.st_nlink);
		if (count > pad_nlink)
			pad_nlink = count;
		count = count_digits(dir_tab[i]->entry_stats.st_size);
		if (count > pad_size)
			pad_size = count;
		total_blocs += dir_tab[i]->entry_stats.st_blocks;
	}

	if (!data->is_arg)
		ft_dprintf(1, "total %u\n", total_blocs / 2);

	for (size_t i = 0; i < length; i++)
	{
		t_print_long	stats = {.auth = {0}, .ext_attr = ' '};
		char full_path[PATH_MAX] = {0};
		ft_memmove(full_path, path, ft_strlen(path));
		ft_memmove(full_path + ft_strlen(path), dir_tab[i]->entry_name, ft_strlen(dir_tab[i]->entry_name));

		fill_stats(data, dir_tab, &stats, i, full_path, &pad_uid, &pad_gid, &is_acl);

		if (S_ISLNK(dir_tab[i]->entry_stats.st_mode))
		{
			char buffer[256] = {0};
			if (readlink(full_path, buffer, 256) < 0)
			{
				ft_dprintf(1, "%s%c %*u%*s%*s %*u %sv %s", stats.auth, stats.ext_attr, pad_nlink, stats.nlinks, pad_uid, stats.uid, pad_gid, stats.gid, pad_size, stats.size, stats.date, stats.entry_name);
				data->return_value = 1;
				continue ;
			}
			struct stat st;
			if (stat(full_path, &st) < 0)
			{
				if (!IS_OPT_color(data->opts))
					ft_dprintf(1, "%s%c %*u%*s%*s %*u %sv %s -> %s", stats.auth, stats.ext_attr, pad_nlink, stats.nlinks, pad_uid, stats.uid, pad_gid, stats.gid, pad_size, stats.size, stats.date, stats.entry_name, buffer);
				else
					ft_dprintf(1, "%s%c %*u%*s%*s %*u %sv \e[40;31;01m%s\e[0m -> \e[37;41;01m%s\e[0m", stats.auth, stats.ext_attr, pad_nlink, stats.nlinks, pad_uid, stats.uid, pad_gid, stats.gid, pad_size, stats.size, stats.date, stats.entry_name, buffer);
			}
			else
			{
				String_view sym_color = get_color(data, buffer, st.st_mode, data->opts);
				ft_dprintf(1, "%s%c %*u%*s%*s %*u %sv \e[%svm%s\e[0m -> \e[%svm%s\e[0m", stats.auth, stats.ext_attr, pad_nlink, stats.nlinks, pad_uid, stats.uid, pad_gid, stats.gid, pad_size, stats.size, stats.date, stats.name_color, stats.entry_name, sym_color, buffer);
			}
		}
		else
			ft_dprintf(1, "%s%c %*u%*s%*s %*u %sv \e[%svm%s\e[0m", stats.auth, stats.ext_attr, pad_nlink, stats.nlinks, pad_uid, stats.uid, pad_gid, stats.gid, pad_size, stats.size, stats.date, stats.name_color, stats.entry_name);
		if (i + 1 != length)
			ft_dprintf(1, "\n");
	}
}