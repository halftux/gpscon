#ifndef MAINCLASS_H
#define MAINCLASS_H
#include <QObject>
#include <QCoreApplication>

class GpsPos;

class MainClass : public QObject
{
	Q_OBJECT
private:
	QCoreApplication *app;
	GpsPos *m_position;
	void deletelogfiles();

public:
	explicit MainClass(QObject *parent = 0);
	int g_runs;
	int g_dumps;
	bool g_lknv;
	bool g_sat;
	bool g_fout;
	bool g_date;
	bool g_time;
	int g_limit;
	int g_interval;
	int g_tout;
	bool g_del;
	bool g_newtrack;
	bool g_script;
	QString g_posm;
	QString g_log;



signals:
	/////////////////////////////////////////////////////////////
	/// Signal to finish, this is connected to Application Quit
	/////////////////////////////////////////////////////////////
	void finished();

public slots:
	/////////////////////////////////////////////////////////////
	/// Call this to quit application
	/////////////////////////////////////////////////////////////
	void quit();

	/////////////////////////////////////////////////////////////
	/// This is the slot that gets called from main to start everything
	/// but, everthing is set up in the Constructor
	/////////////////////////////////////////////////////////////
	void run();

	/////////////////////////////////////////////////////////////
	/// slot that get signal when that application is about to quit
	/////////////////////////////////////////////////////////////
	void aboutToQuitApp();
};
#endif // MAINCLASS_H
