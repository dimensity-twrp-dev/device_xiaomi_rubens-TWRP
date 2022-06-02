/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define LOG_TAG "android.hardware.boot@1.1-mtkimpl"

#include <memory>

#include <log/log.h>
#include <android-base/logging.h>
#include <bootloader_message/bootloader_message.h>

#include "BootControl.h"
#include "boot_region_control_private.h"
#include "boot_control_definition.h"

namespace android {
namespace hardware {
namespace boot {
namespace V1_1 {
namespace implementation {

using ::android::hardware::boot::V1_0::CommandResult;
using namespace android::bootable;

bool BootControl::Init() {
    return impl_.Init();
}

// Methods from ::android::hardware::boot::V1_0::IBootControl follow.
Return<uint32_t> BootControl::getNumberSlots() {
    return impl_.GetNumberSlots();
}

Return<uint32_t> BootControl::getCurrentSlot() {
    return impl_.GetCurrentSlot();
}

Return<void> BootControl::clearAvbbctlFlag() {
  std::string err;
  std::string device = get_bootloader_message_blk_device(&err);
  if (device.empty()) {
    LOG(ERROR) << "Could not find bootloader message block device: " << err;
    return Void();
  }

  bootloader_control boot_ctrl;
  if (!LoadBootloaderControl(device, &boot_ctrl)) {
    LOG(ERROR) << "Failed to load bootloader control block";
    return Void();
  }

  uint32_t computed_crc32 = BootloaderControlLECRC(&boot_ctrl);
  if (boot_ctrl.crc32_le == computed_crc32 &&  boot_ctrl.reserved1[0] == 1) {
    boot_ctrl.reserved1[0] = 0;
    LOG(INFO) << "Clear avb boot control convert flag";
    UpdateAndSaveBootloaderControl(device, &boot_ctrl);
  }
  return Void();
}

Return<void> BootControl::markBootSuccessful(markBootSuccessful_cb _hidl_cb) {
    struct CommandResult cr;
    if (impl_.MarkBootSuccessful()) {
        cr.success = true;
        cr.errMsg = "Success";
        clearAvbbctlFlag();
    } else {
        cr.success = false;
        cr.errMsg = "Operation failed";
    }
    _hidl_cb(cr);
    return Void();
}

Return<void> BootControl::setActiveBootSlot(uint32_t slot, setActiveBootSlot_cb _hidl_cb) {
    struct CommandResult cr;
    if (impl_.SetActiveBootSlot(slot) && implext_.SetBootRegionSlot(slot)) {
        cr.success = true;
        cr.errMsg = "Success";
    } else {
        cr.success = false;
        cr.errMsg = "Operation failed";
    }
    _hidl_cb(cr);
    return Void();
}

Return<void> BootControl::setSlotAsUnbootable(uint32_t slot, setSlotAsUnbootable_cb _hidl_cb) {
    struct CommandResult cr;
    if (impl_.SetSlotAsUnbootable(slot)) {
        cr.success = true;
        cr.errMsg = "Success";
    } else {
        cr.success = false;
        cr.errMsg = "Operation failed";
    }
    _hidl_cb(cr);
    return Void();
}

Return<BoolResult> BootControl::isSlotBootable(uint32_t slot) {
    if (!impl_.IsValidSlot(slot)) {
        return BoolResult::INVALID_SLOT;
    }
    return impl_.IsSlotBootable(slot) ? BoolResult::TRUE : BoolResult::FALSE;
}

Return<BoolResult> BootControl::isSlotMarkedSuccessful(uint32_t slot) {
    if (!impl_.IsValidSlot(slot)) {
        return BoolResult::INVALID_SLOT;
    }
    return impl_.IsSlotMarkedSuccessful(slot) ? BoolResult::TRUE : BoolResult::FALSE;
}

Return<void> BootControl::getSuffix(uint32_t slot, getSuffix_cb _hidl_cb) {
    hidl_string ans;
    const char* suffix = impl_.GetSuffix(slot);
    if (suffix) {
        ans = suffix;
    }
    _hidl_cb(ans);
    return Void();
}

Return<bool> BootControl::setSnapshotMergeStatus(MergeStatus status) {
    return impl_.SetSnapshotMergeStatus(status);
}

Return<MergeStatus> BootControl::getSnapshotMergeStatus() {
    return impl_.GetSnapshotMergeStatus();
}

IBootControl* HIDL_FETCH_IBootControl(const char* /* hal */) {
    auto module = std::make_unique<BootControl>();
    if (!module->Init()) {
        ALOGE("Could not initialize BootControl module");
        return nullptr;
    }
    return module.release();
}

}  // namespace implementation
}  // namespace V1_1
}  // namespace boot
}  // namespace hardware
}  // namespace android
