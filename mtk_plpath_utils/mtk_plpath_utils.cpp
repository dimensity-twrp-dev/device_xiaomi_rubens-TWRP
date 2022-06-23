#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <chrono>
#include <ctime>
#include <iostream>
#include <map>
#include <thread>

#include <android-base/file.h>
#include <android-base/strings.h>
#include <android-base/unique_fd.h>
#include <libdm/dm.h>
#include <log/log.h>

using namespace std::literals::string_literals;
using namespace android::dm;

#define NAME_PL_A "pl_a"
#define NAME_PL_B "pl_b"

#define UFS_PL_A "/dev/block/sda"
#define UFS_PL_B "/dev/block/sdb"
#define UFS_DEV "/sys/class/block/sda/uevent"
#define LINK_PL_A "/dev/block/by-name/preloader_raw_a"
#define LINK_PL_B "/dev/block/by-name/preloader_raw_b"
#define LINK1_PL_A "/dev/block/platform/bootdevice/by-name/preloader_raw_a"
#define LINK1_PL_B "/dev/block/platform/bootdevice/by-name/preloader_raw_b"
#define DM_BLK_SIZE (512)

#define PLHEAD "MMM"
#define UFSHEAD "UFS"
#define EMMCHEAD "EMMC"
#define COMBOHEAD "COMB"
#define EMMCHSZ (0x800)
#define UFSHSZ (0x1000)
#define BLKSZ (512)

static int create_dm(const char* device, const char* name, std::string* path, int start_blk,
                     int blk_cnt) {
  DmTable table;
  std::unique_ptr<DmTarget> target;

  if (!device || !name) {
    ALOGE("%s device or name is null\n", __func__);
    return 1;
  }

  ALOGD("create_dm dev: %s, name %s, start %d, blks %d\n", device, name, start_blk, blk_cnt);
  target = std::make_unique<DmTargetLinear>(0, blk_cnt, device, start_blk);
  if (!table.AddTarget(std::move(target))) {
    ALOGE("Add target fail(%s)", strerror(errno));
    return 1;
  }
  DeviceMapper& dm = DeviceMapper::Instance();
  if (!dm.CreateDevice(name, table, path, std::chrono::milliseconds(500))) {
    ALOGE("Create %s on %s fail(%s)", name, device, strerror(errno));
    return 1;
  }
  ALOGI("Create %s done", (*path).c_str());
  return 0;
}

static void create_pl_link(std::string link, std::string devpath) {
  std::string link_path;

  if (android::base::Readlink(link, &link_path) && link_path != devpath) {
    ALOGE("Remove symlink %s links to: %s", link.c_str(), link_path.c_str());
    if (!android::base::RemoveFileIfExists(link))
      ALOGE("Cannot remove symlink %s", strerror(errno));
  }

  if (symlink(devpath.c_str(), link.c_str()))
    ALOGE("Failed to symlink %s to %s (%s)", devpath.c_str(), link.c_str(), strerror(errno));
}

int create_pl_path(void) {
  int start_blk, blk_cnt, fd;
  off_t pl_size;
  char header_desc[5];
  std::string path_a, path_b, link_path, dev_path, link;
  DeviceMapper& dm = DeviceMapper::Instance();
  ssize_t sz = 0;

  
  fd = open(UFS_PL_A, O_RDONLY);
  if (fd < 0) {
    ALOGE("Cannot open %s (%s)", UFS_PL_A, strerror(errno));
    return 1;
  }

  pl_size = lseek(fd, 0, SEEK_END);
  if (pl_size < 0) {
    ALOGE("lseek fail (%s)", strerror(errno));
    close(fd);
    return 1;
  }

  ALOGD("pl_size: %ld\n", pl_size);
  blk_cnt = pl_size / DM_BLK_SIZE;

  if (lseek(fd, 0, SEEK_SET)) {
    ALOGE("lseek to head fail(%s)\n", strerror(errno));
    close(fd);
    return 1;
  }

  if ((sz = read(fd, header_desc, sizeof(header_desc))) < 0) {
    ALOGE("read fail(%s)", strerror(errno));
    close(fd);
    return 1;
  }
  if (sz != sizeof(header_desc)) ALOGE("%s size is not header_desc\n", __func__);

  close(fd);

  header_desc[sizeof(header_desc) - 1] = 0;
  if (!strncmp(header_desc, EMMCHEAD, strlen(EMMCHEAD))) {
    start_blk = EMMCHSZ / BLKSZ;
  } else if (!strncmp(header_desc, UFSHEAD, strlen(UFSHEAD)) ||
             !strncmp(header_desc, COMBOHEAD, strlen(COMBOHEAD))) {
    start_blk = UFSHSZ / BLKSZ;
  } else {
    ALOGE("Invalid header %s", header_desc);
    return 1;
  }

  blk_cnt -= start_blk;
  
  if (create_dm(UFS_PL_A, NAME_PL_A, &path_a, start_blk, blk_cnt) != 0) {
    return 1;
  }
  if (create_dm(UFS_PL_B, NAME_PL_B, &path_b, start_blk, blk_cnt) != 0) {
    if (dm.DeleteDevice(UFS_PL_A))
      ALOGE("Cannot delete device %s (%s)", NAME_PL_A, strerror(errno));
    return 1;
  }
  

  create_pl_link(LINK_PL_A, path_a);
  create_pl_link(LINK_PL_B, path_b);
  create_pl_link(LINK1_PL_A, path_a);
  create_pl_link(LINK1_PL_B, path_b);
  return 0;
}

int main(void) {
  return create_pl_path();
}
