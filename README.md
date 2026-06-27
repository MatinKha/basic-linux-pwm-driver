# Linux Virtual PWM Driver

A simple virtual PWM driver for the Linux kernel. this demonstrates you can implement a PWM module using the Linux PWM framework.

## Requirements

* Linux kernel headers
* GNU Make
* GCC

Install the kernel headers for your running kernel.

### Debian

```bash
sudo apt update
sudo apt install build-essential linux-headers-$(uname -r)
```

### Fedora

```bash
sudo dnf install kernel-devel kernel-headers gcc make
```

### Arch

```bash
sudo pacman -S base-devel linux-headers
```

## Requirements

## Build

```bash
make
```

## Load Module

```bash
make load
```

manually:

```bash
sudo insmod pwm.ko
```

## Unload Module

```bash
sudo rmmod pwm
```

## Notes

if the module is loaded it should be visible in sysfs tree
```bash
ls /sys/class/pwm
```
