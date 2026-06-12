#include "ft_ls.h"

static char *get_env_color(t_data *data, char *entry_name, unsigned int mode)
{
	if (S_ISDIR(mode))
		return color_map_get(&data->env_colors, "di");
	else if (S_ISLNK(mode))
		return color_map_get(&data->env_colors, "ln");
	else if (S_IXUSR & mode || S_IXGRP & mode || S_IXOTH & mode)
		return color_map_get(&data->env_colors, "ex");
	else if (S_ISBLK(mode) || S_ISCHR(mode))
		return color_map_get(&data->env_colors, "bd");
	else if (S_ISFIFO(mode))
		return color_map_get(&data->env_colors, "pi");
	else if (S_ISSOCK(mode))
		return color_map_get(&data->env_colors, "so");
	
	String_view file_ext = sv_create(entry_name);
	char *res = NULL;
	if (sv_find(file_ext, ".") != NPOS)
	{
		file_ext = sv_cut_delim_right(file_ext, '.', 1, 1);
		char *fext = sv_to_str(file_ext);
		res = color_map_get(&data->env_colors, fext);
		free(fext);
	}
	return res;
}

String_view get_color(t_data *data, char *entry_name, unsigned int mode, uint32_t opts)
{
	String_view res = sv_create("0");
	if (IS_OPT_color(opts) && !data->env_loaded)
	{
		if (S_ISDIR(mode))
			res = sv_join(res, sv_create("1;34"), 0);
		else if (S_ISLNK(mode))
			res = sv_join(res, sv_create("1;36"), 0);
		else if (S_IXUSR & mode || S_IXGRP & mode || S_IXOTH & mode)
			res = sv_join(res, sv_create("1;32"), 0);
		else if (S_ISBLK(mode) || S_ISCHR(mode))
			res = sv_join(res, sv_create("1;33"), 0);
		else if (S_ISFIFO(mode))
			res = sv_join(res, sv_create("33"), 0);
		else if (S_ISSOCK(mode))
			res = sv_join(res, sv_create("35"), 0);
		return res;
	}
	else if (IS_OPT_color(opts))
	{
		char *color = get_env_color(data, entry_name, mode);
		if (color)
			return sv_create(color);
	}
	return (sv_create("0"));
}