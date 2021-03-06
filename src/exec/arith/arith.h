#ifndef __ARITH_H__
#define __ARITH_H__

make_helper(cmp_add_v);
make_helper(cmp_add_b);
make_helper(cmp_rm2r_v);
make_helper(cmp_rm2r_b);
make_helper(cmp_r2r_b);
make_helper(cmp_i2m_b);
make_helper(cmp_r2r_v);
make_helper(cmp_a2i_v);
make_helper(cmp_a2i_b);
make_helper(sub_r2rm_v);
make_helper(sub_rm2r_v);
make_helper(sub_r2m_l);
make_helper(sbb_r2r_l);
make_helper(dec_r_v);

make_helper(add_rm2m_v);
make_helper(add_i2a_l);
make_helper(add_rm2r_v);
make_helper(adc_r2r_l);
make_helper(inc_r_v);

make_helper(imul_ibr2r_l);
make_helper(imul_ilr2r_l);
make_helper(imul_rm2r_l);

make_helper(op_no2rm_v);
//make_helper(imd_l);

#endif
