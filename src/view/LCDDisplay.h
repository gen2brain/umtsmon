/* umtsmon - a program to control/monitor your UMTS card
 * Copyright (C) 2007,2008  Klaas van Gend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation
 * applicable version is GPL version 2 only.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef LCDDISPLAY_H_
#define LCDDISPLAY_H_

#include "MonthlyTraffic.h"

#include <qwidget.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qfont.h>
#include <qcolor.h>
//Added by qt3to4:
#include <QPaintEvent>

/// This GUI class graphically displays various information for the user
class LCDDisplay : public QLabel
{
	Q_OBJECT
	
public:
	LCDDisplay( QWidget * aParent, const char * aName = 0, Qt::WFlags aFlags = 0 );

	/// sets the operatorname and updates the GUI 
	void setOperatorName(const QString& aName, bool needsUpdate=true) 
		{setStringAndUpdate(theOperatorName, aName, needsUpdate);};
	/// sets the radio type string (HSDPA, UMTS, GPRS, etc) and updates the GUI 
	void setRadioType(const QString& aName, bool needsUpdate=true)
		{setStringAndUpdate(theRadioType, aName, needsUpdate);};
	/// sets the profile name and updates the GUI 
	void setProfile(const QString& aName, bool needsUpdate=true)
		{setStringAndUpdate(theProfile, aName, needsUpdate);};
	
	/** sets the upload strings and updates the GUI
	 *  @param anUpload	      upload in bytes
	 *  @param anUploadSpeed  upload speed in bytes/s
	 */
	void setUpload(unsigned long anUpload, 
	               unsigned long anUploadSpeed, 
	               bool needsUpdate=true);
	/** sets the upload strings and updates the GUI
	 *  @param anUpload	      upload in bytes
	 *  @param anUploadSpeed  upload speed in bytes/s
	 */
	void setDownload(unsigned long aDownload, 
	               unsigned long aDownloadSpeed, 
	               bool needsUpdate=true);
	
	/// sets the monthly total data traffic and update the GUI
	void setTotalTraffic(unsigned long aNumber, MonthlyTraffic::ThresholdStatus aStatus, bool needsUpdate=true)
		{
			theTotalStatus = aStatus;
			setStringAndUpdate(theTotal, number2HumanString(aNumber), needsUpdate);
		};

	/// sets the monthly time traffic and update the GUI
	void setTotalTime(unsigned long aNumber, MonthlyTraffic::ThresholdStatus aStatus, bool needsUpdate=true)
		{
			theTotalStatus = aStatus;
			setStringAndUpdate(theTotal, number2TimeString(aNumber), needsUpdate);
		};

	/// sets the connected time and update the GUI
	void setConnectedTime(unsigned long aNumber, bool needsUpdate=true)
		{setStringAndUpdate(theConnectedTime, number2TimeString(aNumber), needsUpdate);};

	/** sets the signal strength of the radio (result of AT+CSQ)
	 *  @param aNewStrength  values of 0-31 and 99 are allowed
	 */
	void setStrength(unsigned int aNewStrength, bool needsUpdate=true);

	/** sets the boolean for roaming or not
	 *  this will have two implications:
	 *    1) if roaming: draw triangle in front of operator name
	 *    2) either home or global icon visible on the right side
	 *  this is not the most critical part of the UI, therefore no immediate update
	 *  @param anIsRoaming  true if roaming
	 */
	void setRoaming(bool anIsRoaming)
		{isRoaming = anIsRoaming;};
	
protected:
	virtual void drawContents ( QPainter * p ); 
	virtual void paintEvent ( QPaintEvent * e ); 
	
private:
	/** internal function that will set a string to a name and make the widget
	 *  update its looks if needed
	 *  @param aString OUTPUT the qstring to put stuff in to
	 *  @param aName   INPUT the qstring with the name to set
	 *  @param needsUpdate  true if you want to redraw the widget, false if not (yet)
	 */ 
	void setStringAndUpdate(QString& aString, const QString& aName, bool needsUpdate)
		{ aString = aName.left(17); if (needsUpdate) {this->update();}; };

	/** converts an ulong into a human readable number string.
	 *  i.e.       31 will become 31 B
	 *           3141 will become 3.07 KiB
	 *         314156 will become 307 KiB
	 *       31415654 will become 30.0 MiB
	 *  @param aNumber the number to convert
	 *  @returns the converted and human readable string
	 */
	QString number2HumanString(unsigned long aNumber) const;

	/** converts an ulong into a human readable number string.
	 *  i.e.       31 will become 0m 31s
	 *           3141 will become 52m 21s
	 *         314156 will become 87h 15m
	 *  @param aNumber the number to convert
	 *  @returns the converted and human readable string
	 */
	QString number2TimeString(unsigned long aNumber) const;

	QPixmap theGaugePixmap;
	
	/** this is the width of theGaugePixmap to draw (max 104, min 0 in 20 steps)
	 *  -1 means "draw n/a"
	 */
	int	theSignalWidth;
	
	/// true if roaming
	bool isRoaming;
	
	QString	theOperatorName;
	QString theRadioType;
	QString theProfile;
	QString theConnectedTime;
	
	QString theUpload;
	QString theUploadSpeed;
	QString theDownload;
	QString theDownloadSpeed;
	QString theTotal;
	MonthlyTraffic::ThresholdStatus theTotalStatus;
	
	// three font sizes to cache 
	QFont   the14Font;
	QFont   the10Font;
	QFont   the8Font;
	
	// two colors to cache
	QColor  theBlack;
	QColor  theShadow;
};


#endif // LCDDISPLAY_H_ 
