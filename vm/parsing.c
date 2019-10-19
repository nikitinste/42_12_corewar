#include "vm.h"

static void	check_magic_header(int fd)
{
	unsigned char	*rd_mag;
	unsigned char	*cnst_mag;
	unsigned int	cnst_int_mag;
	int				i;

	i = 0;
	if (!(rd_mag = (unsigned char *)ft_memalloc(sizeof(unsigned char) * 4)))
		ft_error("Malloc couldn't allocate the memory!");
	if ((read(fd, rd_mag, 4)) < 0)
		ft_error("There is nothing to read from champ file!");
	cnst_int_mag = COREWAR_EXEC_MAGIC;
	if (ft_islitendian())
		cnst_int_mag = ft_reverseint(cnst_int_mag);
	cnst_mag = (unsigned char *)&cnst_int_mag;
	while (i <= 3)
	{
		if (rd_mag[i] != cnst_mag[i])
			ft_error("Wrong magic header!");
		i++;
	}
	free(rd_mag);
}

static void	fill_the_name_champ(t_process *chmp, int fd)
{
	unsigned char	*champ_name;
	char			*name_null;

	if (!(champ_name = (unsigned char *)ft_memalloc(sizeof(unsigned char) *
			PROG_NAME_LENGTH)))
		ft_error("Malloc couldn't allocate the memory!");
	if (read(fd, champ_name, PROG_NAME_LENGTH) < 0)
		ft_error("There is no name in one of the champ files!");
	chmp->cmp_name = ft_strsub((char *)champ_name, 0,
			ft_strlen((char *)champ_name));
	ft_strdel((char **)&champ_name);
	if (!(champ_name = (unsigned char *)ft_memalloc(sizeof(unsigned char) * 4)))
		ft_error("Malloc couldn't allocate the memory!");
	if (read(fd, champ_name, 4) < 0)
		ft_error("There is no NULL after champ name in one of the champ "
"files!");
	name_null = ft_strsub((char *)champ_name, 0, 4);
	if (name_null != NULL)
		ft_error("There is no NULL after champ name in one of the champ "
"files!");
	ft_strdel(&name_null);
	ft_strdel((char **)&champ_name);
}

static void	fill_the_code_size(t_process *chmp, int fd)
{
	unsigned char	*rd_code_size;

	if (!(rd_code_size = (unsigned char *)ft_memalloc(sizeof(unsigned char) *
			4)))
		ft_error("Malloc couldn't allocate the memory!");
	if (read(fd, rd_code_size, 4) < 0)
		ft_error("There is nothing instead of the size of executable code!");
	chmp->code_size = ((size_t *)rd_code_size)[0];
	if (ft_islitendian())
		chmp->code_size = ft_reverseint(chmp->code_size);
	if (chmp->code_size > CHAMP_MAX_SIZE)
		ft_error("The champion's code size is bigger that CHAMP_MAX_SIZE");
	ft_strdel((char **)(&rd_code_size));
}

static void	fill_the_comment(t_process *chmp, int fd)
{
	unsigned char	*champ_comment;
	char			*comment_null;

	if (!(champ_comment = (unsigned char *)ft_memalloc(sizeof(unsigned char) *
													COMMENT_LENGTH)))
		ft_error("Malloc couldn't allocate the memory!");
	if (read(fd, champ_comment, COMMENT_LENGTH) < 0)
		ft_error("There is no comment in one of the champ files!");
	chmp->cmp_cmnt = ft_strsub((char *)champ_comment, 0,
			ft_strlen((char *)champ_comment));
	ft_strdel((char **)&champ_comment);
	if (!(champ_comment = (unsigned char *)ft_memalloc(sizeof(unsigned char) *
			4)))
		ft_error("Malloc couldn't allocate the memory!");
	if (read(fd, champ_comment, 4) < 0)
		ft_error("There is no NULL after champ comment in one of the champ "
"files!");
	comment_null = ft_strsub((char *)champ_comment, 0, 4);
	if (comment_null != NULL)
		ft_error("There is no NULL after champ comment in one of the champ "
"files!");
	ft_strdel(&comment_null);
	ft_strdel((char **)&champ_comment);
}

static void	fill_the_code(t_process *chmp, int fd)
{
	char	*test;
	ssize_t check_read;

	if (!(chmp->code = (unsigned char *)ft_memalloc(sizeof(unsigned char) *
			chmp->code_size)))
		ft_error("Malloc couldn't allocate the memory!");
	check_read = read(fd, chmp->code, chmp->code_size);
	if (check_read < 0)
		ft_error("There is no executable code to read for one of the "
"champs!");
	else if (check_read != chmp->code_size)
		ft_error("Executable code for one of the champions is smaller than "
		   "it should be! Bring me a worthy one!");
	test = ft_strnew(1);
	if (read(fd, test, 1) > 0)
		ft_error("There is something else, after executable code! "
"Please, get rid of this!");
	ft_strdel(&test);
}

static void	fill_the_champ(t_process *chmp, char *file_name)
{
//	char	*clion_file_name;
	int		fd;

	if ((fd = open(file_name, O_RDONLY)) < 0)	//for something real
//	clion_file_name = ft_strjoin("../", file_name); //only for clion
//	if ((fd = open(clion_file_name, O_RDONLY)) < 0) //only for clion
		ft_error("There is nothing to open from champ file!");
	check_magic_header(fd);
	fill_the_name_champ(chmp, fd);
	fill_the_code_size(chmp, fd);
	fill_the_comment(chmp, fd);
	fill_the_code(chmp, fd);

//	ft_strdel(&clion_file_name); //only fo clion
}

static void	check_num(t_process *champs, int num, char *champ_name)
{
	int i;

	i = 0;
	if (num > MAX_PLAYERS)
	{
		ft_printf("Wrong number for the champion %s! ", champ_name);
		ft_error("The number is too big!");
	}
	if (num < 1)
	{
		ft_printf("Wrong number for the champion %s! ", champ_name);
		ft_error("The number is too small!");
	}
	while (i < MAX_PLAYERS)
	{
		if (champs[i].cmp_nbr == num)
		{
			ft_printf("Wrong number for the champion %s! ", champ_name);
			ft_error("The number is a duplicate! "
			"Be more creative next time!");
		}
		i++;
	}
}

static void	check_file_type(char *file_name)
{
	int		i;
	char	*tmp;

	i = ft_strlen(file_name);
	tmp = ft_strsub(file_name, i - 4, 4);
	if (!ft_strequ(tmp, ".cor"))
		ft_error("Wrong type of the champion file!\n");
	ft_strdel(&tmp);
}

static void	check_file_and_fill(char *file_name, t_process *chmp, int num)
{
	int		i;

	check_file_type(file_name);
	i = 0;
	if (num != 0)
	{
		chmp[num - 1].cmp_nbr = num;
		fill_the_champ(&chmp[num - 1], file_name);
	}
	else
	{
		while (i < MAX_PLAYERS)
		{
			if (chmp[i].cmp_nbr == 0)
			{
				chmp[i].cmp_nbr = i + 1;
				fill_the_champ(&chmp[i], file_name);
				break ;
			}
			i++;
		}
		if (i == MAX_PLAYERS)
			ft_error("Too many champs!");
	}
}

static void	how_many_champs(t_vm *vm)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < MAX_PLAYERS)
	{
		if (vm->processes[i].cmp_nbr > 0)
			j++;
		i++;
	}
	vm->champs_count = j;
}

static void	parse_champ(int argc, char **argv, t_vm *vm, int l)
{
	long int	num;

	while (l < argc)
	{
		if (ft_strequ(argv[l], "-n"))
		{
			l++;
			check_num(vm->processes, num = ft_atoi(argv[l]), argv[l + 1]);
			l++;
			check_file_and_fill(argv[l], vm->processes, num);
		}
		else if (ft_strequ(argv[l], "-dump"))
		{
			l++;
			vm->dump_flag = 1;
			vm->dump = ft_atoi(argv[l]);
			if (vm->dump != (long long int)ft_latoi(argv[l]) || vm->dump < 0)
			{
//				ft_printf("Error! The number %d after flat -dump!", vm->dump);
				ft_error("Error! The Invalid number after after flag -dump.");
			}
		}
		else if (ft_strequ(argv[l], "-v"))
			vm->v = 1;
		else if (ft_strequ(argv[l], "-a"))
			vm->a = 1;
		else
			check_file_and_fill(argv[l], vm->processes, 0);
		l++;
	}
	how_many_champs(vm);
}

static void	ft_set_champs_to_null(t_process *champs)
{
	int i;

	i = 0;
	while (i < MAX_PLAYERS)
	{
		champs[i].cmp_nbr = 0;
		champs[i].cmp_name = NULL;
		champs[i].cmp_cmnt = NULL;
		champs[i].code_size = 0;
		champs[i].code = NULL;
		i++;
	}
}

void		parsing(int argc, char **argv, t_vm *vm)
{
	if (argc > 16)
		ft_error("Too many arguments");
	else
	{
		ft_set_champs_to_null(vm->processes);
		parse_champ(argc, argv, vm, 1);
	}
}
