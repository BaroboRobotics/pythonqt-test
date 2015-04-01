#include "PythonQt.h"

#include <QCoreApplication>
#include <QDebug>

#include <chrono>
#include <future>
#include <iostream>

// Runs an infinite loop and terminates abnormally.
int main (int argc, char** argv) {
	QCoreApplication qapp(argc, argv);

	// init PythonQt and Python
	PythonQt::init();

	QObject::connect(PythonQt::self(), &PythonQt::pythonStdOut, [] (QString x) { std::cout << x.toStdString(); });
	QObject::connect(PythonQt::self(), &PythonQt::pythonStdErr, [] (QString x) { std::cerr << x.toStdString(); });
	QObject::connect(PythonQt::self(), &PythonQt::pythonHelpRequest, [] (QByteArray x) { std::cout << "** Help Request:" << x.toStdString() << '\n'; });

	// get the __main__ python module
	auto mainModule = PythonQt::self()->getMainModule();

	auto futureResult = std::async(std::launch::async, [&] {
		return mainModule.evalScript("while 1: pass"/*print(\"Hodor\")"*/, Py_file_input);
	});

	if (std::future_status::timeout == futureResult.wait_for(std::chrono::seconds(1))) {
		// We want to do this instead of terminate: mainModule.abort();
		// (or .cancel() or .stop())

		std::terminate();

		// this naive attempt predictably crashes:
		// qDebug() << mainModule.evalScript("import sys; sys.exit()", Py_file_input);

	}

	qDebug() << futureResult.get();

	return 0;
}