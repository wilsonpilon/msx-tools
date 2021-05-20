/***********************************************************************
* ftermcapquirks-test.cpp - FTermcapQuirks unit tests                  *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2018-2021 Markus Gans                                      *
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

//----------------------------------------------------------------------
namespace test
{

typedef struct
{
  const char* string;
  char  tname[alignof(char*)];
}
tcap_map;

static tcap_map tcap[] =
{
  { 0, "bl" },  // bell
  { 0, "ec" },  // erase_chars
  { 0, "cl" },  // clear_screen
  { 0, "cd" },  // clr_eos
  { 0, "ce" },  // clr_eol
  { 0, "cb" },  // clr_bol
  { 0, "ho" },  // cursor_home
  { 0, "ll" },  // cursor_to_ll
  { 0, "cr" },  // carriage_return
  { 0, "ta" },  // tab
  { 0, "bt" },  // back_tab
  { 0, "ip" },  // insert_padding
  { 0, "ic" },  // insert_character
  { 0, "IC" },  // parm_ich
  { 0, "rp" },  // repeat_char
  { 0, "Ic" },  // initialize_color
  { 0, "Ip" },  // initialize_pair
  { 0, "AF" },  // set_a_foreground
  { 0, "AB" },  // set_a_background
  { 0, "Sf" },  // set_foreground
  { 0, "Sb" },  // set_background
  { 0, "sp" },  // set_color_pair
  { 0, "op" },  // orig_pair
  { 0, "oc" },  // orig_colors
  { 0, "NC" },  // no_color_video
  { 0, "cm" },  // cursor_address
  { 0, "ch" },  // column_address
  { 0, "cv" },  // row_address
  { 0, "vs" },  // cursor_visible
  { 0, "vi" },  // cursor_invisible
  { 0, "ve" },  // cursor_normal
  { 0, "up" },  // cursor_up
  { 0, "do" },  // cursor_down
  { 0, "le" },  // cursor_left
  { 0, "nd" },  // cursor_right
  { 0, "UP" },  // parm_up_cursor
  { 0, "DO" },  // parm_down_cursor
  { 0, "LE" },  // parm_left_cursor
  { 0, "RI" },  // parm_right_cursor
  { 0, "sc" },  // save_cursor
  { 0, "rc" },  // restore_cursor
  { 0, "Ss" },  // set cursor style
  { 0, "sf" },  // scroll_forward
  { 0, "sr" },  // scroll_reverse
  { 0, "ti" },  // enter_ca_mode
  { 0, "te" },  // exit_ca_mode
  { 0, "eA" },  // enable_acs
  { 0, "md" },  // enter_bold_mode
  { 0, "me" },  // exit_bold_mode
  { 0, "mh" },  // enter_dim_mode
  { 0, "me" },  // exit_dim_mode
  { 0, "ZH" },  // enter_italics_mode
  { 0, "ZR" },  // exit_italics_mode
  { 0, "us" },  // enter_underline_mode
  { 0, "ue" },  // exit_underline_mode
  { 0, "mb" },  // enter_blink_mode
  { 0, "me" },  // exit_blink_mode
  { 0, "mr" },  // enter_reverse_mode
  { 0, "me" },  // exit_reverse_mode
  { 0, "so" },  // enter_standout_mode
  { 0, "se" },  // exit_standout_mode
  { 0, "mk" },  // enter_secure_mode
  { 0, "me" },  // exit_secure_mode
  { 0, "mp" },  // enter_protected_mode
  { 0, "me" },  // exit_protected_mode
  { 0, "XX" },  // enter_crossed_out_mode
  { 0, "me" },  // exit_crossed_out_mode
  { 0, "Us" },  // enter_dbl_underline_mode
  { 0, "Ue" },  // exit_dbl_underline_mode
  { 0, "sa" },  // set_attributes
  { 0, "me" },  // exit_attribute_mode
  { 0, "as" },  // enter_alt_charset_mode
  { 0, "ae" },  // exit_alt_charset_mode
  { 0, "S2" },  // enter_pc_charset_mode
  { 0, "S3" },  // exit_pc_charset_mode
  { 0, "im" },  // enter_insert_mode
  { 0, "ei" },  // exit_insert_mode
  { 0, "SA" },  // enter_am_mode
  { 0, "RA" },  // exit_am_mode
  { 0, "ac" },  // acs_chars
  { 0, "ks" },  // keypad_xmit
  { 0, "ke" },  // keypad_local
  { 0, "Km" },  // key_mouse
  { 0, "\0" }
};

}  // namespace test


//----------------------------------------------------------------------
// class FTermcapQuirksTest
//----------------------------------------------------------------------

class FTermcapQuirksTest : public CPPUNIT_NS::TestFixture
{
  public:
    FTermcapQuirksTest();
    ~FTermcapQuirksTest();

  protected:
    void classNameTest();
    void generalTest();
    void xtermTest();
#if defined(__FreeBSD__) || defined(__DragonFly__)
    void freebsdTest();
#endif
    void cygwinTest();
    void linuxTest();
    void rxvtTest();
    void vteTest();
    void kittyTest();
    void puttyTest();
    void teratermTest();
    void sunTest();
    void screenTest();

  private:
    std::string printSequence (const std::string&);

    // Adds code needed to register the test suite
    CPPUNIT_TEST_SUITE (FTermcapQuirksTest);

    // Add a methods to the test suite
    CPPUNIT_TEST (classNameTest);
    CPPUNIT_TEST (generalTest);
    CPPUNIT_TEST (xtermTest);
#if defined(__FreeBSD__) || defined(__DragonFly__)
    CPPUNIT_TEST (freebsdTest);
#endif
    CPPUNIT_TEST (cygwinTest);
    CPPUNIT_TEST (linuxTest);
    CPPUNIT_TEST (rxvtTest);
    CPPUNIT_TEST (vteTest);
    CPPUNIT_TEST (kittyTest);
    CPPUNIT_TEST (puttyTest);
    CPPUNIT_TEST (teratermTest);
    CPPUNIT_TEST (sunTest);
    CPPUNIT_TEST (screenTest);

    // End of test suite definition
    CPPUNIT_TEST_SUITE_END();
};

//----------------------------------------------------------------------
FTermcapQuirksTest::FTermcapQuirksTest()
{ }

//----------------------------------------------------------------------
FTermcapQuirksTest::~FTermcapQuirksTest()
{ }

//----------------------------------------------------------------------
void FTermcapQuirksTest::classNameTest()
{
  finalcut::FTermcapQuirks q;
  const finalcut::FString& classname = q.getClassName();
  CPPUNIT_ASSERT ( classname == "FTermcapQuirks" );
}

//----------------------------------------------------------------------
void FTermcapQuirksTest::generalTest()
{
  auto& caps = finalcut::FTermcap::strings;
  constexpr int last_item = int(sizeof(test::tcap) / sizeof(test::tcap[0])) - 1;

  for (std::size_t i = 0; i < last_item; i++)
    memcpy(&caps[i], &test::tcap[i], sizeof(test::tcap[0]));

  finalcut::FTermcap::tabstop = -1;
  finalcut::FTermcap::attr_without_color = -1;
  finalcut::FTermcap::can_change_color_palette = false;
  finalcut::FTermcapQuirks quirks;
  quirks.terminalFixup();

  CPPUNIT_ASSERT ( finalcut::FTermcap::tabstop == 8 );
  CPPUNIT_ASSERT ( finalcut::FTermcap::attr_without_color == 0 );
  CPPUNIT_ASSERT ( finalcut::FTermcap::can_change_color_palette );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_set_a_foreground)].string
                         , CSI "3%p1%dm" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_set_a_background)].string
                         , CSI "4%p1%dm" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_initialize_color)].string
                         , OSC "P%p1%x"
                               "%p2%{255}%*%{1000}%/%02x"
                               "%p3%{255}%*%{1000}%/%02x"
                               "%p4%{255}%*%{1000}%/%02x" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_enter_ca_mode)].string
                         , ESC "7" CSI "?47h"  );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_ca_mode)].string
                         , CSI "?47l" ESC "8" CSI "m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_cursor_address)].string
                         , CSI "%i%p1%d;%p2%dH" );
  // Non standard ECMA-48 (ANSI X3.64) terminal
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_enter_dbl_underline_mode)].string
                         , 0 );
  caps[int(finalcut::Termcap::t_exit_underline_mode)].string = CSI "24m";
  quirks.terminalFixup();
  // Standard ECMA-48 (ANSI X3.64) terminal
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_enter_dbl_underline_mode)].string
                         , CSI "21m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_dbl_underline_mode)].string
                         , CSI "24m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_bold_mode)].string
                         , CSI "22m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_dim_mode)].string
                         , CSI "22m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_underline_mode)].string
                         , CSI "24m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_blink_mode)].string
                         , CSI "25m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_reverse_mode)].string
                         , CSI "27m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_secure_mode)].string
                         , CSI "28m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_enter_crossed_out_mode)].string
                         , CSI "9m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_crossed_out_mode)].string
                         , CSI "29m" );
  CPPUNIT_ASSERT_CSTRING ( printSequence(caps[int(finalcut::Termcap::t_enter_ca_mode)].string).c_str()
                         , "Esc 7 Esc [ ? 4 7 h " );
}

//----------------------------------------------------------------------
void FTermcapQuirksTest::xtermTest()
{
  auto& caps = finalcut::FTermcap::strings;
  constexpr int last_item = int(sizeof(test::tcap) / sizeof(test::tcap[0])) - 1;

  for (std::size_t i = 0; i < last_item; i++)
    memcpy(&caps[i], &test::tcap[i], sizeof(test::tcap[0]));

  auto& data = finalcut::FTerm::getFTermData();
  auto& detect = finalcut::FTerm::getFTermDetection();
  finalcut::FTermcapQuirks quirks;
  finalcut::FTermcap::can_change_color_palette = false;
  detect.setXTerminal (true);
  data.setTermType ("xterm");
  quirks.terminalFixup();

  CPPUNIT_ASSERT ( finalcut::FTermcap::can_change_color_palette );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_initialize_color)].string
                         , OSC "4;%p1%d;rgb:"
                               "%p2%{255}%*%{1000}%/%2.2X/"
                               "%p3%{255}%*%{1000}%/%2.2X/"
                               "%p4%{255}%*%{1000}%/%2.2X" ESC "\\");
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_cursor_invisible)].string
                         , CSI "?25l" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_cursor_normal)].string
                         , CSI "?12l" CSI "?25h" );
  detect.setXTerminal (false);
}

#if defined(__FreeBSD__) || defined(__DragonFly__)
//----------------------------------------------------------------------
void FTermcapQuirksTest::freebsdTest()
{
  auto& caps = finalcut::FTermcap::strings;
  constexpr int last_item = int(sizeof(test::tcap) / sizeof(test::tcap[0])) - 1;

  for (std::size_t i = 0; i < last_item; i++)
    memcpy(&caps[i], &test::tcap[i], sizeof(test::tcap[0]));

  auto& data = finalcut::FTerm::getFTermData();
  auto& detect = finalcut::FTerm::getFTermDetection();
  finalcut::FTermcap::attr_without_color = -1;
  finalcut::FTermcapQuirks quirks;
  detect.setFreeBSDTerm (true);
  data.setTermType ("xterm-16color");
  quirks.setFTermData(&data);
  quirks.setFTermDetection (&detect);
  quirks.terminalFixup();

  CPPUNIT_ASSERT ( finalcut::FTermcap::attr_without_color == 18 );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_acs_chars)].string
                         , "-\036.\0370\333"
                           "a\260f\370g\361"
                           "h\261j\331k\277"
                           "l\332m\300n\305"
                           "q\304t\303u\264"
                           "v\301w\302x\263"
                           "y\363z\362~\371" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_set_attributes)].string
                         , CSI "0"
                               "%?%p1%p6%|%t;1%;"
                               "%?%p2%t;4%;"
                               "%?%p1%p3%|%t;7%;"
                               "%?%p4%t;5%;m"
                               "%?%p9%t\016%e\017%;" );
  detect.setFreeBSDTerm (false);
}
#endif

//----------------------------------------------------------------------
void FTermcapQuirksTest::cygwinTest()
{
  auto& caps = finalcut::FTermcap::strings;
  constexpr int last_item = int(sizeof(test::tcap) / sizeof(test::tcap[0])) - 1;

  for (std::size_t i = 0; i < last_item; i++)
    memcpy(&caps[i], &test::tcap[i], sizeof(test::tcap[0]));

  auto& data = finalcut::FTerm::getFTermData();
  auto& detect = finalcut::FTerm::getFTermDetection();
  finalcut::FTermcap::background_color_erase = false;
  finalcut::FTermcapQuirks quirks;
  detect.setCygwinTerminal (true);
  data.setTermType ("cygwin");
  quirks.terminalFixup();

  CPPUNIT_ASSERT ( finalcut::FTermcap::background_color_erase == true );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_cursor_invisible)].string
                         , CSI "?25l" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_cursor_visible)].string
                         , CSI "?25h" );
  detect.setCygwinTerminal (false);
}

//----------------------------------------------------------------------
void FTermcapQuirksTest::linuxTest()
{
  auto& caps = finalcut::FTermcap::strings;
  constexpr int last_item = int(sizeof(test::tcap) / sizeof(test::tcap[0])) - 1;

  for (std::size_t i = 0; i < last_item; i++)
    memcpy(&caps[i], &test::tcap[i], sizeof(test::tcap[0]));

  auto& data = finalcut::FTerm::getFTermData();
  auto& detect = finalcut::FTerm::getFTermDetection();
  finalcut::FTermcap::max_color = 8;
  finalcut::FTermcap::attr_without_color = -1;
  finalcut::FTermcapQuirks quirks;
  detect.setLinuxTerm (true);
  data.setTermType ("linux");
  quirks.terminalFixup();

  // 8 colors
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_set_a_foreground)].string
                         , CSI "3%p1%dm" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_set_a_background)].string
                         , CSI "4%p1%dm" );
  CPPUNIT_ASSERT ( finalcut::FTermcap::attr_without_color == 18 );

  // 16 colors
  finalcut::FTermcap::max_color = 16;
  quirks.terminalFixup();

  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_set_a_foreground)].string
                         , CSI "3%p1%{8}%m%d%?%p1%{7}%>%t;1%e;22%;m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_set_a_background)].string
                         , CSI "4%p1%{8}%m%d%?%p1%{7}%>%t;5%e;25%;m" );
  CPPUNIT_ASSERT ( finalcut::FTermcap::attr_without_color == 30 );

  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_set_attributes)].string
                         , CSI "0"
                               "%?%p6%t;1%;"
                               "%?%p1%p3%|%t;7%;"
                               "%?%p4%t;5%;m"
                               "%?%p9%t\016%e\017%;" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_enter_alt_charset_mode)].string
                         , "\016" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_alt_charset_mode)].string
                         , "\017" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_attribute_mode)].string
                         , CSI "0m\017" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_bold_mode)].string
                         , CSI "22m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_blink_mode)].string
                         , CSI "25m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_reverse_mode)].string
                         , CSI "27m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_secure_mode)].string
                         , 0 );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_protected_mode)].string
                         , 0 );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_crossed_out_mode)].string
                         , 0 );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_orig_pair)].string
                         , CSI "39;49;25m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_enter_dim_mode)].string
                         , 0 );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_dim_mode)].string
                         , 0 );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_enter_underline_mode)].string
                         , 0 );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_underline_mode)].string
                         , 0 );
  detect.setLinuxTerm (false);
}

//----------------------------------------------------------------------
void FTermcapQuirksTest::rxvtTest()
{
  auto& caps = finalcut::FTermcap::strings;
  constexpr int last_item = int(sizeof(test::tcap) / sizeof(test::tcap[0])) - 1;

  for (std::size_t i = 0; i < last_item; i++)
    memcpy(&caps[i], &test::tcap[i], sizeof(test::tcap[0]));

  auto& data = finalcut::FTerm::getFTermData();
  auto& detect = finalcut::FTerm::getFTermDetection();
  finalcut::FTermcapQuirks quirks;
  detect.setRxvtTerminal (true);
  data.setTermType ("rxvt");
  quirks.terminalFixup();

  // rxvt
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_enter_alt_charset_mode)].string
                         , 0 );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_alt_charset_mode)].string
                         , 0 );
  // rxvt-16color
  data.setTermType ("rxvt-16color");
  quirks.terminalFixup();
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_enter_alt_charset_mode)].string
                         , ESC "(0" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_alt_charset_mode)].string
                         , ESC "(B" );

  // urxvt
  detect.setUrxvtTerminal (true);
  quirks.terminalFixup();
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_set_a_foreground)].string
                         , CSI "%?%p1%{8}%<%t%p1%{30}%+%e%p1%'R'%+%;%dm" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_set_a_background)].string
                         , CSI "%?%p1%{8}%<%t%p1%'('%+%e%p1%{92}%+%;%dm" );

  detect.setUrxvtTerminal (false);
  detect.setRxvtTerminal (false);
}

//----------------------------------------------------------------------
void FTermcapQuirksTest::vteTest()
{
  auto& caps = finalcut::FTermcap::strings;
  constexpr int last_item = int(sizeof(test::tcap) / sizeof(test::tcap[0])) - 1;

  for (std::size_t i = 0; i < last_item; i++)
    memcpy(&caps[i], &test::tcap[i], sizeof(test::tcap[0]));

  auto& data = finalcut::FTerm::getFTermData();
  auto& detect = finalcut::FTerm::getFTermDetection();
  finalcut::FTermcap::attr_without_color = -1;
  finalcut::FTermcapQuirks quirks;
  detect.setGnomeTerminal (true);
  data.setTermType ("gnome-256color");
  quirks.terminalFixup();

  CPPUNIT_ASSERT ( finalcut::FTermcap::attr_without_color == 0 );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_underline_mode)].string
                         , CSI "24m" );

  detect.setGnomeTerminal (false);
}

//----------------------------------------------------------------------
void FTermcapQuirksTest::kittyTest()
{
  auto& caps = finalcut::FTermcap::strings;
  constexpr int last_item = int(sizeof(test::tcap) / sizeof(test::tcap[0])) - 1;

  for (std::size_t i = 0; i < last_item; i++)
    memcpy(&caps[i], &test::tcap[i], sizeof(test::tcap[0]));

  caps[int(finalcut::Termcap::t_enter_ca_mode)].string = CSI "?1049h";
  caps[int(finalcut::Termcap::t_exit_ca_mode)].string = CSI "?1049l";
  auto& data = finalcut::FTerm::getFTermData();
  auto& detect = finalcut::FTerm::getFTermDetection();
  finalcut::FTermcapQuirks quirks;
  detect.setKittyTerminal (true);
  data.setTermType ("xterm-kitty");
  quirks.terminalFixup();

  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_enter_ca_mode)].string
                         , CSI "?1049h" CSI "22;0;0t" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_ca_mode)].string
                         , CSI "?1049l" CSI "23;0;0t" );

  detect.setKittyTerminal (false);
}

//----------------------------------------------------------------------
void FTermcapQuirksTest::puttyTest()
{
  auto& caps = finalcut::FTermcap::strings;
  constexpr int last_item = int(sizeof(test::tcap) / sizeof(test::tcap[0])) - 1;

  for (std::size_t i = 0; i < last_item; i++)
    memcpy(&caps[i], &test::tcap[i], sizeof(test::tcap[0]));

  auto& data = finalcut::FTerm::getFTermData();
  auto& detect = finalcut::FTerm::getFTermDetection();
  finalcut::FTermcap::background_color_erase = false;
  finalcut::FTermcap::can_change_color_palette = false;
  finalcut::FTermcap::osc_support = false;
  finalcut::FTermcap::attr_without_color = -1;
  finalcut::FTermcapQuirks quirks;
  detect.setPuttyTerminal (true);
  data.setTermType ("putty");
  quirks.terminalFixup();

  CPPUNIT_ASSERT ( finalcut::FTermcap::background_color_erase == true );
  CPPUNIT_ASSERT ( finalcut::FTermcap::osc_support == true );
  CPPUNIT_ASSERT ( finalcut::FTermcap::attr_without_color == 0 );
  CPPUNIT_ASSERT ( finalcut::FTermcap::can_change_color_palette );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_initialize_color)].string
                         , OSC "P%p1%x"
                               "%p2%{255}%*%{1000}%/%02x"
                               "%p3%{255}%*%{1000}%/%02x"
                               "%p4%{255}%*%{1000}%/%02x" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_set_a_foreground)].string
                         , CSI "%?%p1%{8}%<"
                               "%t3%p1%d"
                               "%e%p1%{16}%<"
                               "%t9%p1%{8}%-%d"
                               "%e38;5;%p1%d%;m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_set_a_background)].string
                         , CSI "%?%p1%{8}%<"
                               "%t4%p1%d"
                               "%e%p1%{16}%<"
                               "%t10%p1%{8}%-%d"
                               "%e48;5;%p1%d%;m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_set_attributes)].string
                         , CSI "0"
                               "%?%p1%p6%|%t;1%;"
                               "%?%p5%t;2%;"
                               "%?%p2%t;4%;"
                               "%?%p1%p3%|%t;7%;"
                               "%?%p4%t;5%;m"
                               "%?%p9%t\016%e\017%;" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_enter_dim_mode)].string
                         , CSI "2m" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_dim_mode)].string
                         , CSI "22m" );

  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_clr_bol)].string
                         , CSI "1K" );

  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_orig_pair)].string
                         , CSI "39;49m" );

  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_orig_colors)].string
                         , OSC "R" );

  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_column_address)].string
                         , CSI "%i%p1%dG" );

  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_row_address)].string
                         , CSI "%i%p1%dd" );

  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_enable_acs)].string
                         , ESC "(B" ESC ")0" );

  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_enter_am_mode)].string
                         , CSI "?7h" );

  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_am_mode)].string
                         , CSI "?7l" );

  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_enter_pc_charset_mode)].string
                         , CSI "11m" );

  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_pc_charset_mode)].string
                         , CSI "10m" );

  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_key_mouse)].string
                        , CSI "M" );

  detect.setPuttyTerminal (false);
}

//----------------------------------------------------------------------
void FTermcapQuirksTest::teratermTest()
{
  auto& caps = finalcut::FTermcap::strings;
  constexpr int last_item = int(sizeof(test::tcap) / sizeof(test::tcap[0])) - 1;

  for (std::size_t i = 0; i < last_item; i++)
    memcpy(&caps[i], &test::tcap[i], sizeof(test::tcap[0]));

  auto& data = finalcut::FTerm::getFTermData();
  auto& detect = finalcut::FTerm::getFTermDetection();
  finalcut::FTermcap::eat_nl_glitch = false;
  finalcut::FTermcapQuirks quirks;
  detect.setTeraTerm (true);
  data.setTermType ("teraterm");
  quirks.terminalFixup();

  CPPUNIT_ASSERT ( finalcut::FTermcap::eat_nl_glitch == true );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_set_a_foreground)].string
                         , CSI "38;5;%p1%dm" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_set_a_background)].string
                         , CSI "48;5;%p1%dm" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_exit_attribute_mode)].string
                         , CSI "0m" SI );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_orig_pair)].string
                         , CSI "39;49m" );

  detect.setTeraTerm (false);
}

//----------------------------------------------------------------------
void FTermcapQuirksTest::sunTest()
{
  auto& caps = finalcut::FTermcap::strings;
  constexpr int last_item = int(sizeof(test::tcap) / sizeof(test::tcap[0])) - 1;

  for (std::size_t i = 0; i < last_item; i++)
    memcpy(&caps[i], &test::tcap[i], sizeof(test::tcap[0]));

  auto& data = finalcut::FTerm::getFTermData();
  auto& detect = finalcut::FTerm::getFTermDetection();
  finalcut::FTermcap::eat_nl_glitch = false;
  finalcut::FTermcapQuirks quirks;
  detect.setSunTerminal (true);
  data.setTermType ("sun-color");
  quirks.terminalFixup();

  CPPUNIT_ASSERT ( finalcut::FTermcap::eat_nl_glitch == true );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_parm_up_cursor)].string
                         , CSI "%p1%dA" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_parm_down_cursor)].string
                         , CSI "%p1%dB" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_parm_right_cursor)].string
                         , CSI "%p1%dC" );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_parm_left_cursor)].string
                         , CSI "%p1%dD" );

  for (std::size_t i = 0; finalcut::fc::fkey_cap_table[i].tname[0] != 0; i++)
  {
    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "K2", 2) == 0 )  // center of keypad
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "218z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "kb", 2) == 0 )  // backspace key
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , "\b" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "kD", 2) == 0
      && std::strlen(finalcut::fc::fkey_cap_table[i].tname) == 2 )  // delete-character key
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , "\177" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "@7", 2) == 0 )  // end key
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "220z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "k;", 2) == 0 )  // F10 function key
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "233z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "F1", 2) == 0 )  // F11 function key
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "234z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "F2", 2) == 0 )  // F12 function key
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "235z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "kh", 2) == 0 )  // home key
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "214z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "kI", 2) == 0 )  // insert-character key
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "247z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "kN", 2) == 0 )  // next-page key
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "222z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "%7", 2) == 0 )  // options key
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "194z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "kP", 2) == 0 )  // prev-page key
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "216z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "&5", 2) == 0 )  // resume key
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "193z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "&8", 2) == 0 )  // undo key
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "195z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "K2", 2) == 0 )  // center of keypad
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "218z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "kDx", 3) == 0 )  // keypad delete
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "249z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "@8x", 3) == 0 )  // enter/send key
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "250z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "KP1", 3) == 0 )  // keypad slash
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "212z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "KP2", 3) == 0 )  // keypad asterisk
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "213z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "KP3", 3) == 0 )  // keypad minus sign
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "254z" );

    if ( std::strncmp(finalcut::fc::fkey_cap_table[i].tname, "KP4", 3) == 0 )  // keypad plus sign
      CPPUNIT_ASSERT_CSTRING ( finalcut::fc::fkey_cap_table[i].string
                             , CSI "253z" );
  }

  detect.setSunTerminal (false);
}

//----------------------------------------------------------------------
void FTermcapQuirksTest::screenTest()
{
  auto& caps = finalcut::FTermcap::strings;
  constexpr int last_item = int(sizeof(test::tcap) / sizeof(test::tcap[0])) - 1;

  for (std::size_t i = 0; i < last_item; i++)
    memcpy(&caps[i], &test::tcap[i], sizeof(test::tcap[0]));

  auto& data = finalcut::FTerm::getFTermData();
  auto& detect = finalcut::FTerm::getFTermDetection();
  finalcut::FTermcapQuirks quirks;
  finalcut::FTermcap::can_change_color_palette = false;
  detect.setScreenTerm (true);
  data.setTermType ("screen-256color");
  quirks.terminalFixup();

  CPPUNIT_ASSERT ( finalcut::FTermcap::can_change_color_palette );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_initialize_color)].string
                         , ESC "P" OSC "4;%p1%d;rgb:"
                           "%p2%{255}%*%{1000}%/%2.2X/"
                           "%p3%{255}%*%{1000}%/%2.2X/"
                           "%p4%{255}%*%{1000}%/%2.2X" BEL ESC "\\" );

  detect.setTmuxTerm (true);
  caps[int(finalcut::Termcap::t_initialize_color)].string = 0;
  finalcut::FTermcap::can_change_color_palette = false;
  quirks.terminalFixup();

  CPPUNIT_ASSERT ( finalcut::FTermcap::can_change_color_palette );
  CPPUNIT_ASSERT_CSTRING ( caps[int(finalcut::Termcap::t_initialize_color)].string
                         , ESC "Ptmux;" ESC OSC "4;%p1%d;rgb:"
                           "%p2%{255}%*%{1000}%/%2.2X/"
                           "%p3%{255}%*%{1000}%/%2.2X/"
                           "%p4%{255}%*%{1000}%/%2.2X" BEL ESC "\\" );
  detect.setTmuxTerm (false);
  detect.setScreenTerm (false);
}


// private methods of FOptiMoveTest
//----------------------------------------------------------------------
std::string FTermcapQuirksTest::printSequence (const std::string& s)
{
  std::string sequence;
  const std::string ctrl_character[] =
  {
    "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
    "BS",  "Tab", "LF",  "VT",  "FF",  "CR",  "SO",  "SI",
    "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB",
    "CAN", "EM",  "SUB", "Esc", "FS",  "GS",  "RS",  "US",
    "Space"
  };

  for (std::string::size_type i = 0; i < s.length(); ++i)
  {
    char ch = s[i];

    if ( ch < 0x21 )
      sequence += ctrl_character[uInt(ch)];
    else
      sequence += ch;

    sequence += ' ';
  }

  return sequence;
}

// Put the test suite in the registry
CPPUNIT_TEST_SUITE_REGISTRATION (FTermcapQuirksTest);

// The general unit test main part
#include <main-test.inc>
