Change log for PC Engines fork of SeaBIOS
=========================================

Fourth digit in release number means PC Engines patch.

## [Unreleased]
## [rel-1.14.0.1] - 2021-05-24
### Changed
- rebased on SeaBIOS official repository commit b0d61ec

### Added
- coreboot flashmap (FMAP) driver
- coreboot VPD driver for runtime configuration
- support for persistent bootorder in FMAP region
- TPM success status reporting after TPM Clear operation

## [rel-1.12.1.3] - 2019-07-05
### Fixed
- some corner cases for boot fallback functionality:
    - CBFS payloads were still booting when no HD media found (fixed)
    - CBFS payloads can be loaded only explicitly when entered to boot menu
      with F10 key
    - if no HD media present and PXE is disabled, after boot menu wait timeout
      SeaBIOS will print that no bootable device has been found and will reboot
      after 60 seconds, unless entered to boot menu to choose a CBFS payload

### Changed
- removed obsolete and unused code implemented by previous maintainers in older
  SeaBIOS versions

## [rel-1.12.1.2] - 2019-06-04
### Changed
- increased maximum timeout values in SD interface to correctly detect and
  communicate with SD cards in SD 3.0 mode

### Added
- hard disk fallback boot functionality:
    - SeaBIOS will now loop through all detected hard disks (SATA, mSATA, USB,
      SD) according to bootorder and try to boot from it;
    - SeaBIOS will try next device only if MBR of the current drive    is not
      valid, i.e. the current device is not bootable
    - secondary payloads (memtest, sortbootorder) will not be loaded
      automatically, but only when desired (user has to enter boot menu with
      F10 key and choose the payload explicitly)

## [rel-1.12.1.1] - 2019-04-03
### Changed
- rebased on SeaBIOS official repository commit f4c6e4c

### Added
- [rebase introduced new TPM menu option to enable/disable PCR banks](https://github.com/pcengines/apu2-documentation/blob/master/docs/tpm_menu.md)

## [rel-1.12.0.1] - 2019-01-09
### Changed
- rebased on SeaBIOS rel-1.12.0 stable release

## [rel-1.11.0.7] - 2018-12-03
### Added
- enabled COM2 redirection option

## [rel-1.11.0.6] - 2018-11-07
### Fixed
- e820 memory table that caused errors in 2GB boards in memtest86+

## [rel-1.11.0.5] - 2018-06-08
### Fixed
- USB timings

## [rel-1.11.0.4] - 2018-04-06
### Fixed
- links to comparison between tags

## [rel-1.11.0.3] - 2018-01-31
### Fixed
- timeout issues with USB 3.x

### Added
- support for serial console disabling runtime config
- ATA UDMA enabled

## [rel-1.11.0.2] - 2017-12-22
### Fixed
- bug with serial console printing

## [rel-1.11.0.1] - 2017-11-30
### Changed
- rebased to 1.11.0 stable mainline
- removed SgaBIOS option rom enable/disable sortbootorder config handling

## [rel-1.10.2.1] - 2017-03-31
### Changed
- rebased to 1.10.2 stable mainline

## [rel-1.10.0.1] - 2017-02-23
### Changed
- rebased to 1.10.0 mainline
- added SgaBios option rom enable/disable sortbootorder config handling

## [rel-1.9.2.4] - 2017-01-23
### Changed
- change debug output of version string to show, when using the sgabios and
  also serial output

## [rel-1.9.2.3] - 2017-01-03
### Fixed
- fix the issue that 2 banner messages with version are shown on
  default debug level

## [rel-1.9.2.2] - 2016-12-13
### Changed
- simplify version displayed during boot

### Fixed
- remove screen control character by disable screen cleaning when loading
  sgabios

## [rel-1.9.2.1] - 2016-10-04
### Added
- allowed for one-time `PXE ` boot with `N` key
- enable/disable option for `USB` boot
- enable/disable option for `PXE` boot

### Fixed
- prevented from printing character multiple times

[Unreleased]: https://github.com/pcengines/seabios/compare/rel-1.12.1.3...apu_support
[rel-1.12.1.3]: https://github.com/pcengines/seabios/compare/rel-1.12.1.2...rel-1.12.1.3
[rel-1.12.1.2]: https://github.com/pcengines/seabios/compare/rel-1.12.1.1...rel-1.12.1.2
[rel-1.12.1.1]: https://github.com/pcengines/seabios/compare/rel-1.12.0.1...rel-1.12.1.1
[rel-1.12.0.1]: https://github.com/pcengines/seabios/compare/rel-1.11.0.7...rel-1.12.0.1
[rel-1.11.0.7]: https://github.com/pcengines/seabios/compare/rel-1.11.0.6...rel-1.11.0.7
[rel-1.11.0.6]: https://github.com/pcengines/seabios/compare/rel-1.11.0.5...rel-1.11.0.6
[rel-1.11.0.5]: https://github.com/pcengines/seabios/compare/rel-1.11.0.4...rel-1.11.0.5
[rel-1.11.0.4]: https://github.com/pcengines/seabios/compare/rel-1.11.0.3...rel-1.11.0.4
[rel-1.11.0.3]: https://github.com/pcengines/seabios/compare/rel-1.11.0.2...rel-1.11.0.3
[rel-1.11.0.2]: https://github.com/pcengines/seabios/compare/rel-1.11.0.1...rel-1.11.0.2
[rel-1.11.0.1]: https://github.com/pcengines/seabios/compare/rel-1.10.2.1...rel-1.11.0.1
[rel-1.10.2.1]: https://github.com/pcengines/seabios/compare/rel-1.10.0.1...rel-1.10.2.1
[rel-1.10.0.1]: https://github.com/pcengines/seabios/compare/rel-1.9.2.4...rel-1.10.0.1
[rel-1.9.2.4]: https://github.com/pcengines/seabios/compare/rel-1.9.2.3...rel-1.9.2.4
[rel-1.9.2.3]: https://github.com/pcengines/seabios/compare/rel-1.9.2.2...rel-1.9.2.3
[rel-1.9.2.2]: https://github.com/pcengines/seabios/compare/rel-1.9.2.1...rel-1.9.2.2
[rel-1.9.2.1]: https://github.com/pcengines/seabios/compare/rel-1.9.2...rel-1.9.2.1
