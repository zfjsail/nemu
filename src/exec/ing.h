//w：have been written
//e: have benn execute
//
//add
//add-longlong
//bubble-sort
//fact
//fib
//gotbaha
//if-else
//leap-year
//max
//min3
//matrix-mul
//mov-c
//pascal
//prime
//select-sort
//shuixianhua
//struct delay
//sum
//switch
//to-lower-case
//wanshu

helper_fun opcode_table [256] = {
/* 0x00 */	inv, add_r2rm_l/we, inv, inv, 
/* 0x04 */	inv, add_i32_to_a/we, inv, inv, 
/* 0x08 */	inv, or_r2r_l/we, inv, inv, 
/* 0x0c */	inv, inv, inv, op_plus((8e jle_i32)/we, (af imul r_r_l)/we)
/* 0x10 */	inv, adc_r2r_l/we, inv, inv, 
/* 0x14 */	inv, inv, inv, inv, 
/* 0x18 */	inv, inv, inv, inv, 
/* 0x1c */	inv, inv, inv, inv, 
/* 0x20 */	inv, inv, inv, inv, 
/* 0x24 */	inv, inv, inv, inv,
/* 0x28 */	inv, sub_r2r_l/we, inv, sub_r2m_l/we, 
/* 0x2c */	inv, inv, inv, inv, 
/* 0x30 */	inv, xor_r2r_l/we, inv, inv, 
/* 0x34 */	inv, inv, inv, inv,
/* 0x38 */	cmp_r2r_b/we, cmp_r2r_l/we, inv, inv, 
/* 0x3c */	inv, inv, cmp_r&m_l, inv, 
/* 0x40 */	inv, inv, inv, inv, 
/* 0x44 */	inv, inv, inv, inv,
/* 0x48 */	inv, inv, inv, inv, 
/* 0x4c */	inv, inv, inv, inv, 
/* 0x50 */	inv, inv, inv, push_bx_l/we,//push_r_l 
/* 0x54 */	inv, push_bp_l(只实现了ebp/we), inv, inv,//push_r_l/应该没问题
/* 0x58 */	inv, inv, inv, inv, //pop_r_l
/* 0x5c */	inv, inv, inv, inv, //pop_r_l/应该没问题
/* 0x60 */	inv, inv, inv, inv,
/* 0x64 */	inv, inv, data_size, inv,
/* 0x68 */	inv, imul_ilr2r_l/we, inv, imul_ibr2r_l/we, 
/* 0x6c */	inv, inv, inv, inv, 
/* 0x70 */	inv, inv, inv, inv,
/* 0x74 */	jmp_e_b(je imm8/we), jne_b/we, jbe_b/we, ja_b/we,
/* 0x78 */	inv, inv, inv, inv, 
/* 0x7c */	jl_i8/we, jge_i8/we, jle_i8/we, jg_b/we, 
/* 0x80 */	cmp_i2m_b/we, cmp_add_l/we, nemu_trap, cmp_imm8_l(32位寄存器和8位立即数比/we), sub i8 to r_l/we cmpl i8 to m_l addl i8 to m_l/we  and_i8_rl/we add_i8_rl/we addl_modrm_sib_disp8_imm8/we
/* 0x84 */	test_rr_b/we, test_r_l(test r32,r32/we), inv, inv, 
/* 0x88 */	mov_r2rm_b, mov_r2rm_v, mov_rm2r_b, mov_rm2r_v,
/* 0x8c */	inv, lea_disp8/32(r)_r/we,, inv, inv, 
/* 0x90 */	inv, inv, inv, inv,
/* 0x94 */	inv, inv, inv, inv,
/* 0x98 */	inv, inv, inv, inv, 
/* 0x9c */	inv, inv, inv, inv, 
/* 0xa0 */	mov_moffs2a_b, mov_moffs2a_v, mov_a2moffs_b, mov_a2moffs_v,
/* 0xa4 */	inv, inv, inv, inv,
/* 0xa8 */	inv, inv, inv, inv,
/* 0xac */	inv, inv, inv, inv,
/* 0xb0 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb4 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb8 */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xbc */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xc0 */	inv, sar&shl_rl_b/we, inv, ret_c3,
/* 0xc4 */	inv, inv, mov_i2rm_b, mov_i2rm_v,
/* 0xc8 */	inv, leave/we, inv, inv,
/* 0xcc */	int3, inv, inv, inv,
/* 0xd0 */	inv, inv, inv, inv,
/* 0xd4 */	inv, inv, inv, inv,
/* 0xd8 */	inv, inv, inv, inv,
/* 0xdc */	inv, inv, inv, inv,
/* 0xe0 */	inv, inv, inv, inv,
/* 0xe4 */	inv, inv, inv, inv,
/* 0xe8 */	call_l/we, jmp_l/we, inv, jmp_b/we,
/* 0xec */	inv, inv, inv, inv,
/* 0xf0 */	inv, inv, inv, inv,
/* 0xf4 */	inv, inv, inv, imd_l//* & /,
/* 0xf8 */	inv, inv, idiv_m_l/we, inv,
/* 0xfc */	inv, inv, inv, jmp_nr_l/we
};

make_fun op_plus_table[256] = {
/* 0x00 */	inv, inv, inv, inv, 
/* 0x04 */	inv, inv, inv, inv, 
/* 0x08 */	inv, inv, inv, inv, 
/* 0x10 */	inv, inv, inv, inv, 
/* 0x14 */	inv, inv, inv, inv, 
/* 0x18 */	inv, inv, inv, inv, 
/* 0x1c */	inv, inv, inv, inv, 
/* 0x20 */	inv, inv, inv, inv, 
/* 0x24 */	inv, inv, inv, inv,
/* 0x28 */	inv, inv, inv, inv, 
/* 0x2c */	inv, inv, inv, inv, 
/* 0x30 */	inv, inv, inv, inv, 
/* 0x34 */	inv, inv, inv, inv,
/* 0x38 */	inv, inv, inv, inv, 
/* 0x3c */	inv, inv, inv, inv, 
/* 0x40 */	inv, inv, inv, inv, 
/* 0x44 */	inv, inv, inv, inv,
/* 0x48 */	inv, inv, inv, inv, 
/* 0x4c */	inv, inv, inv, inv, 
/* 0x50 */	inv, inv, inv, inv
/* 0x54 */	inv, inv, inv, inv,
/* 0x58 */	inv, inv, inv, inv, 
/* 0x5c */	inv, inv, inv, inv, 
/* 0x60 */	inv, inv, inv, inv,
/* 0x64 */	inv, inv, inv, inv,
/* 0x68 */	inv, inv, inv, inv, 
/* 0x6c */	inv, inv, inv, inv, 
/* 0x70 */	inv, inv, inv, inv,
/* 0x74 */	inv, inv, inv, inv,
/* 0x78 */	inv, inv, inv, inv, 
/* 0x7c */	inv, inv, inv, inv, 
/* 0x80 */	inv, inv, inv, inv, 
/* 0x84 */	inv, inv, jbe_i_l/we, inv, 
/* 0x88 */	inv, inv, inv, inv, 
/* 0x8c */	inv, inv, jle_i32/we, inv, 
/* 0x90 */	inv, inv, inv, inv,
/* 0x94 */	inv, setne, inv, inv,
/* 0x98 */	inv, inv, inv, inv, 
/* 0x9c */	inv, inv, inv, inv, 
/* 0xa0 */	inv, inv, inv, inv, 
/* 0xa4 */	inv, inv, inv, inv,
/* 0xa8 */	inv, inv, inv, inv,
/* 0xac */	inv, inv, inv, imul_rmr_l/we,
/* 0xb0 */	inv, inv, inv, inv,
/* 0xb4 */	inv, inv, mov_z_bl/we, inv,
/* 0xb8 */	inv, inv, inv, inv,
/* 0xbc */	inv, inv, mov_s_bl/we, inv,
/* 0xc0 */	inv, inv, inv, inv,
/* 0xc4 */	inv, inv, inv, inv,
/* 0xc8 */	inv, inv, inv, inv,
/* 0xcc */	inv, inv, inv, inv,
/* 0xd0 */	inv, inv, inv, inv,
/* 0xd4 */	inv, inv, inv, inv,
/* 0xd8 */	inv, inv, inv, inv,
/* 0xdc */	inv, inv, inv, inv,
/* 0xe0 */	inv, inv, inv, inv,
/* 0xe4 */	inv, inv, inv, inv,
/* 0xe8 */	inv, inv, inv, inv,
/* 0xec */	inv, inv, inv, inv,
/* 0xf0 */	inv, inv, inv, inv,
/* 0xf4 */	inv, inv, inv, inv,
/* 0xf8 */	inv, inv, inv, inv,
/* 0xfc */	inv, inv, inv, inv
};
