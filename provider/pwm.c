#include "asm/io.h"
#include "linux/cdev.h"
#include "linux/export.h"
#include "linux/kern_levels.h"
#include "linux/moduleparam.h"
#include "linux/printk.h"
#include "virtual_pwm.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/pwm.h>
#define PWM_PROVIDER "virtual_provider"

static struct pwm_chip *chip;
static struct device dev;

static int __init pwm_init(void) {
  int ret;

  // we need to define a device that pwm chip is part of
  device_initialize(&dev);
  dev.init_name = "virtual-pwm";
  // before we use this device we have to add it to the kernel
  ret = device_add(&dev);
  if (ret)
    return ret;

  // allocating a pwm on the kernel memory
  chip = pwmchip_alloc(&dev, 1, 0);
  if (!chip)
    return -ENOMEM;

  chip->ops = &pwm_ops;

  // add thing the pwm chip to the kernel as a module
  // you should be able to find this module on sysfs after this
  ret = pwmchip_add(chip);
  if (ret) {
    pwmchip_put(chip);
    device_del(&dev);
    put_device(&dev);
    return ret;
  }

  pr_info("PWM chip registered\n");

  return 0;
}

static void __exit pwm_exit(void) {
  // CLEAN UP
  pwmchip_remove(chip);
  pwmchip_put(chip);

  device_del(&dev);
  put_device(&dev);
}

module_init(pwm_init);
module_exit(pwm_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("virtual PWM provider");
