// /***************************************************************************//
// **
// * \file led_driver.c
// * \details Simple GPIO driver explanation
// * \author EmbeTronicX
// * \Tested with Linux raspberrypi 5.4.51-v7l+
// ******************************************************************************

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h> //copy_to/from_user()
#include <linux/gpio.h> //GPIO
// #include <stdarg.h>

//LED is connected to this GPIO
#define GPIO_21 (21)
#define GPIO_20 (20)
#define GPIO_16 (16)
#define GPIO_26 (26)
#define GPIO_19 (19)
#define GPIO_13 (13)
#define GPIO_6  (6)
#define GPIO_12 (12)

#define GPIO_5 (5)
#define GPIO_0 (0)
#define GPIO_1 (1)
#define GPIO_7 (7)
#define GPIO_8 (8)
#define GPIO_11 (11)
#define GPIO_25 (25) 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
/*************** Driver functions **********************/
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, 
 char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write(struct file *filp, 
 const char *buf, size_t len, loff_t * off);
/******************************************************/
//File operation structure 
static struct file_operations fops =
{
 .owner = THIS_MODULE,
 .read = etx_read,
 .write = etx_write,
 .open = etx_open,
 .release = etx_release,
};
/*
** This function will be called when we open the Device file
*/ 
static int etx_open(struct inode *inode, struct file *file)
{
 pr_info("Device File Opened...!!!\n");
 return 0;
}
/*
** This function will be called when we close the Device file
*/
static int etx_release(struct inode *inode, struct file *file)
{
    pr_info("Device File Closed...!!!\n");
    return 0;
    }
/*
** This function will be called when we read the Device file
*/ 
static ssize_t etx_read(struct file *filp, 
 char __user *buf, size_t len, loff_t *off)
{
    uint8_t gpio_state = 0;
    
    //reading GPIO value
    gpio_state = gpio_get_value(GPIO_21);
    
    //write to user
    len = 1;
    if( copy_to_user(buf, &gpio_state, len) > 0) {
        pr_err("ERROR: Not all the bytes have been copied to user\n");
    }
    
    pr_info("Read function : GPIO_21 = %d \n", gpio_state);
    
    return 0;
}
/*
** This function will be called when we write the Device file
*/ 
static ssize_t etx_write(struct file *filp, 
 const char __user *buf, size_t len, loff_t *off)
{
    uint8_t rec_buf[20] = {0};
 
    if( copy_from_user( rec_buf, buf, len ) > 0) {
        pr_err("ERROR: Not all the bytes have been copied from user\n");
    }
    
    pr_info("Write Function : GPIO_21 Set = %c\n", rec_buf[0]);
    pr_info("Write Function : GPIO_20 Set = %c\n", rec_buf[1]);
    pr_info("Write Function : GPIO_16 Set = %c\n", rec_buf[2]);
    pr_info("Write Function : GPIO_26 Set = %c\n", rec_buf[3]);
    pr_info("Write Function : GPIO_19 Set = %c\n", rec_buf[4]);
    pr_info("Write Function : GPIO_13 Set = %c\n", rec_buf[5]);
    pr_info("Write Function : GPIO_6 Set = %c\n",  rec_buf[6]);
    pr_info("Write Function : GPIO_12 Set = %c\n", rec_buf[7]);

    pr_info("Write Function : GPIO_5 Set = %c\n", rec_buf[8]);
    pr_info("Write Function : GPIO_0 Set = %c\n", rec_buf[9]);
    pr_info("Write Function : GPIO_1 Set = %c\n", rec_buf[10]);
    pr_info("Write Function : GPIO_7 Set = %c\n", rec_buf[11]);
    pr_info("Write Function : GPIO_8 Set = %c\n", rec_buf[12]);
    pr_info("Write Function : GPIO_11 Set = %c\n", rec_buf[13]);
    pr_info("Write Function : GPIO_25 Set = %c\n", rec_buf[14]);

    gpio_set_value(GPIO_21, rec_buf[0]- '0');
    gpio_set_value(GPIO_20, rec_buf[1]- '0');
    gpio_set_value(GPIO_16, rec_buf[2]- '0');
    gpio_set_value(GPIO_26, rec_buf[3]- '0');
    gpio_set_value(GPIO_19, rec_buf[4]- '0');
    gpio_set_value(GPIO_13, rec_buf[5]- '0');
    gpio_set_value(GPIO_6,  rec_buf[6]- '0');
    gpio_set_value(GPIO_12, rec_buf[7]- '0');
    gpio_set_value(GPIO_5, rec_buf[8]- '0');
    gpio_set_value(GPIO_0, rec_buf[9]- '0');
    gpio_set_value(GPIO_1, rec_buf[10]- '0');
    gpio_set_value(GPIO_7, rec_buf[11]- '0');
    gpio_set_value(GPIO_8, rec_buf[12]- '0');
    gpio_set_value(GPIO_11, rec_buf[13]- '0');
    gpio_set_value(GPIO_25, rec_buf[14]- '0');
    // if (rec_buf[0]=='1') {
    // //set the GPIO value to HIGH
    //     gpio_set_value(GPIO_21, 1);
    // } else if (rec_buf[0]=='0') {
    // //set the GPIO value to LOW
    // gpio_set_value(GPIO_21, 0);
    // } else {
    //     pr_err("Unknown command : Please provide either 1 or 0 \n");
    // }

    // if (rec_buf[1] =='1'){
    //     gpio_set_value(GPIO_20, 1);
    // }else if (rec_buf[1]=='0') {
    // //set the GPIO value to LOW
    //     gpio_set_value(GPIO_20, 0);
    // } 
    // else {
    //     pr_err("Unknown command : Please provide either 1 or 0 \n");
    // }

    // if (rec_buf[2] =='1'){
    //     gpio_set_value(GPIO_16, 1);
    // }else if (rec_buf[2]=='0') {
    // //set the GPIO value to LOW
    //     gpio_set_value(GPIO_16, 0);
    // } 
    // else {
    //     pr_err("Unknown command : Please provide either 1 or 0 \n");
    // }
    // if (rec_buf[3] =='1'){
    //     gpio_set_value(GPIO_26, 1);
    // }else if (rec_buf[3]=='0') {
    // //set the GPIO value to LOW
    //     gpio_set_value(GPIO_26, 0);
    // } 
    // else {
    //     pr_err("Unknown command : Please provide either 1 or 0 \n");
    // }
    
    // if (rec_buf[4] =='1'){
    //     gpio_set_value(GPIO_19, 1);
    // }else if (rec_buf[4]=='0') {
    // //set the GPIO value to LOW
    //     gpio_set_value(GPIO_19, 0);
    // } 
    // else {
    //     pr_err("Unknown command : Please provide either 1 or 0 \n");
    // }

    // if (rec_buf[5] =='1'){
    //     gpio_set_value(GPIO_13, 1);
    // }else if (rec_buf[5]=='0') {
    // //set the GPIO value to LOW
    //     gpio_set_value(GPIO_13, 0);
    // } 
    // else {
    //     pr_err("Unknown command : Please provide either 1 or 0 \n");
    // }
    // if (rec_buf[6] == '1'){
    //     gpio_set_value(GPIO_6, 1);
    // }else if (rec_buf[6]== '0') {
    // //set the GPIO value to LOW
    //     gpio_set_value(GPIO_6, 0);
    // } 
    // else {
    //     pr_err("Unknown command : Please provide either 1 or 0 \n");
    // }
    // if (rec_buf[7] == '1'){
    //     gpio_set_value(GPIO_12, 1);
    // }else if (rec_buf[7]== '0') {
    // //set the GPIO value to LOW
    //     gpio_set_value(GPIO_12, 0);
    // } 
    // else {
    //     pr_err("Unknown command : Please provide either 1 or 0 \n");
    // }
 return len;
}
/*
** Module Init function
*/ 
static int __init etx_driver_init(void)
{
 /*Allocating Major number*/
 if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
    pr_err("Cannot allocate major number\n");
 goto r_unreg;
 }
 pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 /*Creating cdev structure*/
 cdev_init(&etx_cdev,&fops);
 /*Adding character device to the system*/
 if((cdev_add(&etx_cdev,dev,1)) < 0){
    pr_err("Cannot add the device to the system\n");
 goto r_del;
 }
 /*Creating struct class*/
 if((dev_class = class_create(THIS_MODULE,"etx_class")) == NULL){
    pr_err("Cannot create the struct class\n");
 goto r_class;
 }
 /*Creating device*/
 if((device_create(dev_class,NULL,dev,NULL,"etx_device")) == NULL){
    pr_err( "Cannot create the Device \n");
 goto r_device;
 }
 
 //Checking the GPIO is valid or not
 if(gpio_is_valid(GPIO_21) == false){
    pr_err("GPIO %d is not valid\n", GPIO_21);
 goto r_device;
 }

 if(gpio_is_valid(GPIO_16) == false){
    pr_err("GPIO %d is not valid\n", GPIO_16);
 goto r_device;
 }
 if(gpio_is_valid(GPIO_20) == false){
 pr_err("GPIO %d is not valid\n", GPIO_20);
 goto r_device;
 }
 if(gpio_is_valid(GPIO_26) == false){
 pr_err("GPIO %d is not valid\n", GPIO_26);
 goto r_device;
 }
 //Requesting the GPIO
 if(gpio_request(GPIO_21,"GPIO_21") < 0){
    pr_err("ERROR: GPIO %d request\n", GPIO_21);
 goto r_gpio;
 }
 if(gpio_request(GPIO_20,"GPIO_20") < 0){
     pr_err("ERROR: GPIO %d request\n", GPIO_20);
 goto r_gpio;
 }
 if(gpio_request(GPIO_16,"GPIO_16") < 0){
    pr_err("ERROR: GPIO %d request\n", GPIO_16);
 goto r_gpio;
 }
 if(gpio_request(GPIO_26,"GPIO_26") < 0){
    pr_err("ERROR: GPIO %d request\n", GPIO_16);
 goto r_gpio;
 }
 //configure the GPIO as output
    gpio_direction_output(GPIO_21, 0);
    gpio_direction_output(GPIO_20, 0);
    gpio_direction_output(GPIO_16, 0);
    gpio_direction_output(GPIO_26, 0);
    gpio_direction_output(GPIO_19, 0);
    gpio_direction_output(GPIO_13, 0);
    gpio_direction_output(GPIO_6, 0);
    gpio_direction_output(GPIO_12, 0);
    gpio_direction_output(GPIO_5, 0);
    gpio_direction_output(GPIO_0, 0);
    gpio_direction_output(GPIO_1, 0);
    gpio_direction_output(GPIO_7, 0);
    gpio_direction_output(GPIO_8, 0);
    gpio_direction_output(GPIO_11, 0);
    gpio_direction_output(GPIO_25, 0);

 /* Using this call the GPIO 21 will be visible in /sys/class/gpio/
 ** Now you can change the gpio values by using below commands also.
 ** echo 1 > /sys/class/gpio/gpio21/value (turn ON the LED)
 ** echo 0 > /sys/class/gpio/gpio21/value (turn OFF the LED)
 ** cat /sys/class/gpio/gpio21/value (read the value LED)
 ** 
 ** the second argument prevents the direction from being changed.
 */
 gpio_export(GPIO_21, false);
  
 pr_info("Device Driver Insert...Done!!!\n");
 return 0;
r_gpio:
 gpio_free(GPIO_21);
 gpio_free(GPIO_20);
 gpio_free(GPIO_16);
 gpio_free(GPIO_26);
r_device:
 device_destroy(dev_class,dev);
r_class:
 class_destroy(dev_class);
r_del:
 cdev_del(&etx_cdev);
r_unreg:
 unregister_chrdev_region(dev,1);
 
 return -1;
}
/*
** Module exit function
*/ 
static void __exit etx_driver_exit(void)
{
    gpio_unexport(GPIO_21);
    gpio_free(GPIO_21);
    gpio_unexport(GPIO_20);
    gpio_free(GPIO_20);
    gpio_unexport(GPIO_16);
    gpio_free(GPIO_16);
    gpio_unexport(GPIO_26);
    gpio_free(GPIO_26);
    gpio_unexport(GPIO_19);
    gpio_free(GPIO_19);
    gpio_unexport(GPIO_13);
    gpio_free(GPIO_13);
    gpio_unexport(GPIO_6);
    gpio_free(GPIO_6);
    gpio_unexport(GPIO_12);
    gpio_free(GPIO_12);
    gpio_unexport(GPIO_5);
    gpio_free(GPIO_5);
    gpio_unexport(GPIO_0);
    gpio_free(GPIO_0);
    gpio_unexport(GPIO_1);
    gpio_free(GPIO_1);
    gpio_unexport(GPIO_7);
    gpio_free(GPIO_7);
    gpio_unexport(GPIO_8);
    gpio_free(GPIO_8);
    gpio_unexport(GPIO_11);
    gpio_free(GPIO_11);
    gpio_unexport(GPIO_25);
    gpio_free(GPIO_25);

    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Device Driver Remove...Done!!\n");
}
module_init(etx_driver_init);
module_exit(etx_driver_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("A simple device driver - GPIO Driver");
MODULE_VERSION("1.32");
