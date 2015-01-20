#include "exec/helper.h"
#include "cpu/modrm.h"

#define DATA_BYTE 1
#include "arith_t.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "arith_t.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "arith_t.h"
#undef DATA_BYTE

extern char suffix;

make_helper(op_no2rm_v) {
	return (suffix == 'l' ? op_no2rm_l(eip) : op_no2rm_w(eip));
}
