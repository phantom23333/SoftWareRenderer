#include "rendererApp.h"
#include <QApplication>


int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	rendererApp w;
	w.setWindowTitle("Software Renderer");
	w.show();

	return a.exec();
}
