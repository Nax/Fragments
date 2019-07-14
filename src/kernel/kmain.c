#include <string.h>
#include <sys/msr.h>
#include <sys/types.h>
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
        kthread->tmpPageAddr = kheap_alloc(PAGESIZE);
    }
}

static void kernel_threads_enter(int threadId)
{
    uint16_t tssEntry;
    uint64_t star;

    tssEntry = GDT_TSS_BASE + 0x10 * threadId;
    __asm__ __volatile__ ("ltr %0\r\n" :: "r"(tssEntry));

    star = 0;
    star |= ((uint64_t)GDT_CODE_RING0) << 32;
    star |= ((uint64_t)GDT_CODE_RING3_32) << 48;

    msr_write(MSR_KERNELGSBASE, (uint64_t)gKernel.kernelThreads[threadId].ctx);
    msr_write(MSR_LSTAR, (uint64_t)&syscall_handler);
    msr_write(MSR_STAR, star);
    msr_write(MSR_SFMASK, (RFLAGS_IF | RFLAGS_AC | RFLAGS_DF | RFLAGS_TF | RFLAGS_RF));
}

static void start_vfsd(void)
{
    pid_t pid;
    const char* image;

    println("Starting VFSd...");
    pid = process_create();
    println("PID allocated");
    image = kimage_open("sbin/vfsd");
    println("Image found");
    process_elf_load(pid, image);
    println("Image loaded");
    process_run(pid);
    println("If you see this message then please call staff");

    putchar('\n');
}

static void enable_efer(void)
{
    uint64_t efer;

    efer = msr_read(MSR_EFER);
    efer |= 0x01;
    msr_write(MSR_EFER, efer);
}

void kmain(FragmentsKernelInfo* info)
{
    uint64_t cr3;

    __asm__ __volatile__ ("mov %%cr3, %%rax\r\n" : "=a"(cr3));

    gKernel.cr3 = cr3;
    gKernel.screen = (uint16_t*)0xb8000;
    memcpy(&gKernel.bootInfo, info, sizeof(gKernel.bootInfo));

    enable_efer();
    clear_screen();
    println("Hello world from the kernel!");
    printhex8(gKernel.bootInfo.drive);
    putchar('\n');
    printhex64((uint64_t)gKernel.bootInfo.kimage);
    putchar('\n');

    vmem_init();
    pmem_init();
    kimage_init();
    gKernel.screen = vmem_io_map(0xb8000, 80 * 25 * 2);
    println("Screen buffer relocated");
    gdt_init();
    println("GDT Initialized");
    //vmem_unmap_lower();
    kernel_threads_init();
    println("Kernel Thread Loaded");
    kernel_threads_enter(0);
    println("Kernel Thread Entered");
    irq_init();
    println("IRQ Initialized");
    irq_disable_all();
    idt_init();
    println("IDT Initialized");

    idt_register(0x21, 0, &int_handler_keyboard);
    //irq_enable(1);

    start_vfsd();

    kernel_wait();
}
