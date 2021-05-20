/***********************************************************************
* fbusyindicator.h - Shows background activity                         *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2020 Markus Gans                                           *
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

/*  Inheritance diagram
 *  ═══════════════════
 *
 * ▕▔▔▔▔▔▔▔▔▔▏ ▕▔▔▔▔▔▔▔▔▔▏
 * ▕ FVTerm  ▏ ▕ FObject ▏
 * ▕▁▁▁▁▁▁▁▁▁▏ ▕▁▁▁▁▁▁▁▁▁▏
 *      ▲           ▲
 *      │           │
 *      └─────┬─────┘
 *            │
 *       ▕▔▔▔▔▔▔▔▔▔▏
 *       ▕ FWidget ▏
 *       ▕▁▁▁▁▁▁▁▁▁▏
 *            ▲
 *            │
 *       ▕▔▔▔▔▔▔▔▔▔▏
 *       ▕ FWindow ▏
 *       ▕▁▁▁▁▁▁▁▁▁▏
 *            ▲
 *            │
 *       ▕▔▔▔▔▔▔▔▔▔▔▏
 *       ▕ FToolTip ▏
 *       ▕▁▁▁▁▁▁▁▁▁▁▏
 *            ▲
 *            │
 *    ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *    ▕ FBusyIndicator ▏
 *    ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 */

#ifndef FBUSYINDICATOR_H
#define FBUSYINDICATOR_H

#if !defined (USE_FINAL_H) && !defined (COMPILE_FINAL_CUT)
  #error "Only <final/final.h> can be included directly."
#endif

#include "final/ftooltip.h"

namespace finalcut
{

//----------------------------------------------------------------------
// class FBusyIndicator
//----------------------------------------------------------------------

class FBusyIndicator : public FToolTip
{
  public:
    // Constructor
    explicit FBusyIndicator (FWidget* = nullptr);

    // Disable copy constructor
    FBusyIndicator (const FBusyIndicator&) = delete;

    // Destructor
    ~FBusyIndicator() noexcept override;

    // Disable copy assignment operator (=)
    FBusyIndicator& operator = (const FBusyIndicator&) = delete;

    // Accessors
    FString             getClassName() const override;

    // Inquiries
    bool                isRunning() const;

    // Methods
    void                start();
    void                stop();

  private:
    // Constants
    static constexpr std::size_t TIMER = 200;

    // Methods
    void init();
    void createIndicatorText();

    // Event handler
    void onTimer (finalcut::FTimerEvent*) override;

    // Callback methods

    // Data members
    wchar_t uni_pattern[8]{L' ', L' ', L'·', L'·', L'•', L'•', L'●', L'●'};
    char    pattern[8]{L' ', L' ', L'.', L'.', L'+', L'+', L'#', L'#'};
    bool    running{false};
};


// FBusyIndicator inline functions
//----------------------------------------------------------------------
inline FString FBusyIndicator::getClassName() const
{ return "FBusyIndicator"; }

//----------------------------------------------------------------------
inline bool FBusyIndicator::isRunning() const
{
  return running;
}

}  // namespace finalcut

#endif  // FBUSYINDICATOR_H


