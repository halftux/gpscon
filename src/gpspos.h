#ifndef GPSPOS_H
#define GPSPOS_H
#if defined(Q_OS_SAILFISH)
#include <QtPositioning/QGeoPositionInfoSource>
#include <QtPositioning/QGeoPositionInfo>
#include <QtPositioning/QGeoSatelliteInfoSource>
#include <QtPositioning/QGeoSatelliteInfo>
#else
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
#include <QGeoSatelliteInfoSource>
#include <QGeoSatelliteInfo>
#endif
#include <QObject>
#include <QTimer>

#if !defined(Q_OS_SAILFISH)
using namespace QtMobility;
#endif
class ConsoleReader;
class GpsPos : public QObject
{
	Q_OBJECT
public:
	GpsPos(QObject *parent = 0);
	GpsPos(QObject *parent = 0, QString logmethod="none");
	GpsPos(QObject *parent = 0, bool fout=true, QString posm="all", bool date=false, bool time=false);
	GpsPos(QObject *parent = 0, int runs=1, int limit=0, int dumps=0, bool lastkv=false, bool showsat=false, bool fout=false, QString posm="onlysat", int interval=0, bool date=false, bool time=false, QString logmethod="none", bool newtrack=false, bool script=true, int tout=0);
	int g_run;
	int g_runs;
	int g_dumps;
	bool g_showsat;
	QTimer *timer;

signals:
	void finished();

public slots:
	void positionUpdated(const QGeoPositionInfo &info);
	void satellitesInViewUpdated(const QList<QGeoSatelliteInfo> &satellites);
	void satellitesInUseUpdated(const QList<QGeoSatelliteInfo> &satellites);
	void viewSatelliteInfo();
	void quit(QString input);
	void update(QString status);
	void timeout();
private:
	void lastknownposition(const QGeoPositionInfo &lastpos);
	void writelogfile(QString line);
	void writegpxdata();
	int full_out;
	int limit_acc;
	int timeout_value;
	bool b_date;
	bool b_time;
	int m_satellitesInView;
	int m_satellitesUsed;
	QString log_method;
	//QGeoPositionInfoSource* source;
	//QGeoSatelliteInfoSource* sats;
	QGeoPositionInfoSource *m_position;
	QGeoSatelliteInfoSource *m_satellite;
	ConsoleReader *m_console;
};

#endif // GPSPOS_H

