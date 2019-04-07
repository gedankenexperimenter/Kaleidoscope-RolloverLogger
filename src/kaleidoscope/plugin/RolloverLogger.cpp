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

#include <Kaleidoscope-RolloverLogger.h>
#include <Kaleidoscope.h>


namespace kaleidoscope {
namespace plugin {

constexpr byte invalid_addr{0xFF};

EventHandlerResult RolloverLogger::onKeyswitchEvent(Key& key,
                                                    byte row,
                                                    byte col,
                                                    byte key_state) {

  // Ignore everything that's not a physical keypress
  if (row >= ROWS || col >= COLS || (key_state & INJECTED) != 0)
    return EventHandlerResult::OK;

  if (detail_level_ == 0) return EventHandlerResult::OK;

  if (keyToggledOn(key_state)) {
    byte event_addr = addr::addr(row, col);

    for (KeyPress& key_press : key_presses_) {
      if (key_press.addr == event_addr) {
        // This key is actually already pressed.
        break;
      }
      if (key_press.addr == invalid_addr) {
        key_press.addr = event_addr;
        key_press.time = Kaleidoscope.millisAtCycleStart();
        break;
      }
    }
  }

  if (keyToggledOff(key_state)) {
    byte event_addr = addr::addr(row, col);

    for (KeyPress& key_press : key_presses_) {
      if (key_press.addr != event_addr) {
        continue;
      }

      if ((key.flags & (SYNTHETIC | RESERVED)) == 0) {
        // This key is a keyboard key
        if (key.keyCode >= HID_KEYBOARD_FIRST_MODIFIER &&
            key.keyCode <= HID_KEYBOARD_LAST_MODIFIER) {
          Serial.print(F("M"));  // "modifier"
        } else {
          Serial.print(F("S"));  // "standard"
        }
      } else if (key.flags == (SYNTHETIC | SWITCH_TO_KEYMAP) &&
                 key.keyCode >= LAYER_SHIFT_OFFSET) {
        Serial.print(F("M"));  // layer shift => "modifier"
      } else {
        Serial.print(F("S"));  // "other"
      }

      if (detail_level_ > 1) {
        Serial.print(F(","));
        if (col == 7 || col == 8) {
          Serial.print(F("t"));  // "thumb"
        } else if (event_addr == 54 || event_addr == 57) {
          Serial.print(F("p"));  // "palm"
        } else {
          Serial.print(F("f"));  // "finger"
        }
      }

      if (detail_level_ > 2) {
        Serial.print(F(","));
        Serial.print(event_addr, HEX);
        Serial.print(F(","));
        Serial.print(row, DEC);
        Serial.print(F(","));
        Serial.print(col, DEC);
      }

      if (detail_level_ > 3) {
        Serial.print(F(","));
        Serial.print(key.raw, HEX);
      }

      Serial.print(F("\t"));
      Serial.print(key_press.time, DEC);
      Serial.print(F("\t"));
      uint16_t current_time = Kaleidoscope.millisAtCycleStart();
      Serial.print(current_time, DEC);
      Serial.print(F("\t"));
      uint16_t elapsed_time = current_time - key_press.time;
      Serial.println(elapsed_time, DEC);

      // Make this key press entry available.
      key_press.addr = invalid_addr;
    }
  }

  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::RolloverLogger RolloverLogger;
