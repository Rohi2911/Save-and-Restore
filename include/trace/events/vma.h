#undef TRACE_SYSTEM
#define TRACE_SYSTEM vma

#if !defined(_TRACE_VMA_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_VMA_H

#include <linux/tracepoint.h>

/* Added this event tracing to observe the stack and heap memory regions*/
TRACE_EVENT(mm_vma_walk,

	TP_PROTO(pid_t pid,
		 unsigned long start,
		 unsigned long end,
		 unsigned long flags,
		 const char *type),

	TP_ARGS(pid, start, end, flags, type),

	TP_STRUCT__entry(
		__field(pid_t, pid)
		__field(unsigned long, start)
		__field(unsigned long, end)
		__field(unsigned long, flags)
		__string(type, type)
	),

	TP_fast_assign(
		__entry->pid   = pid;
		__entry->start = start;
		__entry->end   = end;
		__entry->flags = flags;
		__assign_str(type, type);
	),

	TP_printk("pid=%d [%lx-%lx] flags=%lx type=%s",
		  __entry->pid,
		  __entry->start,
		  __entry->end,
		  __entry->flags,
		  __get_str(type))
);

#endif /* _TRACE_VMA_H */

#include <trace/define_trace.h>
