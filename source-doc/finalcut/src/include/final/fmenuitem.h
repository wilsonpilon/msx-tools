/***********************************************************************
* fmenuitem.h - Widget FMenuItem                                       *
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
 *      ▕▔▔▔▔▔▔▔▔▔▔▔▏*       1▕▔▔▔▔▔▔▔▏
 *      ▕ FMenuItem ▏-┬- - - -▕ FMenu ▏
 *      ▕▁▁▁▁▁▁▁▁▁▁▁▏ :       ▕▁▁▁▁▁▁▁▏
 *                    :
 *                    :      1▕▔▔▔▔▔▔▔▔▔▔▔▏
 *                    └- - - -▕ FMenuList ▏
 *                            ▕▁▁▁▁▁▁▁▁▁▁▁▏
 */

#ifndef FMENUITEM_H
#define FMENUITEM_H

#if !defined (USE_FINAL_H) && !defined (COMPILE_FINAL_CUT)
  #error "Only <final/final.h> can be included directly."
#endif

#include "final/fwidget.h"

namespace finalcut
{

// class forward declaration
class FDialog;
class FMenu;
class FMenuList;

//----------------------------------------------------------------------
// class FMenuItem
//----------------------------------------------------------------------

class FMenuItem : public FWidget
{
  public:
    // Using-declarations
    using FWidget::addAccelerator;
    using FWidget::delAccelerator;
    using FWidget::setEnable;

    // Constructor
    explicit FMenuItem (FWidget* = nullptr);
    explicit FMenuItem (const FString&, FWidget* = nullptr);
    FMenuItem (FKey, const FString&, FWidget* = nullptr);

    // Disable copy constructor
    FMenuItem (const FMenuItem&) = delete;

    // Destructor
    ~FMenuItem() override;

    // Disable copy assignment operator (=)
    FMenuItem& operator = (const FMenuItem&) = delete;

    // Accessors
    FString             getClassName() const override;
    FKey                getHotkey() const;
    FMenu*              getMenu() const;
    std::size_t         getTextLength() const;
    std::size_t         getTextWidth() const;
    FString             getText() const;

    // Mutators
    bool                setEnable (bool = true) override;
    bool                setFocus (bool = true) override;
    bool                unsetFocus() override;
    void                setSelected();
    void                unsetSelected();
    void                setSeparator();
    void                unsetSeparator();
    void                setCheckable();
    void                unsetCheckable();
    void                setChecked();
    void                unsetChecked();
    void                setRadioButton();
    void                unsetRadioButton();
    void                setMenu (FMenu*);
    void                setText (const FString&);

    // Inquiries
    bool                isSelected() const;
    bool                isSeparator() const;
    bool                isCheckable() const;
    bool                isChecked() const;
    bool                isRadioButton() const;
    bool                hasHotkey() const;
    bool                hasMenu() const;

    // Methods
    void                addAccelerator (FKey, FWidget*) override;
    void                delAccelerator (FWidget*) override;
    void                openMenu() const;

    // Event handlers
    void                onKeyPress (FKeyEvent*) override;
    void                onMouseDoubleClick (FMouseEvent*) override;
    void                onMouseDown (FMouseEvent*) override;
    void                onMouseUp (FMouseEvent*) override;
    void                onMouseMove (FMouseEvent*) override;
    void                onAccel (FAccelEvent*) override;
    void                onFocusIn (FFocusEvent*) override;
    void                onFocusOut (FFocusEvent*) override;

  protected:
    // Accessor
    FWidget*            getSuperMenu() const;

    // Mutator
    void                setSuperMenu (FWidget*);

    // Inquiries
    bool                isDialog (const FWidget*) const;
    bool                isMenuBar (const FWidget*) const;
    bool                isMenu (const FWidget*) const;

    // Method
    void                initLayout() override;

  private:
    // Accessor
    FMenuList*          getFMenuList (FWidget&);

    // Methods
    void                init();
    void                calculateTextDimensions();
    void                updateSuperMenuDimensions();
    void                updateMenubarDimensions() const;
    void                processEnable() const;
    void                processDisable() const;
    void                processActivate() const;
    void                processDeactivate() const;
    void                createDialogList (FMenu*) const;
    template <typename T>
    void                passMouseEvent (T, const FMouseEvent*, Event) const;

    // Callback methods
    void                cb_switchToDialog (FDialog*) const;
    void                cb_destroyDialog (FDialog*);

    virtual void        processClicked();

    // Data members
    FString      text{};
    FMenu*       menu{nullptr};
    FWidget*     super_menu{nullptr};
    FDialog*     associated_window{nullptr};
    std::size_t  text_length{0};
    std::size_t  text_width{0};
    FKey         accel_key{FKey::None};
    FKey         hotkey{FKey::None};
    bool         selected{false};
    bool         separator{false};
    bool         checkable{false};
    bool         checked{false};
    bool         radio_button{false};
    bool         dialog_index{false};

    // Friend classes
    friend class FDialogListMenu;
    friend class FMenuList;
    friend class FMenuBar;
    friend class FMenu;
};


// FMenuItem inline functions
//----------------------------------------------------------------------
inline FString FMenuItem::getClassName() const
{ return "FMenuItem"; }

//----------------------------------------------------------------------
inline FKey FMenuItem::getHotkey() const
{ return hotkey; }

//----------------------------------------------------------------------
inline FMenu* FMenuItem::getMenu() const
{ return menu; }

//----------------------------------------------------------------------
inline std::size_t FMenuItem::getTextLength() const
{ return text_length; }

//----------------------------------------------------------------------
inline std::size_t FMenuItem::getTextWidth() const
{ return text_width; }

//----------------------------------------------------------------------
inline FString FMenuItem::getText() const
{ return text; }

//----------------------------------------------------------------------
inline bool FMenuItem::unsetFocus()
{ return setFocus(false); }

//----------------------------------------------------------------------
inline void FMenuItem::setSeparator()
{
  separator = true;
  unsetFocusable();
}

//----------------------------------------------------------------------
inline void FMenuItem::unsetSeparator()
{
  separator = false;
  setFocusable();
}

//----------------------------------------------------------------------
inline void FMenuItem::setCheckable()
{ checkable = true; }

//----------------------------------------------------------------------
inline void FMenuItem::unsetCheckable()
{ checkable = false; }

//----------------------------------------------------------------------
inline void FMenuItem::setChecked()
{ checked = true; }

//----------------------------------------------------------------------
inline void FMenuItem::unsetChecked()
{ checked = false; }

//----------------------------------------------------------------------
inline void FMenuItem::setRadioButton()
{ radio_button = true; }

//----------------------------------------------------------------------
inline void FMenuItem::unsetRadioButton()
{ radio_button = false; }

//----------------------------------------------------------------------
inline void FMenuItem::setMenu(FMenu* m)
{ menu = m; }

//----------------------------------------------------------------------
inline bool FMenuItem::isSelected() const
{ return selected; }

//----------------------------------------------------------------------
inline bool FMenuItem::isSeparator() const
{ return separator; }

//----------------------------------------------------------------------
inline bool FMenuItem::isCheckable() const
{ return checkable; }

//----------------------------------------------------------------------
inline bool FMenuItem::isChecked() const
{ return checked; }

//----------------------------------------------------------------------
inline bool FMenuItem::isRadioButton() const
{ return radio_button; }

//----------------------------------------------------------------------
inline bool FMenuItem::hasHotkey() const
{ return hotkey != FKey::None; }

//----------------------------------------------------------------------
inline bool FMenuItem::hasMenu() const
{ return menu != nullptr; }

//----------------------------------------------------------------------
inline FWidget* FMenuItem::getSuperMenu() const
{ return super_menu; }

//----------------------------------------------------------------------
inline void FMenuItem::setSuperMenu (FWidget* smenu)
{ super_menu = smenu; }

}  // namespace finalcut

#endif  // FMENUITEM_H
