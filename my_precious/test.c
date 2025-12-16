#include <asm/current.h>
#include <linux/gfp.h>
#include <linux/mm_types.h>
#include <asm-generic/memory_model.h>
#include <linux/printk.h>
#include <linux/syscalls.h>
#include <asm/io.h>
#include <linux/mm_types.h>
#include <linux/pgtable.h>
#include <asm/pgtable_types.h>
#include <asm/pgtable.h>
#include <linux/rwsem.h>
#include <linux/sched/mm.h>
#include <asm/tlbflush.h>
#include <asm-generic/memory_model.h>
#include <linux/rmap.h>
#include <linux/pagemap.h>
#include <linux/spinlock_types.h>
//#include <asm-generic/atomic.h>
#include <linux/mmzone.h>
#include <linux/page_ref.h>

#define CREATE_TRACE_POINTS
#include <trace/events/vma.h>

enum {
	SAVE = 0,
	RESTORE
};

static bool is_stack_vma(struct vm_area_struct *vma, struct mm_struct *mm)
{
	if (vma->vm_flags & VM_STACK)
		return true;

	return vma->vm_start <= mm->start_stack &&
	       vma->vm_end   >= mm->start_stack;
}

static bool is_heap_vma(struct vm_area_struct *vma, struct mm_struct *mm)
{
	return vma->vm_start <= mm->brk &&
	       vma->vm_end   >= mm->start_brk;
}

SYSCALL_DEFINE0(test)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;

	if (!mm)
		return 0;

	mmap_read_lock(mm);

	for (vma = mm->mmap; vma; vma = vma->vm_next) {
		const char *type;

		if (is_stack_vma(vma, mm))
			type = "stack";
		else if (is_heap_vma(vma, mm))
			type = "heap";
		else
			type = "other";

		trace_mm_vma_walk(current->pid,
				  vma->vm_start,
				  vma->vm_end,
				  vma->vm_flags,
				  type);
	}

	mmap_read_unlock(mm);
	return 0;
}