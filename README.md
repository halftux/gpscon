# GPSCON

Gpscon is a command line tool to acquire the gps position with the qtlocation api.
It runs on Maemo, MeeGo and Sailfish
To compile for sailfish you need to add a config parameter to qmake when generating the makefile.
qmake gpscon.pro -r -spec linux-g++ "CONFIG+=sailfish"

without parameter gpscon will be in endless loop and print to stdout: Latitude; Longitude; Altitude
The application exits with:x+[enter]
t+[enter]: will mark a new track in the logfile.

[--fout]
will enhance the output with additional values: Direction; Ground Speed; Vertical Speed;
Horizontal Acc;Vertical Acc

[--sat]
with this option also the satellite status will be requested and printed out
"Using x of x satellites"

[posm=string]
this will select the positioning method. Default is all.
string=all |AllPositioningMethods
string=onlysat |SatellitePositioningMethods
string=nonesat |NonSatellitePositioningMethods

[interval=integer]
This property holds the requested interval in milliseconds between each update.
If the update interval is not set (or is set to 0) the source will provide updates as often as necessary.
If the update interval is set, the source will provide updates at an interval as close to the requested interval as possible. If the requested interval is less than the minimumUpdateInterval(), the minimum interval is used instead.

[runs=integer]
sets the amount on how many (shown) requests are made before exit

[dumps=integer]
sets the amount on how many first requests get dumped

[limit=integer]
sets the limit of the needed horizontal/vertical accuracy value before print out.
There will be only one print out and after this the app is closing

[log=string]
writing to a log file
string=txt |write/append gpscon.log to user dir
string=gpx |write/append gpscon.log to user dir after last run or exit, the log file will be converted and written to gpscon.gpx in user dir
string=log2gpx |convert log to gpx and exit.

[tout=integer]
sets the timeout for aquire a gps position in seconds.

[--del]
If log file exists it will be removed

[--date]
adds date to output

[--time]
adds time to output

[--newtrack]
will mark in the log file a new track. When converting to gpx old log file data and new data will be visible as separate track.

[--lknv]
This should output the last known value once at first (debug). If runs=1 it will only print the last known value out and quit.

[--script]
Will deactivate the user input which causes trouble with cron or python scripts. This option is recommended when using gpscon with scripts.

[--help]
prints help

 [gpscon @ maemo.org](http://talk.maemo.org/showthread.php?t=96176).
