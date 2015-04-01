#include "PythonQt.h"

#include <QCoreApplication>
#include <QDebug>

#include <iostream>

int main (int argc, char** argv) {
	QCoreApplication qapp(argc, argv);

	// init PythonQt and Python
	PythonQt::init();

	QObject::connect(PythonQt::self(), &PythonQt::pythonStdOut, [] (QString x) { std::cout << x.toStdString(); });
	QObject::connect(PythonQt::self(), &PythonQt::pythonStdErr, [] (QString x) { std::cerr << x.toStdString(); });
	QObject::connect(PythonQt::self(), &PythonQt::pythonHelpRequest, [] (QByteArray x) { std::cout << "** Help Request:" << x.toStdString() << '\n'; });

	// get the __main__ python module
	auto mainModule = PythonQt::self()->getMainModule();

	// evaluate a simple python script and receive the result a qvariant:
	auto result = mainModule.evalScript("while 1: print(\"Hodor\")", Py_file_input);

	qDebug() << result;

	return 0;
}