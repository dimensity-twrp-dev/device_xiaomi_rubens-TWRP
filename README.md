### TWRP device tree for Redmi K50 (rubens)

=========================================

The Redmi K50 (codenamed _"rubens"_) is a high-end, mid-range smartphone from Xiaomi.

It was released in March 2022.

## Device specifications

Basic   | Spec Sheet
-------:|:-------------------------
CPU     | Octa-core CPU with 4x Arm Cortex-A78 up to 2.85GHz
Chipset | Mediatek Dimensity 8100
GPU     | Mali-G610 MC6
Memory  | 8/12 GB RAM (LPDDR5 6400Mbps)
Shipped Android Version | 12
Storage | 128/256/512 GB (UFS 3.1)
Battery | Li-Po 5500 mAh, non-removable
Display | 1440 x 3200 pixels, 6.67 inches, 60/120 hz

![Redmi K50](https://cdn.cnbj0.fds.api.mi-img.com/b2c-shopapi-pms/pms_1653381863.47942179.png)

## Features

Works:

- [X] ADB
- [X] Decryption (Android 13)
- [X] Display
- [X] Fasbootd
- [X] Flashing
- [X] MTP
- [X] Sideload
- [X] USB OTG
- [X] Vibrator

## Compile

First checkout minimal twrp with aosp tree:

```
repo init --depth=1 -u https://github.com/minimal-manifest-twrp/platform_manifest_twrp_aosp.git -b twrp-12.1
repo sync -j$(nproc --all)
```

Then add these projects to .repo/manifest.xml:

```xml
<project path="device/xiaomi/rubens" name="D8100-9000-TWRP-Device-Tree/device_xiaomi_rubens-TWRP" remote="github" revision="twrp-13" />
```

Finally execute these:

```
source build/envsetup.sh
repopick <needed patch>
lunch twrp_rubens-eng
mka vendorbootimage -j$(nproc --all)
```
## To use it:

```
fastboot flash vendor_boot out/target/product/rubens/vendor_boot.img
```
