/*
 * Compiler.h
 *
 *  Created on: 21 déc. 2014
 *      Author: jfellus
 */

#ifndef COMPILER_H_
#define COMPILER_H_

#include "../promnet/PromNet.h"
#include "../promnet/PromNode.h"

namespace coeos {

class Compiler {
public:
	Compiler();
	virtual ~Compiler();

	static void compile(PromNode* node);

	static std::string get_makefile(PromNode* node);
	static void call_make(PromNode* node, const std::string& target);

protected:
	static void compile_config(PromNode* node);

	static void create_themis_makefile(PromNode* node);

};

}

#endif /* COMPILER_H_ */
