Version History
===============

0.2.0 (Unreleased)
------------------


0.1.1 (2020-12-13)
------------------
### Added
 - Analog stick values should now be accurate to the range of N64 
   controllers

### Changed
 - Deadzone has been lowered to 5%
 - Trigger and joystick activation thresholds for digital outputs have 
   been lowered to 25%
 - `gamecontrollerdb.txt` has been updated to the latest version

### Fixed
 - Analog values in the deadzone range will no longer be cut off and are 
   instead scaled
 - Controllers no longer disconnect only to attempt to reconnect in 
   certain situations (such as loading save states)

0.1.0 (2020-10-27)
------------------
### Added
 - Initial release
