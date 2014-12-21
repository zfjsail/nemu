#include "exec/helper.h"
#include "cpu/modrm.h"

#define DATA_BYTE 1
#include "rep_tpl.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "rep_tpl.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "rep_tpl.h"
#undef DATA_BYTE

extern char suffix;

make_helper(rep_v) {//a5
	return (suffix == 'l' ? rep_l(eip) : rep_w(eip));
}
