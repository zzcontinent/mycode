#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

struct proc_dir_entry *proc_ctcwifi_dir = NULL;
struct proc_dir_entry *ctcwifi_diag_enable = NULL;
unsigned char g_ctcwifiDiagEnable;

/*Interface implementation*/
ssize_t start_ctcwifiDiagEnable_write(struct file *fp,const char __user *buffer,
    size_t count,loff_t *data)
{
    int i = 0;
    unsigned char tmp = 0;

    while(buffer[i] == ' ')
    {
        i++;
    }
    tmp = buffer[i] - '0'; 

    if((tmp == 1) || (tmp == 0))
    {
        g_ctcwifiDiagEnable = tmp;
    }    
    else
    {
        printk("the input parameter out of range\n");
    }

    return count;
}

static int ctcwifiDiagEnable_proc_read(struct seq_file *m, void *v)
{
    seq_printf(m, "%d\n", g_ctcwifiDiagEnable);

    return 1;
}

static int ctcwifiDiagEnable_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, ctcwifiDiagEnable_proc_read, NULL);
}

/*Implement related file_operations  interfaces*/
struct file_operations diag_enable_ops=
{
    .open       = ctcwifiDiagEnable_proc_open,     //对应cat
    .read       = seq_read,
    .write      = start_ctcwifiDiagEnable_write,     //对应echo
    .llseek     = seq_lseek,
    .release    = single_release,
};

int wlan_ctcwifi_init(void)
{
     /*create /proc/ctcwifi*/
#if 0
     proc_ctcwifi_dir = proc_mkdir("hello", NULL);
     if(proc_ctcwifi_dir == NULL)
     {
          printk("\n Can't create /proc/hello\n");
          return -1;
     }
#endif

     /*create /proc/ctcwifi/diag_enable*/
     if (( ctcwifi_diag_enable = proc_create("hello", 0644,
     //    proc_ctcwifi_dir, &diag_enable_ops)) == NULL )
         NULL, &diag_enable_ops)) == NULL )
     {
          printk("\n Can't create /proc/hello file\n");
          return -1;
     }

    return 0;
}

void wlan_ctcwifi_exit(void)
{
    /* remove /proc/ctcwifi/diag_enable */
    if (ctcwifi_diag_enable)
    {
        remove_proc_entry("diag_enable", proc_ctcwifi_dir);
        ctcwifi_diag_enable = NULL;
    }        

    /* remove /proc/ctcwifi*/
    if (proc_ctcwifi_dir)
    {
        remove_proc_entry("ctcwifi", NULL);
        proc_ctcwifi_dir = NULL;
    }

}

module_init(wlan_ctcwifi_init);
module_exit(wlan_ctcwifi_exit);

MODULE_AUTHOR("ZC");
MODULE_LICENSE("Dual BSD/GPL");

