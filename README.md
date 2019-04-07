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

## Information security issues

**This plugin is a keylogger**. I don't recommend including it in your firmware
as a matter of course. If you want to contribute some data to my research
project, or analyze it privately on your own, feel free, but be aware that it
can send information about what you type (depending on your settings, all of the
information) to the keyboard's serial port, and that would bypass any
protections your OS has for things like password entry. I strongly recommend
only using this plugin for a short time to collect some data while typing things
that you are comfortable with sharing publicly. The default settings drop most
of the specific data, so if you send that information to me, I _won't_ see
actual characters typed, but even so, please don't type your passwords while
this is turned on.

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

RolloverLogger can be turned off by calling its `deactivate()` method, which can
be done using the Macros plugin (among others). It can be re-activated with a
call to `activate()`.

## Output

To read the output, you'll need to connect a process on your host machine to the
serial port device used by the keyboard. This can be done with the Arduino IDE's
serial monitor, but that's not a convenient way to log the output to a file. On
UNIX-like systems, Kaleidoscope comes with a script that should be able to find
the correct port name, and the simplest way to get the output is to use
`cat`. For example:

```bash
cat $(Kaleidoscope/bin/find-device-port-linux-udev) | tee /tmp/RolloverLogger.log
```

### Output format

Each line of the output represents a single key press & release. First is a
single character indicating the type of key that was pressed:

|code |description|
|-----|-----------|
| `C` |printable character|
| `M` |modifier (including layer shifts)|
| `O` |other|

Following that are three numbers: a timestamp indicating when the key was
pressed, another timestamp indicating that key's release, and finally the
duration. All three are 16-bit integers in milliseconds, so as long as you don't
hold any key longer than a minute, overflow won't cause any erroneous data.

It is not likely that the original characters typed can be reconstructed from a
log of this data, but I don't recommend typing passwords while this plugin is
active, regardless.

## Optional details

If you want to share more detail, you can configure RolloverLogger to also
record which type of physical key (on the Keyboardio Model01) a keypress
corresponded to. This will include letter code in the report indicating if a key
was a palm key (`p`), a thumb key (`t`), or a finger key (`f`). This level of
detail can be activated by adding the following line in the `setup()` function
in your sketch (e.g. `Model01-Firmware.ino`):

```c++
void setup() {
  RolloverLogger.reportCategories();
  ...
}
```

This can also be done via Macros, of course. The extra information included from
this might be useful, and the keystrokes are still reasonably
anonymous. However, if you use a fairly standard keymap, it would not be very
difficult to identify which keypresses are the `space` key if this logging level
is used, and that could be used along with the timing information to potentially
reconstruct at least some of what was typed. *You have been warned*.

### Unsafe details

If you really want to use this as a keylogger, you can. There are two more
levels of logging, both of which have much more obvious security implications,
and should only be used with great care. If you want to log the actual key
coordinates (row & column) for each key:

```c++
RolloverLogger.reportCoordinates();
```

Be very careful if you turn this on. It's not at all safe, and even if an
attacker doesn't know your keymap, it would still not be very difficult to
decode the text that was typed with this logging level turned on, especially if
you record a lot of typing.

And if you want to log (almost) everything:

```c++
RolloverLogger.reportUnsafe();
```

That will also include the `Key` value looked up from the keymap with every
line. Don't type passwords with this turned on. I wouldn't use this setting for
anything but typing practice.
