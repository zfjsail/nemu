#include "exec/helper.h"

#define DATA_BYTE 1
#include "83-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "83-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "83-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(mis_i2rm_v){
	return (suffix == 'l' ? mis_i2rm_l(eip) : mis_i2rm_w(eip));
}
