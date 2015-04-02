#include "PythonQt.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include <chrono>
#include <future>
#include <iostream>

// Runs an infinite loop and tries to stop the interpreter.
int main (int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "What Python file should I execute, master?\n";
		return 1;
	}

	QCoreApplication qapp(argc, argv);

	// init PythonQt and Python
	PythonQt::init();

	QObject::connect(PythonQt::self(), &PythonQt::pythonStdOut, [] (QString x) { std::cout << x.toStdString(); });
	QObject::connect(PythonQt::self(), &PythonQt::pythonStdErr, [] (QString x) { std::cerr << x.toStdString(); });
	QObject::connect(PythonQt::self(), &PythonQt::pythonHelpRequest, [] (QByteArray x) { std::cout << "** Help Request:" << x.toStdString() << '\n'; });

	QFile file {argv[1]};
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		std::cerr << "Unable to open file, master.\n";
		return 1;
	}
	QTextStream fileStream {&file};

	auto futureResult = std::async(std::launch::async, [&] {
		// get the __main__ python module
		auto mainModule = PythonQt::self()->getMainModule();
		mainModule.evalScript(fileStream.readAll(), Py_file_input);
	});

	if (std::future_status::timeout == futureResult.wait_for(std::chrono::seconds(1))) {
		PythonQt::self()->interrupt();
		if (std::future_status::timeout == futureResult.wait_for(std::chrono::seconds(1))) {
			PythonQt::self()->kill();
		}
	}

	futureResult.get();

	std::cout << "Python interrupt test successful!\n";

	return 0;
}