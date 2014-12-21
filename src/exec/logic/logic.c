#include "exec/helper.h"
#include "cpu/modrm.h"

#define DATA_BYTE 1
#include "logic_tpl.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "logic_tpl.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "logic_tpl.h"
#undef DATA_BYTE

extern char suffix;

make_helper(and_r2i_v) {
	return (suffix == 'l' ? and_r2i_l(eip) : and_r2i_w(eip));
}

make_helper(or_r2i_v) {
	return (suffix == 'l' ? or_r2i_l(eip) : or_r2i_w(eip));
}

make_helper(test_rm2r_v) {
	return test_rm2r_b(eip);
}
