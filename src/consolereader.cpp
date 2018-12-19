// source http://stackoverflow.com/questions/6878507/using-qsocketnotifier-to-select-on-a-char-device/7389622#7389622
/*gpscon - qps console application using qtmobility
    Copyright (C) <2015> <Christian Weniger>

    This file is part of gpscon.

    gpscon is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    gpscon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with gpscon. If not, see <http://www.gnu.org/licenses/>.*/

#include "consolereader.h"
#include <QTextStream>

#include <unistd.h> //Provides STDIN_FILENO

ConsoleReader::ConsoleReader(QObject *parent) :
	QObject(parent),
	notifier(STDIN_FILENO, QSocketNotifier::Read)
{
	connect(&notifier, SIGNAL(activated(int)), this, SLOT(text()));
}

void ConsoleReader::text()
{
	QTextStream qin(stdin);
	QString line = qin.readLine();
	emit textReceived(line);
}
