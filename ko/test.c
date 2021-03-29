#include <linux/init.h>
#include <linux/module.h>
#include <asm/thread_info.h>
#include <linux/sched.h>

static int test_init(void)
{
        int i = 0;
        printk("Process name is ---->%s\n",current->comm);
        return 0;
}

static void test_exit(void)
{
        printk("---Remove---%s\n", current->comm);
}

module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("WSK");
