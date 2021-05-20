/***********************************************************************
* fvterm.cpp - Virtual terminal implementation                         *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2016-2021 Markus Gans                                      *
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

#if defined(__CYGWIN__)
  #include <unistd.h>  // need for ttyname_r
#endif

#include <numeric>
#include <queue>
#include <string>
#include <vector>

#include "final/fapplication.h"
#include "final/fc.h"
#include "final/fcharmap.h"
#include "final/fcolorpair.h"
#include "final/fkeyboard.h"
#include "final/flog.h"
#include "final/fmouse.h"
#include "final/foptiattr.h"
#include "final/foptimove.h"
#include "final/fstyle.h"
#include "final/fsystem.h"
#include "final/fterm.h"
#include "final/ftermbuffer.h"
#include "final/ftermdata.h"
#include "final/ftermdetection.h"
#include "final/ftermfreebsd.h"
#include "final/ftermcap.h"
#include "final/ftypes.h"
#include "final/fvterm.h"

namespace finalcut
{

// static class attributes
bool                 FVTerm::draw_completed{false};
bool                 FVTerm::combined_char_support{false};
bool                 FVTerm::no_terminal_updates{false};
bool                 FVTerm::cursor_hideable{false};
bool                 FVTerm::force_terminal_update{false};
uInt64               FVTerm::flush_wait{MIN_FLUSH_WAIT};
uInt64               FVTerm::flush_average{MIN_FLUSH_WAIT};
uInt64               FVTerm::flush_median{MIN_FLUSH_WAIT};
uInt64               FVTerm::term_size_check_timeout{500000};  // 500 ms
uInt                 FVTerm::erase_char_length{};
uInt                 FVTerm::repeat_char_length{};
uInt                 FVTerm::clr_bol_length{};
uInt                 FVTerm::clr_eol_length{};
uInt                 FVTerm::cursor_address_length{};
struct timeval       FVTerm::time_last_flush{};
struct timeval       FVTerm::last_term_size_check{};
const FVTerm*        FVTerm::init_object{nullptr};
FVTerm::FTermArea*   FVTerm::vterm{nullptr};
FVTerm::FTermArea*   FVTerm::vdesktop{nullptr};
FVTerm::FTermArea*   FVTerm::active_area{nullptr};
FChar                FVTerm::term_attribute{};
FChar                FVTerm::next_attribute{};
FChar                FVTerm::s_ch{};
FChar                FVTerm::i_ch{};


//----------------------------------------------------------------------
// class FVTerm
//----------------------------------------------------------------------

// constructors and destructor
//----------------------------------------------------------------------
FVTerm::FVTerm()
{
  if ( ! init_object )
    init();
  else
  {
    fterm = std::shared_ptr<FTerm>(init_object->fterm);
    term_pos = std::shared_ptr<FPoint>(init_object->term_pos);
    output_buffer = std::shared_ptr<OutputBuffer>(init_object->output_buffer);
    window_list = std::shared_ptr<FVTermList>(init_object->window_list);
  }
}

//----------------------------------------------------------------------
FVTerm::~FVTerm()  // destructor
{
  if ( init_object == this )
    finish();
}


// Overloaded operators
//----------------------------------------------------------------------
FVTerm& FVTerm::operator << (const FTermBuffer& term_buffer)
{
  print (term_buffer);
  return *this;
}

// public methods of FVTerm
//----------------------------------------------------------------------
FPoint FVTerm::getPrintCursor()
{
  const auto& win = getPrintArea();

  if ( win )
    return { win->offset_left + win->cursor_x
           , win->offset_top + win->cursor_y };

  return {0, 0};
}

//----------------------------------------------------------------------
void FVTerm::setTermXY (int x, int y) const
{
  // Sets the hardware cursor to the given (x,y) position

  if ( term_pos->getX() == x && term_pos->getY() == y )
    return;

  const auto term_width = int(FTerm::getColumnNumber());
  const auto term_height = int(FTerm::getLineNumber());

  if ( x >= term_width && term_width > 0 )
  {
    y += x / term_width;
    x %= term_width;
  }

  if ( term_pos->getY() >= term_height )
    term_pos->setY(term_height - 1);

  if ( y >= term_height )
    y = term_height - 1;

  const int term_x = term_pos->getX();
  const int term_y = term_pos->getY();
  const auto& move_str = FTerm::moveCursorString (term_x, term_y, x, y);

  if ( ! move_str.empty() )
    appendOutputBuffer(FTermControl{move_str});

  term_pos->setPoint(x, y);
}

//----------------------------------------------------------------------
void FVTerm::setTerminalUpdates (TerminalUpdate refresh_state) const
{
  if ( refresh_state == TerminalUpdate::Stop )
  {
    no_terminal_updates = true;
  }
  else if ( refresh_state == TerminalUpdate::Continue
         || refresh_state == TerminalUpdate::Start )
  {
    no_terminal_updates = false;
  }

  if ( refresh_state == TerminalUpdate::Start )
    updateTerminal();
}

//----------------------------------------------------------------------
void FVTerm::hideCursor (bool enable) const
{
  // Hides or shows the input cursor on the terminal

  if ( ! cursor_hideable )
    return;

  const char* visibility_str = FTerm::cursorsVisibilityString (enable);

  if ( ! visibility_str )  // Exit the function if the string is empty
    return;

  appendOutputBuffer(FTermControl{visibility_str});
  flush();
}

//----------------------------------------------------------------------
void FVTerm::setPrintCursor (const FPoint& pos)
{
  if ( auto win = getPrintArea() )
  {
    win->cursor_x = pos.getX() - win->offset_left;
    win->cursor_y = pos.getY() - win->offset_top;
  }
}

//----------------------------------------------------------------------
FColor FVTerm::rgb2ColorIndex (uInt8 r, uInt8 g, uInt8 b) const
{
  // Converts a 24-bit RGB color to a 256-color compatible approximation

  const uInt16 ri = (((r * 5) + 127) / 255) * 36;
  const uInt16 gi = (((g * 5) + 127) / 255) * 6;
  const uInt16 bi = (((b * 5) + 127) / 255);
  return FColor(16 + ri + gi + bi);
}

//----------------------------------------------------------------------
void FVTerm::setNonBlockingRead (bool enable)
{
#if defined(__CYGWIN__)
  // Fixes problem with mouse input
  char termfilename[256]{};

  if ( ttyname_r(1, termfilename, sizeof(termfilename)) )
    termfilename[0] = '\0';

  if ( std::strncmp(termfilename, "/dev/cons", 9) == 0 )
  {
    FKeyboard::setNonBlockingInputSupport(false);
    return;
  }
#endif

  uInt64 blocking_time = enable ? 5000 : 100000;  // 5 or 100 ms
  FKeyboard::setReadBlockingTime (blocking_time);
}

//----------------------------------------------------------------------
void FVTerm::clearArea (wchar_t fillchar)
{
  clearArea (vwin, fillchar);
}

//----------------------------------------------------------------------
void FVTerm::createVTerm (const FSize& size)
{
  // initialize virtual terminal

  const FRect box{0, 0, size.getWidth(), size.getHeight()};
  const FSize shadow{0, 0};
  createArea (box, shadow, vterm);
}

//----------------------------------------------------------------------
void FVTerm::resizeVTerm (const FSize& size) const
{
  // resize virtual terminal

  const FRect box{0, 0, size.getWidth(), size.getHeight()};
  const FSize shadow{0, 0};
  resizeArea (box, shadow, vterm);
}

//----------------------------------------------------------------------
void FVTerm::putVTerm() const
{
  for (auto i{0}; i < vterm->height; i++)
  {
    vterm->changes[i].xmin = 0;
    vterm->changes[i].xmax = uInt(vterm->width - 1);
  }

  updateTerminal();
}

//----------------------------------------------------------------------
bool FVTerm::updateTerminal() const
{
  // Updates pending changes to the terminal

  // Check if terminal updates were stopped, application is stopping,
  // VTerm has no changes, or the drawing is not completed
  if ( no_terminal_updates || FApplication::isQuit()
    || ! (isFlushTimeout() || force_terminal_update)
    || ! (hasPendingUpdates(vterm) && draw_completed) )
  {
    return false;
  }

  std::size_t changedlines = 0;

  for (uInt y{0}; y < uInt(vterm->height); y++)
  {
    if ( updateTerminalLine(y) )
      changedlines++;
  }

  vterm->has_changes = false;

  // sets the new input cursor position
  bool cursor_update = updateTerminalCursor();
  return cursor_update || changedlines > 0;
}

//----------------------------------------------------------------------
void FVTerm::addPreprocessingHandler ( const FVTerm* instance
                                     , FPreprocessingFunction&& function )
{
  if ( ! print_area )
    FVTerm::getPrintArea();

  if ( print_area )
  {
    delPreprocessingHandler (instance);
    auto obj = make_unique<FVTermPreprocessing> \
        (instance, std::move(function));
    print_area->preproc_list.emplace_back(std::move(obj));
  }
}

//----------------------------------------------------------------------
void FVTerm::delPreprocessingHandler (const FVTerm* instance)
{
  if ( ! print_area )
    FVTerm::getPrintArea();

  if ( ! print_area || print_area->preproc_list.empty() )
    return;

  auto iter = print_area->preproc_list.begin();

  while ( iter != print_area->preproc_list.end() )
  {
    if ( iter->get()->instance.get() == instance )
      iter = print_area->preproc_list.erase(iter);
    else
      ++iter;
  }
}

//----------------------------------------------------------------------
int FVTerm::print (const FString& string)
{
  if ( string.isNull() )
    return -1;

  FTermBuffer term_buffer{};
  term_buffer.write(string);
  return print (term_buffer);
}

//----------------------------------------------------------------------
int FVTerm::print (FTermArea* area, const FString& string)
{
  if ( ! area || string.isNull() )
    return -1;

  FTermBuffer term_buffer{};
  term_buffer.write(string);
  return print (area, term_buffer);
}

//----------------------------------------------------------------------
int FVTerm::print (const std::vector<FChar>& term_string)
{
  if ( term_string.empty() )
    return -1;

  FTermBuffer term_buffer{term_string.begin(), term_string.end()};
  return print (term_buffer);
}

//----------------------------------------------------------------------
int FVTerm::print (FTermArea* area, const std::vector<FChar>& term_string)
{
  if ( ! area || term_string.empty() )
    return -1;

  FTermBuffer term_buffer{term_string.begin(), term_string.end()};
  return print (area, term_buffer);
}

//----------------------------------------------------------------------
int FVTerm::print (const FTermBuffer& term_buffer)
{
  if ( term_buffer.isEmpty() )
    return -1;

  auto area = getPrintArea();

  if ( ! area )
    return -1;

  return print (area, term_buffer);
}

//----------------------------------------------------------------------
int FVTerm::print (FTermArea* area, const FTermBuffer& term_buffer)
{
  int len{0};
  const auto tabstop = uInt(FTerm::getTabstop());

  if ( ! area || term_buffer.isEmpty() )
    return -1;

  for (auto&& fchar : term_buffer)
  {
    bool printable_character{false};

    switch ( fchar.ch[0] )
    {
      case '\n':
        area->cursor_y++;
        // fall through
      case '\r':
        area->cursor_x = 1;
        break;

      case '\t':
        area->cursor_x = int ( uInt(area->cursor_x)
                             + tabstop
                             - uInt(area->cursor_x)
                             + 1
                             % tabstop );
        break;

      case '\b':
        area->cursor_x--;
        break;

      case '\a':
        FTerm::beep();
        break;

      default:
        print (area, fchar);  // print next character
        printable_character = true;
    }

    if ( ! printable_character && printWrap(area) )
      break;  // end of area reached

    len++;
  }

  return len;
}

//----------------------------------------------------------------------
int FVTerm::print (wchar_t c)
{
  FChar nc{FVTerm::getAttribute()};  // next character
  nc.ch[0] = c;
  nc.attr.byte[2] = 0;
  nc.attr.byte[3] = 0;
  return print (nc);
}

//----------------------------------------------------------------------
int FVTerm::print (FTermArea* area, wchar_t c)
{
  if ( ! area )
    return -1;

  FChar nc = FVTerm::getAttribute();  // next character
  nc.ch[0] = c;
  nc.attr.byte[2] = 0;
  nc.attr.byte[3] = 0;
  return print (area, nc);
}

//----------------------------------------------------------------------
int FVTerm::print (FChar& term_char)
{
  auto area = getPrintArea();

  if ( ! area )
    return -1;

  return print (area, term_char);
}

//----------------------------------------------------------------------
int FVTerm::print (FTermArea* area, const FChar& term_char)
{
  auto fchar = term_char;
  return print (area, fchar);
}

//----------------------------------------------------------------------
int FVTerm::print (FTermArea* area, FChar& term_char)
{
  if ( ! area )
    return -1;

  const int ax = area->cursor_x - 1;
  const int ay = area->cursor_y - 1;

  if ( term_char.attr.bit.char_width == 0 )
    addColumnWidth(term_char);  // add column width

  auto char_width = term_char.attr.bit.char_width;

  if ( char_width == 0 && ! term_char.attr.bit.fullwidth_padding )
    return 0;

  // Print term_char on area at position (ax, ay)
  printCharacterOnCoordinate (area, ax, ay, term_char);
  area->cursor_x++;
  area->has_changes = true;

  // Line break at right margin
  if ( area->cursor_x > area->width + area->right_shadow )
  {
    area->cursor_x = 1;
    area->cursor_y++;
  }
  else if ( char_width == 2 )
    printPaddingCharacter (area, term_char);

  // Prevent up scrolling
  if ( area->cursor_y > area->height + area->bottom_shadow )
  {
    area->cursor_y--;
    return -1;
  }

  return 1;
}

//----------------------------------------------------------------------
void FVTerm::print (const FPoint& p)
{
  setPrintCursor (p);
}

//----------------------------------------------------------------------
void FVTerm::print (const FStyle& style)
{
  Style attr = style.getStyle();

  if ( attr == Style::None )
    setNormal();
  else
  {
    if ( (attr & Style::Bold) != Style::None ) setBold();
    if ( (attr & Style::Dim) != Style::None ) setDim();
    if ( (attr & Style::Italic) != Style::None ) setItalic();
    if ( (attr & Style::Underline) != Style::None ) setUnderline();
    if ( (attr & Style::Blink) != Style::None ) setBlink();
    if ( (attr & Style::Reverse) != Style::None ) setReverse();
    if ( (attr & Style::Standout) != Style::None ) setStandout();
    if ( (attr & Style::Invisible) != Style::None ) setInvisible();
    if ( (attr & Style::Protected) != Style::None ) setProtected();
    if ( (attr & Style::CrossedOut) != Style::None ) setCrossedOut();
    if ( (attr & Style::DoubleUnderline) != Style::None ) setDoubleUnderline();
    if ( (attr & Style::Transparent) != Style::None ) setTransparent();
    if ( (attr & Style::ColorOverlay) != Style::None ) setColorOverlay();
    if ( (attr & Style::InheritBackground) != Style::None ) setInheritBackground();
  }
}

//----------------------------------------------------------------------
void FVTerm::print (const FColorPair& pair)
{
  setColor (pair.getForegroundColor(), pair.getBackgroundColor());
}

//----------------------------------------------------------------------
void FVTerm::flush() const
{
  // Flush the output buffer

  flushTimeAdjustment();

  if ( ! output_buffer || output_buffer->empty()
    || ! (isFlushTimeout() || force_terminal_update) )
    return;

  while ( ! output_buffer->empty() )
  {
    const auto& first = output_buffer->front();
    const auto& type = std::get<0>(first);
    const auto& str = std::get<1>(first);

    if ( type == OutputType::String )
    {
      static const FTerm::defaultPutChar& FTermPutchar = FTerm::putchar();

      if ( ! FTermPutchar )
        return;

      for (auto&& ch : str.wstring)
        FTermPutchar(int(ch));
    }
    else if ( type == OutputType::Control )
      FTerm::putstring (str.string);

    output_buffer->pop();
  }

  std::fflush(stdout);
  auto& mouse = FTerm::getFMouseControl();
  mouse.drawPointer();
  FObject::getCurrentTime (&time_last_flush);
}


// protected methods of FVTerm
//----------------------------------------------------------------------
FVTerm::FTermArea* FVTerm::getPrintArea()
{
  // returns the print area of this object

  if ( print_area )
    return print_area;
  else
  {
    if ( vwin )
    {
      print_area = vwin;
      return print_area;
    }
    else if ( child_print_area )
    {
      print_area = child_print_area;
      return print_area;
    }
  }

  return vdesktop;
}

//----------------------------------------------------------------------
void FVTerm::createArea ( const FRect& box
                        , const FSize& shadow
                        , FTermArea*& area )
{
  // initialize virtual window

  try
  {
    area = new FTermArea;
  }
  catch (const std::bad_alloc&)
  {
    badAllocOutput ("FTermArea");
    return;
  }

  area->setOwner<FVTerm*>(this);
  resizeArea (box, shadow, area);
}

//----------------------------------------------------------------------
void FVTerm::resizeArea ( const FRect& box
                        , const FSize& shadow
                        , FTermArea* area ) const
{
  // Resize the virtual window to a new size.

  const int offset_left = box.getX();
  const int offset_top  = box.getY();
  const auto width = int(box.getWidth());
  const auto height = int(box.getHeight());
  const auto rsw = int(shadow.getWidth());
  const auto bsh = int(shadow.getHeight());

  assert ( offset_top >= 0 );
  assert ( width > 0 && width + rsw > 0 );
  assert ( height > 0 && height + bsh > 0 );
  assert ( rsw >= 0 );
  assert ( bsh >= 0 );

  if ( ! area )
    return;

  if ( width == area->width
    && height == area->height
    && rsw == area->right_shadow
    && bsh == area->bottom_shadow )
  {
    if ( offset_left != area->offset_left )
      area->offset_left = offset_left;

    if ( offset_top != area->offset_top )
      area->offset_top = offset_top;

    return;
  }

  bool realloc_success{false};
  const std::size_t full_width = std::size_t(width) + std::size_t(rsw);
  const std::size_t full_height = std::size_t(height) + std::size_t(bsh);
  const std::size_t area_size = full_width * full_height;

  if ( area->height + area->bottom_shadow != int(full_height) )
  {
    realloc_success = reallocateTextArea ( area
                                         , full_height
                                         , area_size );
  }
  else if ( area->width + area->right_shadow != int(full_width) )
  {
    realloc_success = reallocateTextArea (area, area_size);
  }
  else
    return;

  if ( ! realloc_success )
    return;

  area->offset_left   = offset_left;
  area->offset_top    = offset_top;
  area->width         = width;
  area->height        = height;
  area->right_shadow  = rsw;
  area->bottom_shadow = bsh;
  area->has_changes   = false;

  const FSize size{full_width, full_height};
  resetTextAreaToDefault (area, size);
}

//----------------------------------------------------------------------
void FVTerm::removeArea (FTermArea*& area)
{
  // remove the virtual window

  if ( area == nullptr )
    return;

  if ( area->changes != nullptr )
  {
    delete[] area->changes;
    area->changes = nullptr;
  }

  if ( area->data != nullptr )
  {
    delete[] area->data;
    area->data = nullptr;
  }

  delete area;
  area = nullptr;
}

//----------------------------------------------------------------------
void FVTerm::restoreVTerm (const FRect& box)
{
  if ( ! vterm )
    return;

  int x = box.getX() - 1;
  int y = box.getY() - 1;
  auto w = int(box.getWidth());
  auto h = int(box.getHeight());

  if ( x < 0 )
    x = 0;

  if ( y < 0 )
    y = 0;

  if ( x + w > vterm->width )
    w = vterm->width - x;

  if ( w < 0 )
    return;

  if ( y + h > vterm->height )
    h = vterm->height - y;

  if ( h < 0 )
    return;

  for (auto ty{0}; ty < h; ty++)
  {
    const int ypos = y + ty;

    for (auto tx{0}; tx < w; tx++)
    {
      const int xpos = x + tx;
      auto& tc = vterm->data[ypos * vterm->width + xpos];  // terminal character
      auto sc = generateCharacter(FPoint{xpos, ypos});  // shown character
      std::memcpy (&tc, &sc, sizeof(tc));
    }

    if ( int(vterm->changes[ypos].xmin) > x )
      vterm->changes[ypos].xmin = uInt(x);

    if ( int(vterm->changes[ypos].xmax) < x + w - 1 )
      vterm->changes[ypos].xmax = uInt(x + w - 1);
  }

  vterm->has_changes = true;
}

//----------------------------------------------------------------------
bool FVTerm::updateVTermCursor (const FTermArea* area) const
{
  if ( ! (area && isActive(area) && area->visible) )
    return false;

  if ( area->input_cursor_visible )
  {
    // area cursor position
    const int cx = area->input_cursor_x;
    const int cy = area->input_cursor_y;
    // terminal position = area offset + area cursor position
    const int x  = area->offset_left + cx;
    const int y  = area->offset_top + cy;

    if ( isInsideArea (FPoint{cx, cy}, area)
      && isInsideTerminal (FPoint{x, y})
      && isCovered (FPoint{x, y}, area) == CoveredState::None )
    {
      vterm->input_cursor_x = x;
      vterm->input_cursor_y = y;
      vterm->input_cursor_visible = true;
      vterm->has_changes = true;
      return true;
    }
  }

  vterm->input_cursor_visible = false;
  return false;
}

//----------------------------------------------------------------------
void FVTerm::setAreaCursor ( const FPoint& pos
                           , bool visible
                           , FTermArea* area )
{
  if ( ! area )
    return;

  area->input_cursor_x = pos.getX() - 1;
  area->input_cursor_y = pos.getY() - 1;
  area->input_cursor_visible = visible;
}

//----------------------------------------------------------------------
void FVTerm::getArea (const FPoint& pos, const FTermArea* area)
{
  // Copies a block from the virtual terminal position to the given area

  if ( ! area )
    return;

  const int ax = pos.getX() - 1;
  const int ay = pos.getY() - 1;
  int y_end{};
  int length{};

  if ( area->height + ay > vterm->height )
    y_end = area->height - ay;
  else
    y_end = area->height;

  if ( area->width + ax > vterm->width )
    length = vterm->width - ax;
  else
    length = area->width;

  for (auto y{0}; y < y_end; y++)  // line loop
  {
    const auto& tc = vterm->data[(ay + y) * vterm->width + ax];  // terminal character
    auto& ac = area->data[y * area->width];  // area character
    std::memcpy (&ac, &tc, sizeof(ac) * unsigned(length));

    if ( int(area->changes[y].xmin) > 0 )
      area->changes[y].xmin = 0;

    if ( int(area->changes[y].xmax) < length - 1 )
      area->changes[y].xmax = uInt(length - 1);
  }
}

//----------------------------------------------------------------------
void FVTerm::getArea (const FRect& box, const FTermArea* area)
{
  // Copies a block from the virtual terminal rectangle to the given area

  if ( ! area )
    return;

  const int x = box.getX();
  const int y = box.getY();
  const auto w = int(box.getWidth());
  const auto h = int(box.getHeight());
  const int dx = x - area->offset_left + 1;
  const int dy = y - area->offset_top + 1;
  int y_end{};
  int length{};

  if ( x < 0 || y < 0 )
    return;

  if ( y - 1 + h > vterm->height )
    y_end = vterm->height - y + 1;
  else
    y_end = h - 1;

  if ( x - 1 + w > vterm->width )
    length = vterm->width - x + 1;
  else
    length = w;

  if ( length < 1 )
    return;

  for (auto _y = 0; _y < y_end; _y++)  // line loop
  {
    const int line_len = area->width + area->right_shadow;
    const auto& tc = vterm->data[(y + _y - 1) * vterm->width + x - 1];  // terminal character
    auto& ac = area->data[(dy + _y) * line_len + dx];  // area character
    std::memcpy (&ac, &tc, sizeof(ac) * unsigned(length));

    if ( int(area->changes[dy + _y].xmin) > dx )
      area->changes[dy + _y].xmin = uInt(dx);

    if ( int(area->changes[dy + _y].xmax) < dx + length - 1 )
      area->changes[dy + _y].xmax = uInt(dx + length - 1);
  }
}

//----------------------------------------------------------------------
void FVTerm::putArea (const FTermArea* area) const
{
  // Add area changes to the virtual terminal

  if ( ! area || ! area->visible )
    return;

  int ax  = area->offset_left;
  const int ay  = area->offset_top;
  const int width = area->width + area->right_shadow;
  const int height = area->height + area->bottom_shadow;
  int ol{0};  // Outside left
  int y_end{};

  // Call the preprocessing handler methods
  callPreprocessingHandler(area);

  if ( ax < 0 )
  {
    ol = std::abs(ax);
    ax = 0;
  }

  if ( height + ay > vterm->height )
    y_end = vterm->height - ay;
  else
    y_end = height;

  for (auto y{0}; y < y_end; y++)  // Line loop
  {
    bool modified{false};
    auto line_xmin = int(area->changes[y].xmin);
    auto line_xmax = int(area->changes[y].xmax);

    if ( line_xmin > line_xmax )
      continue;

    if ( ax == 0 )
      line_xmin = ol;

    if ( width + ax - ol >= vterm->width )
      line_xmax = vterm->width + ol - ax - 1;

    if ( ax + line_xmin >= vterm->width )
      continue;

    for (auto x = line_xmin; x <= line_xmax; x++)  // Column loop
    {
      // Global terminal positions
      int tx = ax + x;
      const int ty = ay + y;

      if ( tx < 0 || ty < 0 )
        continue;

      tx -= ol;
      bool update = updateVTermCharacter(area, FPoint{x, y}, FPoint{tx, ty});

      if ( ! modified && ! update )
        line_xmin++;  // Don't update covered character

      if ( update )
        modified = true;
    }

    int _xmin = ax + line_xmin - ol;
    int _xmax = ax + line_xmax;

    if ( _xmin < int(vterm->changes[ay + y].xmin) )
      vterm->changes[ay + y].xmin = uInt(_xmin);

    if ( _xmax >= vterm->width )
      _xmax = vterm->width - 1;

    if ( _xmax > int(vterm->changes[ay + y].xmax) )
      vterm->changes[ay + y].xmax = uInt(_xmax);

    area->changes[y].xmin = uInt(width);
    area->changes[y].xmax = 0;
  }

  vterm->has_changes = true;
  updateVTermCursor(area);
}

//----------------------------------------------------------------------
void FVTerm::putArea (const FPoint& pos, const FTermArea* area)
{
  // Copies the given area block to the virtual terminal position

  if ( ! area || ! area->visible )
    return;

  int ax = pos.getX() - 1;
  const int ay = pos.getY() - 1;
  const int width = area->width + area->right_shadow;
  const int height = area->height + area->bottom_shadow;
  int ol{0};  // outside left
  int y_end{};
  int length{};

  if ( ax < 0 )
  {
    ol = std::abs(ax);
    ax = 0;
  }

  if ( ay + height > vterm->height )
    y_end = vterm->height - ay;
  else
    y_end = height;

  if ( width - ol + ax > vterm->width )
    length = vterm->width - ax;
  else
    length = width - ol;

  if ( length < 1 )
    return;

  for (auto y{0}; y < y_end; y++)  // line loop
  {
    if ( area->changes[y].trans_count == 0 )
    {
      // Line has only covered characters
      const auto& ac = area->data[y * width + ol];           // area character
      auto& tc = vterm->data[(ay + y) * vterm->width + ax];  // terminal character
      putAreaLine (ac, tc, std::size_t(length));
    }
    else
    {
      // Line has one or more transparent characters
      for (auto x{0}; x < length; x++)  // column loop
      {
        const int cx = ax + x;
        const int cy = ay + y;
        const auto& ac = area->data[y * width + ol + x];  // area character
        auto& tc = vterm->data[cy * vterm->width + cx];   // terminal character
        putAreaCharacter (FPoint{cx, cy}, area, ac, tc);
      }
    }

    if ( ax < int(vterm->changes[ay + y].xmin) )
      vterm->changes[ay + y].xmin = uInt(ax);

    if ( ax + length - 1 > int(vterm->changes[ay + y].xmax) )
      vterm->changes[ay + y].xmax = uInt(ax + length - 1);
  }

  vterm->has_changes = true;
}

//----------------------------------------------------------------------
int FVTerm::getLayer (const FVTerm* obj)
{
  // returns the layer from the FVTerm object

  if ( ! getWindowList() || getWindowList()->empty() )
    return -1;

  auto iter = getWindowList()->begin();
  const auto end = getWindowList()->end();

  while ( iter != end )
  {
    if ( *iter == obj )
      break;

    ++iter;
  }

  return int(std::distance(getWindowList()->begin(), iter) + 1);
}

//----------------------------------------------------------------------
void FVTerm::scrollAreaForward (FTermArea* area) const
{
  // Scrolls the entire area up line down

  if ( ! area )
    return;

  if ( area->height <= 1 )
    return;

  const int length = area->width;
  const int total_width = area->width + area->right_shadow;
  const int y_max = area->height - 1;

  for (auto y{0}; y < y_max; y++)
  {
    const int pos1 = y * total_width;
    const int pos2 = (y + 1) * total_width;
    const auto& sc = area->data[pos2];  // source character
    auto& dc = area->data[pos1];  // destination character
    std::memcpy (&dc, &sc, sizeof(dc) * unsigned(length));
    area->changes[y].xmin = 0;
    area->changes[y].xmax = uInt(area->width - 1);
  }

  // insert a new line below
  FChar nc{};  // next character
  auto bottom_right = std::size_t((y_max * total_width) - area->right_shadow - 1);
  const auto& lc = area->data[bottom_right];  // last character
  std::memcpy (&nc, &lc, sizeof(nc));
  nc.ch[0] = L' ';
  auto& dc = area->data[y_max * total_width];  // destination character
  std::fill_n (&dc, area->width, nc);
  area->changes[y_max].xmin = 0;
  area->changes[y_max].xmax = uInt(area->width - 1);
  area->has_changes = true;

  if ( area == vdesktop && TCAP(t_scroll_forward) )
  {
    setTermXY (0, vdesktop->height);
    FTerm::scrollTermForward();
    putArea (FPoint{1, 1}, vdesktop);

    // avoid update lines from 0 to (y_max - 1)
    for (auto y{0}; y < y_max; y++)
    {
      area->changes[y].xmin = uInt(area->width - 1);
      area->changes[y].xmax = 0;
    }
  }
}

//----------------------------------------------------------------------
void FVTerm::scrollAreaReverse (FTermArea* area) const
{
  // Scrolls the entire area one line down

  if ( ! area )
    return;

  if ( area->height <= 1 )
    return;

  const int length = area->width;
  const int total_width = area->width + area->right_shadow;
  const int y_max = area->height - 1;

  for (auto y = y_max; y > 0; y--)
  {
    const int pos1 = (y - 1) * total_width;
    const int pos2 = y * total_width;
    const auto& sc = area->data[pos1];  // source character
    auto& dc = area->data[pos2];  // destination character
    std::memcpy (&dc, &sc, sizeof(dc) * unsigned(length));
    area->changes[y].xmin = 0;
    area->changes[y].xmax = uInt(area->width - 1);
  }

  // insert a new line above
  FChar nc{};  // next character
  const auto& lc = area->data[total_width];  // last character
  std::memcpy (&nc, &lc, sizeof(nc));
  nc.ch[0] = L' ';
  auto& dc = area->data[0];  // destination character
  std::fill_n (&dc, area->width, nc);
  area->changes[0].xmin = 0;
  area->changes[0].xmax = uInt(area->width - 1);
  area->has_changes = true;

  if ( area == vdesktop && TCAP(t_scroll_reverse)  )
  {
    setTermXY (0, 0);
    FTerm::scrollTermReverse();
    putArea (FPoint{1, 1}, vdesktop);

    // avoid update lines from 1 to y_max
    for (auto y{1}; y <= y_max; y++)
    {
      area->changes[y].xmin = uInt(area->width - 1);
      area->changes[y].xmax = 0;
    }
  }
}

//----------------------------------------------------------------------
void FVTerm::clearArea (FTermArea* area, wchar_t fillchar) const
{
  // Clear the area with the current attributes

  FChar nc{};  // next character

  // Current attributes with a space character
  std::memcpy (&nc, &next_attribute, sizeof(nc));
  nc.ch[0] = fillchar;

  if ( ! (area && area->data) )
  {
    clearTerm (fillchar);
    return;
  }

  const auto w = uInt(area->width + area->right_shadow);

  if ( area->right_shadow == 0 )
  {
    if ( clearFullArea(area, nc) )
      return;
  }
  else
    clearAreaWithShadow(area, nc);

  for (auto i{0}; i < area->height; i++)
  {
    area->changes[i].xmin = 0;
    area->changes[i].xmax = w - 1;

    if ( nc.attr.bit.transparent
      || nc.attr.bit.color_overlay
      || nc.attr.bit.inherit_background )
      area->changes[i].trans_count = w;
    else if ( area->right_shadow != 0 )
      area->changes[i].trans_count = uInt(area->right_shadow);
    else
      area->changes[i].trans_count = 0;
  }

  for (auto i{0}; i < area->bottom_shadow; i++)
  {
    const int y = area->height + i;
    area->changes[y].xmin = 0;
    area->changes[y].xmax = w - 1;
    area->changes[y].trans_count = w;
  }

  area->has_changes = true;
}

//----------------------------------------------------------------------
void FVTerm::forceTerminalUpdate() const
{
  force_terminal_update = true;
  processTerminalUpdate();
  flush();
  force_terminal_update = false;
}

//----------------------------------------------------------------------
bool FVTerm::processTerminalUpdate() const
{
  // Checks if the resizing of the terminal is not finished
  if ( FTerm::hasChangedTermSize()  )
    return false;

  // Update data on VTerm
  updateVTerm();

  // Update the visible terminal
  return updateTerminal();
}

//----------------------------------------------------------------------
void FVTerm::startDrawing()
{
  // Pauses the terminal updates for the printing phase
  draw_completed = false;
}

//----------------------------------------------------------------------
void FVTerm::finishDrawing()
{
  // After the printing phase is completed, the terminal will be updated
  draw_completed = true;
}

//----------------------------------------------------------------------
void FVTerm::initTerminal()
{
  if ( fterm )
    fterm->initTerminal();

  // Hide the input cursor
  cursor_hideable = FTerm::isCursorHideable();
  hideCursor();

  // Initialize character lengths
  init_characterLengths();

  // Check for support for combined characters
  init_combined_character();
}


// private methods of FVTerm
//----------------------------------------------------------------------
inline void FVTerm::resetTextAreaToDefault ( const FTermArea* area
                                           , const FSize& size ) const
{
  FChar default_char;
  FLineChanges unchanged;

  default_char.ch[0]        = L' ';
  default_char.fg_color     = FColor::Default;
  default_char.bg_color     = FColor::Default;
  default_char.attr.byte[0] = 0;
  default_char.attr.byte[1] = 0;
  default_char.attr.byte[2] = 0;
  default_char.attr.byte[3] = 0;

  std::fill_n (area->data, size.getArea(), default_char);

  unchanged.xmin = uInt(size.getWidth());
  unchanged.xmax = 0;
  unchanged.trans_count = 0;

  std::fill_n (area->changes, size.getHeight(), unchanged);
}

//----------------------------------------------------------------------
inline bool FVTerm::reallocateTextArea ( FTermArea* area
                                       , std::size_t height
                                       , std::size_t size )
{
  // Reallocate "height" lines for changes
  // and "size" bytes for the text area

  if ( area->changes != nullptr )
    delete[] area->changes;

  if ( area->data != nullptr )
    delete[] area->data;

  try
  {
    area->changes = new FLineChanges[height];
    area->data    = new FChar[size];
  }
  catch (const std::bad_alloc&)
  {
    badAllocOutput ("FLineChanges[height] or FChar[size]");
    return false;
  }

  return true;
}

//----------------------------------------------------------------------
inline bool FVTerm::reallocateTextArea (FTermArea* area, std::size_t size)
{
  // Reallocate "size" bytes for the text area

  if ( area->data != nullptr )
    delete[] area->data;

  try
  {
    area->data = new FChar[size];
  }
  catch (const std::bad_alloc&)
  {
    badAllocOutput ("FChar[size]");
    return false;
  }

  return true;
}

//----------------------------------------------------------------------
FVTerm::CoveredState FVTerm::isCovered ( const FPoint& pos
                                       , const FTermArea* area )
{
  // Determines the covered state for the given position

  if ( ! area )
    return CoveredState::None;

  auto is_covered = CoveredState::None;

  if ( getWindowList() && ! getWindowList()->empty() )
  {
    bool found{ area == vdesktop };

    for (auto& win_obj : *getWindowList())
    {
      const auto& win = win_obj->getVWin();

      if ( ! (win && win->visible) )
        continue;

      const int& win_x = win->offset_left;
      const int& win_y = win->offset_top;
      const FRect geometry { win_x , win_y
          , std::size_t(win->width) + std::size_t(win->right_shadow)
          , std::size_t(win->height) + std::size_t(win->bottom_shadow) };

      if ( found && geometry.contains(pos) )
      {
        const int width = win->width + win->right_shadow;
        const int& x = pos.getX();
        const int& y = pos.getY();
        const auto& tmp = &win->data[(y - win_y) * width + (x - win_x)];

        if ( tmp->attr.bit.color_overlay )
        {
          is_covered = CoveredState::Half;
        }
        else if ( ! tmp->attr.bit.transparent )
        {
          is_covered = CoveredState::Full;
          break;
        }
      }

      if ( area == win )
        found = true;
    }
  }

  return is_covered;
}

//----------------------------------------------------------------------
inline void FVTerm::updateOverlappedColor ( const FChar& area_char
                                          , const FChar& over_char
                                          , FChar& vterm_char )
{
  // Add the overlapping color to this character

  // New character
  FChar nc{};
  std::memcpy (&nc, &area_char, sizeof(nc));
  nc.fg_color = over_char.fg_color;
  nc.bg_color = over_char.bg_color;
  nc.attr.bit.reverse  = false;
  nc.attr.bit.standout = false;

  if ( nc.ch[0] == UniChar::LowerHalfBlock
    || nc.ch[0] == UniChar::UpperHalfBlock
    || nc.ch[0] == UniChar::LeftHalfBlock
    || nc.ch[0] == UniChar::RightHalfBlock
    || nc.ch[0] == UniChar::MediumShade
    || nc.ch[0] == UniChar::FullBlock )
    nc.ch[0] = L' ';

  nc.attr.bit.no_changes = bool(vterm_char.attr.bit.printed && vterm_char == nc);
  std::memcpy (&vterm_char, &nc, sizeof(vterm_char));
}

//----------------------------------------------------------------------
inline void FVTerm::updateOverlappedCharacter ( FChar& cover_char
                                              , FChar& vterm_char )
{
  // Restore one character on vterm

  cover_char.attr.bit.no_changes = \
      bool(vterm_char.attr.bit.printed && vterm_char == cover_char);
  std::memcpy (&vterm_char, &cover_char, sizeof(vterm_char));
}

//----------------------------------------------------------------------
inline void FVTerm::updateShadedCharacter ( const FChar& area_char
                                          , FChar& cover_char
                                          , FChar& vterm_char )
{
  // Get covered character + add the current color

  cover_char.fg_color = area_char.fg_color;
  cover_char.bg_color = area_char.bg_color;
  cover_char.attr.bit.reverse  = false;
  cover_char.attr.bit.standout = false;

  if ( cover_char.ch[0] == UniChar::LowerHalfBlock
    || cover_char.ch[0] == UniChar::UpperHalfBlock
    || cover_char.ch[0] == UniChar::LeftHalfBlock
    || cover_char.ch[0] == UniChar::RightHalfBlock
    || cover_char.ch[0] == UniChar::MediumShade
    || cover_char.ch[0] == UniChar::FullBlock )
    cover_char.ch[0] = L' ';

  cover_char.attr.bit.no_changes = \
      bool(vterm_char.attr.bit.printed && vterm_char == cover_char);
  std::memcpy (&vterm_char, &cover_char, sizeof(vterm_char));
}

//----------------------------------------------------------------------
inline void FVTerm::updateInheritBackground ( const FChar& area_char
                                            , const FChar& cover_char
                                            , FChar& vterm_char )
{
  // Add the covered background to this character

  // New character
  FChar nc{};
  std::memcpy (&nc, &area_char, sizeof(nc));
  nc.bg_color = cover_char.bg_color;
  nc.attr.bit.no_changes = \
      bool(vterm_char.attr.bit.printed && vterm_char == nc);
  std::memcpy (&vterm_char, &nc, sizeof(vterm_char));
}

//----------------------------------------------------------------------
inline void FVTerm::updateCharacter (const FChar& area_char, FChar& vterm_char)
{
  // Copy a area character to the virtual terminal

  std::memcpy (&vterm_char, &area_char, sizeof(vterm_char));

  if ( vterm_char.attr.bit.printed && vterm_char == area_char )
    vterm_char.attr.bit.no_changes = true;
  else
    vterm_char.attr.bit.no_changes = false;
}

//----------------------------------------------------------------------
bool FVTerm::updateVTermCharacter ( const FTermArea* area
                                  , const FPoint& area_pos
                                  , const FPoint& terminal_pos )
{
  // Area character
  const int width = area->width + area->right_shadow;
  const int area_index = area_pos.getY() * width + area_pos.getX();
  const auto& ac = area->data[area_index];
  // Terminal character
  const int terminal_index = terminal_pos.getY() * vterm->width
                           + terminal_pos.getX();
  auto& tc = vterm->data[terminal_index];

  // Get covered state
  const auto is_covered = isCovered(terminal_pos, area);

  if ( is_covered == CoveredState::Full )
    return false;

  if ( is_covered == CoveredState::Half )
  {
    // Overlapped character
    auto oc = getOverlappedCharacter (terminal_pos, area);
    updateOverlappedColor (ac, oc, tc);
  }
  else if ( ac.attr.bit.transparent )   // Transparent
  {
    // Covered character
    auto cc = getCoveredCharacter (terminal_pos, area);
    updateOverlappedCharacter (cc, tc);
  }
  else  // Not transparent
  {
    if ( ac.attr.bit.color_overlay )  // Transparent shadow
    {
      // Covered character
      auto cc = getCoveredCharacter (terminal_pos, area);
      updateShadedCharacter (ac, cc, tc);
    }
    else if ( ac.attr.bit.inherit_background )
    {
      // Covered character
      auto cc = getCoveredCharacter (terminal_pos, area);
      updateInheritBackground (ac, cc, tc);
    }
    else  // Default
    {
      updateCharacter (ac, tc);
    }
  }

  return true;
}

//----------------------------------------------------------------------
void FVTerm::updateVTerm() const
{
  // Updates the character data from all areas to VTerm

  if ( hasPendingUpdates(vdesktop) )
  {
    putArea(vdesktop);
    vdesktop->has_changes = false;
  }

  if ( ! getWindowList() || getWindowList()->empty() )
    return;

  for (auto&& window : *getWindowList())
  {
    auto v_win = window->getVWin();

    if ( ! (v_win && v_win->visible) )
      continue;

    if ( hasPendingUpdates(v_win) )
    {
      putArea(v_win);
      v_win->has_changes = false;
    }
    else if ( hasChildAreaChanges(v_win) )
    {
      putArea(v_win);  // and call the child area processing handler there
      clearChildAreaChanges(v_win);
    }
  }
}

//----------------------------------------------------------------------
void FVTerm::callPreprocessingHandler (const FTermArea* area)
{
  // Call preprocessing handler

  if ( ! area || area->preproc_list.empty() )
    return;

  for (auto&& pcall : area->preproc_list)
  {
    // call the preprocessing handler
    auto preprocessingHandler = pcall->function;
    preprocessingHandler();
  }
}

//----------------------------------------------------------------------
bool FVTerm::hasChildAreaChanges (FTermArea* area) const
{
  if ( ! area || area->preproc_list.empty() )
    return false;

  return std::any_of ( area->preproc_list.begin()
                     , area->preproc_list.end()
                     , [] (const std::unique_ptr<FVTermPreprocessing>& pcall)
                       {
                         return pcall->instance
                             && pcall->instance->child_print_area
                             && pcall->instance->child_print_area->has_changes;
                       }
                     );
}

//----------------------------------------------------------------------
void FVTerm::clearChildAreaChanges (const FTermArea* area) const
{
  if ( ! area || area->preproc_list.empty() )
    return;

  for (auto&& pcall : area->preproc_list)
  {
    if ( pcall->instance && pcall->instance->child_print_area )
      pcall->instance->child_print_area->has_changes = false;
  }
}

//----------------------------------------------------------------------
bool FVTerm::isInsideArea (const FPoint& pos, const FTermArea* area)
{
  // Check whether the coordinates are within the area

  const auto aw = std::size_t(area->width);
  const auto ah = std::size_t(area->height);
  FRect area_geometry{0, 0, aw, ah};

  if ( area_geometry.contains(pos) )
    return true;
  else
    return false;
}

//----------------------------------------------------------------------
FChar FVTerm::generateCharacter (const FPoint& pos)
{
  // Generates characters for a given position considering all areas

  const int x = pos.getX();
  const int y = pos.getY();
  auto sc = &vdesktop->data[y * vdesktop->width + x];  // shown character

  if ( ! getWindowList() || getWindowList()->empty() )
    return *sc;

  for (auto& win_obj : *getWindowList())
  {
    const auto& win = win_obj->getVWin();

    if ( ! win || ! win->visible )
      continue;

    const int win_x = win->offset_left;
    const int win_y = win->offset_top;
    const FRect geometry { win_x, win_y
        , std::size_t(win->width) + std::size_t(win->right_shadow)
        , std::size_t(win->height) + std::size_t(win->bottom_shadow) };

    // Window is visible and contains current character
    if ( geometry.contains(x, y) )
    {
      const int line_len = win->width + win->right_shadow;
      auto tmp = &win->data[(y - win_y) * line_len + (x - win_x)];

      if ( ! tmp->attr.bit.transparent )   // Current character not transparent
      {
        if ( tmp->attr.bit.color_overlay )  // Transparent shadow
        {
          // Keep the current vterm character
          if ( sc != &s_ch )
            std::memcpy (&s_ch, sc, sizeof(s_ch));

          s_ch.fg_color = tmp->fg_color;
          s_ch.bg_color = tmp->bg_color;
          s_ch.attr.bit.reverse  = false;
          s_ch.attr.bit.standout = false;

          if ( s_ch.ch[0] == UniChar::LowerHalfBlock
            || s_ch.ch[0] == UniChar::UpperHalfBlock
            || s_ch.ch[0] == UniChar::LeftHalfBlock
            || s_ch.ch[0] == UniChar::RightHalfBlock
            || s_ch.ch[0] == UniChar::MediumShade
            || s_ch.ch[0] == UniChar::FullBlock )
            s_ch.ch[0] = L' ';

          sc = &s_ch;
        }
        else if ( tmp->attr.bit.inherit_background )
        {
          // Add the covered background to this character
          std::memcpy (&i_ch, tmp, sizeof(i_ch));
          i_ch.bg_color = sc->bg_color;  // Last background color
          sc = &i_ch;
        }
        else  // Default
          sc = tmp;
      }
    }
  }

  return *sc;
}

//----------------------------------------------------------------------
FChar FVTerm::getCharacter ( CharacterType char_type
                           , const FPoint& pos
                           , const FTermArea* area )
{
  // Gets the overlapped or the covered character for a given position

  const int x = pos.getX();
  const int y = pos.getY();
  int xx = ( x > 0 ) ? x : 0;
  int yy = ( y > 0 ) ? y : 0;

  if ( xx >= vterm->width )
    xx = vterm->width - 1;

  if ( yy >= vterm->height )
    yy = vterm->height - 1;

  auto cc = &vdesktop->data[yy * vdesktop->width + xx];  // covered character

  if ( ! area || ! getWindowList() || getWindowList()->empty() )
    return *cc;

  // Get the window layer of this widget object
  const auto has_an_owner = area->hasOwner();
  const auto area_owner = area->getOwner<FVTerm*>();
  const int layer = has_an_owner ? getLayer(area_owner) : 0;

  for (auto&& win_obj : *getWindowList())
  {
    bool significant_char{false};

    // char_type can be "overlapped_character"
    // or "covered_character"
    if ( char_type == CharacterType::Covered )
      significant_char = bool(layer >= getLayer(win_obj));
    else
      significant_char = bool(layer < getLayer(win_obj));

    if ( has_an_owner && area_owner != win_obj && significant_char )
    {
      const auto& win = win_obj->getVWin();

      if ( ! win || ! win->visible )
        continue;

      const FRect geometry { win->offset_left, win->offset_top
          , std::size_t(win->width) + std::size_t(win->right_shadow)
          , std::size_t(win->height) + std::size_t(win->bottom_shadow) };

      // Window visible and contains current character
      if ( geometry.contains(x, y) )
        getAreaCharacter (FPoint{x, y}, win, cc);
    }
    else if ( char_type == CharacterType::Covered )
      break;
  }

  return *cc;
}

//----------------------------------------------------------------------
inline FChar FVTerm::getCoveredCharacter (const FPoint& pos, const FTermArea* area)
{
  // Gets the covered character for a given position
  return getCharacter (CharacterType::Covered, pos, area);
}

//----------------------------------------------------------------------
inline FChar FVTerm::getOverlappedCharacter (const FPoint& pos, const FTermArea* area)
{
  // Gets the overlapped character for a given position
  return getCharacter (CharacterType::Overlapped, pos, area);
}

//----------------------------------------------------------------------
void FVTerm::init()
{
  init_object = this;
  vterm       = nullptr;
  vdesktop    = nullptr;

  try
  {
    fterm         = std::make_shared<FTerm>();
    term_pos      = std::make_shared<FPoint>(-1, -1);
    output_buffer = std::make_shared<OutputBuffer>();
    window_list   = std::make_shared<FVTermList>();
  }
  catch (const std::bad_alloc&)
  {
    badAllocOutput ("FTerm, FPoint, or std::queue<int>");
    return;
  }

  // Presetting of the current locale for full-width character support.
  // The final setting is made later in FTerm::init_locale().
  std::setlocale (LC_ALL, "");

  // term_attribute stores the current state of the terminal
  term_attribute.ch           = {{ L'\0' }};
  term_attribute.fg_color     = FColor::Default;
  term_attribute.bg_color     = FColor::Default;
  term_attribute.attr.byte[0] = 0;
  term_attribute.attr.byte[1] = 0;
  term_attribute.attr.byte[2] = 0;
  term_attribute.attr.byte[3] = 0;

  // next_attribute contains the state of the next printed character
  std::memcpy (&next_attribute, &term_attribute, sizeof(next_attribute));

  // Create virtual terminal
  FRect term_geometry {0, 0, FTerm::getColumnNumber(), FTerm::getLineNumber()};
  createVTerm (term_geometry.getSize());

  // Create virtual desktop area
  FSize shadow_size{0, 0};
  createArea (term_geometry, shadow_size, vdesktop);
  vdesktop->visible = true;
  active_area = vdesktop;

  // Initialize the flush and last terminal size check time
  time_last_flush.tv_sec = 0;
  time_last_flush.tv_usec = 0;
  last_term_size_check.tv_sec = 0;
  last_term_size_check.tv_usec = 0;
}

//----------------------------------------------------------------------
void FVTerm::init_characterLengths()
{
  const auto& opti_move = FTerm::getFOptiMove();
  cursor_address_length = opti_move.getCursorAddressLength();
  erase_char_length     = opti_move.getEraseCharsLength();
  repeat_char_length    = opti_move.getRepeatCharLength();
  clr_bol_length        = opti_move.getClrBolLength();
  clr_eol_length        = opti_move.getClrEolLength();

  if ( cursor_address_length == 0 )
    cursor_address_length = INT_MAX;

  if ( erase_char_length == 0 )
    erase_char_length = INT_MAX;

  if ( repeat_char_length == 0 )
    repeat_char_length = INT_MAX;

  if ( clr_bol_length == 0 )
    clr_bol_length = INT_MAX;

  if ( clr_eol_length == 0 )
    clr_eol_length = INT_MAX;
}

//----------------------------------------------------------------------
void FVTerm::init_combined_character()
{
#if defined(__FreeBSD__) || defined(__DragonFly__) || defined(UNIT_TEST)
  if ( FTermFreeBSD::isFreeBSDConsole() )
    return;
#endif

  if ( FTerm::getEncoding() != Encoding::UTF8 )
    return;

  const auto& term_detection = FTerm::getFTermDetection();

  if ( term_detection.isCygwinTerminal() )
    return;

  if ( term_detection.isXTerminal()
    || term_detection.isUrxvtTerminal()
    || term_detection.isMinttyTerm()
    || term_detection.isPuttyTerminal() )
  {
    combined_char_support = true;
  }
}

//----------------------------------------------------------------------
void FVTerm::finish() const
{
  // Show the input cursor
  showCursor();

  // Clear the terminal
  setNormal();

  if ( FTerm::hasAlternateScreen()
    && FTerm::getFTermData().isInAlternateScreen() )
    clearTerm();

  forceTerminalUpdate();

  // remove virtual terminal + virtual desktop area
  removeArea (vdesktop);
  removeArea (vterm);

  init_object = nullptr;
}

//----------------------------------------------------------------------
void FVTerm::putAreaLine (const FChar& area_char, FChar& vterm_char, std::size_t length)
{
  // copy "length" characters from area to terminal

  std::memcpy (&vterm_char, &area_char, sizeof(vterm_char) * length);
}

//----------------------------------------------------------------------
void FVTerm::putAreaCharacter ( const FPoint& pos, const FTermArea* area
                              , const FChar& area_char, FChar& vterm_char )
{
  if ( area_char.attr.bit.transparent )  // Transparent
  {
    // Restore one character on vterm
    FChar ch = getCoveredCharacter (pos, area);
    std::memcpy (&vterm_char, &ch, sizeof(vterm_char));
  }
  else  // Mot transparent
  {
    if ( area_char.attr.bit.color_overlay )  // Transparent shadow
    {
      // Get covered character + add the current color
      FChar ch = getCoveredCharacter (pos, area);
      ch.fg_color = area_char.fg_color;
      ch.bg_color = area_char.bg_color;
      ch.attr.bit.reverse  = false;
      ch.attr.bit.standout = false;

      if ( ch.ch[0] == UniChar::LowerHalfBlock
        || ch.ch[0] == UniChar::UpperHalfBlock
        || ch.ch[0] == UniChar::LeftHalfBlock
        || ch.ch[0] == UniChar::RightHalfBlock
        || ch.ch[0] == UniChar::MediumShade
        || ch.ch[0] == UniChar::FullBlock )
        ch.ch[0] = L' ';

      std::memcpy (&vterm_char, &ch, sizeof(vterm_char));
    }
    else if ( area_char.attr.bit.inherit_background )
    {
      // Add the covered background to this character
      FChar ch{};
      std::memcpy (&ch, &area_char, sizeof(ch));
      FChar cc = getCoveredCharacter (pos, area);
      ch.bg_color = cc.bg_color;
      std::memcpy (&vterm_char, &ch, sizeof(vterm_char));
    }
    else  // Default
      std::memcpy (&vterm_char, &area_char, sizeof(vterm_char));
  }
}

//----------------------------------------------------------------------
void FVTerm::getAreaCharacter ( const FPoint& pos, const FTermArea* area
                              , FChar*& cc )
{
  const int area_x = area->offset_left;
  const int area_y = area->offset_top;
  const int line_len = area->width + area->right_shadow;
  const int x = pos.getX();
  const int y = pos.getY();
  auto& tmp = area->data[(y - area_y) * line_len + (x - area_x)];

  // Current character not transparent
  if ( ! tmp.attr.bit.transparent )
  {
    if ( tmp.attr.bit.color_overlay )  // transparent shadow
    {
      // Keep the current vterm character
      std::memcpy (&s_ch, cc, sizeof(s_ch));
      s_ch.fg_color = tmp.fg_color;
      s_ch.bg_color = tmp.bg_color;
      s_ch.attr.bit.reverse  = false;
      s_ch.attr.bit.standout = false;
      cc = &s_ch;
    }
    else if ( tmp.attr.bit.inherit_background )
    {
      // Add the covered background to this character
      std::memcpy (&i_ch, &tmp, sizeof(i_ch));
      i_ch.bg_color = cc->bg_color;  // last background color
      cc = &i_ch;
    }
    else  // default
      cc = &tmp;
  }
}

//----------------------------------------------------------------------
bool FVTerm::clearTerm (wchar_t fillchar) const
{
  // Clear the real terminal and put cursor at home

  const auto& cl = TCAP(t_clear_screen);
  const auto& cd = TCAP(t_clr_eos);
  const auto& cb = TCAP(t_clr_eol);
  const bool ut = FTermcap::background_color_erase;
  const bool normal = FTerm::isNormal (next_attribute);
  appendAttributes (next_attribute);

  if ( ! ( (cl || cd || cb) && (normal || ut) )
    || fillchar != L' '
    || ! draw_completed )
  {
    return false;
  }

  if ( cl )  // Clear screen
  {
    appendOutputBuffer (FTermControl{cl});
    term_pos->setPoint(0, 0);
  }
  else if ( cd )  // Clear to end of screen
  {
    setTermXY (0, 0);
    appendOutputBuffer (FTermControl{cd});
    term_pos->setPoint(-1, -1);
  }
  else if ( cb )  // Clear to end of line
  {
    term_pos->setPoint(-1, -1);

    for (auto i{0}; i < int(FTerm::getLineNumber()); i++)
    {
      setTermXY (0, i);
      appendOutputBuffer (FTermControl{cb});
    }

    setTermXY (0, 0);
  }

  flush();
  return true;
}

//----------------------------------------------------------------------
bool FVTerm::clearFullArea (const FTermArea* area, FChar& nc) const
{
  // Clear area
  const int area_size = area->width * area->height;
  std::fill_n (area->data, area_size, nc);

  if ( area != vdesktop )  // Is the area identical to the desktop?
    return false;

  // Try to clear the terminal rapidly with a control sequence
  if ( clearTerm (nc.ch[0]) )
  {
    nc.attr.bit.printed = true;
    std::fill_n (vterm->data, area_size, nc);
  }
  else
  {
    for (auto i{0}; i < vdesktop->height; i++)
    {
      vdesktop->changes[i].xmin = 0;
      vdesktop->changes[i].xmax = uInt(vdesktop->width) - 1;
      vdesktop->changes[i].trans_count = 0;
    }

    vdesktop->has_changes = true;
  }

  return true;
}

//----------------------------------------------------------------------
void FVTerm::clearAreaWithShadow (const FTermArea* area, const FChar& nc)
{
  FChar t_char = nc;
  const int total_width = area->width + area->right_shadow;
  t_char.attr.bit.transparent = true;

  for (auto y{0}; y < area->height; y++)
  {
    const int pos = y * total_width;
    // Clear area
    std::fill_n (&area->data[pos], total_width, nc);
    // Make right shadow transparent
    std::fill_n (&area->data[pos + area->width], area->right_shadow, t_char);
  }

  // Make bottom shadow transparent
  for (auto y{0}; y < area->bottom_shadow; y++)
  {
    const int pos = total_width * (y + area->height);
    std::fill_n (&area->data[pos], total_width, t_char);
  }
}

//----------------------------------------------------------------------
bool FVTerm::canClearToEOL (uInt xmin, uInt y)
{
  // Is the line from xmin to the end of the line blank?
  // => clear to end of line

  auto& vt = vterm;
  const auto& ce = TCAP(t_clr_eol);
  const auto& min_char = vt->data[y * uInt(vt->width) + xmin];

  if ( ce && min_char.ch[0] == L' ' )
  {
    uInt beginning_whitespace = 1;
    const bool normal = FTerm::isNormal(min_char);
    const bool& ut = FTermcap::background_color_erase;

    for (uInt x = xmin + 1; x < uInt(vt->width); x++)
    {
      const auto& ch = vt->data[y * uInt(vt->width) + x];

      if ( min_char == ch )
        beginning_whitespace++;
      else
        break;
    }

    if ( beginning_whitespace == uInt(vt->width) - xmin
      && (ut || normal)
      && clr_eol_length < beginning_whitespace )
      return true;
  }

  return false;
}

//----------------------------------------------------------------------
bool FVTerm::canClearLeadingWS (uInt& xmin, uInt y)
{
  // Line has leading whitespace
  // => clear from xmin to beginning of line

  auto& vt = vterm;
  const auto& cb = TCAP(t_clr_bol);
  const auto& first_char = vt->data[y * uInt(vt->width)];

  if ( cb && first_char.ch[0] == L' ' )
  {
    uInt leading_whitespace = 1;
    const bool normal = FTerm::isNormal(first_char);
    const bool& ut = FTermcap::background_color_erase;

    for (uInt x{1}; x < uInt(vt->width); x++)
    {
      const auto& ch = vt->data[y * uInt(vt->width) + x];

      if ( first_char == ch )
        leading_whitespace++;
      else
        break;
    }

    if ( leading_whitespace > xmin
      && (ut || normal)
      && clr_bol_length < leading_whitespace )
    {
      xmin = leading_whitespace - 1;
      return true;
    }
  }

  return false;
}

//----------------------------------------------------------------------
bool FVTerm::canClearTrailingWS (uInt& xmax, uInt y)
{
  // Line has trailing whitespace
  // => clear from xmax to end of line

  auto& vt = vterm;
  const auto& ce = TCAP(t_clr_eol);
  const auto& last_char = vt->data[(y + 1) * uInt(vt->width) - 1];

  if ( ce && last_char.ch[0] == L' ' )
  {
    uInt trailing_whitespace = 1;
    const bool normal = FTerm::isNormal(last_char);
    const bool& ut = FTermcap::background_color_erase;

    for (uInt x = uInt(vt->width) - 1; x >  0 ; x--)
    {
      const auto& ch = vt->data[y * uInt(vt->width) + x];

      if ( last_char == ch )
        trailing_whitespace++;
      else
        break;
    }

    if ( trailing_whitespace > uInt(vt->width) - xmax
      && (ut || normal)
      && clr_bol_length < trailing_whitespace )
    {
      xmax = uInt(vt->width) - trailing_whitespace;
      return true;
    }
  }

  return false;
}

//----------------------------------------------------------------------
bool FVTerm::skipUnchangedCharacters (uInt& x, uInt xmax, uInt y) const
{
  // Skip characters without changes if it is faster than redrawing

  auto& vt = vterm;
  auto& print_char = vt->data[y * uInt(vt->width) + x];
  print_char.attr.bit.printed = true;

  if ( print_char.attr.bit.no_changes )
  {
    uInt count{1};

    for (uInt i = x + 1; i <= xmax; i++)
    {
      const auto& ch = vt->data[y * uInt(vt->width) + i];

      if ( ch.attr.bit.no_changes )
        count++;
      else
        break;
    }

    if ( count > cursor_address_length )
    {
      setTermXY (int(x + count), int(y));
      x = x + count - 1;
      return true;
    }
  }

  return false;
}

//----------------------------------------------------------------------
void FVTerm::printRange ( uInt xmin, uInt xmax, uInt y
                        , bool draw_trailing_ws ) const
{
  for (uInt x = xmin; x <= xmax; x++)
  {
    auto& vt = vterm;
    const auto& ec = TCAP(t_erase_chars);
    const auto& rp = TCAP(t_repeat_char);
    auto& print_char = vt->data[y * uInt(vt->width) + x];
    print_char.attr.bit.printed = true;
    replaceNonPrintableFullwidth (x, print_char);

    // skip character with no changes
    if ( skipUnchangedCharacters(x, xmax, y) )
      continue;

    // Erase character
    if ( ec && print_char.ch[0] == L' ' )
    {
      PrintState erase_state = \
          eraseCharacters(x, xmax, y, draw_trailing_ws);

      if ( erase_state == PrintState::LineCompletelyPrinted )
        break;
    }
    else if ( rp )  // Repeat one character n-fold
    {
      repeatCharacter(x, xmax, y);
    }
    else  // General character output
    {
      bool min_and_not_max( x == xmin && xmin != xmax );
      printCharacter (x, y, min_and_not_max, print_char);
    }
  }
}

//----------------------------------------------------------------------
inline void FVTerm::replaceNonPrintableFullwidth ( uInt x
                                                 , FChar& print_char ) const
{
  // Replace non-printable full-width characters that are truncated
  // from the right or left terminal side

  if ( x == 0 && isFullWidthPaddingChar(print_char) )
  {
    print_char.ch[0] = wchar_t(UniChar::SingleLeftAngleQuotationMark);  // ‹
    print_char.ch[1] = L'\0';
    print_char.attr.bit.fullwidth_padding = false;
  }
  else if ( x == uInt(vterm->width - 1)
         && isFullWidthChar(print_char) )
  {
    print_char.ch[0] = wchar_t(UniChar::SingleRightAngleQuotationMark);  // ›
    print_char.ch[1] = L'\0';
    print_char.attr.bit.char_width = 1;
  }
}

//----------------------------------------------------------------------
void FVTerm::printCharacter ( uInt& x, uInt y, bool min_and_not_max
                            , FChar& print_char) const
{
  // General character output on terminal

  if ( x < uInt(vterm->width - 1) && isFullWidthChar(print_char) )
  {
    printFullWidthCharacter (x, y, print_char);
  }
  else if ( x > 0 && x < uInt(vterm->width - 1)
         && isFullWidthPaddingChar(print_char)  )
  {
    printFullWidthPaddingCharacter (x, y, print_char);
  }
  else if ( x > 0 && min_and_not_max )
  {
    printHalfCovertFullWidthCharacter (x, y, print_char);
  }
  else
  {
    // Print a half-width character
    appendCharacter (print_char);
    markAsPrinted (x, y);
  }
}

//----------------------------------------------------------------------
void FVTerm::printFullWidthCharacter ( uInt& x, uInt y
                                     , FChar& print_char ) const
{
  const auto vt = vterm;
  auto& next_char = vt->data[y * uInt(vt->width) + x + 1];

  if ( print_char.attr.byte[0] == next_char.attr.byte[0]
    && print_char.attr.byte[1] == next_char.attr.byte[1]
    && print_char.fg_color == next_char.fg_color
    && print_char.bg_color == next_char.bg_color
    && isFullWidthChar(print_char)
    && isFullWidthPaddingChar(next_char) )
  {
    // Print a full-width character
    appendCharacter (print_char);
    markAsPrinted (x, y);
    skipPaddingCharacter (x, y, print_char);
  }
  else
  {
    // Print ellipses for the 1st full-width character column
    appendAttributes (print_char);
    appendOutputBuffer (FTermChar{wchar_t(UniChar::HorizontalEllipsis)});
    term_pos->x_ref()++;
    markAsPrinted (x, y);

    if ( isFullWidthPaddingChar(next_char) )
    {
      // Print ellipses for the 2nd full-width character column
      x++;
      appendAttributes (next_char);
      appendOutputBuffer (FTermChar{wchar_t(UniChar::HorizontalEllipsis)});
      term_pos->x_ref()++;
      markAsPrinted (x, y);
    }
  }
}

//----------------------------------------------------------------------
void FVTerm::printFullWidthPaddingCharacter ( uInt& x, uInt y
                                            , FChar& print_char) const
{
  const auto vt = vterm;
  auto& prev_char = vt->data[y * uInt(vt->width) + x - 1];

  if ( print_char.attr.byte[0] == prev_char.attr.byte[0]
    && print_char.attr.byte[1] == prev_char.attr.byte[1]
    && print_char.fg_color == prev_char.fg_color
    && print_char.bg_color == prev_char.bg_color
    && isFullWidthChar(prev_char)
    && isFullWidthPaddingChar(print_char) )
  {
    // Move cursor one character to the left
    const auto& le = TCAP(t_cursor_left);
    const auto& LE = TCAP(t_parm_left_cursor);

    if ( le )
      appendOutputBuffer (FTermControl{le});
    else if ( LE )
      appendOutputBuffer (FTermControl{FTermcap::encodeParameter(LE, 1)});
    else
    {
      skipPaddingCharacter (x, y, prev_char);
      return;
    }

    // Print a full-width character
    x--;
    term_pos->x_ref()--;
    appendCharacter (prev_char);
    markAsPrinted (x, y);
    skipPaddingCharacter (x, y, prev_char);
  }
  else
  {
    // Print ellipses for the 1st full-width character column
    appendAttributes (print_char);
    appendOutputBuffer (FTermChar{wchar_t(UniChar::HorizontalEllipsis)});
    term_pos->x_ref()++;
    markAsPrinted (x, y);
  }
}

//----------------------------------------------------------------------
void FVTerm::printHalfCovertFullWidthCharacter ( uInt& x, uInt y
                                               , FChar& print_char ) const
{
  const auto vt = vterm;
  auto& prev_char = vt->data[y * uInt(vt->width) + x - 1];

  if ( isFullWidthChar(prev_char) && ! isFullWidthPaddingChar(print_char) )
  {
    // Move cursor one character to the left
    const auto& le = TCAP(t_cursor_left);
    const auto& LE = TCAP(t_parm_left_cursor);

    if ( le )
      appendOutputBuffer (FTermControl{le});
    else if ( LE )
      appendOutputBuffer (FTermControl{FTermcap::encodeParameter(LE, 1)});

    if ( le || LE )
    {
      // Print ellipses for the 1st full-width character column
      x--;
      term_pos->x_ref()--;
      appendAttributes (prev_char);
      appendOutputBuffer (FTermChar{wchar_t(UniChar::HorizontalEllipsis)});
      term_pos->x_ref()++;
      markAsPrinted (x, y);
      x++;
    }
  }

  // Print a half-width character
  appendCharacter (print_char);
  markAsPrinted (x, y);
}

//----------------------------------------------------------------------
inline void FVTerm::skipPaddingCharacter ( uInt& x, uInt y
                                         , const FChar& print_char ) const
{
  if ( isFullWidthChar(print_char) )  // full-width character
  {
    x++;  // Skip the following padding character
    term_pos->x_ref()++;
    markAsPrinted (x, y);
  }
}

//----------------------------------------------------------------------
FVTerm::PrintState FVTerm::eraseCharacters ( uInt& x, uInt xmax, uInt y
                                          , bool draw_trailing_ws ) const
{
  // Erase a number of characters to draw simple whitespaces

  const auto& vt = vterm;
  const auto& ec = TCAP(t_erase_chars);
  auto& print_char = vt->data[y * uInt(vt->width) + x];

  if ( ! ec || print_char.ch[0] != L' ' )
    return PrintState::NothingPrinted;

  uInt whitespace{1};

  for (uInt i = x + 1; i <= xmax; i++)
  {
    const auto& ch = vt->data[y * uInt(vt->width) + i];

    if ( print_char == ch )
      whitespace++;
    else
      break;
  }

  if ( whitespace == 1 )
  {
    appendCharacter (print_char);
    markAsPrinted (x, y);
  }
  else
  {
    const uInt start_pos = x;
    const bool& ut = FTermcap::background_color_erase;
    const bool normal = FTerm::isNormal(print_char);

    if ( whitespace > erase_char_length + cursor_address_length
      && (ut || normal) )
    {
      appendAttributes (print_char);
      appendOutputBuffer (FTermControl{FTermcap::encodeParameter(ec, whitespace)});

      if ( x + whitespace - 1 < xmax || draw_trailing_ws )
        setTermXY (int(x + whitespace), int(y));
      else
        return PrintState::LineCompletelyPrinted;

      x = x + whitespace - 1;
    }
    else
    {
      x--;

      for (uInt i{0}; i < whitespace; i++)
      {
        appendCharacter (print_char);
        x++;
      }
    }

    markAsPrinted (start_pos, x, y);
  }

  return PrintState::WhitespacesPrinted;
}

//----------------------------------------------------------------------
FVTerm::PrintState FVTerm::repeatCharacter (uInt& x, uInt xmax, uInt y) const
{
  // Repeat one character n-fold

  const auto& vt = vterm;
  const auto& rp = TCAP(t_repeat_char);
  auto& print_char = vt->data[y * uInt(vt->width) + x];

  if ( ! rp )
    return PrintState::NothingPrinted;

  uInt repetitions{1};

  for (uInt i = x + 1; i <= xmax; i++)
  {
    const auto& ch = vt->data[y * uInt(vt->width) + i];

    if ( print_char == ch )
      repetitions++;
    else
      break;
  }

  if ( repetitions == 1 )
  {
    appendCharacter (print_char);
    markAsPrinted (x, y);
  }
  else
  {
    const uInt start_pos = x;

    if ( repetitions > repeat_char_length
      && is7bit(print_char.ch[0]) && print_char.ch[1] == L'\0' )
    {
      newFontChanges (print_char);
      charsetChanges (print_char);
      appendAttributes (print_char);
      appendOutputBuffer (FTermControl{FTermcap::encodeParameter(rp, print_char.ch[0], repetitions)});
      term_pos->x_ref() += int(repetitions);
      x = x + repetitions - 1;
    }
    else
    {
      x--;

      for (uInt i{0}; i < repetitions; i++)
      {
        appendCharacter (print_char);
        x++;
      }
    }

    markAsPrinted (start_pos, x, y);
  }

  return PrintState::RepeatCharacterPrinted;
}

//----------------------------------------------------------------------
inline bool FVTerm::isFullWidthChar (const FChar& ch) const
{
  return ch.attr.bit.char_width == 2;
}

//----------------------------------------------------------------------
inline bool FVTerm::isFullWidthPaddingChar (const FChar& ch) const
{
  return ch.attr.bit.fullwidth_padding;
}

//----------------------------------------------------------------------
void FVTerm::cursorWrap() const
{
  // Wrap the cursor
  const auto& vt = vterm;

  if ( term_pos->getX() >= vt->width )
  {
    if ( term_pos->getY() == vt->height - 1 )
      term_pos->x_ref()--;
    else
    {
      if ( FTermcap::eat_nl_glitch )
      {
        term_pos->setPoint(-1, -1);
      }
      else if ( FTermcap::automatic_right_margin )
      {
        term_pos->setX(0);
        term_pos->y_ref()++;
      }
      else
        term_pos->x_ref()--;
    }
  }
}

//----------------------------------------------------------------------
bool FVTerm::printWrap (FTermArea* area) const
{
  bool end_of_area{false};
  const int width  = area->width;
  const int height = area->height;
  const int rsh    = area->right_shadow;
  const int bsh    = area->bottom_shadow;

  // Line break at right margin
  if ( area->cursor_x > width + rsh )
  {
    area->cursor_x = 1;
    area->cursor_y++;
  }

  // Prevent up scrolling
  if ( area->cursor_y > height + bsh )
  {
    area->cursor_y--;
    end_of_area = true;
  }

  return end_of_area;
}

//----------------------------------------------------------------------
inline void FVTerm::printCharacterOnCoordinate ( FTermArea* area
                                               , const int& ax
                                               , const int& ay
                                               , const FChar& ch) const
{
  if ( area->cursor_x <= 0
    || area->cursor_y <= 0
    || ax >= area->width + area->right_shadow
    || ay >= area->height + area->bottom_shadow )
    return;

  const int line_len = area->width + area->right_shadow;
  auto& ac = area->data[ay * line_len + ax];  // area character

  if ( ac != ch )  // compare with an overloaded operator
  {
    if ( ( ! ac.attr.bit.transparent && ch.attr.bit.transparent )
      || ( ! ac.attr.bit.color_overlay && ch.attr.bit.color_overlay )
      || ( ! ac.attr.bit.inherit_background && ch.attr.bit.inherit_background ) )
    {
      // add one transparent character form line
      area->changes[ay].trans_count++;
    }

    if ( ( ac.attr.bit.transparent && ! ch.attr.bit.transparent )
      || ( ac.attr.bit.color_overlay && ! ch.attr.bit.color_overlay )
      || ( ac.attr.bit.inherit_background && ! ch.attr.bit.inherit_background ) )
    {
      // remove one transparent character from line
      area->changes[ay].trans_count--;
    }

    // copy character to area
    std::memcpy (&ac, &ch, sizeof(ac));

    if ( ax < int(area->changes[ay].xmin) )
      area->changes[ay].xmin = uInt(ax);

    if ( ax > int(area->changes[ay].xmax) )
      area->changes[ay].xmax = uInt(ax);
  }
}

//----------------------------------------------------------------------
void FVTerm::printPaddingCharacter (FTermArea* area, const FChar& term_char)
{
  // Creates a padding-character from the current character (term_char)
  // and prints it. It is a placeholder for the column after
  // a full-width character.

  FChar pc;  // padding character

  // Copy character to padding character
  std::memcpy (&pc, &term_char, sizeof(pc));

  if ( FTerm::getEncoding() == Encoding::UTF8 )
  {
    pc.ch = {{ L'\0' }};
    pc.attr.bit.fullwidth_padding = true;
    pc.attr.bit.char_width = 0;
  }
  else
  {
    pc.ch[0] = L'.';
    pc.ch[1] = L'\0';
    pc.attr.bit.char_width = 1;
  }

  // Print the padding-character
  print (area, pc);
}

//----------------------------------------------------------------------
bool FVTerm::updateTerminalLine (uInt y) const
{
  // Updates pending changes from line y to the terminal

  bool ret{false};
  const auto& vt = vterm;
  uInt& xmin = vt->changes[y].xmin;
  uInt& xmax = vt->changes[y].xmax;

  if ( xmin <= xmax )  // Line has changes
  {
    ret = true;
    bool draw_leading_ws = false;
    bool draw_trailing_ws = false;
    const auto& ce = TCAP(t_clr_eol);

    // Clear rest of line
    bool is_eol_clean = canClearToEOL (xmin, y);

    if ( ! is_eol_clean )
    {
      // leading whitespace
      draw_leading_ws = canClearLeadingWS (xmin, y);

      // trailing whitespace
      draw_trailing_ws = canClearTrailingWS (xmax, y);
    }

    setTermXY (int(xmin), int(y));

    if ( is_eol_clean )
    {
      auto& min_char = vt->data[y * uInt(vt->width) + xmin];
      appendAttributes (min_char);
      appendOutputBuffer (FTermControl{ce});
      markAsPrinted (xmin, uInt(vt->width - 1), y);
    }
    else
    {
      if ( draw_leading_ws )
      {
        const auto& cb = TCAP(t_clr_bol);
        auto& first_char = vt->data[y * uInt(vt->width)];
        appendAttributes (first_char);
        appendOutputBuffer (FTermControl{cb});
        markAsPrinted (0, xmin, y);
      }

      printRange (xmin, xmax, y, draw_trailing_ws);

      if ( draw_trailing_ws )
      {
        auto& last_char = vt->data[(y + 1) * uInt(vt->width) - 1];
        appendAttributes (last_char);
        appendOutputBuffer (FTermControl{ce});
        markAsPrinted (xmax + 1, uInt(vt->width - 1), y);
      }
    }

    // Reset line changes
    xmin = uInt(vt->width);
    xmax = 0;
  }

  cursorWrap();
  return ret;
}

//----------------------------------------------------------------------
bool FVTerm::updateTerminalCursor() const
{
  // Updates the input cursor visibility and the position
  if ( vterm && vterm->input_cursor_visible )
  {
    const int x = vterm->input_cursor_x;
    const int y = vterm->input_cursor_y;

    if ( isInsideTerminal(FPoint{x, y}) )
    {
      setTermXY (x, y);
      showCursor();
      return true;
    }
  }
  else
    hideCursor();

  return false;
}

//----------------------------------------------------------------------
bool FVTerm::isInsideTerminal (const FPoint& pos) const
{
  // Check whether the coordinates are within the virtual terminal

  const FRect term_geometry {0, 0, FTerm::getColumnNumber(), FTerm::getLineNumber()};

  if ( term_geometry.contains(pos) )
    return true;
  else
    return false;
}

//----------------------------------------------------------------------
inline bool FVTerm::isTermSizeChanged() const
{
  if ( ! isTermSizeCheckTimeout() )
    return false;

  FObject::getCurrentTime (&last_term_size_check);
  auto& fterm_data = FTerm::getFTermData();
  const auto& old_term_geometry = fterm_data.getTermGeometry();
  FTerm::detectTermSize();
  auto term_geometry = fterm_data.getTermGeometry();
  term_geometry.move (-1, -1);

  if ( old_term_geometry.getSize() != term_geometry.getSize() )
    return true;

  return false;
}

//----------------------------------------------------------------------
inline void FVTerm::flushTimeAdjustment() const
{
  timeval now;
  FObject::getCurrentTime(&now);
  timeval diff = now - time_last_flush;

  if ( diff.tv_sec > 0 || diff.tv_usec > 400000 )
  {
    flush_wait = MIN_FLUSH_WAIT;  // Reset to minimum values after 400 ms
    flush_average = MIN_FLUSH_WAIT;
    flush_median = MIN_FLUSH_WAIT;
  }
  else
  {
    auto usec = uInt64(diff.tv_usec);

    if ( usec < MIN_FLUSH_WAIT )
      usec = MIN_FLUSH_WAIT;
    else if ( usec > MAX_FLUSH_WAIT )
      usec = MAX_FLUSH_WAIT;

    if ( usec >= flush_average )
      flush_average += (usec - flush_average) / 10;
    else
    {
      uInt64 delta = (flush_average - usec) / 10;

      if ( flush_average >= delta )  // Avoid uInt64 underflow
        flush_average -= delta;
    }

    if ( usec >= flush_median )
      flush_median += flush_average / 5;
    else
    {
      uInt64 delta = flush_average / 5;

      if ( flush_median >= delta )  // Avoid uInt64 underflow
        flush_median -= delta;
    }

    flush_wait = flush_median;
  }
}

//----------------------------------------------------------------------
inline bool FVTerm::isFlushTimeout()
{
  return FObject::isTimeout (&time_last_flush, flush_wait);
}

//----------------------------------------------------------------------
inline bool FVTerm::isTermSizeCheckTimeout()
{
  return FObject::isTimeout (&last_term_size_check, term_size_check_timeout);
}

//----------------------------------------------------------------------
inline bool FVTerm::hasPendingUpdates (const FTermArea* area)
{
  return ( area && area->has_changes ) ? true : false;
}

//----------------------------------------------------------------------
inline void FVTerm::markAsPrinted (uInt pos, uInt line)
{
  // Marks a character as printed

  vterm->data[line * uInt(vterm->width) + pos].attr.bit.printed = true;
}

//----------------------------------------------------------------------
inline void FVTerm::markAsPrinted (uInt from, uInt to, uInt line)
{
  // Marks characters in the specified range [from .. to] as printed

  for (uInt x = from; x <= to; x++)
    vterm->data[line * uInt(vterm->width) + x].attr.bit.printed = true;
}

//----------------------------------------------------------------------
inline void FVTerm::newFontChanges (FChar& next_char)
{
  // NewFont special cases
  if ( ! FTerm::isNewFont() )
    return;

  if ( next_char.ch[0] == UniChar::LowerHalfBlock )
  {
    next_char.ch[0] = wchar_t(UniChar::UpperHalfBlock);
    next_char.attr.bit.reverse = true;
  }
  else if ( isReverseNewFontchar(next_char.ch[0]) )
    next_char.attr.bit.reverse = true;  // Show in reverse video
}

//----------------------------------------------------------------------
inline void FVTerm::charsetChanges (FChar& next_char)
{
  const wchar_t& ch = next_char.ch[0];
  std::copy( next_char.ch.begin()
           , next_char.ch.end()
           , next_char.encoded_char.begin() );

  if ( FTerm::getEncoding() == Encoding::UTF8 )
    return;

  const wchar_t ch_enc = FTerm::charEncode(ch);

  if ( ch_enc == ch )
    return;

  if ( ch_enc == 0 )
  {
    next_char.encoded_char[0] = wchar_t(FTerm::charEncode(ch, Encoding::ASCII));
    return;
  }

  next_char.encoded_char[0] = ch_enc;

  if ( FTerm::getEncoding() == Encoding::VT100 )
    next_char.attr.bit.alt_charset = true;
  else if ( FTerm::getEncoding() == Encoding::PC )
  {
    next_char.attr.bit.pc_charset = true;

    if ( FTerm::isPuttyTerminal() )
      return;

    if ( FTerm::isXTerminal() && ch_enc < 0x20 )  // Character 0x00..0x1f
    {
      if ( FTerm::hasUTF8() )
        next_char.encoded_char[0] = int(FTerm::charEncode(ch, Encoding::ASCII));
      else
      {
        next_char.encoded_char[0] += 0x5f;
        next_char.attr.bit.alt_charset = true;
      }
    }
  }
}

//----------------------------------------------------------------------
inline void FVTerm::appendCharacter (FChar& next_char) const
{
  const int term_width = vterm->width - 1;
  const int term_height = vterm->height - 1;

  if ( term_pos->getX() == term_width
    && term_pos->getY() == term_height )
    appendLowerRight (next_char);
  else
    appendChar (next_char);

  term_pos->x_ref()++;
}

//----------------------------------------------------------------------
inline void FVTerm::appendChar (FChar& next_char) const
{
  newFontChanges (next_char);
  charsetChanges (next_char);
  appendAttributes (next_char);
  characterFilter (next_char);

  for (auto&& ch : next_char.encoded_char)
  {
    if ( ch != L'\0')
      appendOutputBuffer (FTermChar{ch});

    if ( ! combined_char_support )
      return;
  }
}

//----------------------------------------------------------------------
inline void FVTerm::appendAttributes (FChar& next_attr) const
{
  // generate attribute string for the next character
  const auto& attr_str = FTerm::changeAttribute (term_attribute, next_attr);

  if ( attr_str )
    appendOutputBuffer (FTermControl{attr_str});
}

//----------------------------------------------------------------------
void FVTerm::appendLowerRight (FChar& last_char) const
{
  const auto& SA = TCAP(t_enter_am_mode);
  const auto& RA = TCAP(t_exit_am_mode);

  if ( ! FTermcap::automatic_right_margin )
  {
    appendChar (last_char);
  }
  else if ( SA && RA )
  {
    appendOutputBuffer (FTermControl{RA});
    appendChar (last_char);
    appendOutputBuffer (FTermControl{SA});
  }
  else
  {
    const auto& IC = TCAP(t_parm_ich);
    const auto& im = TCAP(t_enter_insert_mode);
    const auto& ei = TCAP(t_exit_insert_mode);
    const auto& ip = TCAP(t_insert_padding);
    const auto& ic = TCAP(t_insert_character);

    const int x = int(FTerm::getColumnNumber()) - 2;
    const int y = int(FTerm::getLineNumber()) - 1;
    setTermXY (x, y);
    appendChar (last_char);
    term_pos->x_ref()++;

    setTermXY (x, y);
    FChar& second_last = *(&last_char - 1);

    if ( IC )
    {
      appendOutputBuffer (FTermControl{FTermcap::encodeParameter(IC, 1)});
      appendChar (second_last);
    }
    else if ( im && ei )
    {
      appendOutputBuffer (FTermControl{im});
      appendChar (second_last);

      if ( ip )
        appendOutputBuffer (FTermControl{ip});

      appendOutputBuffer (FTermControl{ei});
    }
    else if ( ic )
    {
      appendOutputBuffer (FTermControl{ic});
      appendChar (second_last);

      if ( ip )
        appendOutputBuffer (FTermControl{ip});
    }
  }
}

//----------------------------------------------------------------------
inline void FVTerm::characterFilter (FChar& next_char) const
{
  auto& sub_map = fterm->getCharSubstitutionMap();
  const auto& entry = sub_map[next_char.encoded_char[0]];

  if ( entry )
    next_char.encoded_char[0] = entry;
}

//----------------------------------------------------------------------
inline bool FVTerm::isOutputBufferLimitReached() const
{
  return output_buffer->size() >= TERMINAL_OUTPUT_BUFFER_LIMIT;
}

//----------------------------------------------------------------------
inline void FVTerm::appendOutputBuffer (const FTermControl& ctrl) const
{
  output_buffer->emplace(std::make_tuple(OutputType::Control, TermString(ctrl.string)));

  if ( isOutputBufferLimitReached() )
    flush();
}

//----------------------------------------------------------------------
inline void FVTerm::appendOutputBuffer (const FTermChar& c) const
{
  if ( c.ch != L'\0' )
    appendOutputBuffer(FTermString{std::wstring(1, c.ch)});
}

//----------------------------------------------------------------------
void FVTerm::appendOutputBuffer (const FTermString& str) const
{
  if ( ! output_buffer->empty()
    && std::get<0>(output_buffer->back()) == OutputType::String )
  {
    // Append string data to the back element
    auto& string_buf = std::get<1>(output_buffer->back());
    std::transform ( str.string.begin()
                   , str.string.end()
                   , std::back_inserter(string_buf.wstring)
                   , [] (wchar_t ch)
                     {
                       return ch;
                     }
                   );
  }
  else
    output_buffer->emplace(std::make_tuple(OutputType::String, TermString(str.string)));

  if ( isOutputBufferLimitReached() )
    flush();
}

}  // namespace finalcut
