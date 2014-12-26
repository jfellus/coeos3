/*
 * Launcher.h
 *
 *  Created on: 17 nov. 2014
 *      Author: jfellus
 */

#ifndef LAUNCHER_H_
#define LAUNCHER_H_

#include "../PromProject.h"
#include "../promnet/PromNode.h"
#include <sys/stat.h>
#include "Compiler.h"


using namespace libboiboites;
namespace coeos {


class PromNodeThread {
public:
	class IRuntimeListener {
	public:
		IRuntimeListener() {}
		virtual ~IRuntimeListener() {}
		virtual void on_start(PromNodeThread* t) = 0;
		virtual void on_stop(PromNodeThread* t) = 0;
	};
public:
	pthread_t thread = 0;
	PromNode* node;

	bool bGui = false;

	bool bStarted = false;

	std::vector<IRuntimeListener*> runtime_listeners;

	PromNodeThread(PromNode* node, bool bGui = false);
	virtual ~PromNodeThread();

	void add_runtime_listener(IRuntimeListener* l) {runtime_listeners.push_back(l);}

	virtual void run();
};

class Launcher {
public:
	static std::vector<PromNodeThread*> threads;
public:

	static void start(PromProject* project, bool bGui = false);
	static void stop(PromProject* project);

	static void start(PromNode* node, bool bGui = false);
	static void stop(PromNode* node);

	static std::string get_stop_file(PromNode* node);
	static std::string get_start_file(PromNode* node);

protected:
	static void create_thread(PromNode* node, bool bGui = false);
};


}

#endif /* LAUNCHER_H_ */
