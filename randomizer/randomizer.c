#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/random.h>


#define DEVICE_NAME "Number_Randomizer"
#define DEVICE_CLASS "Generator_Module"
#define DEVICE_AUTHOR "18120019_18120052"

static dev_t major_number;                    
static struct cdev randomizer_device; 
static struct class* randomizer_class;   
static unsigned char random_number[64];


static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file*, char*, size_t, loff_t*);


static struct file_operations file_operator =
{
    .open = device_open,
    .read = device_read,
    .release = device_release,
};


static int __init init_randomizer(void)
{

    if (alloc_chrdev_region(&major_number, 0, 1, DEVICE_NAME) < 0)
    {
        printk(KERN_ALERT "Number_Randomizer: Failed Registering Major Number\n");

        return -1;
    }
    

    randomizer_class = class_create(THIS_MODULE, DEVICE_CLASS);

    if (randomizer_class == NULL)
    { 
        unregister_chrdev_region(major_number, 1);

        printk(KERN_ALERT "Number_Randomizer: Failed Registering Device Class\n");

        return -1; 
    }


    if (device_create(randomizer_class, NULL, major_number, NULL, DEVICE_NAME) == NULL)
    {
        class_destroy(randomizer_class);

        unregister_chrdev_region(major_number, 1);

        printk(KERN_ALERT "Number_Randomizer: Failed Creating Device \n");

        return -1;
    }


    cdev_init(&randomizer_device, &file_operator);

    if (cdev_add(&randomizer_device, major_number, 1) == -1)
    {
        device_destroy(randomizer_class, major_number);

        class_destroy(randomizer_class);

        unregister_chrdev_region(major_number, 1);

        printk(KERN_ALERT "Number_Randomizer: Failed Initializing Device File \n");

        return -1;
    }


    printk(KERN_INFO "Number_Randomizer: Succesfully Created Device \n");

    return 0;
}



static void __exit exit_randomizer(void)
{
    cdev_del(&randomizer_device);

    device_destroy(randomizer_class, major_number);                      

    class_destroy(randomizer_class);                         
    
    unregister_chrdev_region(major_number, 1);      

    printk(KERN_INFO "Number_Randomizer: Correctly Destroyed Device \n");
}



static ssize_t device_read(struct file *file, char *buffer, size_t length, loff_t *offset)
{
    get_random_bytes(random_number, length);

    if (copy_to_user(buffer, random_number, length) != 0) return -EFAULT;

    printk(KERN_INFO "Number_Randomizer: Generated %ld Bytes \n", length);

    while (length--)
    {
        printk(KERN_INFO "Number_Randomizer: Byte[%ld] = %d \n", length, random_number[length]);
    }
       
    return 0;
}

static int device_open(struct inode *inode_ptr, struct file *file_ptr)
{
    printk(KERN_INFO "Number_Randomizer: Currently Openning Device \n");

    return 0;
}

static int device_release(struct inode *inode_ptr, struct file *file_ptr)
{
    printk(KERN_INFO "Number_Randomizer: Succesfully Released Device \n");

    return 0;
}


module_init(init_randomizer)
module_exit(exit_randomizer)

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DEVICE_AUTHOR);
