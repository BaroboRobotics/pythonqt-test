#include "PythonQt.h"

#include <QCoreApplication>
#include <QDebug>

#include <chrono>
#include <future>
#include <iostream>

// Runs an infinite loop and tries to stop the interpreter.
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
		return mainModule.evalScript(
"while 1:\n"
"\ttry:\n"
"\t\twhile 1:\n"
"\t\t\tpass\n"
"\texcept:\n"
"\t\tpass", Py_file_input);
	});

	if (std::future_status::timeout == futureResult.wait_for(std::chrono::seconds(1))) {
		PythonQt::self()->setInterrupt();
	}

	qDebug() << futureResult.get();

	return 0;
}