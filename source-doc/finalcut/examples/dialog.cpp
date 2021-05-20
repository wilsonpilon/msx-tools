/***********************************************************************
* dialog.cpp - A FDialog example                                       *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2015-2020 Markus Gans                                      *
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

using finalcut::FPoint;
using finalcut::FSize;

// function prototype
void cb_quit (const finalcut::FApplication&);


//----------------------------------------------------------------------
// callback function
//----------------------------------------------------------------------
void cb_quit (const finalcut::FApplication& app)
{
  app.quit();
}


//----------------------------------------------------------------------
//                               main part
//----------------------------------------------------------------------
int main (int argc, char* argv[])
{
  // Create the application object
  finalcut::FApplication app{argc, argv};

  // Create a simple dialog box
  finalcut::FDialog dgl{&app};
  dgl.setText ("FDialog");
  dgl.setGeometry (FPoint{4, 3}, FSize{41, 11});

  // Create text labels
  finalcut::FLabel label_1{&dgl};
  finalcut::FLabel label_2{&dgl};

  label_1 << finalcut::UniChar::BlackUpPointingTriangle
          << std::wstring{L"\n"}
          << finalcut::UniChar::BoxDrawingsUpAndRight
          << finalcut::FString{2, finalcut::UniChar::BoxDrawingsHorizontal}
          << " Double click the title bar button,";
  label_2 << "press Q on the keyboard,\n"
          << "or push the button below to exit\n"
          << "the program.";

  label_1.setGeometry (FPoint{1, 1}, FSize{38, 2});
  label_2.setGeometry (FPoint{5, 3}, FSize{34, 3});

  // Create the quit button
  finalcut::FButton btn{"&Quit", &dgl};
  btn.setGeometry (FPoint{16, 7}, FSize{9, 1});

  // Connect the button signal "clicked" with the callback function
  btn.addCallback
  (
    "clicked",
    &cb_quit,
    std::ref(app)
  );

  // Set dialog object as main widget
  finalcut::FWidget::setMainWidget(&dgl);

  // Show and start the application
  dgl.show();
  return app.exec();
}
