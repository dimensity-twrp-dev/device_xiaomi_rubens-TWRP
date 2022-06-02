# device_xiaomi_rubens-TWRP


[WIP] 注意：本项目编译出的 vendor_boot 并不能成功启动，且运气不好可能导致成砖！

(至少 22.05.30 的提交不能，因为手机寄了，所以暂时无法继续测试了)

目前已知情况：

```
出厂 Android 12

使用 Boot Header Version 4

ramdisk 使用 lz4-l 压缩算法

DTB 在 vendor_boot 内，KERNEL 在 boot 里

RECOVERY 资源 在 vendor_boot 的 Ramdisk 里

DTBO 是分离的单独分区

所以内核应属于 GKI 2.0 产品
```

从文档可知：

```
vendor_boot 分区随 GKI 引入。该分区是采用虚拟 A/B 的 A/B 分区，包含一个头文件、供应商 ramdisk 和设备树 Blob。

供应商 ramdisk 是一个 CPIO 归档，其中包含设备启动所需的供应商模块。这包括用于启用关键 SoC 功能的模块，以及启动设备和显示启动画面所需的存储和显示驱动程序。

ramdisk 包含恢复资源。启动到恢复模式时，引导加载程序必须将这些 ramdisk 加载到内存中。

ramdisk 的 modules.load.recovery，用于指示要加载的模块及相应的加载顺序，用于 A/B 和虚拟 A/B 设备，位于 /lib/modules。
```

参考文档:

[供应商启动分区](https://source.android.com/devices/bootloader/partitions/vendor-boot-partitions)

[通用内核映像](https://source.android.com/devices/architecture/kernel/generic-kernel-image)

[内核模块支持](https://source.android.com/devices/architecture/kernel/kernel-module-support)
