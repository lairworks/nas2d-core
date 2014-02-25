// ==================================================================================
// = The Legend of Mazzeroth
// = Copyright © 2008 - 2010, New Age Software
// ==================================================================================
// = This file is part of The Legend of Mazzeroth.
// = 
// = The Legend of Mazzeroth is free software; you can redistribute it and/or modify
// = it under the terms of the GNU General Public License version 3 as published by
// = the Free Software Foundation.
// = 
// = The Legend of Mazzeroth is distributed in the hope that it will be useful,
// = but WITHOUT ANY WARRANTY; without even the implied warranty of
// = MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// = GNU General Public License for more details.
// = 
// = You should have received a copy of the GNU General Public License
// = along with The Legend of Mazzeroth; if not, write to the Free Software
// = Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// ==================================================================================

#ifndef _LOM_GUI_PROGRESSBAR_
#define _LOM_GUI_PROGRESSBAR_

#include "Control.h"


/**
 * \class	ProgressBar
 * \brief	Shows progress of operations for visual feedback.
 *
 * \note	Can show percentage or text.
 */
class ProgressBar : public Control
{
public:
	ProgressBar(const std::string& name, int minWidth = 16, int minHeight = 16);

	void setProgress(double progress);
	void showPercent(bool toggle);

	void setBarColor(int r, int g, int b);
	
protected:
	
private:
	void drawSkin();
	void drawPlain();
	void logic();

	int		mBarWidth;		/**< Width of the progress bar. */

	int		mBarRed;
	int		mBarGreen;
	int		mBarBlue;

	double	mProgress;		/**< Current progress. */
	
	bool	mShowPercent;	/**< Toggle showing percent. */
};
#endif