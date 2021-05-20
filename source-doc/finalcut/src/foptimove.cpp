/***********************************************************************
* foptimove.cpp - Cursor movement optimization                         *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2015-2021 Markus Gans                                      *
*                                                                      *
* FINAL CUT is free software; you can redistribute it and/or modify    *
* it under the terms of the GNU Lesser General Public License as       *
* published by the Free Software Foundation; either version 3 of       *
* the License, or (at your option) any later version.                  *
*                                                                      *
* FINAL CUT is distributed in the hope that it will be useful, but     *
* WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU Lesser General Public License for more details.                  *
*                                                                      *
* You should have received a copy of the GNU Lesser General Public     *
* License along with this program.  If not, see                        *
* <http://www.gnu.org/licenses/>.                                      *
***********************************************************************/

#include <array>
#include <cstring>

#include "final/fapplication.h"
#include "final/fc.h"
#include "final/foptimove.h"
#include "final/ftermcap.h"

namespace finalcut
{

//----------------------------------------------------------------------
// class FOptiMove
//----------------------------------------------------------------------

// constructors and destructor
//----------------------------------------------------------------------
FOptiMove::FOptiMove (int baud)
  : baudrate{baud}
{
  assert ( baud >= 0 );

  calculateCharDuration();

  // ANSI set cursor address preset for undefined terminals
  set_cursor_address (CSI "%i%p1%d;%p2%dH");
  // Set carriage return preset
  set_carriage_return ("\r");
  // Set cursor down preset
  set_cursor_down ("\n");
}


// public methods of FOptiMove
//----------------------------------------------------------------------
void FOptiMove::setBaudRate (int baud)
{
  assert ( baud >= 0 );
  baudrate = baud;
  calculateCharDuration();
}

//----------------------------------------------------------------------
void FOptiMove::setTabStop (int t)
{
  assert ( t > 0 );
  tabstop = t;
}

//----------------------------------------------------------------------
void FOptiMove::setTermSize (std::size_t w, std::size_t h)
{
  assert ( w > 0 );
  assert ( h > 0 );
  screen_width = w;
  screen_height = h;
}

//----------------------------------------------------------------------
void FOptiMove::setTermEnvironment (const TermEnv& term_env)
{
  // Set all required termcap values at once

  set_cursor_home (term_env.t_cursor_home);
  set_cursor_to_ll (term_env.t_cursor_to_ll);
  set_carriage_return (term_env.t_carriage_return);
  set_tabular (term_env.t_tab);
  set_back_tab (term_env.t_back_tab);
  set_cursor_up (term_env.t_cursor_up);
  set_cursor_down (term_env.t_cursor_down);
  set_cursor_left (term_env.t_cursor_left);
  set_cursor_right (term_env.t_cursor_right);
  set_cursor_address (term_env.t_cursor_address);
  set_column_address (term_env.t_column_address);
  set_row_address (term_env.t_row_address);
  set_parm_up_cursor (term_env.t_parm_up_cursor);
  set_parm_down_cursor (term_env.t_parm_down_cursor);
  set_parm_left_cursor (term_env.t_parm_left_cursor);
  set_parm_right_cursor (term_env.t_parm_right_cursor);
  set_erase_chars (term_env.t_erase_chars);
  set_repeat_char (term_env.t_repeat_char);
  set_clr_bol (term_env.t_clr_bol);
  set_clr_eol (term_env.t_clr_eol);
  setTabStop (term_env.tabstop);
  set_auto_left_margin (term_env.automatic_left_margin);
  set_eat_newline_glitch (term_env.eat_nl_glitch);
}

//----------------------------------------------------------------------
void FOptiMove::set_cursor_home (const char cap[])
{
  if ( cap )
  {
    F_cursor_home.cap = cap;
    F_cursor_home.duration = capDuration (cap, 0);
    F_cursor_home.length = capDurationToLength (F_cursor_home.duration);
  }
  else
  {
    F_cursor_home.cap = nullptr;
    F_cursor_home.duration = \
    F_cursor_home.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_cursor_to_ll (const char cap[])
{
  if ( cap )
  {
    F_cursor_to_ll.cap = cap;
    F_cursor_to_ll.duration = capDuration (cap, 0);
    F_cursor_to_ll.length = capDurationToLength (F_cursor_to_ll.duration);
  }
  else
  {
    F_cursor_to_ll.cap = nullptr;
    F_cursor_to_ll.duration = \
    F_cursor_to_ll.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_carriage_return (const char cap[])
{
  if ( cap )
  {
    F_carriage_return.cap = cap;
    F_carriage_return.duration = capDuration (cap, 0);
    F_carriage_return.length = capDurationToLength (F_carriage_return.duration);
  }
  else
  {
    F_carriage_return.cap = nullptr;
    F_carriage_return.duration = \
    F_carriage_return.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_tabular (const char cap[])
{
  if ( cap )
  {
    F_tab.cap = cap;
    F_tab.duration = capDuration (cap, 0);
    F_tab.length = capDurationToLength (F_tab.duration);
  }
  else
  {
    F_tab.cap = nullptr;
    F_tab.duration = \
    F_tab.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_back_tab (const char cap[])
{
  if ( cap )
  {
    F_back_tab.cap = cap;
    F_back_tab.duration = capDuration (cap, 0);
    F_back_tab.length = capDurationToLength (F_back_tab.duration);
  }
  else
  {
    F_back_tab.cap = nullptr;
    F_back_tab.duration = \
    F_back_tab.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_cursor_up (const char cap[])
{
  if ( cap )
  {
    F_cursor_up.cap = cap;
    F_cursor_up.duration = capDuration (cap, 0);
    F_cursor_up.length = capDurationToLength (F_cursor_up.duration);
  }
  else
  {
    F_cursor_up.cap = nullptr;
    F_cursor_up.duration = \
    F_cursor_up.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_cursor_down (const char cap[])
{
  if ( cap )
  {
    F_cursor_down.cap = cap;
    F_cursor_down.duration = capDuration (cap, 0);
    F_cursor_down.length = capDurationToLength (F_cursor_down.duration);
  }
  else
  {
    F_cursor_down.cap = nullptr;
    F_cursor_down.duration = \
    F_cursor_down.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_cursor_left (const char cap[])
{
  if ( cap )
  {
    F_cursor_left.cap = cap;
    F_cursor_left.duration = capDuration (cap, 0);
    F_cursor_left.length = capDurationToLength (F_cursor_left.duration);
  }
  else
  {
    F_cursor_left.cap = nullptr;
    F_cursor_left.duration = \
    F_cursor_left.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_cursor_right (const char cap[])
{
  if ( cap )
  {
    F_cursor_right.cap = cap;
    F_cursor_right.duration = capDuration (cap, 0);
    F_cursor_right.length = capDurationToLength (F_cursor_right.duration);
  }
  else
  {
    F_cursor_right.cap = nullptr;
    F_cursor_right.duration = \
    F_cursor_right.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_cursor_address (const char cap[])
{
  if ( cap && FTermcap::isInitialized() )
  {
    const auto& temp = FTermcap::encodeMotionParameter(cap, 23, 23);
    F_cursor_address.cap = cap;
    F_cursor_address.duration = capDuration (temp.data(), 1);
    F_cursor_address.length = capDurationToLength (F_cursor_address.duration);
  }
  else
  {
    F_cursor_address.cap = nullptr;
    F_cursor_address.duration = \
    F_cursor_address.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_column_address (const char cap[])
{
  if ( cap && FTermcap::isInitialized() )
  {
    const auto& temp = FTermcap::encodeParameter(cap, 23);
    F_column_address.cap = cap;
    F_column_address.duration = capDuration (temp.data(), 1);
    F_column_address.length = capDurationToLength (F_column_address.duration);
  }
  else
  {
    F_column_address.cap = nullptr;
    F_column_address.duration = \
    F_column_address.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_row_address (const char cap[])
{
  if ( cap && FTermcap::isInitialized() )
  {
    const auto& temp = FTermcap::encodeParameter(cap, 23);
    F_row_address.cap = cap;
    F_row_address.duration = capDuration (temp.data(), 1);
    F_row_address.length = capDurationToLength (F_row_address.duration);
  }
  else
  {
    F_row_address.cap = nullptr;
    F_row_address.duration = \
    F_row_address.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_parm_up_cursor (const char cap[])
{
  if ( cap && FTermcap::isInitialized() )
  {
    const auto& temp = FTermcap::encodeParameter(cap, 23);
    F_parm_up_cursor.cap = cap;
    F_parm_up_cursor.duration = capDuration (temp.data(), 1);
    F_parm_up_cursor.length = capDurationToLength (F_parm_up_cursor.duration);
  }
  else
  {
    F_parm_up_cursor.cap = nullptr;
    F_parm_up_cursor.duration = \
    F_parm_up_cursor.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_parm_down_cursor (const char cap[])
{
  if ( cap && FTermcap::isInitialized() )
  {
    const auto& temp = FTermcap::encodeParameter(cap, 23);
    F_parm_down_cursor.cap = cap;
    F_parm_down_cursor.duration = capDuration (temp.data(), 1);
    F_parm_down_cursor.length = capDurationToLength (F_parm_down_cursor.duration);
  }
  else
  {
    F_parm_down_cursor.cap = nullptr;
    F_parm_down_cursor.duration = \
    F_parm_down_cursor.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_parm_left_cursor (const char cap[])
{
  if ( cap && FTermcap::isInitialized() )
  {
    const auto& temp = FTermcap::encodeParameter(cap, 23);
    F_parm_left_cursor.cap = cap;
    F_parm_left_cursor.duration = capDuration (temp.data(), 1);
    F_parm_left_cursor.length = capDurationToLength (F_parm_left_cursor.duration);
  }
  else
  {
    F_parm_left_cursor.cap = nullptr;
    F_parm_left_cursor.duration = \
    F_parm_left_cursor.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_parm_right_cursor (const char cap[])
{
  if ( cap && FTermcap::isInitialized() )
  {
    const auto& temp = FTermcap::encodeParameter(cap, 23);
    F_parm_right_cursor.cap = cap;
    F_parm_right_cursor.duration = capDuration (temp.data(), 1);
    F_parm_right_cursor.length = capDurationToLength (F_parm_right_cursor.duration);
  }
  else
  {
    F_parm_right_cursor.cap = nullptr;
    F_parm_right_cursor.duration = \
    F_parm_right_cursor.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_erase_chars (const char cap[])
{
  if ( cap && FTermcap::isInitialized() )
  {
    const auto& temp = FTermcap::encodeParameter(cap, 23);
    F_erase_chars.cap = cap;
    F_erase_chars.duration = capDuration (temp.data(), 1);
    F_erase_chars.length = capDurationToLength (F_erase_chars.duration);
  }
  else
  {
    F_erase_chars.cap = nullptr;
    F_erase_chars.duration = \
    F_erase_chars.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_repeat_char (const char cap[])
{
  if ( cap && FTermcap::isInitialized() )
  {
    const auto& temp = FTermcap::encodeParameter(cap, ' ');
    F_repeat_char.cap = cap;
    F_repeat_char.duration = capDuration (temp.data(), 1);
    F_repeat_char.length = capDurationToLength (F_repeat_char.duration);
  }
  else
  {
    F_repeat_char.cap = nullptr;
    F_repeat_char.duration = \
    F_repeat_char.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_clr_bol (const char cap[])
{
  if ( cap )
  {
    F_clr_bol.cap = cap;
    F_clr_bol.duration = capDuration (cap, 0);
    F_clr_bol.length = capDurationToLength (F_clr_bol.duration);
  }
  else
  {
    F_clr_bol.cap = nullptr;
    F_clr_bol.duration = \
    F_clr_bol.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::set_clr_eol (const char cap[])
{
  if ( cap )
  {
    F_clr_eol.cap = cap;
    F_clr_eol.duration = capDuration (cap, 0);
    F_clr_eol.length = capDurationToLength (F_clr_eol.duration);
  }
  else
  {
    F_clr_eol.cap = nullptr;
    F_clr_eol.duration = \
    F_clr_eol.length   = LONG_DURATION;
  }
}

//----------------------------------------------------------------------
void FOptiMove::check_boundaries ( int& xold, int& yold
                                 , int& xnew, int& ynew ) const
{
  if ( xold < 0 || xold >= int(screen_width) )
    xold = -1;

  if ( yold < 0 || yold >= int(screen_height) )
    yold = -1;

  if ( xnew < 0 )
    xnew = 0;

  if ( ynew < 0 )
    ynew = 0;

  if ( xnew >= int(screen_width) )
    xnew = int(screen_width) - 1;

  if ( ynew >= int(screen_height) )
    ynew = int(screen_height) - 1;
}

//----------------------------------------------------------------------
const char* FOptiMove::moveCursor (int xold, int yold, int xnew, int ynew)
{
  int method{0};
  int move_time{LONG_DURATION};

  check_boundaries (xold, yold, xnew, ynew);

  // Method 0: direct cursor addressing
  if ( isMethod0Faster(move_time, xnew, ynew)
    && ( xold < 0
      || yold < 0
      || isWideMove (xold, yold, xnew, ynew) ) )
  {
    return ( move_time < LONG_DURATION ) ? move_buf : nullptr;
  }

  // Method 1: local movement
  if ( isMethod1Faster(move_time, xold, yold, xnew, ynew) )
    method = 1;

  // Method 2: carriage-return + local movement
  if ( isMethod2Faster(move_time, yold, xnew, ynew) )
    method = 2;

  // Method 3: home-cursor + local movement
  if ( isMethod3Faster(move_time, xnew, ynew) )
    method = 3;

  // Method 4: home-down + local movement
  if ( isMethod4Faster(move_time, xnew, ynew) )
    method = 4;

  // Method 5: left margin for wrap to right-hand side
  if ( isMethod5Faster(move_time, yold, xnew, ynew) )
    method = 5;

  // Copy the escape sequence for the chosen method in move_buf
  moveByMethod (method, xold, yold, xnew, ynew);

  if ( move_time < LONG_DURATION )
    return move_buf;
  else
    return nullptr;
}


// private methods of FApplication
//----------------------------------------------------------------------
void FOptiMove::calculateCharDuration()
{
  if ( baudrate != 0 )
  {
    static constexpr int baudbyte = 9;  // = 7 bit + 1 parity + 1 stop
    char_duration = (baudbyte * 1000 * 10)
                  / (baudrate > 0 ? baudrate : 9600);  // milliseconds

    if ( char_duration <= 0 )
      char_duration = 1;
  }
  else
    char_duration = 1;
}

//----------------------------------------------------------------------
int FOptiMove::capDuration (const char cap[], int affcnt) const
{
  // calculate the duration in milliseconds of a given operation
  // cap    - the term capability
  // affcnt - the number of lines affected

  if ( ! cap )
    return LONG_DURATION;

  float ms{0};
  const char* p = cap;

  while ( *p )
  {
    // check for delay with padding character
    if ( p[0] == '$' && p[1] == '<' && std::strchr(p, '>') )
    {
      float num = 0;
      p += 2;

      while ( *p != '>' )
      {
        if ( std::isdigit(uChar(*p)) )
          num = num * 10 + float(*p - '0');
        else if ( *p == '*' )
          num *= float(affcnt);
        else if ( *p == '.' )
        {
          p++;

          if ( *p != '>' && std::isdigit(uChar(*p)) )
            num += float((*p - '0') / 10.0);
        }

        p++;
      }

      ms += num * 10;
    }
    else
      ms += float(char_duration);

    p++;
  }

  return int(ms);
}

//----------------------------------------------------------------------
int FOptiMove::capDurationToLength (int duration) const
{
  if ( duration != LONG_DURATION )
    return (duration + char_duration - 1) / char_duration;
  else
    return LONG_DURATION;
}

//----------------------------------------------------------------------
int FOptiMove::repeatedAppend ( const Capability& o
                              , int count
                              , char* dst ) const
{
  const std::size_t src_len = std::strlen(o.cap);
  const std::size_t dst_len = ( dst != nullptr ) ? std::strlen(dst) : 0;
  int total{0};

  if ( (dst_len + uInt(count) * src_len) < BUF_SIZE - 1 )
  {
    total += count * o.duration;

    if ( dst )
    {
      dst += dst_len;
      std::size_t free = BUF_SIZE - dst_len - 2;
      int cnt = count;

      while ( cnt-- > 0 )
      {
        std::strncpy (dst, o.cap, free);
        dst += src_len;
        free -= src_len;
      }
    }
  }
  else
    total = LONG_DURATION;

  return total;
}

//----------------------------------------------------------------------
int FOptiMove::relativeMove ( char move[]
                            , int from_x, int from_y
                            , int to_x, int to_y ) const
{
  int vtime{0};
  int htime{0};

  if ( move )
    move[0] = '\0';

  if ( to_y != from_y )  // vertical move
  {
    vtime = verticalMove (move, from_y, to_y);

    if ( vtime >= LONG_DURATION )
      return LONG_DURATION;
  }

  if ( to_x != from_x )  // horizontal move
  {
    char hmove[BUF_SIZE]{};
    htime = horizontalMove (hmove, from_x, to_x);

    if ( htime >= LONG_DURATION )
      return LONG_DURATION;

    if ( move )
    {
      if ( *move )
        std::strncat (move, hmove, BUF_SIZE - std::strlen(move));
      else
        std::strncpy (move, hmove, BUF_SIZE);

      move[BUF_SIZE - 1] = '\0';
    }
  }

  return vtime + htime;
}

//----------------------------------------------------------------------
inline int FOptiMove::verticalMove (char move[], int from_y, int to_y) const
{
  int vtime{LONG_DURATION};

  if ( F_row_address.cap )
  {
    if ( move )
    {
      std::strncpy ( move
                   , FTermcap::encodeParameter(F_row_address.cap, to_y).data()
                   , BUF_SIZE );
      move[BUF_SIZE - 1] = '\0';
    }

    vtime = F_row_address.duration;
  }

  if ( to_y > from_y )
    downMove (move, vtime, from_y, to_y);
  else  // to_y < from_y
    upMove (move, vtime, from_y, to_y);

  return vtime;
}

//----------------------------------------------------------------------
inline void FOptiMove::downMove ( char move[], int& vtime
                                , int from_y, int to_y ) const
{
  const int num = to_y - from_y;

  if ( F_parm_down_cursor.cap && F_parm_down_cursor.duration < vtime )
  {
    if ( move )
    {
      std::strncpy ( move
                   , FTermcap::encodeParameter(F_parm_down_cursor.cap, num).data()
                   , BUF_SIZE );
      move[BUF_SIZE - 1] = '\0';
    }

    vtime = F_parm_down_cursor.duration;
  }

  if ( F_cursor_down.cap && (num * F_cursor_down.duration < vtime) )
  {
    if ( move )
      move[0] = '\0';

    vtime = repeatedAppend (F_cursor_down, num, move);
  }
}

//----------------------------------------------------------------------
inline void FOptiMove::upMove ( char move[], int& vtime
                              , int from_y, int to_y ) const
{
  const int num = from_y - to_y;

  if ( F_parm_up_cursor.cap && F_parm_up_cursor.duration < vtime )
  {
    if ( move )
    {
      std::strncpy ( move
                   , FTermcap::encodeParameter(F_parm_up_cursor.cap, num).data()
                   , BUF_SIZE );
      move[BUF_SIZE - 1] = '\0';
    }

    vtime = F_parm_up_cursor.duration;
  }

  if ( F_cursor_up.cap && (num * F_cursor_up.duration < vtime) )
  {
    if ( move )
      move[0] = '\0';

    vtime = repeatedAppend (F_cursor_up, num, move);
  }
}

//----------------------------------------------------------------------
inline int FOptiMove::horizontalMove (char hmove[], int from_x, int to_x) const
{
  int htime{LONG_DURATION};

  if ( F_column_address.cap )
  {
    // Move to fixed column position1
    std::strncat ( hmove
                 , FTermcap::encodeParameter(F_column_address.cap, to_x).data()
                 , BUF_SIZE - std::strlen(hmove) - 1 );
    hmove[BUF_SIZE - 1] = '\0';
    htime = F_column_address.duration;
  }

  if ( to_x > from_x )
    rightMove (hmove, htime, from_x, to_x);
  else  // to_x < from_x
    leftMove (hmove, htime, from_x, to_x);

  return htime;
}

//----------------------------------------------------------------------
inline void FOptiMove::rightMove ( char hmove[], int& htime
                                 , int from_x, int to_x ) const
{
  int num = to_x - from_x;

  if ( F_parm_right_cursor.cap && F_parm_right_cursor.duration < htime )
  {
    std::strncpy ( hmove
                 , FTermcap::encodeParameter(F_parm_right_cursor.cap, num).data()
                 , BUF_SIZE - 1);
    hmove[BUF_SIZE - 1] = '\0';
    htime = F_parm_right_cursor.duration;
  }

  if ( F_cursor_right.cap )
  {
    std::array<char, BUF_SIZE> str{};
    int htime_r{0};
    str[0] = '\0';

    // try to use tab
    if ( tabstop > 0 && F_tab.cap )
    {
      int pos = from_x;

      while ( true )
      {
        const int tab_pos = pos + tabstop - (pos % tabstop);

        if ( tab_pos > to_x )
          break;

        htime_r += repeatedAppend (F_tab, 1, str.data());

        if ( htime_r >= LONG_DURATION )
          break;

        pos = tab_pos;
      }

      num = to_x - pos;
    }

    htime_r += repeatedAppend (F_cursor_right, num, str.data());

    if ( htime_r < htime )
    {
      std::strncpy (hmove, str.data(), BUF_SIZE);
      hmove[BUF_SIZE - 1] = '\0';
      htime = htime_r;
    }
  }
}

//----------------------------------------------------------------------
inline void FOptiMove::leftMove ( char hmove[], int& htime
                                , int from_x, int to_x ) const
{
  int num = from_x - to_x;

  if ( F_parm_left_cursor.cap && F_parm_left_cursor.duration < htime )
  {
    std::strncpy ( hmove
                 , FTermcap::encodeParameter(F_parm_left_cursor.cap, num).data()
                 , BUF_SIZE - 1);
    hmove[BUF_SIZE - 1] = '\0';
    htime = F_parm_left_cursor.duration;
  }

  if ( F_cursor_left.cap )
  {
    std::array<char, BUF_SIZE> str{};
    int htime_l{0};
    str[0] = '\0';

    // try to use backward tab
    if ( tabstop > 0 && F_back_tab.cap )
    {
      int pos = from_x;

      while ( true )
      {
        const int tab_pos = ( pos > 0 ) ? ((pos - 1) / tabstop) * tabstop : -1;

        if ( tab_pos < to_x )
          break;

        htime_l += repeatedAppend (F_back_tab, 1, str.data());

        if ( htime_l >= LONG_DURATION )
          break;

        pos = tab_pos;
      }

      num = pos - to_x;
    }

    htime_l += repeatedAppend (F_cursor_left, num, str.data());

    if ( htime_l < htime )
    {
      std::strncpy (hmove, str.data(), BUF_SIZE);
      hmove[BUF_SIZE - 1] = '\0';
      htime = htime_l;
    }
  }
}

//----------------------------------------------------------------------
inline bool FOptiMove::isWideMove ( int xold, int yold
                                  , int xnew, int ynew ) const
{
  return xnew > MOVE_LIMIT
      && xnew < int(screen_width) - 1 - MOVE_LIMIT
      && std::abs(xnew - xold) + std::abs(ynew - yold) > MOVE_LIMIT;
}

//----------------------------------------------------------------------
inline bool FOptiMove::isMethod0Faster ( int& move_time
                                       , int xnew, int ynew )
{
  // Test method 0: direct cursor addressing

  if ( ! F_cursor_address.cap )
    return false;

  const auto move_xy = \
    FTermcap::encodeMotionParameter(F_cursor_address.cap, xnew, ynew);

  if ( ! move_xy.empty() )
  {
    std::strncpy ( static_cast<char*>(move_buf)
                 , move_xy.data(), BUF_SIZE - 1 );
    move_buf[BUF_SIZE - 1] = '\0';
    move_time = F_cursor_address.duration;
    return true;
  }

  return false;
}

//----------------------------------------------------------------------
inline bool FOptiMove::isMethod1Faster ( int& move_time
                                       , int xold, int yold
                                       , int xnew, int ynew ) const
{
  // Test method 1: local movement

  if ( xold >= 0 && yold >= 0 )
  {
    std::array<char, BUF_SIZE> null_result{};
    const int new_time = relativeMove (null_result.data(), xold, yold, xnew, ynew);

    if ( new_time < LONG_DURATION && new_time < move_time )
    {
      move_time = new_time;
      return true;
    }
  }

  return false;
}

//----------------------------------------------------------------------
inline bool FOptiMove::isMethod2Faster ( int& move_time
                                       , int yold
                                       , int xnew, int ynew ) const
{
  // Test method 2: carriage-return + local movement

  if ( yold >= 0 && F_carriage_return.cap )
  {
    std::array<char, BUF_SIZE> null_result{};
    const int new_time = relativeMove (null_result.data(), 0, yold, xnew, ynew);

    if ( new_time < LONG_DURATION
      && F_carriage_return.duration + new_time < move_time )
    {
      move_time = F_carriage_return.duration + new_time;
      return true;
    }
  }

  return false;
}

//----------------------------------------------------------------------
inline bool FOptiMove::isMethod3Faster ( int& move_time
                                       , int xnew, int ynew ) const
{
  // Test method 3: home-cursor + local movement

  if ( F_cursor_home.cap )
  {
    std::array<char, BUF_SIZE> null_result{};
    const int new_time = relativeMove (null_result.data(), 0, 0, xnew, ynew);

    if ( new_time < LONG_DURATION
      && F_cursor_home.duration + new_time < move_time )
    {
      move_time = F_cursor_home.duration + new_time;
      return true;
    }
  }

  return false;
}

//----------------------------------------------------------------------
inline bool FOptiMove::isMethod4Faster ( int& move_time
                                       , int xnew, int ynew ) const
{
  // Test method 4: home-down + local movement
  if ( F_cursor_to_ll.cap )
  {
    std::array<char, BUF_SIZE> null_result{};
    const int new_time = relativeMove ( null_result.data()
                                      , 0, int(screen_height) - 1
                                      , xnew, ynew );

    if ( new_time < LONG_DURATION
      && F_cursor_to_ll.duration + new_time < move_time )
    {
      move_time = F_cursor_to_ll.duration + new_time;
      return true;
    }
  }

  return false;
}

//----------------------------------------------------------------------
inline bool FOptiMove::isMethod5Faster ( int& move_time
                                       , int yold
                                       , int xnew, int ynew ) const
{
  // Test method 5: left margin for wrap to right-hand side
  if ( automatic_left_margin
    && ! eat_nl_glitch
    && yold > 0
    && F_cursor_left.cap )
  {
    std::array<char, BUF_SIZE> null_result{};
    const int new_time = relativeMove ( null_result.data()
                                      , int(screen_width) - 1, yold - 1
                                      , xnew, ynew );

    if ( new_time < LONG_DURATION
      && F_carriage_return.cap
      && F_carriage_return.duration
       + F_cursor_left.duration + new_time < move_time )
    {
      move_time = F_carriage_return.duration
                + F_cursor_left.duration + new_time;

      return true;
    }
  }

  return false;
}

//----------------------------------------------------------------------
void FOptiMove::moveByMethod ( int method
                             , int xold, int yold
                             , int xnew, int ynew )
{
  char* move_ptr = move_buf;

  switch ( method )
  {
    case 1:
      relativeMove (move_ptr, xold, yold, xnew, ynew);
      break;

    case 2:
      if ( F_carriage_return.cap )
      {
        std::strncpy (move_ptr, F_carriage_return.cap, BUF_SIZE - 1);
        move_ptr[BUF_SIZE - 1] ='\0';
        move_ptr += F_carriage_return.length;
        relativeMove (move_ptr, 0, yold, xnew, ynew);
      }
      break;

    case 3:
      std::strncpy (move_ptr, F_cursor_home.cap, BUF_SIZE - 1);
      move_ptr[BUF_SIZE - 1] ='\0';
      move_ptr += F_cursor_home.length;
      relativeMove (move_ptr, 0, 0, xnew, ynew);
      break;

    case 4:
      std::strncpy (move_ptr, F_cursor_to_ll.cap, BUF_SIZE - 1);
      move_ptr[BUF_SIZE - 1] ='\0';
      move_ptr += F_cursor_to_ll.length;
      relativeMove (move_ptr, 0, int(screen_height) - 1, xnew, ynew);
      break;

    case 5:
      move_buf[0] = '\0';

      if ( xold >= 0 )
        std::strncat ( move_ptr
                     , F_carriage_return.cap
                     , BUF_SIZE - std::strlen(move_ptr) - 1 );

      std::strncat ( move_ptr
                   , F_cursor_left.cap
                   , BUF_SIZE - std::strlen(move_ptr) - 1);
      move_ptr[BUF_SIZE - 1] ='\0';
      move_ptr += std::strlen(move_buf);
      relativeMove (move_ptr, int(screen_width) - 1, yold - 1, xnew, ynew);
      break;

    default:
      break;
  }
}

// FOptiMove non-member function
//----------------------------------------------------------------------
void printDurations (const FOptiMove& om)
{
  std::clog << "            speed: "
            << om.baudrate << " baud" << std::flush;
  std::clog << "    char_duration: "
            << om.char_duration << " ms" << std::flush;
  std::clog << "      cursor_home: "
            << om.F_cursor_home.duration << " ms" << std::flush;
  std::clog << "     cursor_to_ll: "
            << om.F_cursor_to_ll.duration << " ms" << std::flush;
  std::clog << "  carriage_return: "
            << om.F_carriage_return.duration << " ms" << std::flush;
  std::clog << "              tab: "
            << om.F_tab.duration << " ms" << std::flush;
  std::clog << "         back_tab: "
            << om.F_back_tab.duration << " ms" << std::flush;
  std::clog << "        cursor_up: "
            << om.F_cursor_up.duration << " ms" << std::flush;
  std::clog << "      cursor_down: "
            << om.F_cursor_down.duration << " ms" << std::flush;
  std::clog << "      cursor_left: "
            << om.F_cursor_left.duration << " ms" << std::flush;
  std::clog << "     cursor_right: "
            << om.F_cursor_right.duration << " ms" << std::flush;
  std::clog << "   cursor_address: "
            << om.F_cursor_address.duration << " ms" << std::flush;
  std::clog << "   column_address: "
            << om.F_column_address.duration << " ms" << std::flush;
  std::clog << "      row_address: "
            << om.F_row_address.duration << " ms" << std::flush;
  std::clog << "   parm_up_cursor: "
            << om.F_parm_up_cursor.duration << " ms" << std::flush;
  std::clog << " parm_down_cursor: "
            << om.F_parm_down_cursor.duration << " ms" << std::flush;
  std::clog << " parm_left_cursor: "
            << om.F_parm_left_cursor.duration << " ms" << std::flush;
  std::clog << "parm_right_cursor: "
            << om.F_parm_right_cursor.duration << " ms" << std::flush;
}

}  // namespace finalcut
