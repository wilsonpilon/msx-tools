/***********************************************************************
* ftermopenbsd.h - Contains the NetBSD/OpenBSD terminal functions      *
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

/*  Standalone class
 *  ════════════════
 *
 * ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 * ▕ FTermOpenBSD ▏
 * ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 */

#ifndef FTERMOPENBSD_H
#define FTERMOPENBSD_H

#if !defined (USE_FINAL_H) && !defined (COMPILE_FINAL_CUT)
  #error "Only <final/final.h> can be included directly."
#endif

#include <sys/ioctl.h>

#if defined(UNIT_TEST)
  #define WSKBDIO_GETENCODING uInt32(0x4004570F)
  #define WSKBDIO_SETENCODING uInt32(0x80045710)
  #define WSKBDIO_GETDEFAULTBELL uInt32(0x40105706)
  #define WSKBDIO_SETBELL uInt32(0x80105703)
  #define WSKBD_BELL_DOPITCH  0x1  // get/set pitch
  #define WSKBD_BELL_DOPERIOD 0x2  // get/set period
  #define WSKBD_BELL_DOVOLUME 0x4  // get/set volume
  #define WSKBD_BELL_DOALL    0x7  // all of the above

  using kbd_t = uInt32;

  struct wskbd_bell_data
  {
    uInt which;   // values to get/set
    uInt pitch;   // pitch, in Hz
    uInt period;  // period, in milliseconds
    uInt volume;  // percentage of max volume
  };
#elif defined(__NetBSD__) || defined(__OpenBSD__)
  #include <sys/time.h>
  #include <dev/wscons/wsconsio.h>
#endif

namespace finalcut
{

//----------------------------------------------------------------------
// class FTermOpenBSD
//----------------------------------------------------------------------

class FTermOpenBSD final
{
  public:
    // Accessor
    FString            getClassName() const;

    // Inquiries
    static bool        isBSDConsole();

    // Mutators
    static void        disableMetaSendsEscape();
    static void        enableMetaSendsEscape();

    // Methods
    static void        init();
    static void        finish();
    static bool        setBeep (int, int);
    static bool        resetBeep();

#if defined(__NetBSD__) || defined(__OpenBSD__) || defined(UNIT_TEST)
  private:
    // Methods
    static void        warnNotInitialized();
    static bool        saveBSDConsoleEncoding();
    static bool        setBSDConsoleEncoding (kbd_t);
    static bool        setBSDConsoleMetaEsc();
    static bool        resetBSDConsoleEncoding();

    // Data members
    static kbd_t       bsd_keyboard_encoding;
    static bool        meta_sends_escape;
#endif  // defined(__NetBSD__) || defined(__OpenBSD__) || defined(UNIT_TEST)
};


// FTermOpenBSD inline functions
//----------------------------------------------------------------------
inline FString FTermOpenBSD::getClassName() const
{ return "FTermOpenBSD"; }

//----------------------------------------------------------------------
#if defined(__NetBSD__) || defined(__OpenBSD__) || defined(UNIT_TEST)
inline void FTermOpenBSD::enableMetaSendsEscape()
{ meta_sends_escape = true; }

//----------------------------------------------------------------------
inline void FTermOpenBSD::disableMetaSendsEscape()
{ meta_sends_escape = false; }

#endif  // defined(__NetBSD__) || defined(__OpenBSD__) || defined(UNIT_TEST)

}  // namespace finalcut

#endif  // FTERMOPENBSD_H
