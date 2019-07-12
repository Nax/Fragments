#include <string.h>
#include <sys/msr.h>
#include "kernel.h"

KernelContext gKernel;

static void kernel_threads_init()
{
    static const size_t kThreadsCount = 1;
    KernelThreadContext* kthread;
    void* stackBase;

    gKernel.kernelThreads = kmalloc(0, kThreadsCount * sizeof(KernelThreadContext));
    for (size_t i = 0; i < kThreadsCount; ++i)
    {
        stackBase = kmalloc_raw(PAGESIZE * 2);
        kthread = gKernel.kernelThreads + i;
        kthread->ctx = kthread;
        kthread->stack = (char*)stackBase + (PAGESIZE * 2);
        kthread->tss = kmalloc_raw(sizeof(Tss));
        memset(kthread->tss, 0, sizeof(Tss));
        kthread->tss->rsp0Lo = ((uint64_t)kthread->stack) & 0xffffffff;
        kthread->tss->rsp0Hi = ((uint64_t)kthread->stack) >> 32;
        kthread->tss->iopb = sizeof(Tss);
        gdt_describe_tss(GDT_TSS_BASE + i * 0x10, kthread->tss, sizeof(Tss) - 1);
    }
}

static void kernel_threads_enter(int threadId)
{
    uint16_t tssEntry;

    tssEntry = GDT_TSS_BASE + 0x10 * threadId;
    msr_write(MSR_KERNELGSBASE, (uint64_t)gKernel.kernelThreads[threadId].ctx);
    __asm__ __volatile__ ("ltr %0\r\n" :: "r"(tssEntry));
}

void kmain(FragmentsKernelInfo* info)
{
    memcpy(&gKernel.bootInfo, info, sizeof(gKernel.bootInfo));

    clear_screen();
    println("Hello world from the kernel!");
    printhex8(gKernel.bootInfo.drive);
    putchar('\n');

    pmem_init();
    gdt_init();
    kernel_threads_init();
    kernel_threads_enter(0);
    irq_init();
    irq_disable_all();
    idt_init();

    idt_register(0x21, 0, &int_handler_keyboard);
    irq_enable(1);

    kernel_wait();
}
