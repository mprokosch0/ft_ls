#include "ft_ls.h"

int main(int ac, char **av)
{
	t_data data = {.opts = 0, .files = av, .help = false, .num_entries = 0, .return_value = 0,
					.id_cache = NULL, .gid_cache = NULL, .env_loaded = 0, .is_arg = 0};
	(void)ac;
	parse_args(&data);
	if (data.return_value >= 2)
		return data.return_value;

	if (data.help)
		return 0;
	
	exec_ls(&data);

	ft_freee((void **)data.cur_dir_files, data.cur_dir_files_size);
	id_cache_free(data.id_cache);
	id_cache_free(data.gid_cache);
	free_map(&data.env_colors);
	return data.return_value;
}