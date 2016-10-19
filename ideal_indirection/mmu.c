/**
* Ideal Indirection Lab
* CS 241 - Fall 2016
*/
#include "kernel.h"
#include "mmu.h"
#include <assert.h>
#include <stdio.h>

MMU *MMU_create() {
	MMU *mmu = calloc(1, sizeof(MMU));
	mmu->tlb = TLB_create();
	mmu->curr_pid = 0;
	return mmu;
}


void *MMU_get_physical_address(MMU *mmu, void *virtual_address, size_t pid) {
	assert(pid < MAX_PROCESS_ID);
	if(pid != mmu -> curr_pid) {
		TLB_flush(&(mmu -> tlb));
		mmu -> curr_pid = pid;
	}
	PageTable *pt = mmu->base_pts[pid];
	size_t masked = (((size_t) virtual_address) << 13) >> 13;
	size_t offset = masked & ((0xfff << 3) | 0xf);
	size_t vpn3 = (masked >> 15) & 0xfff;
	size_t vpn2 = (masked >> 27) & 0xfff;
	size_t vpn1 = (masked >> 39) & 0xfff;
	size_t key  = (masked >> 15);

	void* ret = TLB_get_physical_address(&(mmu->tlb), (void*) key);
	if(ret) {
		return ret + offset;
	}
	MMU_tlb_miss(mmu, virtual_address, pid);
	void* pt1 = PageTable_get_entry(pt, vpn1);
	if(!pt1) {
		pt1 = PageTable_create();
		MMU_raise_page_fault(mmu, virtual_address, pid);
		PageTable_set_entry(pt, vpn1, pt1);
	}
	void* pt2 = PageTable_get_entry(pt1, vpn2);
	if(!pt2) {
		pt2 = PageTable_create();
		MMU_raise_page_fault(mmu, virtual_address, pid);
		PageTable_set_entry(pt1, vpn2, pt2);
	}
	void* pt3 = PageTable_get_entry(pt2, vpn3);
	if(!pt3) {
		pt3 = ask_kernel_for_frame();
		MMU_raise_page_fault(mmu, virtual_address, pid);
		PageTable_set_entry(pt2, vpn3, pt3);
	}
	TLB_add_physical_address(&(mmu -> tlb), (void*) key, pt3);
	size_t result = (size_t) pt3 + offset;

	return (void *)result;
}

void MMU_tlb_miss(MMU *mmu, void *address, size_t pid) {
	assert(pid < MAX_PROCESS_ID);
	mmu->num_tlb_misses++;
	printf("Process [%lu] tried to access [%p] and it couldn't find it in the "
	"TLB so the MMU has to check the PAGE TABLES\n",
	pid, address);
}

void MMU_raise_page_fault(MMU *mmu, void *address, size_t pid) {
	assert(pid < MAX_PROCESS_ID);
	mmu->num_page_faults++;
	printf(
		"Process [%lu] tried to access [%p] and the MMU got an invalid entry\n",
		pid, address);
	}

	void MMU_add_process(MMU *mmu, size_t pid) {
		assert(pid < MAX_PROCESS_ID);
		mmu->base_pts[pid] = PageTable_create();
	}

	void MMU_free_process_tables(MMU *mmu, size_t pid) {
		assert(pid < MAX_PROCESS_ID);
		PageTable *base_pt = mmu->base_pts[pid];
		Pagetable_delete_tree(base_pt);
	}

	void MMU_delete(MMU *mmu) {
		for (size_t i = 0; i < MAX_PROCESS_ID; i++) {
			MMU_free_process_tables(mmu, i);
		}
		TLB_delete(mmu->tlb);
		free(mmu);
	}
