README for Apache Log Filter (logfilter)
========================================

INTRODUCTION
------------

logfilter is a quick and easy way to search your Apache combined logs.
logfilter can search specific fields for information you want.  Like
grep, logfilter will return matching records (lines in your log files)
to standard output.  You can use logfilter's output just like you
would use any other UNIX style program.

INSTALLATION
------------

logfilter is written in C.  It can be built using regular make commands
familiar to many UNIX programmers.

First, download the source file from http://github.com/openxtra/logfilter

Next, uncompress & untar the archive:

	tar -xzf logfilter-0.3.tar.gz

You should now have a directory named after the tarball.  Go into
that directory and type 'make'.  Then, as root, type 'make install'

Assuming everything went OK you should have a program called logfilter
in your /usr/local/bin directory.  Type 'logfilter' to see if it's in
your path and working correctly.

USAGE
-----

Each line in an Apache combined log represents a single interaction
with a client machine.  For each page requested by your browser will,
in all probabilty, require several interactions with the web server.

The following fields are present in an Apache combined log:

	+ client
	+ identity
	+ user ID
	+ time
	+ request method (like GET or POST)/ URI / protocol
	+ status code
	+ size
	+ user agent

Each of the above fields can be searched on by logfilter.  The request
field has been split up into its constituents.  More than one field
can be matched at a time.  To find all GET requests for /somepage.html
that returned a 200 status code, this will work:

	logfilter -f 'uri:/somepage.html method:GET status:200' some_log_file 

