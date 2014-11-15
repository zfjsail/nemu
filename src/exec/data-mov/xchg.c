#include "exec/helper.h"

#define DATA_BYTE 1
#include "xchg-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "xchg-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "xchg-template.h"
#undef DATA_BYTE

//extern char suffix;

make_helper(xchg_a2r_v){
	return 1;
}
