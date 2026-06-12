# Ft_ls - A UNIX/Linux command

---

## Description

**ft_ls** is a reimplementation of the ls command; command that list a directory

The project supports the following options:
  - -a, --all:       listing all the entries even the hidden ones
    
  - --color:         color the output
    
  - -d, --directory: list directories themselves, not their contents
    
  - -f:              same as -a -U
    
  - -g:              like -l, but doesn't list owners
    
  - -G, --no-group:  in a long listing format (e.g -l or -g), doesn't print the groups names
    
  - -l:              use a long listing format
    
  - -r, --reverse:   reverse order while sorting
    
  - -R, --recursive: list subdirectories recursively
    
  - -t:              sort by time, newest first
    
  - -u:              with -lt: sort by, and show, access time;<br>
                     with -l: show access time and sort by name;<br>
                     otherwise: sort by access time, newest first<br>

  - -U:              do not sort directory entries

---

## Compilation

```bash
git clone git@github.com:mprokosch0/ft_ls.git
cd ft_ls

make        # Compile the project
make clean  # Remove the object files
make fclean # Remove the executable + object files
make re     # Do fclean + rebuild the project
```
---

### Usage

```bash
./ft_ls [options] [files/folders]
```
can use the option --help to see more infos

```bash
./ft_ls --help
```
---

## Author

**Mprokosch0** — [GitHub](https://github.com/mprokosch0)

---

Built in C.
