/* https://github.com/mrmoje/QArgParser

./myApp  firstKey=Value1 --secondKey Value2 thirdKey=val3.1,val3.2,val3.3 --enable-foo

QString param1   = QArgByKey("firstkey",'='); // Returns `Value1` from first pair
QString param2   = QArgByKey("--secondkey"); // Returns `Value2` from second pair
QString param3-1 = QArgByKey("thirdkey",'=').split(',').at(0); // Returns `val3.1`
bool fooEnabled  = qApp->arguments().contains("--enable-foo"); //To check for `--enable-foo`
*/

//#include <QApplication>
#include <QtCore/QCoreApplication>
#include <QStringList>
#include "qargparser.h"

QString QArgByKey(QString key, QChar sep )
{
	bool sepd=sep!=QChar('\0');
	int pos=sepd?qApp->arguments().indexOf(QRegExp('^'+key+sep+"\\S*")):qApp->arguments().indexOf(QRegExp(key));
	return pos==-1?QString::null:
	       (sepd?qApp->arguments().at(pos).split(sep).at(1):(++pos<qApp->arguments().size()?qApp->arguments().at(pos):QString::null));
}

