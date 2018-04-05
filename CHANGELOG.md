Change log for PC Engines fork of SeaBIOS
=========================================

Fourth digit in release number means PC Engines patch.

## [Unreleased]

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

[Unreleased]: https://github.com/pcengines/seabios/compare/rel-1.11.0.4...apu_support
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
