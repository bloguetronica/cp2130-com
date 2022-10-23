/* CP2130 Commander - Version 4.0 for Debian Linux
   Copyright (c) 2022 Samuel Lourenço

   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program.  If not, see <https://www.gnu.org/licenses/>.


   Please feel free to contact me via e-mail: samuel.fmlourenco@gmail.com */


// Includes
#include <QPointer>
#include "aboutdialog.h"
#include "common.h"

// Definitions
QPointer<AboutDialog> aboutDialog_;

// Closes the about dialog
void closeAboutDialog()
{
    if (!aboutDialog_.isNull()) {
        aboutDialog_->close();  // Close the about dialog if open
    }
}

// Shows the about dialog
void showAboutDialog()
{
    if (aboutDialog_.isNull()) {  // If the dialog wasn't previously open
        aboutDialog_ = new AboutDialog;  // Note that the about dialog doesn't have a parent
        aboutDialog_->show();
    } else {
        aboutDialog_->showNormal();  // Required if the dialog is minimized
        aboutDialog_->activateWindow();  // Set focus on the previous dialog (dialog is raised and selected)
    }
}
