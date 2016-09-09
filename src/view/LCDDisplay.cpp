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


#include "LCDDisplay.h"

#include <qpainter.h>

// unfortunately, there's quite a lot of initialising here :-(
// we cache a lot to speed up drawing

LCDDisplay::LCDDisplay( QWidget* aParent, const char* aName, WFlags aFlags) 
	: QLabel(aParent, aName, aFlags), theSignalWidth(0), isRoaming(false), 
	theRadioType(tr("n/a")), theProfile(""),
	the14Font("Helvetica", 14), the10Font("Helvetica", 10), the8Font("Helvetica", 8),
	theBlack(20,20,20), theShadow(0x98, 0x9c, 0x62)
{
	// This creates a extra pixmap on the QWidget
	//QLabel::setPixmap(QPixmap::fromMimeSource("LCDBackground.png"));
	
	// This set the pixmap to the background of the image, unfortunatly it produce black border/corner 
	//setPaletteBackgroundColor(parentWidget()->paletteBackgroundColor())
	//setPaletteBackgroundPixmap(QPixmap::fromMimeSource("LCDBackground.png"));
	
	// This read the background color of the parent widget, fill a QPixmap with it
	// overlay the LCD background PNG and put it into the widget background
	QPixmap bg = QPixmap (336, 88);
	QPixmap pix = QPixmap::fromMimeSource("LCDBackground.png");
	bg.fill (parentWidget()->paletteBackgroundColor());
	QPainter p( &bg );
	p.drawPixmap( pix.rect(), pix );
	setPaletteBackgroundPixmap(bg);
	
	theGaugePixmap = QPixmap::fromMimeSource("LCDSignal.png");

	the14Font.setPixelSize(15);
	the10Font.setPixelSize(11);
	
	// and... let's draw the thing!
	this->update();
}



void LCDDisplay::setDownload(
		unsigned long aDownload, 
		unsigned long aDownloadSpeed, 
		bool needsUpdate)
{
	if (aDownload!=0)
		setStringAndUpdate(theDownload, number2HumanString(aDownload), false);
	else
		setStringAndUpdate(theDownload, "", needsUpdate);
	
	if (aDownloadSpeed!=0)
		setStringAndUpdate( theDownloadSpeed,
		                    number2HumanString(aDownloadSpeed)+"/s", 
	                        needsUpdate);
	else
		setStringAndUpdate(theDownloadSpeed, "", needsUpdate);
}

void LCDDisplay::setStrength(unsigned int aNewStrength, bool needsUpdate)
{
	const unsigned int SIGNAL_SATURATION=24;
	
	if (aNewStrength == 99)
		theSignalWidth = -1;
	else
	{
		if (aNewStrength > SIGNAL_SATURATION)
			aNewStrength = SIGNAL_SATURATION;
		theSignalWidth = (theGaugePixmap.width() * aNewStrength) / SIGNAL_SATURATION;
	}

	if (needsUpdate)
		this->update();
}


void LCDDisplay::setUpload(
		unsigned long anUpload, 
		unsigned long anUploadSpeed, 
		bool needsUpdate)
{
	if (anUpload!=0)
		setStringAndUpdate(theUpload, number2HumanString(anUpload), false);
	else
		setStringAndUpdate(theUpload, "", needsUpdate);
	
	if (anUploadSpeed!=0)
		setStringAndUpdate( theUploadSpeed,
		                    number2HumanString(anUploadSpeed)+"/s", 
	                        needsUpdate);
	else
		setStringAndUpdate(theUploadSpeed, "", needsUpdate);
}


void LCDDisplay::drawContents ( QPainter * p )
{
	// draws the background --> if you have the background in an extra QPixmap
	//QLabel::drawContents(p);

	// let's make sure the string fits in the screen,
	// compensate font size for the string length
	if (theOperatorName.length() > 12)
		p->setFont(the10Font);
	else
		p->setFont(the14Font);

	int myOperatorNameX = 170;
	if (isRoaming)
	{
		myOperatorNameX += 10;
		QPointArray myPoints(4);
		p->setPen(theShadow);
		myPoints.putPoints(0, 4, 171,27, 174,17, 178,27, 171,27);
		p->drawPolygon(myPoints);

		p->setPen(theBlack);
		myPoints.putPoints(0, 4, 170,26, 173,16, 177,26, 170,26);
		p->drawPolygon(myPoints);
	}
	
	
	p->setPen(theShadow);
	p->drawText(myOperatorNameX+1,28, theOperatorName);
	p->setPen(theBlack);
	p->drawText(myOperatorNameX,27, theOperatorName);

	// draw the labels - allow for i18n
	p->setPen(theShadow);
	p->setFont(the8Font);
	
	// and draw the 10-size shadows & blacks of the other data
	p->drawText( 48,57, theUpload);
	p->drawText(104,57, theUploadSpeed);
	p->drawText( 48,75, theDownload);
	p->drawText(104,75, theDownloadSpeed);
	p->drawText(246,52, theRadioType);
	p->drawText(197,76, theTotal);	
	p->drawText(285,76, theConnectedTime);	
	if (!theProfile.isEmpty())	
		p->drawText(172,52, theProfile);
	
	p->setPen(theBlack);
	p->drawText( 47,56, theUpload);
	p->drawText(103,56, theUploadSpeed);
	p->drawText( 47,74, theDownload);
	p->drawText(103,74, theDownloadSpeed);
	p->drawText(245,51, theRadioType);
	p->drawText(284,75, theConnectedTime);	
	if (!theProfile.isEmpty())	
		p->drawText(171,51, theProfile);
	
	if (theSignalWidth == -1)
	{
		p->drawText(52,24, tr("n/a"));
	}
	else
		p->drawPixmap( 42,17, theGaugePixmap, 0,0, theSignalWidth, theGaugePixmap.height() );

	// draw the total in either green, orange or red, depending on the status
	switch (theTotalStatus)
	{
		case MonthlyTraffic::OVER_LIMIT:
			p->setPen(QColor(200,20,20)); // red
			break;
		case MonthlyTraffic::BETWEEN:
			p->setPen(QColor(150,100,20)); // orange
			break;
		case MonthlyTraffic::BELOW:
			p->setPen(QColor(0,170,0)); // green
			break;
		case MonthlyTraffic::LIMITS_DISABLED:
			p->setPen(theBlack); // green
			break;
	};
	p->drawText(196,76, theTotal);	

} 

QString 
LCDDisplay::number2HumanString(unsigned long aNumber) const
{
	QString theUnit = "B";
	double myNumber = aNumber;
	
	if (myNumber < 1024.)
	{
		return QString::number(aNumber) + " " + theUnit;
	}
	
	// reduce the order of the number	
	while ((myNumber/999.9) >= 1.)
	{
		myNumber /= 1024;
		switch(theUnit[0].latin1())
		{
			case 'B':
				theUnit = "KiB";
				break;
			case 'K':
				theUnit = "MiB";
				break;
			case 'M':
				theUnit = "GiB";
				break;
			case 'G':
				theUnit = "TiB";
				break;
		}
	}

	// let's differentiate between '3.14', '31.4' and '314.'
	if (myNumber >= 100.)
	{
		return QString::number(myNumber, 'f', 0) + " " + theUnit;
	}
	if (myNumber >= 10.)
	{
		return QString::number(myNumber, 'f', 1) + " " + theUnit;
	}
	return QString::number(myNumber, 'f', 2) + " " + theUnit;
}


QString 
LCDDisplay::number2TimeString(unsigned long aNumber) const
{
	const int SECONDS_IN_HOUR = 3600;
	const int SECONDS_IN_DAY = 24*SECONDS_IN_HOUR;
	
	unsigned long theDays    = aNumber / (SECONDS_IN_DAY);
	aNumber %= SECONDS_IN_DAY;
	
	unsigned long theHours   = aNumber / SECONDS_IN_HOUR;
	aNumber %= SECONDS_IN_HOUR;
	
	unsigned long theMinutes = aNumber / 60;
	unsigned long theSeconds = aNumber % 60;
	
	if (theDays > 0)
	{
		return QString(tr("%1d %2h")) // e.g. 12d 12h => 12 days 13 hours
				.arg(QString::number(theDays)) 
				.arg(QString::number(theHours));
	}
	if (theHours > 0)
	{
		return QString(tr("%1h %2m")) // e.g. 12h 35m => 12 hours 35 minutes
				.arg(QString::number(theHours)) 
				.arg(QString::number(theMinutes));
	}
	return QString(tr("%1m %2s")) // e.g. 13m 3s => 13 minutes 3 seconds
			.arg(QString::number(theMinutes)) 
			.arg(QString::number(theSeconds));
}
