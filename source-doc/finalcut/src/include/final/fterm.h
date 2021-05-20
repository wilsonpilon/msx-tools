/***********************************************************************
* fterm.h - Base class for terminal control                            *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2012-2021 Markus Gans                                      *
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

/*  Base class
 *  ══════════
 *
 * ▕▔▔▔▔▔▔▔▏1       1▕▔▔▔▔▔▔▔▔▔▔▏
 * ▕ FTerm ▏-┬- - - -▕ FTermios ▏
 * ▕▁▁▁▁▁▁▁▏ :       ▕▁▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      1▕▔▔▔▔▔▔▔▔▔▔▏
 *           :- - - -▕ FTermcap ▏
 *           :       ▕▁▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      1▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *           :- - - -▕ FTermcapQuirks ▏
 *           :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      1▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *           :- - - -▕ FTermDetection ▏
 *           :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      1▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *           :- - - -▕ FTermXTerminal ▏
 *           :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      1▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *           :- - - -▕   FTermLinux   ▏
 *           :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      1▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *           :- - - -▕  FTermFreeBSD  ▏
 *           :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      1▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *           :- - - -▕  FTermOpenBSD  ▏
 *           :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      1▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *           :- - - -▕ FColorPalette ▏
 *           :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      1▕▔▔▔▔▔▔▔▔▔▔▔▏
 *           :- - - -▕ FOptiAttr ▏
 *           :       ▕▁▁▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      1▕▔▔▔▔▔▔▔▔▔▔▔▏
 *           :- - - -▕ FOptiMove ▏
 *           :       ▕▁▁▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      1▕▔▔▔▔▔▔▔▔▔▔▔▏
 *           :- - - -▕ FKeyboard ▏
 *           :       ▕▁▁▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      1▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *           :- - - -▕ FMouseControl ▏
 *           :       ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      *▕▔▔▔▔▔▔▔▔▔▏
 *           :- - - -▕ FString ▏
 *           :       ▕▁▁▁▁▁▁▁▁▁▏
 *           :
 *           :      *▕▔▔▔▔▔▔▔▔▏
 *           :- - - -▕ FPoint ▏
 *           :       ▕▁▁▁▁▁▁▁▁▏
 *           :
 *           :      *▕▔▔▔▔▔▔▔▏
 *           └- - - -▕ FRect ▏
 *                   ▕▁▁▁▁▁▁▁▏
 */

#ifndef FTERM_H
#define FTERM_H

#if !defined (USE_FINAL_H) && !defined (COMPILE_FINAL_CUT)
  #error "Only <final/final.h> can be included directly."
#endif

#include "final/fconfig.h"

#include <sys/ioctl.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <langinfo.h>

#if defined(F_HAVE_GETTTYNAM) && defined(F_HAVE_TTYENT_H)
  #include <ttyent.h>
#endif

#include <clocale>
#include <cmath>
#include <csignal>
#include <functional>
#include <memory>
#include <queue>
#include <utility>
#include <string>
#include <vector>

#include "final/fc.h"
#include "final/fcolorpalette.h"
#include "final/fstring.h"
#include "final/fsystem.h"

namespace finalcut
{

// class forward declaration
class FColorPalette;
class FKeyboard;
class FMouseControl;
class FOptiAttr;
class FOptiMove;
class FPoint;
class FStartOptions;
class FSize;
class FString;
class FTermBuffer;
class FTermData;
class FTermDebugData;
class FTermDetection;
class FTermXTerminal;

#if defined(__linux__) || defined(UNIT_TEST)
  class FTermLinux;
#endif

#if defined(__FreeBSD__) || defined(__DragonFly__) || defined(UNIT_TEST)
  class FTermFreeBSD;
#endif

#if defined(__NetBSD__) || defined(__OpenBSD__) || defined(UNIT_TEST)
  class FTermOpenBSD;
#endif

//----------------------------------------------------------------------
// class FTerm
//----------------------------------------------------------------------

class FTerm final
{
  public:
    // Using-declarations
    using defaultPutChar = std::function<int(int)>;
    using FSetPalette = FColorPalette::FSetPalette;

    // Constructor
    FTerm();

    // Disable copy constructor
    FTerm (const FTerm&) = delete;

    // Destructor
    ~FTerm();

    // Disable copy assignment operator (=)
    FTerm& operator = (const FTerm&) = delete;

    // Accessors
    static FString           getClassName();
    static std::size_t       getLineNumber();
    static std::size_t       getColumnNumber();
    static FString           getKeyName (FKey);
    charSubstitution&        getCharSubstitutionMap();
    static int               getTTYFileDescriptor();
    static std::string       getTermType();
    static std::string       getTermFileName();
    static int               getTabstop();
    static int               getMaxColor();
    static auto              getColorPaletteTheme() -> std::shared_ptr<FColorPalette>&;
    static auto              getFSystem() -> std::unique_ptr<FSystem>&;
    static auto              getFTermData() -> FTermData&;
    static auto              getFOptiMove() -> FOptiMove&;
    static auto              getFOptiAttr() -> FOptiAttr&;
    static auto              getFTermDetection() -> FTermDetection&;
    static auto              getFTermXTerminal() -> FTermXTerminal&;
    static auto              getFKeyboard() -> FKeyboard&;
    static auto              getFMouseControl() -> FMouseControl&;

#if defined(__linux__) || defined(UNIT_TEST)
    static auto              getFTermLinux() -> FTermLinux&;
#endif

#if defined(__FreeBSD__) || defined(__DragonFly__) || defined(UNIT_TEST)
    static auto              getFTermFreeBSD() -> FTermFreeBSD&;
#endif

#if defined(__NetBSD__) || defined(__OpenBSD__) || defined(UNIT_TEST)
    static auto              getFTermOpenBSD() -> FTermOpenBSD&;
#endif

#if DEBUG
    static auto              getFTermDebugData() -> FTermDebugData&;
#endif

    // Inquiries
    static bool              isNormal (const FChar&);
    static bool              isRaw();
    static bool              hasUTF8();
    static bool              hasVT100();
    static bool              hasASCII();
    static bool              isMonochron();
    static bool              isAnsiTerminal();
    static bool              isXTerminal();
    static bool              isRxvtTerminal();
    static bool              isUrxvtTerminal();
    static bool              isKdeTerminal();
    static bool              isGnomeTerminal();
    static bool              isPuttyTerminal();
    static bool              isWindowsTerminal();
    static bool              isTeraTerm();
    static bool              isCygwinTerminal();
    static bool              isMinttyTerm();
    static bool              isLinuxTerm();
    static bool              isFreeBSDTerm();
    static bool              isNetBSDTerm();
    static bool              isOpenBSDTerm();
    static bool              isSunTerminal();
    static bool              isScreenTerm();
    static bool              isTmuxTerm();
    static bool              isKtermTerminal();
    static bool              isMltermTerminal();
    static bool              isKittyTerminal();
    static bool              isNewFont();
    static bool              isInitialized();
    static bool              isCursorHideable();
    static bool              hasChangedTermSize();
    static bool              hasShadowCharacter();
    static bool              hasHalfBlockCharacter();
    static bool              hasAlternateScreen();
    static bool              canChangeColorPalette();

    // Mutators
    static void              setFSystem (std::unique_ptr<FSystem>&);
    static void              setTermType (const std::string&);
    static void              setInsertCursor (bool = true);
    static void              unsetInsertCursor();
    static void              redefineDefaultColors (bool = true);
    static void              setDblclickInterval (const uInt64);
    static void              useAlternateScreen (bool = true);
    static bool              setUTF8 (bool = true);
    static bool              unsetUTF8();

    // Methods
    static bool              setVGAFont();
    static bool              setNewFont();
    static bool              resetFont();
    static int               openConsole();
    static int               closeConsole();
    static std::string       moveCursorString (int, int, int, int);
    static const char*       cursorsVisibilityString (bool = true);
    static void              detectTermSize();
    static void              setTermSize (const FSize&);
    static void              setTermTitle (const FString&);
    static void              setKDECursor (KdeKonsoleCursorShape);
    static void              saveColorMap();
    static void              resetColorMap();
    static void              setPalette (FColor, int, int, int);
    template <typename ClassT>
    static void              setColorPaletteTheme (const FSetPalette& = &FTerm::setPalette);
    static void              setBeep (int, int);
    static void              resetBeep();
    static void              beep();

    static void              setEncoding (Encoding);
    static Encoding          getEncoding();
    static std::string       getEncodingString();
    static bool              charEncodable (wchar_t);
    static wchar_t           charEncode (wchar_t);
    static wchar_t           charEncode (wchar_t, Encoding);

    static bool              scrollTermForward();
    static bool              scrollTermReverse();

    static defaultPutChar&   putchar();  // function pointer
    template <typename... Args>
    static void              putstringf (const std::string&, Args&&...);
    static void              putstring (const std::string&, int = 1);
    static int               putchar_ASCII (int);
    static int               putchar_UTF8  (int);

    void                     initTerminal();
    static void              initScreenSettings();
    static const char*       changeAttribute (FChar&, FChar&);
    static void              changeTermSizeFinished();

  private:
    // Methods
    static FStartOptions&    getStartOptions();
    static void              init_global_values();
    static void              init_terminal_device_path();
    static void              oscPrefix();
    static void              oscPostfix();
    static void              init_alt_charset();
    static void              init_pc_charset();
    static void              init_cygwin_charmap();
    static void              init_teraterm_charmap();
    static void              init_fixed_max_color();
    static void              init_termcap();
    static void              init_quirks();
    static void              init_optiMove();
    static void              init_optiAttr();
    static bool              init_font();
    static void              init_locale();
    static void              init_encoding();
    static void              init_encoding_set();
    static void              init_term_encoding();
    static void              init_individual_term_encoding();
    static void              init_force_vt100_encoding();
    static void              init_utf8_without_alt_charset();
    static void              init_tab_quirks();
    static void              init_captureFontAndTitle();
    static bool              hasNoFontSettingOption();
    static bool              isDefaultPaletteTheme();
    static void              redefineColorPalette();
    static void              restoreColorPalette();
    static void              setInsertCursorStyle();
    static void              setOverwriteCursorStyle();
    static const char*       enableCursorString();
    static const char*       disableCursorString();
    static void              enableMouse();
    static void              disableMouse();
    static void              enableApplicationEscKey();
    static void              disableApplicationEscKey();
    static void              enableKeypad();
    static void              disableKeypad();
    static void              enableAlternateCharset();
    static void              useAlternateScreenBuffer();
    static void              useNormalScreenBuffer();
    void                     init();
    bool                     init_terminal() const;
    void                     initOSspecifics() const;
    void                     initTermspecifics() const;
    void                     initBaudRate() const;
    void                     finish() const;
    void                     finishOSspecifics() const;
    void                     finish_encoding() const;
    static void              printExitMessage();
    static void              terminalSizeChange();
    [[noreturn]] static void processTermination (int);
    static void              setSignalHandler();
    static void              resetSignalHandler();
    static void              signal_handler (int);
};


// non-member function forward declarations
// implemented in fterm_functions.cpp
//----------------------------------------------------------------------
uInt env2uint (const char*);
bool isReverseNewFontchar (wchar_t);
bool hasFullWidthSupports();
wchar_t cp437_to_unicode (uChar);
uChar unicode_to_cp437 (wchar_t);
FString getFullWidth (const FString&);
FString getHalfWidth (const FString&);
FString getColumnSubString (const FString&, std::size_t, std::size_t);
std::size_t getLengthFromColumnWidth (const FString&, std::size_t);
std::size_t getColumnWidth (const FString&, std::size_t);
std::size_t getColumnWidth (const FString&);
std::size_t getColumnWidth (const wchar_t);
std::size_t getColumnWidth (const FChar&);
std::size_t getColumnWidth (const FTermBuffer&);
void addColumnWidth (FChar&);
int getCharLength (const FString&, std::size_t);
int getPrevCharLength (const FString&, std::size_t);
std::size_t searchLeftCharBegin (const FString&, std::size_t);
std::size_t searchRightCharBegin (const FString&, std::size_t);
FPoint readCursorPos();

// Check for 7-bit ASCII
template<typename CharT>
inline bool is7bit (CharT ch)
{
  using char_type = typename std::make_unsigned<CharT>::type;
  return static_cast<char_type>(ch) < 128;
}

// FTerm inline functions
//----------------------------------------------------------------------
inline FString FTerm::getClassName()
{ return "FTerm"; }

//----------------------------------------------------------------------
inline void FTerm::setFSystem (std::unique_ptr<FSystem>& fsystem)
{
  getFSystem().swap(fsystem);
}

//----------------------------------------------------------------------
inline void FTerm::unsetInsertCursor()
{ return setInsertCursor(false); }

//----------------------------------------------------------------------
inline bool FTerm::unsetUTF8()
{ return setUTF8(false); }

//----------------------------------------------------------------------
template <typename ClassT>
inline void FTerm::setColorPaletteTheme (const FSetPalette& f)
{
  getColorPaletteTheme() = std::make_shared<ClassT>(f);
  getColorPaletteTheme()->setColorPalette();
}

//----------------------------------------------------------------------
template <typename... Args>
inline void FTerm::putstringf (const std::string& format, Args&&... args)
{
  const int size = std::snprintf (nullptr, 0, format.data(), args...) + 1;

  if ( size == -1 )
    return;

  const auto count = std::size_t(size);
  std::vector<char> buf(count);
  std::snprintf (&buf[0], count, format.data(), std::forward<Args>(args)...);
  putstring (std::string(&buf[0]), 1);
}

//----------------------------------------------------------------------
inline void FTerm::initTerminal()
{ init(); }

}  // namespace finalcut

//----------------------------------------------------------------------
inline std::ostream& operator << (std::ostream& os, finalcut::UniChar c)
{
  return os << static_cast<char>(c);
}

//----------------------------------------------------------------------
inline std::wostream& operator << (std::wostream& os, finalcut::UniChar c)
{
  return os << static_cast<wchar_t>(c);
}

#endif  // FTERM_H
