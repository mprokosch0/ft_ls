#ifndef FT_LS_H

# define FT_LS_H

# define _GNU_SOURCE

# include <stdint.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <pwd.h>
# include <grp.h>
# include <dirent.h>
# include <stdbool.h>
# include <time.h>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>
# include <sys/ioctl.h>
# include <sys/xattr.h>
# include "libft.h"

typedef enum opts
{
	OPT_a		= 0b00000000001,
	OPT_color	= 0b00000000010,
	OPT_d		= 0b00000000100,
	OPT_g		= 0b00000001000,
	OPT_G		= 0b00000010000,
	OPT_l		= 0b00000100000,
	OPT_r		= 0b00001000000,
	OPT_R		= 0b00010000000,
	OPT_t		= 0b00100000000,
	OPT_u		= 0b01000000000,
	OPT_U		= 0b10000000000
}	e_opts;

# define IS_OPT_a(val) ((val) & OPT_a)
# define IS_OPT_color(val) ((val) & OPT_color)
# define IS_OPT_d(val) ((val) & OPT_d)
# define IS_OPT_g(val) ((val) & OPT_g)
# define IS_OPT_G(val) ((val) & OPT_G)
# define IS_OPT_l(val) ((val) & OPT_l)
# define IS_OPT_r(val) ((val) & OPT_r)
# define IS_OPT_R(val) ((val) & OPT_R)
# define IS_OPT_t(val) ((val) & OPT_t)
# define IS_OPT_u(val) ((val) & OPT_u)
# define IS_OPT_U(val) ((val) & OPT_U)

# define GETDENTS_BUFFER_SIZE 2048

struct linux_dirent
{
	uint64_t		d_ino;
	int64_t			d_off;
	unsigned short	d_reclen;
	unsigned char	d_type;
	char			d_name[];
};

typedef struct s_id_cache
{
	unsigned int		id;
	struct s_id_cache	*next;
	char				*id_str;
}	t_id_cache;

typedef struct s_color_entry
{
	unsigned int			hash;
	char 					*key;
	char					*value;
	struct s_color_entry	*next;
}	t_color_entry;

# define HASH_SIZE 4096

typedef struct s_color_map
{
	t_color_entry *list[HASH_SIZE];
}	t_color_map;

typedef struct ft_ls
{
	uint32_t		opts;
	char			**files;
	char			**cur_dir_files;
	size_t			cur_dir_files_size;
	bool			help;
	size_t			num_entries;
	int				return_value;
	t_id_cache		*id_cache;
	t_id_cache		*gid_cache;
	t_color_map		env_colors;
	int				env_loaded;
	bool			is_arg;
}	t_data;

typedef struct entry_info
{
	char *entry_name;
	struct stat entry_stats;

}	t_entry_info;

typedef struct print_long
{
	String_view		date;
	String_view		name_color;
	char			auth[11];
	char			*uid;
	char			*gid;
	char			*entry_name;
	unsigned long	nlinks;
	unsigned long	size;
	char			ext_attr;
}	t_print_long;

// pars_args.c

void	parse_args(t_data *data);

// print.c

void	print_dir_long_format(t_data *data, t_entry_info **dir_tab, size_t length, char *path);
int		print_dir(t_data *data, t_entry_info **dir_tab, size_t length, char *path);

// color.c

String_view get_color(t_data *data, char *entry_name, unsigned int mode, uint32_t opts);

// exec_ls.c

void	exec_ls(t_data *data);

// utils.c

void 		swap_addr(t_entry_info **addr1, t_entry_info **addr2);
void		free_struct(t_entry_info **st, size_t len);
t_id_cache	*id_cache_create(unsigned int id, char *name);
void		id_cache_add(t_id_cache **head, t_id_cache *new);
char		*id_cache_get_str_by_id(t_id_cache *head, unsigned int id);
void		id_cache_free(t_id_cache *head);
void		free_map(t_color_map *map);
char		*color_map_get(t_color_map *map, char *key);
void		color_map_insert(t_color_map *map, char *key, char *value);

#endif