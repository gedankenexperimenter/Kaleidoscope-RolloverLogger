/* -*- mode: c++ -*-
 * Kaleidoscope-RolloverLogger
 * Copyright (C) 2019  Michael Richters
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <Kaleidoscope.h>

#include <kaleidoscope/addr.h>

namespace kaleidoscope {
namespace plugin {

struct KeyPress {
  byte     addr;
  uint16_t time;
};

class RolloverLogger : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onKeyswitchEvent(Key& key, byte row, byte col, byte state);

 private:
  static constexpr byte max_key_presses_{8};
  KeyPress              key_presses_[max_key_presses_];
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::RolloverLogger RolloverLogger;
