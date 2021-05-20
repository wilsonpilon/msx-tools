/***********************************************************************
* menu.cpp - A menu example                                            *
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

#include <final/final.h>

using FKey = finalcut::FKey;
using finalcut::FPoint;
using finalcut::FSize;


//----------------------------------------------------------------------
// class Menu
//----------------------------------------------------------------------

class Menu final : public finalcut::FDialog
{
  public:
    // Constructor
    explicit Menu (finalcut::FWidget* = nullptr);

    // Disable copy constructor
    Menu (const Menu&) = delete;

    // Destructor
    ~Menu() override = default;

    // Disable copy assignment operator (=)
    Menu& operator = (const Menu&) = delete;

  private:
    // Methods
    void configureFileMenuItems();
    void configureEditMenuItems();
    void configureChoiceMenuItems();
    void configureColorMenuItems();
    void configureStyleMenuItems();
    void configureBorderMenuItems();
    void defaultCallback (const finalcut::FMenuList*);
    void initLayout() override;
    void adjustSize() override;

    // Event handler
    void onClose (finalcut::FCloseEvent*) override;

    // Callback method
    void cb_message (const finalcut::FMenuItem*);

    // Data members
    finalcut::FString        line{13, finalcut::UniChar::BoxDrawingsHorizontal};
    finalcut::FMenuBar       Menubar{this};
    finalcut::FMenu          File{"&File", &Menubar};
    finalcut::FMenu          Edit{"&Edit", &Menubar};
    finalcut::FMenu          Choice{"&Choice", &Menubar};
    finalcut::FMenuItem      Window{"&Window", &Menubar};
    finalcut::FMenuItem      Help{"&Help", &Menubar};
    finalcut::FMenuItem      New{"&New", &File};
    finalcut::FMenuItem      Open{"&Open...", &File};
    finalcut::FMenuItem      Save{"&Save", &File};
    finalcut::FMenuItem      SaveAs{"&Save as...", &File};
    finalcut::FMenuItem      Close{"&Close", &File};
    finalcut::FMenuItem      Line1{&File};
    finalcut::FMenuItem      Print{"&Print", &File};
    finalcut::FMenuItem      Line2{&File};
    finalcut::FMenuItem      Quit{"&Quit", &File};
    finalcut::FMenuItem      Undo{FKey::Ctrl_z, "&Undo", &Edit};
    finalcut::FMenuItem      Redo{FKey::Ctrl_y, "&Redo", &Edit};
    finalcut::FMenuItem      Line3{&Edit};
    finalcut::FMenuItem      Cut{FKey::Ctrl_x, "Cu&t", &Edit};
    finalcut::FMenuItem      Copy{FKey::Ctrl_c, "&Copy", &Edit};
    finalcut::FMenuItem      Paste{FKey::Ctrl_v, "&Paste", &Edit};
    finalcut::FMenuItem      Line4{&Edit};
    finalcut::FMenuItem      Search{FKey::Ctrl_f, "&Search", &Edit};
    finalcut::FMenuItem      Next{FKey::F3, "Search &next", &Edit};
    finalcut::FMenuItem      Line5{&Edit};
    finalcut::FMenuItem      SelectAll{FKey::Ctrl_a, "Select &all", &Edit};
    finalcut::FMenu          Color{"&Color", &Choice};
    finalcut::FMenu          Style{"&Style", &Choice};
    finalcut::FMenu          Border{"&Border", &Choice};
    finalcut::FRadioMenuItem Color1{"Red", &Color};
    finalcut::FRadioMenuItem Color2{"Green", &Color};
    finalcut::FRadioMenuItem Color3{"Yellow", &Color};
    finalcut::FRadioMenuItem Color4{"Brue", &Color};
    finalcut::FRadioMenuItem Color5{"Black", &Color};
    finalcut::FCheckMenuItem Bold{"Bold", &Style};
    finalcut::FCheckMenuItem Italic{"Italic", &Style};
    finalcut::FMenu          BColor{"&Color", &Border};
    finalcut::FMenu          BStyle{"&Style", &Border};
    finalcut::FRadioMenuItem BColor1{"Red", &BColor};
    finalcut::FRadioMenuItem BColor2{"Blue", &BColor};
    finalcut::FRadioMenuItem BStyle1{line, &BStyle};
    finalcut::FRadioMenuItem BStyle2{"-------------", &BStyle};
    finalcut::FRadioMenuItem BStyle3{"- - - - - - -", &BStyle};
    finalcut::FRadioMenuItem BStyle4{"-  -  -  -  -", &BStyle};
    finalcut::FStatusBar     Statusbar{this};
    finalcut::FLabel         Headline1{this};
    finalcut::FLabel         Headline2{this};
    finalcut::FLabel         Info{this};
};

//----------------------------------------------------------------------
Menu::Menu (finalcut::FWidget* parent)
  : finalcut::FDialog{parent}
{
  // Menu bar itms
  File.setStatusbarMessage ("File management commands");
  Edit.setStatusbarMessage ("Cut-and-paste editing commands");
  Choice.setStatusbarMessage ("Choice menu");
  Window.setDisable();
  Help.setStatusbarMessage ("Show version and copyright information");

  // Menu items
  configureFileMenuItems();
  configureEditMenuItems();
  configureChoiceMenuItems();

  // Add default menu item callback
  defaultCallback (&Menubar);

  // Statusbar at the bottom
  Statusbar.setMessage("Status bar message");

  // Headline labels
  Headline1 << " Key ";
  Headline1.ignorePadding();
  Headline1.setEmphasis();

  Headline2 << " Function ";
  Headline2.ignorePadding();
  Headline2.setEmphasis();

  // Info label
  Info << "<F10>            Activate menu bar\n"
       << "<Ctrl>+<Space>   Activate menu bar\n"
       << "<Menu>           Activate menu bar\n"
       << "<Shift>+<Menu>   Open dialog menu\n"
       << "<Meta>+<X>       Exit";
}

//----------------------------------------------------------------------
void Menu::configureFileMenuItems()
{
  // "File" menu items
  New.addAccelerator (FKey::Ctrl_n);  // Ctrl + N
  New.setStatusbarMessage ("Create a new file");
  Open.addAccelerator (FKey::Ctrl_o);  // Ctrl + O
  Open.setStatusbarMessage ("Locate and open a text file");
  Save.addAccelerator (FKey::Ctrl_s);  // Ctrl + S
  Save.setStatusbarMessage ("Save the file");
  SaveAs.setStatusbarMessage ("Save the current file under a different name");
  Close.addAccelerator (FKey::Ctrl_w);  // Ctrl + W
  Close.setStatusbarMessage ("Close the current file");
  Line1.setSeparator();
  Print.addAccelerator (FKey::Ctrl_p);  // Ctrl + P
  Print.setStatusbarMessage ("Print the current file");
  Line2.setSeparator();
  Quit.addAccelerator (FKey::Meta_x);  // Meta/Alt + X
  Quit.setStatusbarMessage ("Exit the program");

  // Add quit menu item callback
  Quit.addCallback
  (
    "clicked",
    finalcut::getFApplication(),
    &finalcut::FApplication::cb_exitApp,
    this
  );
}

//----------------------------------------------------------------------
void Menu::configureEditMenuItems()
{
  // "Edit" menu items
  Undo.setStatusbarMessage ("Undo the previous operation");
  Redo.setDisable();
  Line3.setSeparator();
  Cut.setStatusbarMessage ("Remove the input text "
                           "and put it in the clipboard");
  Copy.setStatusbarMessage ("Copy the input text into the clipboad");
  Paste.setStatusbarMessage ("Insert text form clipboard");
  Line4.setSeparator();
  Search.setStatusbarMessage ("Search for text");
  Next.setStatusbarMessage ("Repeat the last search command");
  Line5.setSeparator();
  SelectAll.setStatusbarMessage ("Select the whole text");
}

//----------------------------------------------------------------------
void Menu::configureChoiceMenuItems()
{
  // "Choice" menu items
  Color.setStatusbarMessage ("Choose a color");
  Style.setStatusbarMessage ("Choose a Style");
  Border.setStatusbarMessage ("Choose Border");

  configureColorMenuItems();
  configureStyleMenuItems();
  configureBorderMenuItems();
}

//----------------------------------------------------------------------
void Menu::configureColorMenuItems()
{
  // "Color" menu items
  Color1.setStatusbarMessage ("Set text red");
  Color2.setStatusbarMessage ("Set text green");
  Color3.setStatusbarMessage ("Set text yellow");
  Color4.setStatusbarMessage ("Set text brue");
  Color5.setStatusbarMessage ("Set text black");
  Color5.setChecked();
}

//----------------------------------------------------------------------
void Menu::configureStyleMenuItems()
{
  // "Style" menu items
  Bold.setStatusbarMessage ("Set text bold");
  Italic.setStatusbarMessage ("Set text italic");
}

//----------------------------------------------------------------------
void Menu::configureBorderMenuItems()
{
  // "Border" menu items
  BColor.setStatusbarMessage ("Choose the border color");
  BStyle.setStatusbarMessage ("Choose the border Style");

  // "BColor" menu items
  BColor1.setStatusbarMessage ("Set red border");
  BColor2.setStatusbarMessage ("Set blue border");

  // "BStyle" menu items
  BStyle1.setChecked();
  BStyle1.setStatusbarMessage ("Set border 1");
  BStyle2.setStatusbarMessage ("Set border 2");
  BStyle3.setStatusbarMessage ("Set border 3");
  BStyle4.setStatusbarMessage ("Set border 4");
}

//----------------------------------------------------------------------
void Menu::defaultCallback (const finalcut::FMenuList* mb)
{
  for (uInt i{1}; i <= mb->getCount(); i++)
  {
    auto item = mb->getItem(int(i));

    if ( item
      && item->isEnabled()
      && item->acceptFocus()
      && item->isVisible()
      && ! item->isSeparator()
      && item->getText() != "&Quit" )
    {
      // Add the callback function
      item->addCallback
      (
        "clicked",
        this, &Menu::cb_message,
        item
      );

      // Call sub-menu
      if ( item->hasMenu() )
        defaultCallback (item->getMenu());
    }
  }
}

//----------------------------------------------------------------------
void Menu::initLayout()
{
  Headline1.setGeometry (FPoint{3, 2}, FSize{5, 1});
  Headline2.setGeometry (FPoint{19, 2}, FSize{10, 1});
  Info.setGeometry(FPoint{2, 1}, FSize{36, 5});
  FDialog::initLayout();
}

//----------------------------------------------------------------------
void Menu::adjustSize()
{
  const auto pw = int(getDesktopWidth());
  const auto ph = int(getDesktopHeight());
  setX (1 + (pw - int(getWidth())) / 2, false);
  setY (1 + (ph - int(getHeight())) / 4, false);
  finalcut::FDialog::adjustSize();
}

//----------------------------------------------------------------------
void Menu::onClose (finalcut::FCloseEvent* ev)
{
  finalcut::FApplication::closeConfirmationDialog (this, ev);
}

//----------------------------------------------------------------------
void Menu::cb_message (const finalcut::FMenuItem* menuitem)
{
  auto text = menuitem->getText();
  text = text.replace('&', "");
  finalcut::FMessageBox::info ( this
                              , "Info"
                              , "You have chosen \"" + text + "\"" );
}


//----------------------------------------------------------------------
//                               main part
//----------------------------------------------------------------------

int main (int argc, char* argv[])
{
  // Create the application object
  finalcut::FApplication app {argc, argv};

  // Create main dialog object
  Menu main_dlg {&app};
  main_dlg.setText ("Menu example");
  main_dlg.setSize ({40, 8});
  main_dlg.setShadow();

  // Set dialog main_dlg as main widget
  finalcut::FWidget::setMainWidget (&main_dlg);

  // Show and start the application
  main_dlg.show();
  return app.exec();
}
