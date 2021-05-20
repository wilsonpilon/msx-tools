/***********************************************************************
* fkeyboard-test.cpp - FKeyboard unit tests                            *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2018-2020 Markus Gans                                      *
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

#include <string>

#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

#include <final/final.h>

#define CPPUNIT_ASSERT_CSTRING(expected, actual) \
            check_c_string (expected, actual, CPPUNIT_SOURCELINE())

//----------------------------------------------------------------------
void check_c_string ( const char* s1
                    , const char* s2
                    , CppUnit::SourceLine sourceLine )
{
  if ( s1 == 0 && s2 == 0 )  // Strings are equal
    return;

  if ( s1 && s2 && std::strcmp (s1, s2) == 0 )  // Strings are equal
      return;

  ::CppUnit::Asserter::fail ("Strings are not equal", sourceLine);
}

namespace finalcut
{

namespace internal
{

struct var
{
  // Global application object is need for FApplication::isQuit()
  static FApplication* app_object;
};

FApplication*  var::app_object {nullptr};
}  // namespace internal

}  // namespace finalcut


namespace test
{

struct FKeyCapMap
{
  finalcut::FKey num;
  const char* string;
  char tname[4];
};

using original_type = std::array<finalcut::fc::FKeyCapMap, 188>;
using test_type = std::array<FKeyCapMap, 188>;

test_type fkey =
{{
  { finalcut::FKey::Backspace       , "\177"     , "kb" },  // backspace key
  { finalcut::FKey::Clear_all_tabs  , 0          , "ka" },  // clear-all-tabs key
  { finalcut::FKey::Clear           , 0          , "kC" },  // clear-screen or erase key
  { finalcut::FKey::Clear_tab       , CSI "3~"   , "kt" },  // clear-tab key
  { finalcut::FKey::Del_char        , 0          , "kD" },  // delete-character key
  { finalcut::FKey::Del_line        , 0          , "kL" },  // delete-line key
  { finalcut::FKey::Down            , ESC "OB"   , "kd" },  // down-arrow key
  { finalcut::FKey::Exit_insert     , 0          , "kM" },  // sent by rmir or smir in insert mode
  { finalcut::FKey::Clear_eol       , 0          , "kE" },  // clear-to-end-of-line key
  { finalcut::FKey::Clear_eos       , 0          , "kS" },  // clear-to-end-of-screen key
  { finalcut::FKey::F0              , 0          , "k0" },  // F0 function key
  { finalcut::FKey::F1              , ESC "OP"   , "k1" },  // F1 function key
  { finalcut::FKey::F2              , ESC "OQ"   , "k2" },  // F2 function key
  { finalcut::FKey::F3              , ESC "OR"   , "k3" },  // F3 function key
  { finalcut::FKey::F4              , ESC "OS"   , "k4" },  // F4 function key
  { finalcut::FKey::F5              , CSI "15~"  , "k5" },  // F5 function key
  { finalcut::FKey::F6              , CSI "17~"  , "k6" },  // F6 function key
  { finalcut::FKey::F7              , CSI "18~"  , "k7" },  // F7 function key
  { finalcut::FKey::F8              , CSI "19~"  , "k8" },  // F8 fucntion key
  { finalcut::FKey::F9              , CSI "20~"  , "k9" },  // F9 function key
  { finalcut::FKey::F10             , CSI "21~"  , "k;" },  // F10 function key
  { finalcut::FKey::Home            , ESC "OH"   , "kh" },  // home key
  { finalcut::FKey::Insert          , CSI "2~"   , "kI" },  // insert-character key
  { finalcut::FKey::Insert_line     , 0          , "kA" },  // insert-line key
  { finalcut::FKey::Left            , ESC "OD"   , "kl" },  // left-arrow key
  { finalcut::FKey::Home_down       , 0          , "kH" },  // last-line key
  { finalcut::FKey::Page_down       , CSI "6~"   , "kN" },  // next-page key
  { finalcut::FKey::Page_up         , CSI "5~"   , "kP" },  // prev-page key
  { finalcut::FKey::Right           , ESC "OC"   , "kr" },  // right-arrow key
  { finalcut::FKey::Scroll_forward  , CSI "1;2B" , "kF" },  // scroll-forward key (shift-up)
  { finalcut::FKey::Scroll_backward , CSI "1;2A" , "kR" },  // scroll-backward key (shift-down)
  { finalcut::FKey::Set_tab         , 0          , "kT" },  // set-tab key
  { finalcut::FKey::Up              , ESC "OA"   , "ku" },  // up-arrow key
  { finalcut::FKey::Upper_left      , 0          , "K1" },  // upper left of keypad
  { finalcut::FKey::Upper_right     , 0          , "K3" },  // upper right of keypad
  { finalcut::FKey::Center          , CSI "E"    , "K2" },  // center of keypad
  { finalcut::FKey::Lower_left      , 0          , "K4" },  // lower left of keypad
  { finalcut::FKey::Lower_right     , 0          , "K5" },  // lower right of keypad
  { finalcut::FKey::Back_tab        , CSI "Z"    , "kB" },  // back-tab key
  { finalcut::FKey::Begin           , 0          , "@1" },  // begin key
  { finalcut::FKey::Cancel          , 0          , "@2" },  // cancel key
  { finalcut::FKey::Close           , 0          , "@3" },  // close key
  { finalcut::FKey::Command         , 0          , "@4" },  // command key
  { finalcut::FKey::Copy            , 0          , "@5" },  // copy key
  { finalcut::FKey::Create          , 0          , "@6" },  // create key
  { finalcut::FKey::End             , ESC "OF"   , "@7" },  // end key
  { finalcut::FKey::Enter           , 0          , "@8" },  // enter/send key
  { finalcut::FKey::Exit            , 0          , "@9" },  // exit key
  { finalcut::FKey::Find            , CSI "1~"   , "@0" },  // find key
  { finalcut::FKey::Help            , 0          , "%1" },  // help key
  { finalcut::FKey::Mark            , 0          , "%2" },  // mark key
  { finalcut::FKey::Message         , 0          , "%3" },  // message key
  { finalcut::FKey::Move            , 0          , "%4" },  // move key
  { finalcut::FKey::Next            , 0          , "%5" },  // next key
  { finalcut::FKey::Open            , 0          , "%6" },  // open key
  { finalcut::FKey::Options         , 0          , "%7" },  // options key
  { finalcut::FKey::Previous        , 0          , "%8" },  // previous key
  { finalcut::FKey::Print           , 0          , "%9" },  // print key
  { finalcut::FKey::Redo            , 0          , "%0" },  // redo key
  { finalcut::FKey::Reference       , 0          , "&1" },  // reference key
  { finalcut::FKey::Refresh         , 0          , "&2" },  // refresh key
  { finalcut::FKey::Replace         , 0          , "&3" },  // replace key
  { finalcut::FKey::Restart         , 0          , "&4" },  // restart key
  { finalcut::FKey::Resume          , 0          , "&5" },  // resume key
  { finalcut::FKey::Save            , 0          , "&6" },  // save key
  { finalcut::FKey::Suspend         , 0          , "&7" },  // suspend key
  { finalcut::FKey::Undo            , 0          , "&8" },  // undo key
  { finalcut::FKey::Shift_begin     , 0          , "&9" },  // shifted begin key
  { finalcut::FKey::Shift_cancel    , 0          , "&0" },  // shifted cancel key
  { finalcut::FKey::Shift_command   , 0          , "*1" },  // shifted command key
  { finalcut::FKey::Shift_copy      , 0          , "*2" },  // shifted copy key
  { finalcut::FKey::Shift_create    , 0          , "*3" },  // shifted create key
  { finalcut::FKey::Shift_del_char  , CSI "3;2~" , "*4" },  // shifted delete-character key
  { finalcut::FKey::Shift_dl        , 0          , "*5" },  // shifted delete-line key
  { finalcut::FKey::Select          , CSI "4~"   , "*6" },  // select key
  { finalcut::FKey::Shift_end       , CSI "1;2F" , "*7" },  // shifted end key
  { finalcut::FKey::Shift_clear_eol , 0          , "*8" },  // shifted clear-to-end-of-line key
  { finalcut::FKey::Shift_exit      , 0          , "*9" },  // shifted exit key
  { finalcut::FKey::Shift_find      , 0          , "*0" },  // shifted find key
  { finalcut::FKey::Shift_help      , 0          , "#1" },  // shifted help key
  { finalcut::FKey::Shift_home      , CSI "1;2H" , "#2" },  // shifted home key
  { finalcut::FKey::Shift_insert    , CSI "2;2~" , "#3" },  // shifted insert-character key
  { finalcut::FKey::Shift_left      , CSI "1;2D" , "#4" },  // shifted left-arrow key
  { finalcut::FKey::Shift_message   , 0          , "%a" },  // shifted message key
  { finalcut::FKey::Shift_move      , 0          , "%b" },  // shifted move key
  { finalcut::FKey::Shift_page_down , CSI "6;2~" , "%c" },  // shifted next key
  { finalcut::FKey::Shift_options   , 0          , "%d" },  // shifted options key
  { finalcut::FKey::Shift_page_up   , CSI "5;2~" , "%e" },  // shifted previous key
  { finalcut::FKey::Shift_print     , 0          , "%f" },  // shifted print key
  { finalcut::FKey::Shift_redo      , 0          , "%g" },  // shifted redo key
  { finalcut::FKey::Shift_replace   , 0          , "%h" },  // shifted replace key
  { finalcut::FKey::Shift_right     , CSI "1;2C" , "%i" },  // shifted right-arrow key
  { finalcut::FKey::Shift_rsume     , 0          , "%j" },  // shifted resume key
  { finalcut::FKey::Shift_save      , 0          , "!1" },  // shifted save key
  { finalcut::FKey::Shift_suspend   , 0          , "!2" },  // shifted suspend key
  { finalcut::FKey::Shift_undo      , 0          , "!3" },  // shifted undo key
  { finalcut::FKey::F11             , CSI "23~"  , "F1" },  // F11 function key
  { finalcut::FKey::F12             , CSI "24~"  , "F2" },  // F12 function key
  { finalcut::FKey::F13             , ESC "O1;2P", "F3" },  // F13 function key
  { finalcut::FKey::F14             , ESC "O1;2Q", "F4" },  // F14 function key
  { finalcut::FKey::F15             , ESC "O1;2R", "F5" },  // F15 function key
  { finalcut::FKey::F16             , ESC "O1;2S", "F6" },  // F16 function key
  { finalcut::FKey::F17             , CSI "15;2~", "F7" },  // F17 function key
  { finalcut::FKey::F18             , CSI "17;2~", "F8" },  // F18 function key
  { finalcut::FKey::F19             , CSI "18;2~", "F9" },  // F19 function key
  { finalcut::FKey::F20             , CSI "19;2~", "FA" },  // F20 function key
  { finalcut::FKey::F21             , CSI "20;2~", "FB" },  // F21 function key
  { finalcut::FKey::F22             , CSI "21;2~", "FC" },  // F22 function key
  { finalcut::FKey::F23             , CSI "23;2~", "FD" },  // F23 function key
  { finalcut::FKey::F24             , CSI "24;2~", "FE" },  // F24 function key
  { finalcut::FKey::F25             , ESC "O1;5P", "FF" },  // F25 function key
  { finalcut::FKey::F26             , ESC "O1;5Q", "FG" },  // F26 function key
  { finalcut::FKey::F27             , ESC "O1;5R", "FH" },  // F27 function key
  { finalcut::FKey::F28             , ESC "O1;5S", "FI" },  // F28 function key
  { finalcut::FKey::F29             , CSI "15;5~", "FJ" },  // F29 function key
  { finalcut::FKey::F30             , CSI "17;5~", "FK" },  // F30 function key
  { finalcut::FKey::F31             , CSI "18;5~", "FL" },  // F31 function key
  { finalcut::FKey::F32             , CSI "19;5~", "FM" },  // F32 function key
  { finalcut::FKey::F33             , CSI "20;5~", "FN" },  // F33 function key
  { finalcut::FKey::F34             , CSI "21;5~", "FO" },  // F34 function key
  { finalcut::FKey::F35             , CSI "23;5~", "FP" },  // F35 function key
  { finalcut::FKey::F36             , CSI "24;5~", "FQ" },  // F36 function key
  { finalcut::FKey::F37             , ESC "O1;6P", "FR" },  // F37 function key
  { finalcut::FKey::F38             , ESC "O1;6Q", "FS" },  // F38 function key
  { finalcut::FKey::F39             , ESC "O1;6R", "FT" },  // F39 function key
  { finalcut::FKey::F40             , ESC "O1;6S", "FU" },  // F40 function key
  { finalcut::FKey::F41             , CSI "15;6~", "FV" },  // F41 function key
  { finalcut::FKey::F42             , CSI "17;6~", "FW" },  // F42 function key
  { finalcut::FKey::F43             , CSI "18;6~", "FX" },  // F43 function key
  { finalcut::FKey::F44             , CSI "19;6~", "FY" },  // F44 function key
  { finalcut::FKey::F45             , CSI "20;6~", "FZ" },  // F45 function key
  { finalcut::FKey::F46             , CSI "21;6~", "Fa" },  // F46 function key
  { finalcut::FKey::F47             , CSI "23;6~", "Fb" },  // F47 function key
  { finalcut::FKey::F48             , CSI "24;6~", "Fc" },  // F48 function key
  { finalcut::FKey::F49             , ESC "O1;3P", "Fd" },  // F49 function key
  { finalcut::FKey::F50             , ESC "O1;3Q", "Fe" },  // F50 function key
  { finalcut::FKey::F51             , ESC "O1;3R", "Ff" },  // F51 function key
  { finalcut::FKey::F52             , ESC "O1;3S", "Fg" },  // F52 function key
  { finalcut::FKey::F53             , CSI "15;3~", "Fh" },  // F53 function key
  { finalcut::FKey::F54             , CSI "17;3~", "Fi" },  // F54 function key
  { finalcut::FKey::F55             , CSI "18;3~", "Fj" },  // F55 function key
  { finalcut::FKey::F56             , CSI "19;3~", "Fk" },  // F56 function key
  { finalcut::FKey::F57             , CSI "20;3~", "Fl" },  // F57 function key
  { finalcut::FKey::F58             , CSI "21;3~", "Fm" },  // F58 function key
  { finalcut::FKey::F59             , CSI "23;3~", "Fn" },  // F59 function key
  { finalcut::FKey::F60             , CSI "24;3~", "Fo" },  // F60 function key
  { finalcut::FKey::F61             , ESC "O1;4P", "Fp" },  // F61 function key
  { finalcut::FKey::F62             , ESC "O1;4Q", "Fq" },  // F62 function key
  { finalcut::FKey::F63             , ESC "O1;4R", "Fr" },  // F63 function key
  // vt100 key codes for arrow and function keys
  { finalcut::FKey::F1              , ESC "OP"   , "k1x"},  // PF1 (application mode)
  { finalcut::FKey::F2              , ESC "OQ"   , "k2x"},  // PF2 (application mode)
  { finalcut::FKey::F3              , ESC "OR"   , "k3x"},  // PF3 (application mode)
  { finalcut::FKey::F4              , ESC "OS"   , "k4x"},  // PF4 (application mode)
  { finalcut::FKey::Left            , CSI "D"    , "klx"},  // left-arrow key (standard mode)
  { finalcut::FKey::Left            , ESC "OD"   , "klX"},  // left-arrow key (application mode)
  { finalcut::FKey::Right           , CSI "C"    , "krx"},  // right-arrow key (standard mode)
  { finalcut::FKey::Right           , ESC "OC"   , "krX"},  // right-arrow key (application mode)
  { finalcut::FKey::Up              , CSI "A"    , "kux"},  // up-arrow key (standard mode)
  { finalcut::FKey::Up              , ESC "OA"   , "kuX"},  // up-arrow key (application mode)
  { finalcut::FKey::Down            , CSI "B"    , "kdx"},  // down-arrow key (standard mode)
  { finalcut::FKey::Down            , ESC "OB"   , "kdX"},  // down-arrow key (application mode)
  { finalcut::FKey::Scroll_forward  , CSI "a"    , "kFx"},  // scroll-forward key (shift-up)
  { finalcut::FKey::Scroll_backward , CSI "b"    , "kRx"},  // scroll-backward key (shift-down)
  // Fallback for rxvt with TERM=xterm
  { finalcut::FKey::Home            , CSI "7~"   , "khx"},  // home key
  { finalcut::FKey::End             , CSI "8~"   , "@7x"},  // end key
  { finalcut::FKey::F1              , CSI "11~"  , "k1X"},  // F1 function key
  { finalcut::FKey::F2              , CSI "12~"  , "k2X"},  // F2 function key
  { finalcut::FKey::F3              , CSI "13~"  , "k3X"},  // F3 function key
  { finalcut::FKey::F4              , CSI "14~"  , "k4X"},  // F4 function key
  // Fallback for TERM=ansi
  { finalcut::FKey::Home            , CSI "H"    , "khX"},  // home key
  { finalcut::FKey::End             , CSI "F"    , "@7X"},  // end key
  { finalcut::FKey::End             , CSI "K"    , "@7y"},  // end key (Microsoft HyperTerminal)
  // Keypad keys
  { finalcut::FKey::Enter           , ESC "OM"   , "@8x"},  // enter key
  { finalcut::FKey::Slash           , ESC "Oo"   , "KP1"},  // keypad slash
  { finalcut::FKey::Asterisk        , ESC "Oj"   , "KP2"},  // keypad asterisk
  { finalcut::FKey::Minus_sign      , ESC "Om"   , "KP3"},  // keypad minus sign
  { finalcut::FKey::Plus_sign       , ESC "Ok"   , "KP4"},  // keypad plus sign
  { finalcut::FKey::Insert          , ESC "Op"   , "kIx"},  // keypad insert
  { finalcut::FKey::Del_char        , ESC "On"   , "kDx"},  // keypad delete
  { finalcut::FKey::Left            , ESC "Ot"   , "kly"},  // keypad left-arrow
  { finalcut::FKey::Right           , ESC "Ov"   , "kry"},  // keypad right-arrow
  { finalcut::FKey::Up              , ESC "Ox"   , "kuy"},  // keypad up-arrow
  { finalcut::FKey::Down            , ESC "Or"   , "kdy"},  // keypad down-arrow
  { finalcut::FKey::Upper_left      , ESC "Ow"   , "K1x"},  // keypad upper left
  { finalcut::FKey::Upper_right     , ESC "Oy"   , "K3x"},  // keypad upper right
  { finalcut::FKey::Center          , ESC "Ou"   , "K2x"},  // keypad center
  { finalcut::FKey::Lower_left      , ESC "Oq"   , "K4x"},  // keypad lower left
  { finalcut::FKey::Lower_right     , ESC "Os"   , "K5x"}   // keypad lower right
}};

}  // namespace test


//----------------------------------------------------------------------
// class FKeyboardTest
//----------------------------------------------------------------------

class FKeyboardTest : public CPPUNIT_NS::TestFixture
{
  public:
    FKeyboardTest();
    ~FKeyboardTest();

  protected:
    void classNameTest();
    void noArgumentTest();
    void escapeKeyTest();
    void characterwiseInputTest();
    void severalKeysTest();
    void functionKeyTest();
    void metaKeyTest();
    void sequencesTest();
    void mouseTest();
    void utf8Test();
    void unknownKeyTest();

  private:
    // Adds code needed to register the test suite
    CPPUNIT_TEST_SUITE (FKeyboardTest);

    // Add a methods to the test suite
    CPPUNIT_TEST (classNameTest);
    CPPUNIT_TEST (noArgumentTest);
    CPPUNIT_TEST (escapeKeyTest);
    CPPUNIT_TEST (characterwiseInputTest);
    CPPUNIT_TEST (severalKeysTest);
    CPPUNIT_TEST (functionKeyTest);
    CPPUNIT_TEST (metaKeyTest);
    CPPUNIT_TEST (sequencesTest);
    CPPUNIT_TEST (mouseTest);
    CPPUNIT_TEST (utf8Test);
    CPPUNIT_TEST (unknownKeyTest);

    // End of test suite definition
    CPPUNIT_TEST_SUITE_END();
    void init();
    void input (std::string);
    void processInput();
    void clear();
    void keyPressed();
    void keyReleased();
    void escapeKeyPressed();
    void mouseTracking();

    // Data members
    finalcut::FKey key_pressed{finalcut::FKey::None};
    finalcut::FKey key_released{finalcut::FKey::None};
    int  number_of_keys{0};
    finalcut::FKeyboard* keyboard{nullptr};
};

//----------------------------------------------------------------------
FKeyboardTest::FKeyboardTest()
{
  init();
}

//----------------------------------------------------------------------
FKeyboardTest::~FKeyboardTest()
{
  delete keyboard;
}

//----------------------------------------------------------------------
void FKeyboardTest::classNameTest()
{
  finalcut::FKeyboard k;
  const finalcut::FString& classname = k.getClassName();
  CPPUNIT_ASSERT ( classname == "FKeyboard" );
}

//----------------------------------------------------------------------
void FKeyboardTest::noArgumentTest()
{
  CPPUNIT_ASSERT ( keyboard->getKey() == finalcut::FKey::None );
  finalcut::FKeyboard::keybuffer& buffer = keyboard->getKeyBuffer();
  std::size_t size = sizeof(buffer);

  CPPUNIT_ASSERT ( size == finalcut::FKeyboard::FIFO_BUF_SIZE );
  CPPUNIT_ASSERT ( buffer[0] == 0 );
  int sum = 0;

  for (std::size_t i = 0; i < size; i++)
    sum += int(buffer[i]);

  CPPUNIT_ASSERT ( sum == 0 );

  timeval* time = keyboard->getKeyPressedTime();
  CPPUNIT_ASSERT ( time->tv_sec == 0);
  CPPUNIT_ASSERT ( time->tv_usec == 0);

  CPPUNIT_ASSERT ( ! keyboard->hasUnprocessedInput() );
  CPPUNIT_ASSERT ( ! keyboard->isKeyPressed() );

  keyboard->clearKeyBufferOnTimeout();

  if ( keyboard->isKeyPressed() )
    keyboard->fetchKeyCode();

  // special case: Esc key
  keyboard->escapeKeyHandling();

  CPPUNIT_ASSERT ( keyboard->getKey() == finalcut::FKey::None );

  // Keypress timeout
  CPPUNIT_ASSERT ( keyboard->getKeypressTimeout() == 100 * 1000 );

  keyboard->setKeypressTimeout(0);  // 0 ms
  CPPUNIT_ASSERT ( keyboard->getKeypressTimeout() == 0 );

  keyboard->setKeypressTimeout(100000);  // 100 ms
  CPPUNIT_ASSERT ( keyboard->getKeypressTimeout() == 100 * 1000 );

  // Read blocking time
  CPPUNIT_ASSERT ( keyboard->getReadBlockingTime() == 100 * 1000 );

  keyboard->setReadBlockingTime(1000000);  // 1000 ms
  CPPUNIT_ASSERT ( keyboard->getReadBlockingTime() == 1000 * 1000 );

  keyboard->setReadBlockingTime(0);  // 0 ms
  CPPUNIT_ASSERT ( keyboard->getReadBlockingTime() == 0 );

  keyboard->setReadBlockingTime(50000);  // 50 ms
  CPPUNIT_ASSERT ( keyboard->getReadBlockingTime() == 50 * 1000 );

  keyboard->setReadBlockingTime(100000);  // 100 ms
  CPPUNIT_ASSERT ( keyboard->getReadBlockingTime() == 100 * 1000 );

  // Check key map
  CPPUNIT_ASSERT ( test::fkey[0].num == finalcut::FKey::Backspace );
  CPPUNIT_ASSERT_CSTRING ( test::fkey[0].string, "\177" );
  CPPUNIT_ASSERT_CSTRING ( test::fkey[0].tname, "kb" );
}

//----------------------------------------------------------------------
void FKeyboardTest::escapeKeyTest()
{
  std::cout << std::endl;

  // Mintty application escape key
  input("\033O[");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Escape_mintty );
  clear();

  // Normal escape (needs a timeout)
  input("\033");
  processInput();
  // Wait 100 ms (= 100,000,000 ns)
  const struct timespec ms[]{{0, 100000000L}};
  nanosleep (ms, NULL);
  keyboard->escapeKeyHandling();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Escape );
  keyboard->clearKeyBufferOnTimeout();
  clear();
}

//----------------------------------------------------------------------
void FKeyboardTest::characterwiseInputTest()
{
  std::cout << std::endl;

  // Cursor down in character by character input (e.g. rxvt-cygwin-native)
  input("\033");
  processInput();
  input("[");
  processInput();
  input("B");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( number_of_keys == 1 );
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Down );
  clear();
}

//----------------------------------------------------------------------
void FKeyboardTest::severalKeysTest()
{
  std::cout << std::endl;

  // Input of two 1 byte characters
  input("A");
  input("B");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( number_of_keys == 2 );
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey('B') );
  CPPUNIT_ASSERT ( key_released == finalcut::FKey('B') );
  clear();

  // F1, F2, F3
  input("\033[11~\033[12~\033[13~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( number_of_keys == 3 );
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F3 );
  CPPUNIT_ASSERT ( key_released == finalcut::FKey::F3 );
  clear();
}

//----------------------------------------------------------------------
void FKeyboardTest::functionKeyTest()
{
  // Function key F1 (numeric keypad PF1)
  input("\033OP");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F1 );
  clear();

  // Function key F1
  input("\033[11~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F1 );
  clear();

  // Function key F2 (numeric keypad PF2)
  input("\033OQ");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F2 );
  clear();

  // Function key F2
  input("\033[12~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F2 );
  clear();

  // Function key F3 (numeric keypad PF3)
  input("\033OR");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F3 );
  clear();

  // Function key F3
  input("\033[13~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F3 );
  clear();

  // Function key F4 (numeric keypad PF3)
  input("\033OS");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F4 );
  clear();

  // Function key F4
  input("\033[14~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F4 );
  clear();

  // Function key F5
  input("\033[15~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F5 );
  clear();

  // Function key F6
  input("\033[17~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F6 );
  clear();

  // Function key F7
  input("\033[18~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F7 );
  clear();

  // Function key F8
  input("\033[19~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F8 );
  clear();

  // Function key F9
  input("\033[20~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F9 );
  clear();

  // Function key F10
  input("\033[21~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F10 );
  clear();

  // Function key F11
  input("\033[23~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F11 );
  clear();

  // Function key F12
  input("\033[24~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F12 );
  clear();

  // Function key F13 (shift + F1)
  input("\033O1;2P");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F13 );
  clear();

  // Function key F14 (shift + F2)
  input("\033O1;2Q");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F14 );
  clear();

  // Function key F15 (shift + F3)
  input("\033O1;2R");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F15 );
  clear();

  // Function key F16 (shift + F4)
  input("\033O1;2S");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F16 );
  clear();

  // Function key F17 (shift + F5)
  input("\033[15;2~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F17 );
  clear();

  // Function key F18 (shift + F6)
  input("\033[17;2~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F18 );
  clear();

  // Function key F19 (shift + F7)
  input("\033[18;2~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F19 );
  clear();

  // Function key F20 (shift + F8)
  input("\033[19;2~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F20 );
  clear();

  // Function key F21 (shift + F9)
  input("\033[20;2~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F21 );
  clear();

  // Function key F22 (shift + F10)
  input("\033[21;2~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F22 );
  clear();

  // Function key F23 (shift + F11)
  input("\033[23;2~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F23 );
  clear();

  // Function key F24 (shift + F12)
  input("\033[24;2~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F24 );
  clear();

  // Function key F25 (ctrl + F1)
  input("\033O1;5P");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F25 );
  clear();

  // Function key F26 (ctrl + F2)
  input("\033O1;5Q");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F26 );
  clear();

  // Function key F27 (ctrl + F3)
  input("\033O1;5R");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F27 );
  clear();

  // Function key F28 (ctrl + F4)
  input("\033O1;5S");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F28 );
  clear();

  // Function key F29 (ctrl + F5)
  input("\033[15;5~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F29 );
  clear();

  // Function key F30 (ctrl + F6)
  input("\033[17;5~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F30 );
  clear();

  // Function key F31 (ctrl + F7)
  input("\033[18;5~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F31 );
  clear();

  // Function key F32 (ctrl + F8)
  input("\033[19;5~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F32 );
  clear();

  // Function key F33 (ctrl + F9)
  input("\033[20;5~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F33 );
  clear();

  // Function key F34 (ctrl + F10)
  input("\033[21;5~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F34 );
  clear();

  // Function key F35 (ctrl + F11)
  input("\033[23;5~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F35 );
  clear();

  // Function key F36 (ctrl + F12)
  input("\033[24;5~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F36 );
  clear();

  // Function key F37 (shift + ctrl + F1)
  input("\033O1;6P");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F37 );
  clear();

  // Function key F38 (shift + ctrl + F2)
  input("\033O1;6Q");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F38 );
  clear();

  // Function key F39 (shift + ctrl + F3)
  input("\033O1;6R");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F39 );
  clear();

  // Function key F40 (shift + ctrl + F4)
  input("\033O1;6S");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F40 );
  clear();

  // Function key F41 (shift + ctrl + F5)
  input("\033[15;6~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F41 );
  clear();

  // Function key F42 (shift + ctrl + F6)
  input("\033[17;6~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F42 );
  clear();

  // Function key F43 (shift + ctrl + F7)
  input("\033[18;6~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F43 );
  clear();

  // Function key F44 (shift + ctrl + F8)
  input("\033[19;6~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F44 );
  clear();

  // Function key F45 (shift + ctrl + F9)
  input("\033[20;6~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F45 );
  clear();

  // Function key F46 (shift + ctrl + F10)
  input("\033[21;6~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F46 );
  clear();

  // Function key F47 (shift + ctrl + F11)
  input("\033[23;6~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F47 );
  clear();

  // Function key F48 (shift + ctrl + F12)
  input("\033[24;6~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F48 );
  clear();

  // Function key F49 (meta + F1)
  input("\033O1;3P");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F49 );
  clear();

  // Function key F50 (meta + F2)
  input("\033O1;3Q");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F50 );
  clear();

  // Function key F51 (meta + F3)
  input("\033O1;3R");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F51 );
  clear();

  // Function key F52 (meta + F4)
  input("\033O1;3S");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F52 );
  clear();

  // Function key F53 (meta + F5)
  input("\033[15;3~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F53 );
  clear();

  // Function key F54 (meta + F6)
  input("\033[17;3~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F54 );
  clear();

  // Function key F55 (meta + F7)
  input("\033[18;3~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F55 );
  clear();

  // Function key F56 (meta + F8)
  input("\033[19;3~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F56 );
  clear();

  // Function key F57 (meta + F9)
  input("\033[20;3~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F57 );
  clear();

  // Function key F58 (meta + F10)
  input("\033[21;3~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F58 );
  clear();

  // Function key F59 (meta + F11)
  input("\033[23;3~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F59 );
  clear();

  // Function key F60 (meta + F12)
  input("\033[24;3~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F60 );
  clear();

  // Function key F61 (shift + meta + F1)
  input("\033O1;4P");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F61 );
  clear();

  // Function key F62 (shift + meta + F2)
  input("\033O1;4Q");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F62 );
  clear();

  // Function key F63 (shift + meta + F3)
  input("\033O1;4R");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::F63 );
  clear();
}

//----------------------------------------------------------------------
void FKeyboardTest::metaKeyTest()
{
  std::cout << std::endl;

  // meta-insert
  input("\033[2;3~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_insert );
  clear();

  // meta-insert
  input("\033\033[2~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_insert );
  clear();

  // meta-delete
  input("\033[3;3~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_del_char );
  clear();

  // meta-delete
  input("\033\033[3~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_del_char );
  clear();

  // meta-home
  input("\033[1;3H");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_home );
  clear();

  // meta-home
  input("\033\033[1~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_home );
  clear();

  // meta-end
  input("\033[1;3F");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_end );
  clear();

  // meta-end
  input("\033\033[4~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_end );
  clear();

  // meta-prev-page
  input("\033[5;3~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_page_up );
  clear();

  // meta-prev-page
  input("\033\033[5~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_page_up );
  clear();

  // meta-next-page
  input("\033[6;3~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_page_down );
  clear();

  // meta-next-page
  input("\033\033[6~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_page_down );
  clear();

  // meta-f1
  input("\033[1;3P");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f1 );
  clear();

  // meta-f1
  input("\033\033[11~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f1 );
  clear();

  // meta-f2
  input("\033[1;3Q");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f2 );
  clear();

  // meta-f2
  input("\033\033[12~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f2 );
  clear();

  // meta-f3
  input("\033[1;3R");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f3 );
  clear();

  // meta-f3
  input("\033\033[13~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f3 );
  clear();

  // meta-f4
  input("\033[1;3S");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f4 );
  clear();

  // meta-f4
  input("\033\033[14~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f4 );
  clear();

  // meta-f5
  input("\033\033[15~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f5 );
  clear();

  // meta-f6
  input("\033\033[17~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f6 );
  clear();

  // meta-f7
  input("\033\033[18~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f7 );
  clear();

  // meta-f8
  input("\033\033[19~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f8 );
  clear();

  // meta-f9
  input("\033\033[20~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f9 );
  clear();

  // meta-f10
  input("\033\033[21~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f10 );
  clear();

  // meta-f11
  input("\033\033[23~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f11 );
  clear();

  // meta-f12
  input("\033\033[24~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f12 );
  clear();

  // meta-up
  input("\033[1;3A");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_up );
  clear();

  // meta-up
  input("\033\033[A");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_up );
  clear();

  // meta-down
  input("\033[1;3B");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_down );
  clear();

  // meta-down
  input("\033\033[B");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_down );
  clear();

  // meta-right
  input("\033[1;3C");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_right );
  clear();

  // meta-right
  input("\033\033[C");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_right );
  clear();

  // meta-left
  input("\033[1;3D");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_left );
  clear();

  // meta-left
  input("\033\033[D");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_left );
  clear();

  // shift-meta-insert
  input("\033[2;4~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_insert );
  clear();

  // shift-meta-delete
  input("\033[3;4~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_del_char );
  clear();

  // shift-meta-home
  input("\033[1;4H");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_home );
  clear();

  // shift-meta-end
  input("\033[1;4F");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_end );
  clear();

  // shift-meta-prev-page
  input("\033[5;4~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_page_up );
  clear();

  // shift-meta-next-page
  input("\033[6;4~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_page_down );
  clear();

  // shift-meta-f1
  input("\033[1;4P");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_f1 );
  clear();

  // shift-meta-f2
  input("\033[1;4Q");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_f2 );
  clear();

  // shift-meta-f3
  input("\033[1;4R");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_f3 );
  clear();

  // shift-meta-f4
  input("\033[1;4S");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_f4 );
  clear();

  // shift-meta-f5
  input("\033[15;4~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_f5 );
  clear();

  // shift-meta-f6
  input("\033[17;4~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_f6 );
  clear();

  // shift-meta-f7
  input("\033[18;4~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_f7 );
  clear();

  // shift-meta-f8
  input("\033[19;4~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_f8 );
  clear();

  // shift-meta-f9
  input("\033[20;4~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_f9 );
  clear();

  // shift-meta-f10
  input("\033[21;4~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_f10 );
  clear();

  // shift-meta-f11
  input("\033[23;4~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_f11 );
  clear();

  // shift-meta-f12
  input("\033[24;4~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_f12 );
  clear();

  // shift-meta-up
  input("\033[1;4A");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_up );
  clear();

  // shift-meta-down
  input("\033[1;4B");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_down );
  clear();

  // shift-meta-right
  input("\033[1;4C");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_right );
  clear();

  // shift-meta-left
  input("\033[1;4D");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_left );
  clear();

  // ctrl-insert
  input("\033[2;5~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_insert );
  clear();

  // ctrl-delete
  input("\033[3;5~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_del_char );
  clear();

  // ctrl-home
  input("\033[1;5H");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_home );
  clear();

  // ctrl-end
  input("\033[1;5F");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_end );
  clear();

  // ctrl-prev-page
  input("\033[5;5~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_page_up );
  clear();

  // ctrl-next-page
  input("\033[6;5~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_page_down );
  clear();

  // ctrl-up
  input("\033[1;5A");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_up );
  clear();

  // ctrl-down
  input("\033[1;5B");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_down );
  clear();

  // ctrl-right
  input("\033[1;5C");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_right );
  clear();

  // ctrl-left
  input("\033[1;5D");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_left );
  clear();

  // shift-ctrl-insert
  input("\033[2;6~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_insert );
  clear();

  // shift-ctrl-delete
  input("\033[3;6~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_del_char );
  clear();

  // shift-ctrl-home
  input("\033[1;6H");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_home );
  clear();

  // shift-ctrl-end
  input("\033[1;6F");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_end );
  clear();

  // shift-ctrl-prev-page
  input("\033[5;6~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_page_up );
  clear();

  // shift-ctrl-next-page
  input("\033[6;6~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_page_down );
  clear();

  // shift-ctrl-up
  input("\033[1;6A");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_up );
  clear();

  // shift-ctrl-down
  input("\033[1;6B");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_down );
  clear();

  // shift-ctrl-right
  input("\033[1;6C");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_right );
  clear();

  // shift-ctrl-left
  input("\033[1;6D");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_left );
  clear();

  // ctrl-meta-insert
  input("\033[2;7~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_Meta_insert );
  clear();

  // ctrl-meta-delete
  input("\033[3;7~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_Meta_del_char );
  clear();

  // ctrl-meta-home
  input("\033[1;7H");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_Meta_home );
  clear();

  // ctrl-meta-end
  input("\033[1;7F");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_Meta_end );
  clear();

  // ctrl-meta-prev-page
  input("\033[5;7~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_Meta_page_up );
  clear();

  // ctrl-meta-next-page
  input("\033[6;7~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_Meta_page_down );
  clear();

  // ctrl-meta-up
  input("\033[1;7A");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_Meta_up );
  clear();

  // ctrl-meta-down
  input("\033[1;7B");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_Meta_down );
  clear();

  // ctrl-meta-right
  input("\033[1;7C");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_Meta_right );
  clear();

  // ctrl-meta-left
  input("\033[1;7D");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_Meta_left );
  clear();

  // shift-ctrl-meta-insert
  input("\033[2;8~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_insert );
  clear();

  // shift-ctrl-meta-delete
  input("\033[3;8~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_del_char );
  clear();

  // shift-ctrl-meta-home
  input("\033[1;8H");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_home );
  clear();

  // shift-ctrl-meta-end
  input("\033[1;8F");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_end );
  clear();

  // shift-ctrl-meta-prev-page
  input("\033[5;8~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_page_up );
  clear();

  // shift-ctrl-meta-next-page
  input("\033[6;8~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_page_down );
  clear();

  // shift-ctrl-meta-f1
  input("\033[1;8P");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_f1 );
  clear();

  // shift-ctrl-meta-f2
  input("\033[1;8Q");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_f2 );
  clear();

  // shift-ctrl-meta-f3
  input("\033[1;8R");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_f3 );
  clear();

  // shift-ctrl-meta-f4
  input("\033[1;8S");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_f4 );
  clear();

  // shift-ctrl-meta-f5
  input("\033[15;8~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_f5 );
  clear();

  // shift-ctrl-meta-f6
  input("\033[17;8~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_f6 );
  clear();

  // shift-ctrl-meta-f7
  input("\033[18;8~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_f7 );
  clear();

  // shift-ctrl-meta-f8
  input("\033[19;8~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_f8 );
  clear();

  // shift-ctrl-meta-f9
  input("\033[20;8~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_f9 );
  clear();

  // shift-ctrl-meta-f10
  input("\033[21;8~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_f10 );
  clear();

  // shift-ctrl-meta-f11
  input("\033[23;8~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_f11 );
  clear();

  // shift-ctrl-meta-f12
  input("\033[24;8~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_f12 );
  clear();

  // shift-ctrl-meta-up
  input("\033[1;8A");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_up );
  clear();

  // shift-ctrl-meta-down
  input("\033[1;8B");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_down );
  clear();

  // shift-ctrl-meta-right
  input("\033[1;8C");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_right );
  clear();

  // shift-ctrl-meta-left
  input("\033[1;8D");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_left );
  clear();

  // menu
  input("\033[29~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Menu );
  clear();

  // shift-menu
  input("\033[29$");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_menu );
  clear();

  // shift-menu
  input("\033[29;2~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_menu );
  clear();

  // ctrl-menu
  input("\033[29^");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_menu );
  clear();

  // ctrl-menu
  input("\033[29;5~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_menu );
  clear();

  // shift-ctrl-menu
  input("\033[29@");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_menu );
  clear();

  // shift-ctrl-menu
  input("\033[29;6~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_menu );
  clear();

  // meta-menu
  input("\033[29;3~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_menu );
  clear();

  // shift-meta-menu
  input("\033[29;4~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Meta_menu );
  clear();

  // ctrl-meta-menu
  input("\033[29;7~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Ctrl_Meta_menu );
  clear();

  // shift-ctrl-meta-menu
  input("\033[29;8~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_Ctrl_Meta_menu );
  clear();

  // meta-tab
  input("\033\t");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_tab );
  clear();

  // meta-enter
  input("\033\n");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_enter );
  clear();

  // meta-enter
  input("\033\r");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_enter );
  clear();

  // meta-' '
  input("\033 ");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_space );
  clear();

  // meta-!
  input("\033!");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_bang );
  clear();

  // meta-"
  input("\033\"");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_quotes );
  clear();

  // meta-#
  input("\033#");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_hash );
  clear();

  // meta-$
  input("\033$");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_dollar );
  clear();

  // meta-%
  input("\033%");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_percent );
  clear();

  // meta-&
  input("\033&");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_ampersand );
  clear();

  // meta-'
  input("\033'");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_apostrophe );
  clear();

  // meta-(
  input("\033(");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_left_parenthesis );
  clear();

  // meta-)
  input("\033)");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_right_parenthesis );
  clear();

  // meta-*
  input("\033*");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_asterisk );
  clear();

  // meta-+
  input("\033+");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_plus );
  clear();

  // meta-,
  input("\033,");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_comma );
  clear();

  // meta-'-'
  input("\033-");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_minus );
  clear();

  // meta-.
  input("\033.");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_full_stop );
  clear();

  // meta-/
  input("\033/");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_slash );
  clear();

  // meta-0
  input("\0330");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_0 );
  clear();

  // meta-1
  input("\0331");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_1 );
  clear();

  // meta-2
  input("\0332");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_2 );
  clear();

  // meta-3
  input("\0333");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_3 );
  clear();

  // meta-4
  input("\0334");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_4 );
  clear();

  // meta-5
  input("\0335");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_5 );
  clear();

  // meta-6
  input("\0336");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_6 );
  clear();

  // meta-7
  input("\0337");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_7 );
  clear();

  // meta-8
  input("\0338");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_8 );
  clear();

  // meta-9
  input("\0339");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_9 );
  clear();

  // meta-:
  input("\033:");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_colon );
  clear();

  // meta-;
  input("\033;");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_semicolon );
  clear();

  // meta-<
  input("\033<");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_less_than );
  clear();

  // meta-=
  input("\033=");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_equals );
  clear();

  // meta->
  input("\033>");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_greater_than );
  clear();

  // meta-?
  input("\033?");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_question_mark );
  clear();

  // meta-@
  input("\033@");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_at );
  clear();

  // shifted meta-A
  input("\033A");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_A );
  clear();

  // shifted meta-B
  input("\033B");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_B );
  clear();

  // shifted meta-C
  input("\033C");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_C );
  clear();

  // shifted meta-D
  input("\033D");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_D );
  clear();

  // shifted meta-E
  input("\033E");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_E );
  clear();

  // shifted meta-F
  input("\033F");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_F );
  clear();

  // shifted meta-G
  input("\033G");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_G );
  clear();

  // shifted meta-H
  input("\033H");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_H );
  clear();

  // shifted meta-I
  input("\033I");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_I );
  clear();

  // shifted meta-J
  input("\033J");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_J );
  clear();

  // shifted meta-K
  input("\033K");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_K );
  clear();

  // shifted meta-L
  input("\033L");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_L );
  clear();

  // shifted meta-M
  input("\033M");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_M );
  clear();

  // shifted meta-N
  input("\033N");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_N );
  clear();

  // shifted meta-O
  input("\033O");
  processInput();
  // Wait 100 ms - Substring keys needs a timeout
  const struct timespec ms[]{{0, 100000000L}};
  nanosleep (ms, NULL);
  keyboard->escapeKeyHandling();
  keyboard->processQueuedInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_O );
  clear();

  // shifted meta-P
  input("\033P");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_P );
  clear();

  // shifted meta-Q
  input("\033Q");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_Q );
  clear();

  // shifted meta-R
  input("\033R");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_R );
  clear();

  // shifted meta-S
  input("\033S");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_S );
  clear();

  // shifted meta-T
  input("\033T");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_T );
  clear();

  // shifted meta-U
  input("\033U");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_U );
  clear();

  // shifted meta-V
  input("\033V");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_V );
  clear();

  // shifted meta-W
  input("\033W");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_W );
  clear();

  // shifted meta-X
  input("\033X");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_X );
  clear();

  // shifted meta-Y
  input("\033Y");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_Y );
  clear();

  // shifted meta-Z
  input("\033Z");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_Z );
  clear();

  // meta-[
  input("\033[");
  processInput();
  // Wait 100 ms - Substring keys needs a timeout
  nanosleep (ms, NULL);
  keyboard->escapeKeyHandling();
  keyboard->processQueuedInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_left_square_bracket );
  clear();

  // meta-'\'
  input("\033\\");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_backslash );
  clear();

  // meta-]
  input("\033]");
  processInput();
  // Wait 100 ms - Substring keys needs a timeout
  nanosleep (ms, NULL);
  keyboard->escapeKeyHandling();
  keyboard->processQueuedInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_right_square_bracket );
  clear();

  // meta-^
  input("\033^");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_caret );
  clear();

  // meta-_
  input("\033_");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_underscore );
  clear();

  // meta-`
  input("\033`");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_grave_accent );
  clear();

  // meta-a
  input("\033a");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_a );
  clear();

  // meta-b
  input("\033b");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_b );
  clear();

  // meta-c
  input("\033c");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_c );
  clear();

  // meta-d
  input("\033d");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_d );
  clear();

  // meta-e
  input("\033e");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_e );
  clear();

  // meta-f
  input("\033f");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_f );
  clear();

  // meta-g
  input("\033g");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_g );
  clear();

  // meta-h
  input("\033h");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_h );
  clear();

  // meta-i
  input("\033i");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_i );
  clear();

  // meta-j
  input("\033j");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_j );
  clear();

  // meta-k
  input("\033k");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_k );
  clear();

  // meta-l
  input("\033l");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_l );
  clear();

  // meta-m
  input("\033m");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_m );
  clear();

  // meta-n
  input("\033n");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_n );
  clear();

  // meta-o
  input("\033o");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_o );
  clear();

  // meta-p
  input("\033p");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_p );
  clear();

  // meta-q
  input("\033q");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_q );
  clear();

  // meta-r
  input("\033r");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_r );
  clear();

  // meta-s
  input("\033s");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_s );
  clear();

  // meta-t
  input("\033t");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_t );
  clear();

  // meta-u
  input("\033u");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_u );
  clear();

  // meta-v
  input("\033v");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_v );
  clear();

  // meta-w
  input("\033w");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_w );
  clear();

  // meta-x
  input("\033x");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_x );
  clear();

  // meta-y
  input("\033y");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_y );
  clear();

  // meta-z
  input("\033z");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_z );
  clear();

  // meta-{
  input("\033{");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_left_curly_bracket );
  clear();

  // meta-|
  input("\033|");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_vertical_bar );
  clear();

  // meta-}
  input("\033}");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_right_curly_bracket );
  clear();

  // meta-~
  input("\033~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Meta_tilde );
  clear();
}

//----------------------------------------------------------------------
void FKeyboardTest::sequencesTest()
{
  std::cout << std::endl;

  // Clear-tab
  input("\033[3~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Clear_tab );
  clear();

  // Cursor up key in applications mode
  input("\033OA");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Up );
  clear();

  // Cursor up key in positioning mode
  input("\033[A");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Up );
  clear();

  // Cursor down key in applications mode
  input("\033OB");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Down );
  clear();

  // Cursor down key in positioning mode
  input("\033[B");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Down );
  clear();

  // Cursor right key in applications mode
  input("\033OC");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Right );
  clear();

  // Cursor right key in positioning mode
  input("\033[C");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Right );
  clear();

  // Cursor left key in applications mode
  input("\033OD");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Left );
  clear();

  // Cursor left key in applications mode
  input("\033OD");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Left );
  clear();

  // Home key in positioning mode
  input("\033[7~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Home );
  clear();

  // Home key in applications mode
  input("\033OH");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Home );
  clear();

  // Home key (ANSI terminal)
  input("\033[H");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Home );
  clear();

  // End key in positioning mode
  input("\033[8~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::End );
  clear();

  // End key in applications mode
  input("\033OF");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::End );
  clear();

  // End key (ANSI terminal)
  input("\033[F");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::End );
  clear();

  // End key (Microsoft HyperTerminal)
  input("\033[K");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::End );
  clear();

  // Next-page key (Page down)
  input("\033[6~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Page_down );
  clear();

  // Previous-page key (Page up)
  input("\033[5~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Page_up );
  clear();

  // Insert key
  input("\033[2~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Insert );
  clear();

  // Scroll-forward key (shift + up-arrow)
  input("\033[1;2B");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Scroll_forward );
  clear();

  // Scroll-forward key (shift + up-arrow) in applications mode
  input("\033[a");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Scroll_forward );
  clear();

  // Scroll-backward key (shift + down-arrow)
  input("\033[1;2A");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Scroll_backward );
  clear();

  // Scroll-backward key (shift + down-arrow) in applications mode
  input("\033[b");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Scroll_backward );
  clear();

  // Center of keypad
  input("\033[E");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Center );
  clear();

  // back-tab key
  input("\033[Z");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Back_tab );
  clear();

  // find key
  input("\033[1~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Find );
  clear();

  // select key
  input("\033[4~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Select );
  clear();

  // shifted delete-character key
  input("\033[3;2~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_del_char );
  clear();

  // shifted end key
  input("\033[1;2F");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_end );
  clear();

  // shifted home key
  input("\033[1;2H");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_home );
  clear();

  // shifted insert-character key
  input("\033[2;2~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_insert );
  clear();

  // shifted left-arrow key
  input("\033[1;2D");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_left );
  clear();

  // shifted next key
  input("\033[6;2~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_page_down );
  clear();

  // shifted previous key
  input("\033[5;2~");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_page_up );
  clear();

  // shifted right-arrow key
  input("\033[1;2C");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Shift_right );
  clear();

  // Keypad slash (numlock off)
  input("\033Oo");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Slash );
  clear();

  // Keypad asterisk (numlock off)
  input("\033Oj");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Asterisk );
  clear();

  // Keypad minus sign (numlock off)
  input("\033Om");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Minus_sign );
  clear();

  // Keypad plus sign (numlock off)
  input("\033Ok");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Plus_sign );
  clear();

  // Keypad insert (numlock off)
  input("\033Op");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Insert );
  clear();

  // Keypad delete (numlock off)
  input("\033On");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Del_char );
  clear();

  // Keypad left-arrow (numlock off)
  input("\033Ot");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Left );
  clear();

  // Keypad right-arrow (numlock off)
  input("\033Ov");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Right );
  clear();

  // Keypad up-arrow (numlock off)
  input("\033Ox");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Up );
  clear();

  // Keypad down-arrow (numlock off)
  input("\033Or");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Down );
  clear();

  // Keypad upper left (numlock off)
  input("\033Ow");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Upper_left );
  clear();

  // Keypad upper right (numlock off)
  input("\033Oy");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Upper_right );
  clear();

  // Keypad center (numlock off)
  input("\033Ou");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Center );
  clear();

  // Keypad lower left (numlock off)
  input("\033Oq");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Lower_left );
  clear();

  // Keypad lower right (numlock off)
  input("\033Os");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Lower_right );
  clear();

}

//----------------------------------------------------------------------
void FKeyboardTest::mouseTest()
{
  std::cout << std::endl;

  // X11 mouse
  input("\033[M Z2");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::X11mouse );
  clear();

  // SGR mouse
  input("\033[<0;11;7M");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Extended_mouse );
  clear();

  // URXVT mouse
  input("\033[32;11;7M");
  processInput();
  std::cout << " - Key: " << keyboard->getKeyName(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::Urxvt_mouse );
  clear();

  // Without mouse support
  keyboard->disableMouseSequences();
  input("\033[M Z2");
  processInput();
  CPPUNIT_ASSERT ( key_pressed != finalcut::FKey::X11mouse );
  clear();
  input("\033[<0;11;7M");
  processInput();
  CPPUNIT_ASSERT ( key_pressed != finalcut::FKey::Extended_mouse );
  clear();
  input("\033[32;11;7M");
  processInput();
  CPPUNIT_ASSERT ( key_pressed != finalcut::FKey::Urxvt_mouse );
  clear();
}

//----------------------------------------------------------------------
void FKeyboardTest::utf8Test()
{
  std::cout << std::endl;

  // UTF-8 encoded character

  // Linear B syllable (4 bytes)
  input("\360\220\200\200");
  processInput();
  std::cout << " - code: " << uInt32(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey(0x10000) );

  // Euro sign (3 bytes)
  input("\342\202\254");
  processInput();
  std::cout << " - code: " << uInt32(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey(0x20ac) );

  // u with two Dots (2 bytes)
  input("\303\274");
  processInput();
  std::cout << " - code: " << uInt32(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey(0x00fc) );

  // @ (1 bytes)
  input("@");
  processInput();
  std::cout << " - code: " << uInt32(key_pressed) << std::endl;
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey(0x0040) );

  // Invalid UTF-8
  key_pressed = finalcut::FKey(0xffffffff);
  input("\377");
  processInput();
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey(0xffffffff) );

  // Without UTF-8 support
  keyboard->disableUTF8();
  input("\360\220\200\200");
  processInput();
  CPPUNIT_ASSERT ( key_released == finalcut::FKey(128) );
  clear();
}

//----------------------------------------------------------------------
void FKeyboardTest::unknownKeyTest()
{
  std::cout << std::endl;

  // Unknown key code
  CPPUNIT_ASSERT ( keyboard->getKeyName(finalcut::FKey(0xf8d0)) == "" );

  // Unknown input
  input("\033[_.");
  processInput();
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::None );
  CPPUNIT_ASSERT ( keyboard->getKeyName(key_pressed) == "" );
}

//----------------------------------------------------------------------
void FKeyboardTest::init()
{
  finalcut::internal::var::app_object \
      = reinterpret_cast<finalcut::FApplication*>(this);  // Need for isQuit()
  keyboard = new finalcut::FKeyboard();
  auto cmd1 = std::bind(&FKeyboardTest::keyPressed, this);
  auto cmd2 = std::bind(&FKeyboardTest::keyReleased, this);
  auto cmd3 = std::bind(&FKeyboardTest::escapeKeyPressed, this);
  auto cmd4 = std::bind(&FKeyboardTest::mouseTracking, this);
  finalcut::FKeyboardCommand key_cmd1 (cmd1);
  finalcut::FKeyboardCommand key_cmd2 (cmd2);
  finalcut::FKeyboardCommand key_cmd3 (cmd3);
  finalcut::FKeyboardCommand key_cmd4 (cmd4);
  keyboard->setPressCommand (key_cmd1);
  keyboard->setReleaseCommand (key_cmd2);
  keyboard->setEscPressedCommand (key_cmd3);
  keyboard->setMouseTrackingCommand (key_cmd4);
  keyboard->setKeypressTimeout (100000);  // 100 ms
  processInput();
  CPPUNIT_ASSERT ( key_pressed == finalcut::FKey::None );
  keyboard->enableUTF8();
  keyboard->enableMouseSequences();

  auto ptr = &test::fkey;
  const auto& ref = *reinterpret_cast<test::original_type*>(ptr);
  keyboard->setTermcapMap (ref);
}

//----------------------------------------------------------------------
void FKeyboardTest::input (std::string s)
{
  // Simulates keystrokes

  const char EOT = 0x04;  // End of Transmission

  std::string::const_iterator iter;
  iter = s.begin();

  while ( iter != s.end() )
  {
    char c = *iter;

    if ( ioctl (finalcut::FTermios::getStdIn(), TIOCSTI, &c) < 0 )
      break;

    ++iter;
  }

  if ( ioctl (finalcut::FTermios::getStdIn(), TIOCSTI, &EOT) < 0 )
    return;
}

//----------------------------------------------------------------------
void FKeyboardTest::processInput()
{
  keyboard->escapeKeyHandling();  // special case: Esc key
  keyboard->clearKeyBufferOnTimeout();

  if ( keyboard->isKeyPressed() )
    keyboard->fetchKeyCode();

  keyboard->processQueuedInput();
  // Keyboard interval timeout 75 ms (= 75,000,000 ns)
  const struct timespec ms[]{{0, 75000000L}};
  nanosleep (ms, NULL);
}

//----------------------------------------------------------------------
void FKeyboardTest::clear()
{
  keyboard->clearKeyBuffer();
  number_of_keys = 0;
  key_pressed = finalcut::FKey::None;
  key_released = finalcut::FKey::None;
}

//----------------------------------------------------------------------
void FKeyboardTest::keyPressed()
{
  key_pressed = keyboard->getKey();
  number_of_keys++;
}

//----------------------------------------------------------------------
void FKeyboardTest::keyReleased()
{
  key_released = keyboard->getKey();
}

//----------------------------------------------------------------------
void FKeyboardTest::escapeKeyPressed()
{
  key_pressed = finalcut::FKey::Escape;
  key_released = finalcut::FKey::Escape;
  number_of_keys++;
}

//----------------------------------------------------------------------
void FKeyboardTest::mouseTracking()
{
  key_pressed = keyboard->getKey();
}

// Put the test suite in the registry
CPPUNIT_TEST_SUITE_REGISTRATION (FKeyboardTest);

// The general unit test main part
#include <main-test.inc>
