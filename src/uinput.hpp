/* 
**  Xbox360 USB Gamepad Userspace Driver
**  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HEADER_UINPUT_HPP
#define HEADER_UINPUT_HPP

#include <vector>
#include <memory>
#include "xboxdrv.hpp"
#include "linux_uinput.hpp"

class Xbox360Msg;
class Xbox360GuitarMsg;
class XboxMsg;

struct Event
{
  static Event create(int type, int code)
  {
    Event ev;
    ev.type = type;
    ev.code = code;
    return ev;
  }

  static Event invalid;

  /** EV_KEY, EV_ABS, EV_REL */
  int type;

  /** BTN_A, REL_X, ABS_X, ... */
  int code;
};

inline bool operator!=(const Event& lhs, const Event& rhs)
{
  return lhs.type != rhs.type || lhs.code != rhs.code;
}

class uInputCfg
{
public:
  bool trigger_as_button;
  bool dpad_as_button;
  bool trigger_as_zaxis;
  bool dpad_only;
  bool force_feedback;

  Event btn_map[XBOX_BTN_MAX];
  Event axis_map[XBOX_AXIS_MAX];

  uInputCfg();
};

class uInput
{
private:
  std::auto_ptr<LinuxUinput> joystick_uinput;
  std::auto_ptr<LinuxUinput> keyboard_uinput;
  std::auto_ptr<LinuxUinput> mouse_uinput;
  uInputCfg cfg;

  int  axis_state[XBOX_AXIS_MAX];
  bool button_state[XBOX_BTN_MAX];

  std::vector<int> rel_axis;

public:
  uInput(GamepadType type, uInputCfg cfg = uInputCfg());
  ~uInput();

  void setup_xbox360_gamepad(GamepadType type);
  void setup_xbox360_guitar();

  void send(XboxGenericMsg& msg); 
  void send(Xbox360Msg& msg);
  void send(Xbox360GuitarMsg& msg);
  void send(XboxMsg& msg);

  void add_axis(int code, int min, int max);
  void add_button(int code);

  void send_button(int code, bool value);
  void send_axis(int code, int32_t value);

  void update(float delta);
};

#endif

/* EOF */
