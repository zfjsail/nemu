#ifndef __CONTR_TRANSF__
#define __CONTR_TRANSF__

make_helper(jmp_e_b);
make_helper(jmp_b);
make_helper(jmp_l);
make_helper(jle_b);
make_helper(jl_b);
make_helper(jne_b);
make_helper(jbe_b);
make_helper(jle_i_8);

make_helper(op_plus_l);

make_helper(call_i_l);
make_helper(ret);

#endif