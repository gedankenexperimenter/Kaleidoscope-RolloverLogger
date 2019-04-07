# Kaleidoscope-RolloverLogger

A data collection tool for a research project into typing habits. Primarily this
is intended to measure differences in how people use chorded modifier keys in
order to improve Qukeys for some people.

It is basically a very limited keystroke logger. It works by reporting
information about what type of key was pressed (modifier, "standard", or
"other"), and two timestamps showing when the key was pressed and when it was
released. Importantly, it does not report exactly which key was pressed (neither
the keycode nor the precise key position is reported), so it should be very
difficult to extract any personal information from the recorded data.

## Using the plugin

No configuration is necessary. Simply include the header and add the plugin to
the plugin initialization list:

```c++
#include <Kaleidoscope-RolloverLogger.h>

KALEIDOSCOPE_INIT_PLUGINS(
  RolloverLogger,
  ...
)
```

**Important note**: For the rollover logger to work properly, it should be the
first plugin in the parameter list for `KALEIDOSCOPE_INIT_PLUGINS`, so that it
gets accurate data about key press and release times. It should not interfere
with the operation of any other plugins.
