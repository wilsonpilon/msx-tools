/***********************************************************************
* fdialoglistmenu.h - Widget FDialogListMenu                           *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2016-2020 Markus Gans                                      *
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
 *       ▕▔▔▔▔▔▔▔▔▔▏ ▕▔▔▔▔▔▔▔▔▔▔▔▏
 *       ▕ FWindow ▏ ▕ FMenuList ▏
 *       ▕▁▁▁▁▁▁▁▁▁▏ ▕▁▁▁▁▁▁▁▁▁▁▁▏
 *            ▲           ▲
 *            │           │
 *            └─────┬─────┘
 *                  │
 *              ▕▔▔▔▔▔▔▔▏
 *              ▕ FMenu ▏
 *              ▕▁▁▁▁▁▁▁▏
 *                  ▲
 *                  │
 *         ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏           ▕▔▔▔▔▔▔▔▔▔▔▔▏
 *         ▕ FDialogListMenu ▏- - - - - -▕ FMenuItem ▏
 *         ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏1         1▕▁▁▁▁▁▁▁▁▁▁▁▏
 */

#ifndef FDIALOGLISTMENU_H
#define FDIALOGLISTMENU_H
#if !defined (USE_FINAL_H) && !defined (COMPILE_FINAL_CUT)
  #error "Only <final/final.h> can be included directly."
#endif

#include "final/fmenu.h"

namespace finalcut
{

//----------------------------------------------------------------------
// class FDialogListMenu
//----------------------------------------------------------------------

class FDialogListMenu : public FMenu
{
  public:
    // Constructors
    explicit FDialogListMenu (FWidget* = nullptr);
    explicit FDialogListMenu (const FString&, FWidget* = nullptr);

    // Disable copy constructor
    FDialogListMenu (const FDialogListMenu&) = delete;

    // Destructor
    ~FDialogListMenu() noexcept override;

    // Disable copy assignment operator (=)
    FDialogListMenu& operator = (const FDialogListMenu&) = delete;

    // Accessors
    FString getClassName() const override;

  private:
    // Method
    void init();
};


// FDialogListMenu inline functions
//----------------------------------------------------------------------
inline FString FDialogListMenu::getClassName() const
{ return "FDialogListMenu"; }

}  // namespace finalcut

#endif  // FDIALOGLISTMENU_H
