#ifndef VM_H
# define VM_H

# include <fcntl.h>
# include "./libft/libft.h"
# include "op.h"

# define SLEEP 0//20000

typedef struct s_vm		t_vm;
typedef struct s_carry	t_carry;
typedef struct s_cycle	t_cycle;
typedef	void			(*t_command)(t_carry *cr);


/*
** Carriage params: cr
*/

struct	s_carry
{
	unsigned int		car_nbr;//carry number
	unsigned char		alive;// are you still alive? <- check it var please
	unsigned char		carry;//flag "carry"
	unsigned char		cmd_code;//current operation code
	unsigned char		jump_len;//bytes to second instruction
	int					last_live;//cycles from last live call
	int					last_champ;//last alive champ nbr
	unsigned int		last_alive_cycle;
	unsigned int		cycles_before;//cycles before current operation
	int					position;//carry position in game memory area
	unsigned char		color;//carry color (champ color)
	unsigned int		reg[16];//registr
	t_cycle				*cycle;
	t_vm				*vm;
	struct s_carry		*next;
};

/*
** Process params: p
*/

typedef struct	s_process
{
	int					cmp_nbr;
	char				*cmp_name;
	char				*cmp_cmnt;
	int					code_size;
	unsigned char		*code;
}				t_process;

/*
** Command params: cmd
*/

typedef struct	s_cmd_prms
{
	unsigned char		cmd_code;//1
	unsigned char		dir_size;//2
	unsigned char		descript;//3
	unsigned char		prms_count;//4
	unsigned short		cycles_before;//5
	unsigned char		prm_types[3];//6
	char				*name;//7
}				t_cmd_prms;

/*
** Current cycle params: cycle
*/

struct			s_cycle
{
	unsigned char		descript[3];//1
	unsigned char		regs[3];
	int					shift;
};


/*
** Virtual Maschine params: vm
*/

struct			s_vm
{
	char 				a;
	char				v;
	char				debug;
	char				l_endian;
	unsigned char		champs_count;
	unsigned int		car_count;
	unsigned int		last_alive;//number of last alive champion
	unsigned int		last_alive_cycle;
	unsigned char		*area;
	unsigned char		*back;
	unsigned char		*light;
	unsigned int		cycles_from_start;
	char 				dump_flag;
	unsigned int		dump;
	int					cycles_to_die;
	int					ctd_counter;//cycles to die counter
	int					check_counter;
	unsigned int		live_counter;//summary cycles_to_die alive counter
	t_carry				*carriages;
	t_process			processes[MAX_PLAYERS];
	t_command			command[16];
};

/*
** Carriage params: ldi
*/

typedef struct	s_ldi
{
	int					prm1;
	short				dir1;
	short				dir2;
	int					reg2;
	int					position;
	short 				indir;
}				t_ldi;

/*
** Carriage params: sti
*/

typedef struct	s_sti
{
	unsigned int		reg1;
	int					prm2;
	short				dir2;
	short				dir3;
	int					reg3;
	int					position;
	short 				indir;
}				t_sti;

/*
** GLOBAL Operation params:
*/

static t_cmd_prms			g_cmd_prms[16] =
{
	{1, 4, 0, 1, 10, {T_DIR}, "live"},/*alive*/
	{2, 4, 1, 2, 5, {T_DIR | T_IND, T_REG}, "ld"},/*ld*/
	{3, 4, 1, 2, 5, {T_REG, T_IND | T_REG}, "st"},/*st*/
	{4, 4, 1, 3, 10, {T_REG, T_REG, T_REG}, "add"},/*add*/
	{5, 4, 1, 3, 10, {T_REG, T_REG, T_REG}, "sub"},/*sub*/
	{6, 4, 1, 3, 6, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}, "and"},/*and*/
	{7, 4, 1, 3, 6, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, "or"},/*or*/
	{8, 4, 1, 3, 6, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, "xor"},/*xor*/
	{9, 2, 0, 1, 20, {T_DIR}, "zjmp"},/*zjmp*/
	{10, 2, 1, 3, 25, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, "ldi"},/*ldi*/
	{11, 2, 1, 3, 25, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}, "sti"},/*sti*/
	{12, 2, 0, 1, 800, {T_DIR}, "frk"},/*frk*/
	{13, 4, 1, 2, 10, {T_DIR | T_IND, T_REG}, "lld"},/*lld*/
	{14, 2, 1, 3, 50, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, "lldi"},/*lldi*/
	{15, 2, 0, 1, 1000, {T_DIR}, "lfrk"},/*lfrk*/
	{16, 4, 1, 1, 2, {T_REG}, "aff"}/*aff*/
};

/*
** Main command's functions:
*/

void	live(t_carry *cr);
void	ld(t_carry *cr);
void	st(t_carry *cr);
void	add(t_carry *cr);
void	sub(t_carry *cr);
void	and(t_carry *cr);
void	or(t_carry *cr);
void	xor(t_carry *cr);
void	zjmp(t_carry *cr);
void	ldi(t_carry *cr);
void	sti(t_carry *cr);
void	frk(t_carry *cr);
void	lld(t_carry *cr);
void	lldi(t_carry *cr);
void	lfrk(t_carry *cr);
void	aff(t_carry *cr);

/*
** Common functions:
*/

int				display_usage();
void			parsing(int argc, char **argv, t_vm	*vm);
void			vm_init(t_vm *vm);
int				ft_error(char *str);
void			short_ind(unsigned char *dst, unsigned const char *src);
void			rotate_or_not(t_carry *cr, unsigned char *dest, int src_i);
short			indir_position(short indir, t_carry *cr);
void			from_memory_to_var(t_carry *cr, void *var, int position,
		int size);
void			get_param_plus(t_carry *cr, void *param, int prm_nbr);
unsigned int	get_param(t_carry *cr, int prm_nbr);
unsigned int	get_param3(t_carry *cr);
void			game(t_vm *vm);
int				check_operation(t_vm *vm, t_carry *cr, t_cycle *cycle);
void			calc_shift();
void			carriages_init(t_vm *vm, int champ_nbr, int position);
int				check_position(int position);
void			game_area_frame(t_vm *vm);
void			print_game_area(t_vm *vm);
void			from_var_to_memory(t_carry *cr, void *var, int position, \
	int	size);

#endif
