/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ld_ldi_lld_lldi_cmds.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfrost-a <dfrost-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 13:06:54 by dfrost-a          #+#    #+#             */
/*   Updated: 2019/10/30 19:30:39 by uhand            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static void	print_ldetc(t_carry *cr, t_ldi *ldi, short sw)
{
	unsigned int print;

	print = 0;
	if (sw == 1)
	{
		ft_printf("P%*d | ldi %d %d r%d\n       | -> load from %d + %d = %d (wi"
"th pc and mod %d)\n", ((ft_hw_mn_orders(cr->car_nbr) < 5) ? 5 :
ft_hw_mn_orders(cr->car_nbr) + 1), cr->car_nbr, ldi->prm1 + ldi->dir1, ldi->
dir2 + ldi->reg2, cr->cycle->regs[2] + 1, ldi->prm1 + ldi->dir1, ldi->dir2 +
ldi->reg2, ldi->prm1 + ldi->dir1 + ldi->dir2 + ldi->reg2, ldi->position);
	}
	if (sw == 2)
	{
		from_memory_to_var(cr, &print, check_position(ldi->lld_pst), REG_SIZE);
		ft_printf("P%*d | lld %d r%d\n", ((ft_hw_mn_orders(cr->car_nbr) < 5) ?
		5 : ft_hw_mn_orders(cr->car_nbr) + 1), cr->car_nbr, (cr->cycle->
		descript[0] == 2) ? cr->reg[cr->cycle->regs[1]] : print,
				cr->cycle->regs[1] + 1);
	}
}

void		ld(t_carry *cr)
{
	short			indir;
	unsigned int	posit;

	if (cr->cycle->descript[0] == 2)
		cr->reg[cr->cycle->regs[1]] = get_param(cr, 0);
	else
	{
		indir = get_param(cr, 0);
		posit = check_position(cr->position + indir % IDX_MOD);
		from_memory_to_var(cr, &cr->reg[cr->cycle->regs[1]], posit, REG_SIZE);
	}
	if (!cr->reg[cr->cycle->regs[1]])
		cr->carry = 1;
	else
		cr->carry = 0;
	if (cr->vm->verbose.v && cr->vm->verbose.v_4)
	{
		ft_printf("P%*d | ld %d r%d\n",
				((ft_hw_mn_orders(cr->car_nbr) < 5) ? 5 :
				ft_hw_mn_orders(cr->car_nbr) + 1), cr->car_nbr,
				cr->reg[cr->cycle->regs[1]], cr->cycle->regs[1] + 1);
	}
	if (cr->vm->debug)
		ft_printf("ld ");
}

void		ldi(t_carry *cr)
{
	t_ldi		ldi;

	ft_bzero(&ldi, sizeof(ldi));
	if (cr->cycle->descript[0] == 1)
		get_param_plus(cr, &ldi.prm1, 0);
	else if (cr->cycle->descript[0] == 2)
		get_param_plus(cr, &ldi.dir1, 0);
	else
	{
		get_param_plus(cr, &ldi.indir, 0);
		from_memory_to_var(cr, &ldi.prm1, check_position(cr->position +
		ldi.indir % IDX_MOD), REG_SIZE);
	}
	if (cr->cycle->descript[1] == 1)
		get_param_plus(cr, &ldi.reg2, 1);
	else
		get_param_plus(cr, &ldi.dir2, 1);
	ldi.position = cr->position + ((ldi.prm1 + ldi.dir1 + ldi.dir2 + ldi.reg2) %
			IDX_MOD);
	from_memory_to_var(cr, &cr->reg[cr->cycle->regs[2]], check_position(ldi.
	position), REG_SIZE);
	if (cr->vm->verbose.v && cr->vm->verbose.v_4)
		print_ldetc(cr, &ldi, 1);
}

void		lld(t_carry *cr)
{
	t_ldi	ldi;

	ft_bzero(&ldi, sizeof(ldi));
	if (cr->cycle->descript[0] == 2)
		cr->reg[cr->cycle->regs[1]] = get_param(cr, 0);
	else
	{
		ldi.indir = get_param(cr, 0);
		ldi.lld_pst = check_position(cr->position + ldi.indir);
		if (STUPID)
			from_memory_to_var(cr, &cr->reg[cr->cycle->regs[1]], check_position(
					ldi.lld_pst), 2);
		else
			from_memory_to_var(cr, &cr->reg[cr->cycle->regs[1]],
					check_position(ldi.lld_pst), REG_SIZE);
	}
	if (!cr->reg[cr->cycle->regs[1]])
		cr->carry = 1;
	else
		cr->carry = 0;
	if (cr->vm->verbose.v && cr->vm->verbose.v_4)
		print_ldetc(cr, &ldi, 2);
}

void		lldi(t_carry *cr)
{
	t_ldi		ldi;

	ft_bzero(&ldi, sizeof(ldi));
	if (cr->cycle->descript[0] == 1)
		get_param_plus(cr, &ldi.prm1, 0);
	else if (cr->cycle->descript[0] == 2)
		get_param_plus(cr, &ldi.dir1, 0);
	else
	{
		get_param_plus(cr, &ldi.indir, 0);
		from_memory_to_var(cr, &ldi.prm1, check_position(cr->position +
						ldi.indir % IDX_MOD), REG_SIZE);
	}
	if (cr->cycle->descript[1] == 1)
		get_param_plus(cr, &ldi.reg2, 1);
	else
		get_param_plus(cr, &ldi.dir2, 1);
	ldi.position = check_position(cr->position + \
		((ldi.prm1 + ldi.dir1 + ldi.dir2 + ldi.reg2)));
	from_memory_to_var(cr, &cr->reg[cr->cycle->regs[2]], ldi.position,
			REG_SIZE);
	print_lldi(cr, &ldi);
}
