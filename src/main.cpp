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

#include <QtCore/QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <QStringList>
#include "mainclass.h"
#include "qargparser.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	bool showhelp=false;

	// create the main class
	MainClass myMain;

	// connect up the signals
	QObject::connect(&myMain, SIGNAL(finished()),
	                 &app, SLOT(quit()));
	QObject::connect(&app, SIGNAL(aboutToQuit()),
	                 &myMain, SLOT(aboutToQuitApp()));

	//get arguments
	if (QArgByKey("runs",'=').toInt()!=0) myMain.g_runs = QArgByKey("runs",'=').toInt();
	else myMain.g_runs=-1;
	myMain.g_dumps = QArgByKey("dumps",'=').toInt();
	myMain.g_limit = QArgByKey("limit",'=').toInt();
	myMain.g_tout = QArgByKey("tout",'=').toInt();
	myMain.g_sat = qApp->arguments().contains("--sat");
	myMain.g_fout = qApp->arguments().contains("--fout");
	myMain.g_lknv = qApp->arguments().contains("--lknv");
	myMain.g_script = qApp->arguments().contains("--script");
	myMain.g_date = qApp->arguments().contains("--date");
	myMain.g_time = qApp->arguments().contains("--time");
	myMain.g_newtrack = qApp->arguments().contains("--newtrack");
	myMain.g_interval = QArgByKey("intval",'=').toInt();
	myMain.g_del = qApp->arguments().contains("--del");
	if (QArgByKey("log",'=').toLatin1()!="") myMain.g_log = QArgByKey("log",'=').toLatin1();
	else myMain.g_log = "none";
	if (QArgByKey("posm",'=').toLatin1()!="") myMain.g_posm = QArgByKey("posm",'=').toLatin1();
	else myMain.g_posm = "all";
	showhelp  = qApp->arguments().contains("--help");


	// This code will show the help and in
	// 10ms it will quit the MainClass routine;
	if (showhelp)
	{
		QTextStream cout(stdout);
		cout << endl;
		cout << "gpscon v0.0.12 by halftux" << endl << endl;
		cout << "Options:" << endl;
		cout << "--------" << endl;
		cout << "  [--help] help output" << endl;
		cout << "  [runs=integer] max update runs before quit" << endl;
		cout << "  [dumps=integer] ignoring first runs" << endl;
		cout << "  [limit=integer] accuracy vert/hori < limit 1x and quit" << endl;
		cout << "  [posm=string]Position method: nonesat / all / onlysat" << endl;
		cout << "  [interval=integer] update interval in s" << endl;
		cout << "  [log=string] txt/gpx/log2gpx" << endl;
		cout << "  [tout=integer] timeout to exit in s" << endl;
		cout << "  [--del] deleting log files" << endl;
		cout << "  [--sat] with satellite output" << endl;
		cout << "  [--fout] full position output" << endl;
		cout << "  [--date] date output" << endl;
		cout << "  [--time] time output" << endl;
		cout << "  [--newtrack] mark in log file" << endl;
		cout << "  [--lknv] output lastknownposition (qdebug)" << endl;
		cout << "  [--script] use the option for scripts (no user input)" << endl << endl;
		cout << "Without runs or limit option the app is in endless loop." << endl;
		cout << "With user input:" << endl;
		cout << "You can exit with: x+[enter]." << endl;
		cout << "You can mark a newtrack with: t+[enter]." << endl;
		cout << "You can mark a new track segment with: s+[enter]." << endl;

		QTimer::singleShot(10, &myMain, SLOT(quit()));
	}
	else
	{
		// This code will start the messaging engine in QT and in
		// 10ms it will start the execution in the MainClass.run routine;
		QTimer::singleShot(10, &myMain, SLOT(run()));
	}

	return app.exec();
}
