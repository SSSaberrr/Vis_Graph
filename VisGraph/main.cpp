#include "VisGraph.h"
#include <QtWidgets/QApplication>
#include <QtOpenGL/QGLFormat>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	VisGraph w;
	w.show();

	return a.exec();
}