#ifndef __CONTR_TRANSF__
#define __CONTR_TRANSF__

make_helper(jmp_e_b);
make_helper(jmp_b);
make_helper(jmp_l);
make_helper(uct_nr_v);

make_helper(jle_b);
make_helper(jl_b);
make_helper(jne_b);
make_helper(jbe_b);
make_helper(jge_b);
make_helper(jg_b);
make_helper(ja_b);
make_helper(jb_b);
make_helper(jae_b);
make_helper(js_b);
make_helper(jle_l);
make_helper(jbe_l);
make_helper(jl_l);
make_helper(je_l);
make_helper(jg_l);
make_helper(js_l);
make_helper(jne_l);
make_helper(ja_l);

make_helper(op_plus_l);

make_helper(call_i_l);
make_helper(ret);
make_helper(ret_iw);

#endif
