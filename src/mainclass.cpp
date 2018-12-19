// Template http://treyweaver.blogspot.de/2013/02/qt-console-application-template-tutorial.html
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

#include "gpspos.h"
#include "mainclass.h"
#include <QDebug>
#include <QDir>
#include <QFile>
MainClass::MainClass(QObject *parent) :
	QObject(parent)
{
	// get the instance of the main application
	app = QCoreApplication::instance();
	// setup everything here
	// create any global objects
	// setup debug and warning mode
}

// 10ms after the application starts this method will run
// all QT messaging is running at this point so threads, signals and slots
// will all work as expected.
void MainClass::run()
{
	bool task=true;
	// Add your main code here
	//qDebug() << "MainClass.Run is executing";(
	if (g_interval>g_tout)
	{
		qDebug() << "update intaval > timeout";
		task=false;
		quit();
	}
	if (g_del) deletelogfiles();
	if (g_log=="log2gpx")
	{
		m_position = new GpsPos(this, g_log);
		task=false;
		quit();
		//connect(m_position, SIGNAL(finished()),this, SLOT(quit()));
	}
	if ((g_lknv==true) && (g_runs==1))
	{
		m_position = new GpsPos(this, g_fout, g_posm, g_date, g_time);
		quit();
	}
	else if (task)
	{
		m_position = new GpsPos(this, g_runs, g_limit, g_dumps, g_lknv, g_sat, g_fout, g_posm, g_interval, g_date, g_time, g_log, g_newtrack, g_script, g_tout);
		connect(m_position, SIGNAL(finished()),this, SLOT(quit()));
	}

	// you must call quit when complete or the program will stay in the
	// messaging loop
	//quit();
}

// call this routine to quit the application
void MainClass::quit()
{
	// you can do some cleanup here
	// then do emit finished to signal CoreApplication to quit
	emit finished();
}

// shortly after quit is called the CoreApplication will signal this routine
// this is a good place to delete any objects that were created in the
// constructor and/or to stop any threads
void MainClass::aboutToQuitApp()
{
	// stop threads
	//sleep(1);   // wait for threads to stop.
	// delete any objects
}

void MainClass::deletelogfiles()
{
	QDir userdir=QDir::homePath();
	QFile file(userdir.absolutePath()+"/gpscon.gpx");
	QFile logfile(userdir.absolutePath()+"/gpscon.log");
	if (file.exists()) file.remove();
	if (logfile.exists()) logfile.remove();
}

