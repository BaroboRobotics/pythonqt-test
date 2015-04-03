#include "baromesh/linkbot.hpp"

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
	PythonQt::init(PythonQt::RedirectStdOut);

	QObject::connect(PythonQt::self(), &PythonQt::pythonStdOut, [] (QString x) { std::cout << x.toStdString(); });
	QObject::connect(PythonQt::self(), &PythonQt::pythonStdErr, [] (QString x) { std::cerr << x.toStdString(); });
	QObject::connect(PythonQt::self(), &PythonQt::pythonHelpRequest, [] (QByteArray x) { std::cout << "** Help Request:" << x.toStdString() << '\n'; });

	QFile file {argv[1]};
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		std::cerr << "Unable to open file, master.\n";
		return 1;
	}
	auto script = QTextStream{&file}.readAll();

	auto nIterations = 10;
	for (auto i = 0; i < 10; ++i) {
		std::cout << "Running script, iteration " << i << "/" << nIterations << '\n';

		auto futureResult = std::async(std::launch::async, [&] {
			// get the __main__ python module
			auto mainModule = PythonQt::self()->getMainModule();
			mainModule.evalScript(script, Py_file_input);
		});

		Linkbot l { "1P2P" };
		l.setLedColor(0, 255, 0);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		l.setLedColor(0, 0, 255);

		if (std::future_status::timeout == futureResult.wait_for(std::chrono::seconds(10))) {
			std::cout << "Attempting to interrupt\n";
			PythonQt::self()->interrupt();
			if (std::future_status::timeout == futureResult.wait_for(std::chrono::seconds(10))) {
				std::cout << "Attempting to kill\n";
				PythonQt::self()->kill();
			}
		}

		futureResult.get();
	}

	std::cout << "Python interrupt test successful!\n";

	return 0;
}