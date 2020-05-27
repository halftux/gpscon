// source http://mirror.thecust.net/meego_1.2_harmattan_developer_documentation/html/qtmobility/location-overview.html?tab=0#location-positioning
// gpx 1.1 https://de.wikipedia.org/wiki/GPS_Exchange_Format
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
#include "consolereader.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QXmlStreamWriter>

GpsPos::GpsPos(QObject *parent) :
	QObject(parent)
{
#if defined(Q_OS_SAILFISH)
	qRegisterMetaType<QGeoPositionInfo>("QGeoPositionInfo");
#endif
	//QGeoPositionInfoSource *source = QGeoPositionInfoSource::createDefaultSource(this);
	m_position = QGeoPositionInfoSource::createDefaultSource(this);
	if (m_position)
	{
		connect(m_position, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
		m_position->startUpdates();
	}
}
//call for convert
GpsPos::GpsPos(QObject *parent, QString logmethod) :
	QObject(parent)
{
	Q_UNUSED(logmethod);
	writegpxdata();
}
//call for last known value
GpsPos::GpsPos(QObject *parent, bool fout, QString posm, bool date, bool time, int dshift) :
	QObject(parent)
{
#if defined(Q_OS_SAILFISH)
	qRegisterMetaType<QGeoPositionInfo>("QGeoPositionInfo");
#endif
	b_date=date;
	b_time=time;
	g_dshift=dshift;
	//QGeoPositionInfoSource *source = QGeoPositionInfoSource::createDefaultSource(this);
	m_position = QGeoPositionInfoSource::createDefaultSource(this);
	if (m_position)
	{
		connect(m_position, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
		bool satonly;
		if (posm=="onlysat") satonly=true;
		else satonly=false;
		//qDebug() << "lastknownposition (gps/satonly): " << source->lastKnownPosition(true);
		QGeoPositionInfo last = m_position->lastKnownPosition(satonly);
		if (fout) qDebug() << last;
		else lastknownposition(last);
	}
}
// call for loop
GpsPos::GpsPos(QObject *parent, int runs, int limit, int dumps, bool lastkv, bool showsat, bool fout, QString posm, int interval, bool date, bool time, QString logmethod, bool newtrack, bool script, int tout, int dshift) :
	QObject(parent)
{
#if defined(Q_OS_SAILFISH)
	qRegisterMetaType<QGeoPositionInfo>("QGeoPositionInfo");
#endif
	g_run=0;
	g_dumps=dumps;
	g_runs=runs;
	g_showsat=showsat;
	g_dshift=dshift;
	full_out=fout;
	limit_acc=limit;
	log_method=logmethod;
	b_date=date;
	b_time=time;
	timeout_value=tout;
	if (timeout_value > 0)
	{
		timeout_value=timeout_value*1000;
		timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
		timer->start(timeout_value);
	}
	if (!script)
	{
		m_console = new ConsoleReader(this);
		connect(m_console, SIGNAL(textReceived(QString)), this, SLOT(quit(QString)));
	}

	if (newtrack) writelogfile("newtrack");

	//QGeoPositionInfoSource *source = QGeoPositionInfoSource::createDefaultSource(this);
	m_position = QGeoPositionInfoSource::createDefaultSource(this);
	if (interval!=0) m_position->setUpdateInterval(interval*1000);
	if (posm=="onlysat") m_position->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
	else if (posm=="nonesat") m_position->setPreferredPositioningMethods(QGeoPositionInfoSource::NonSatellitePositioningMethods);
	else if (posm=="all") m_position->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
	if (m_position)
	{
		//qDebug() << source->supportedPositioningMethods();
		//connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)),static_cast<Qt::ConnectionType>(Qt::DirectConnection | Qt::UniqueConnection));
		connect(m_position, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
		if (lastkv)
		{
			qDebug() << "lastknownposition: " << m_position->lastKnownPosition(false);
		}
		m_position->startUpdates();
	}
	if (showsat && posm!="nonesat")
	{
		//QGeoSatelliteInfoSource *sats = QGeoSatelliteInfoSource::createDefaultSource(this);
		m_satellite = QGeoSatelliteInfoSource::createDefaultSource(this);
#if defined(Q_OS_SAILFISH)
		if (interval!=0) m_satellite->setUpdateInterval(interval*1000);
#endif
		// Listen satellite status changes
		if (m_satellite != 0)
		{
			connect(m_satellite, SIGNAL(satellitesInViewUpdated(const QList<QGeoSatelliteInfo>&)), this, SLOT(satellitesInViewUpdated(const QList<QGeoSatelliteInfo>&)));
			connect(m_satellite, SIGNAL(satellitesInUseUpdated(const QList<QGeoSatelliteInfo>&)), this, SLOT(satellitesInUseUpdated(const QList<QGeoSatelliteInfo>&)));
			m_satellite->startUpdates();
		}
	}
}

void GpsPos::satellitesInViewUpdated(const QList<QGeoSatelliteInfo> &satellites)
{
	m_satellitesInView = satellites.count();
	viewSatelliteInfo();
}

void GpsPos::satellitesInUseUpdated(const QList<QGeoSatelliteInfo> &satellites)
{
	m_satellitesUsed = satellites.count();
	viewSatelliteInfo();
}

void GpsPos::viewSatelliteInfo()
{
	QTextStream cout(stdout);
	cout << "Using " << m_satellitesUsed << " of " << m_satellitesInView << " satellites" << endl;
}

void GpsPos::positionUpdated(const QGeoPositionInfo &info)
{
	if (info.isValid())
	{
		qreal horizontalacc = info.attribute(QGeoPositionInfo::HorizontalAccuracy);
		qreal verticalacc = info.attribute(QGeoPositionInfo::VerticalAccuracy);
		QTextStream cout(stdout);
		if (timeout_value>0)
		{
			timer->stop();
			timer->start();
		}

		g_run++;

		if (g_run>g_dumps)
		{
			if ((limit_acc==0) || ((verticalacc < limit_acc) && (horizontalacc < limit_acc)))
			{
				QDateTime date_time = info.timestamp();
				QDate date_v = date_time.date();
				QTime time_v = date_time.time();
				QString s_time = time_v.toString(Qt::TextDate);
				QGeoCoordinate coord = info.coordinate();
				double d_latitude = coord.latitude();
				double d_longitude = coord.longitude();
				double d_altitude = coord.altitude();
				QString s_latitude = QString::number(d_latitude);
				QString s_longitude = QString::number(d_longitude);
				QString s_altitude = QString::number(d_altitude);
				qreal direction = info.attribute(QGeoPositionInfo::Direction);
				qreal grspeed = info.attribute(QGeoPositionInfo::GroundSpeed);
				qreal vertspeed = info.attribute(QGeoPositionInfo::VerticalSpeed);
				QString s_direction = QString::number(direction);
				QString s_grspeed = QString::number(grspeed);
				QString s_vertspeed = QString::number(vertspeed);
				QString s_hacc = QString::number(horizontalacc);
				QString s_vacc = QString::number(verticalacc);

				if (g_dshift!=0)
				{
					qint64 date_number = date_v.toJulianDay();
					date_number = date_number + g_dshift;
					date_v = QDate::fromJulianDay(date_number);
				}
				QString s_date = date_v.toString(Qt::ISODate);

				if (full_out)
				{
					//qDebug() << "Position updated: " << info << " run: "<< g_run;
					if (b_date || b_time)
					{
						if (b_date) cout << "Date: " << s_date << "; ";
						if (b_time) cout << "Time: " << s_time;
						cout << endl;
					}
					cout << "Latitude: " << s_latitude << "; Longitude: " << s_longitude << "; Altitude: " << s_altitude << endl;
					cout << "Direction: " << s_direction << "; Ground Speed: " << s_grspeed << "; Vertical Speed: " << s_vertspeed << endl;
					cout << "Horizontal Acc: " << s_hacc << "; Vertical Acc: " << s_vacc << "; run: "<< g_run << endl << endl;
				}
				else
				{
					if (b_date || b_time)
					{
						if (b_date) cout << "Date: " << s_date << "; ";
						if (b_time) cout << "Time: " << s_time;
						cout << endl;
					}
					cout << "Latitude: " << s_latitude << "; Longitude: " << s_longitude << "; Altitude: " << s_altitude << endl;
				}
				if (log_method=="txt" || log_method=="gpx")
				{
					QString sline;
					sline.append("Date: " + s_date + "; Time: " + s_time + "; Latitude: " + s_latitude + "; Longitude: " + s_longitude + "; Altitude: " + s_altitude + "; Direction: " + s_direction + "; Ground Speed: " + s_grspeed + "; Vertical Speed: " + s_vertspeed + "; Horizontal Acc: " + s_hacc + "; Vertical Acc: " + s_vacc);
					writelogfile(sline);
				}
				if (limit_acc!=0)
				{
					quit("x");
				}
			}
		}
		if (g_run==g_runs+g_dumps)
		{
			quit("x");
		}
	}
}

void GpsPos::lastknownposition(const QGeoPositionInfo &lastpos)
{
	QTextStream cout(stdout);
	if (lastpos.isValid())
	{
		if (b_date || b_time)
		{
			QDateTime date_time = lastpos.timestamp();
			qDebug() << date_time;
			QDate date_v = date_time.date();
			QTime time_v = date_time.time();
			if (g_dshift!=0)
			{
				qint64 date_number = date_v.toJulianDay();
				date_number = date_number + g_dshift;
				date_v = QDate::fromJulianDay(date_number);
			}
			QString s_date = date_v.toString();
			QString s_time = time_v.toString();
			if (b_date) cout << "Date: " << s_date << "; ";
			if (b_time) cout << "Time: " << s_time;
			cout << endl;
		}
		QGeoCoordinate coord = lastpos.coordinate();
		double d_latitude = coord.latitude();
		double d_longitude = coord.longitude();
		double d_altitude = coord.altitude();
		QString s_latitude = QString::number(d_latitude);
		QString s_longitude = QString::number(d_longitude);
		QString s_altitude = QString::number(d_altitude);
		cout << "Latitude: " << s_latitude << "; Longitude: " << s_longitude << "; Altitude: " << s_altitude << endl;
	}
}

void GpsPos::writelogfile(QString line)
{
	QDir userdir=QDir::homePath();
	//QString homePath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
	//qDebug() << "savepath:" << userdir;
	QFile file(userdir.absolutePath()+"/gpscon.log");

	if(!file.open(QIODevice::Append | QIODevice::Text))
	{
		qDebug() << file.errorString();
	}
	QTextStream out(&file);
	out << line << endl;
	//qDebug() << line;
	file.close();
}

void GpsPos::writegpxdata()
{
	QTextStream cout(stdout);
	int tracknr=1;

	QDir userdir=QDir::homePath();
	QFile file(userdir.absolutePath()+"/gpscon.gpx");
	QFile logfile(userdir.absolutePath()+"/gpscon.log");
	if (!file.open(QIODevice::WriteOnly))
	{
		qDebug() << file.errorString();
		return;
	}
	if (!logfile.open(QIODevice::ReadOnly))
	{
		qDebug() << "no log file";
		return;
	}
	qDebug() << "writing gpx file: " << file.fileName() + " ...";
	int tloglines=0;
	int tgpxlines=0;
	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("gpx");
	xmlWriter.writeAttribute("version","1.1");
	xmlWriter.writeAttribute("creator","gpscon");
	xmlWriter.writeStartElement("trk");
	xmlWriter.writeStartElement("name");
	xmlWriter.writeCharacters("Track" + QString::number(tracknr));
	xmlWriter.writeEndElement();
	xmlWriter.writeStartElement("trkseg");
	QTextStream in(&logfile);

	while(!in.atEnd())
	{
		tloglines++;
		QString line = in.readLine();
		if (line=="newtrack" || line=="newseg")
		{
			if (tloglines > 1 && line=="newtrack")
			{
				tracknr++;
				xmlWriter.writeEndElement();
				xmlWriter.writeEndElement();
				xmlWriter.writeStartElement("trk");
				xmlWriter.writeStartElement("name");
				xmlWriter.writeCharacters("Track" + QString::number(tracknr));
				xmlWriter.writeEndElement();
				xmlWriter.writeStartElement("trkseg");
			}
			else if (tloglines > 1 && line=="newseg")
			{
				xmlWriter.writeEndElement();
				xmlWriter.writeStartElement("trkseg");
			}
		}
		else
		{
			QStringList value = line.split(QRegExp(";"));
			QString lat=value.at(2);
			int k=lat.indexOf(":");
			lat=lat.right(lat.count()-(k+2));
			QString lon=value.at(3);
			k=lon.indexOf(":");
			lon=lon.right(lon.count()-(k+2));
			QString ele=value.at(4);
			k=ele.indexOf(":");
			ele=ele.right(ele.count()-(k+2));
			QString date=value.at(0);
			k=date.indexOf(":");
			date=date.right(date.count()-(k+2));
			QString time=value.at(1);
			k=time.indexOf(":");
			time=time.right(time.count()-(k+2));
			QString time_gpx= date + "T" + time + "Z";
			if (ele!="nan")
			{
				tgpxlines++;
				if (tgpxlines > 1) update(QString::number(tgpxlines) + "/" + QString::number(tloglines));
				else cout << tgpxlines << "/" << tloglines;
				xmlWriter.writeStartElement("trkpt");
				xmlWriter.writeAttribute("lat",lat);
				xmlWriter.writeAttribute("lon",lon);
				xmlWriter.writeStartElement("ele");
				xmlWriter.writeCharacters(ele);
				xmlWriter.writeEndElement();
				xmlWriter.writeStartElement("time");
				xmlWriter.writeCharacters(time_gpx);
				xmlWriter.writeEndElement();
				xmlWriter.writeEndElement(); //closing track point
			}
		}
	}
	xmlWriter.writeEndElement(); //closing segment
	xmlWriter.writeEndElement(); //closing track
	xmlWriter.writeEndDocument();
	logfile.close();
	file.close();
	cout << "\33[2K\r";
	cout << tgpxlines << "/" << tloglines << endl;
	qDebug() << "..finished, " << tracknr << "Tracks found.";
}

void GpsPos::timeout()
{
	qDebug() << "timeout from user input reached";
	quit("x");
}

void GpsPos::quit(QString input)
{
	//qDebug() << "input:" << input << ";test";
	if (input=="x")
	{
		if (g_showsat)
		{
			m_satellite->stopUpdates();
			//delete m_satellite;
		}
		m_position->stopUpdates();
		//delete m_position;
		if (log_method=="gpx") writegpxdata();
		emit finished();
	}
	else if (input=="t")
	{
		writelogfile("newtrack");
	}
	else if (input=="s")
	{
		writelogfile("newseg");
	}
}

void GpsPos::update(QString status)
{
	QTextStream cout(stdout);
	cout << "\33[2K\r";
	cout << status;
}
