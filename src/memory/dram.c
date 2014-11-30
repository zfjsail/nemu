#include "common.h"
#include "lib/misc.h"

/* Simulate the (main) behavor of DRAM. Although this will lower the performace of NEMU,
 * it makes you clear about how DRAM is read/written.
 */

#define COL_WIDTH 10
#define ROW_WIDTH 10
#define BANK_WIDTH 3
#define RANK_WIDTH (27 - COL_WIDTH - ROW_WIDTH - BANK_WIDTH)

typedef union {
	struct {
		uint32_t col	: COL_WIDTH;
		uint32_t row	: ROW_WIDTH;
		uint32_t bank	: BANK_WIDTH;
		uint32_t rank	: RANK_WIDTH;
	};
	uint32_t addr;
} dram_addr;


#define NR_COL (1 << COL_WIDTH)
#define NR_ROW (1 << ROW_WIDTH)
#define NR_BANK (1 << BANK_WIDTH)
#define NR_RANK (1 << RANK_WIDTH)

#define HW_MEM_SIZE (1 << (COL_WIDTH + ROW_WIDTH + BANK_WIDTH + RANK_WIDTH))

#define BURST_LEN 8
#define BURST_MASK (BURST_LEN - 1)

uint8_t dram[NR_RANK][NR_BANK][NR_ROW][NR_COL];
uint8_t *hw_mem = (void *)dram;

typedef struct {
	uint8_t buf[NR_COL];
	int32_t row_idx;
	bool valid;
} RB;

RB rowbufs[NR_RANK][NR_BANK];

void init_dram() {
	int i, j;
	for(i = 0; i < NR_RANK; i ++) {
		for(j = 0; j < NR_BANK; j ++) {
			rowbufs[i][j].valid = false;
		}
	}
}

static void ddr3_read(hwaddr_t addr, void *data) {
	test(addr < HW_MEM_SIZE, "addr = %x\n", addr);

	dram_addr temp;
	temp.addr = addr & ~BURST_MASK;//~7
	uint32_t rank = temp.rank;
	uint32_t bank = temp.bank;
	uint32_t row = temp.row;
	uint32_t col = temp.col;

	if(!(rowbufs[rank][bank].valid && rowbufs[rank][bank].row_idx == row) ) {
		/* read a row into row buffer */
		memcpy(rowbufs[rank][bank].buf, dram[rank][bank][row], NR_COL);
		rowbufs[rank][bank].row_idx = row;
		rowbufs[rank][bank].valid = true;
	}

	/* burst read */
	memcpy(data, rowbufs[rank][bank].buf + col, BURST_LEN);//8
}

static void ddr3_write(hwaddr_t addr, void *data, uint8_t *mask) {
	test(addr < HW_MEM_SIZE, "addr = %x\n", addr);

	dram_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t rank = temp.rank;
	uint32_t bank = temp.bank;
	uint32_t row = temp.row;
	uint32_t col = temp.col;

	if(!(rowbufs[rank][bank].valid && rowbufs[rank][bank].row_idx == row) ) {
		/* read a row into row buffer */
		memcpy(rowbufs[rank][bank].buf, dram[rank][bank][row], NR_COL);
		rowbufs[rank][bank].row_idx = row;
		rowbufs[rank][bank].valid = true;
	}

	/* burst write */
	memcpy_with_mask(rowbufs[rank][bank].buf + col, data, BURST_LEN, mask);

	/* write back to dram */
	memcpy(dram[rank][bank][row], rowbufs[rank][bank].buf, NR_COL);
}

//还没看懂（从这里开始）
uint32_t dram_read(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	
	ddr3_read(addr, temp);

	if( (addr ^ (addr + len - 1)) & ~(BURST_MASK)  ) {
		/* data cross the burst boundary */
		ddr3_read(addr + BURST_LEN, temp + BURST_LEN);
	}

	return *(uint32_t *)(temp + offset) & (~0u >> ((4 - len) << 3));
}

void dram_write(hwaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	uint8_t mask[2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_LEN);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	ddr3_write(addr, temp, mask);

	if( (addr ^ (addr + len - 1)) & ~(BURST_MASK)  ) {
		/* data cross the burst boundary */
		ddr3_write(addr + BURST_LEN, temp + BURST_LEN, mask + BURST_LEN);
	}
}

//Now cache shows up......

#define BLOCK_WIDTH 6
#define SET_WIDTH 7

#define BLOCK_SZ 1 << 6
#define SET_SZ 1 << 7
#define ROW_OF_SET 1 << 3

typedef struct {
	uint8_t block[BLOCK_SZ];
	int32_t tag;
	bool valid;
} cache1_unit;

typedef union {
	struct {
		uint32_t off : BLOCK_WIDTH;//lower bits
		uint32_t set : SET_WIDTH;
		uint32_t tag : 32 - BLOCK_WIDTH - SET_WIDTH;
	};
	uint32_t addr;
} cache_addr;

typedef cache1_unit CU_1;

CU_1 cache1[SET_SZ][ROW_OF_SET];

uint8_t srand_07(){
//inv
//just testing...
	return 0;
}

void init_cache1(){
	int i,j;
	for(i = 0; i < SET_SZ; i++)
		for(j = 0; j < ROW_OF_SET; j++)
			cache1[i][j].valid = false;
}

uint32_t seek_row1(hwaddr_t addr){
	cache_addr chaddr;
	chaddr.addr = addr;
	uint32_t tag = chaddr.tag;
	uint32_t set = chaddr.set;

	int i;
	for(i = 0; i < ROW_OF_SET; i++)
		if(cache1[set][i].valid && cache1[set][i].tag == tag) break;
	return i;
}

void cache1_write(hwaddr_t addr,size_t len,uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	cache_addr chaddr;
	chaddr.addr = addr;
	uint32_t set = chaddr.set;
	uint32_t row = seek_row1(addr);
	assert(row != 8);
	uint32_t off = chaddr.off;

	uint32_t temp = data;
	memcpy(cache1[set][row].block + off, &temp, len);
}

void write_through(hwaddr_t addr,size_t len,uint32_t data) {
	if(seek_row1(addr) < 8){
		cache1_write(addr,len,data);
		dram_write(addr,len,data);
	}
	else
		dram_write(addr,len,data);
}

uint8_t isReplace(hwaddr_t addr) {
	cache_addr chaddr;
	chaddr.addr = addr;
	uint32_t set = chaddr.set;
	int i;
	for(i = 0; i < ROW_OF_SET; i++)
		if(cache1[set][i].valid == false) return i;
	return 8;//need replace
}

/* including loading blocks */
uint32_t cache1_read(hwaddr_t addr,size_t len){
	assert(len == 1 || len == 2 || len == 4);
	cache_addr chaddr;
	chaddr.addr = addr;
	uint32_t tag = chaddr.tag;
	uint32_t set = chaddr.set;
	uint32_t row = seek_row1(addr);
	uint32_t off = chaddr.off;
	if(row == ROW_OF_SET) {//miss
		if((row = isReplace(addr)) == 8) row = srand_07();
		cache1[set][row].valid = true;
		cache1[set][row].tag = tag;
		
		dram_read(addr, len);

		dram_addr temp;
		temp.addr = addr;
		uint32_t rank = temp.rank;
		uint32_t bank = temp.bank;
//		uint32_t rrow = temp.row;//blending...
		uint32_t col = temp.col;
		uint32_t off_rbuf = col >> BLOCK_WIDTH;

		memcpy(cache1[set][row].block, rowbufs[rank][bank].buf + (off_rbuf << BLOCK_WIDTH), BLOCK_SZ);

//		memcpy(cache1[set][row].block,(void *)addr_start,BLOCK_SZ);
	}
	
	return *(uint32_t *)(cache1[set][row].block + off) & (~0u >> ((4 - len) << 3));
}
