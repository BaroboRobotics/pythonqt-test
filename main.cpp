#include "PythonQt.h"

#include <QCoreApplication>
#include <QDebug>

int main (int argc, char** argv) {
	QCoreApplication qapp(argc, argv);

	// init PythonQt and Python
	PythonQt::init();

	// get the __main__ python module
	auto mainModule = PythonQt::self()->getMainModule();

	// evaluate a simple python script and receive the result a qvariant:
	auto result = mainModule.evalScript("19*2+4", Py_eval_input);

	qDebug() << result;

	return 0;
}