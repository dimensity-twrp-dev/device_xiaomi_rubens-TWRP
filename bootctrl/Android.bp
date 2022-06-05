//
// Copyright (C) 2018 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

cc_library_shared {
    name: "android.hardware.boot@1.1-mtkimpl",
    stem: "android.hardware.boot@1.0-impl-1.1-mtkimpl",
    defaults: [
        "hidl_defaults",
        "libboot_control_defaults",
    ],
    relative_install_path: "hw",
    vendor: true,
    recovery_available: true,
    srcs: ["BootControl.cpp", "boot_region_control.cpp"],

    header_libs: ["device_kernel_headers"],

    shared_libs: [
        "liblog",
        "libhidlbase",
        "libhardware",
        "libutils",
        "android.hardware.boot@1.0",
        "android.hardware.boot@1.1",
    ],
    static_libs: [
        "libboot_control",
        "libfstab",
    ],
}
