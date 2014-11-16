/*
 * PromNode.h
 *
 *  Created on: 6 nov. 2014
 *      Author: jfellus
 */

#ifndef PROMNODE_H_
#define PROMNODE_H_

#include <module/Property.h>
#include "../promscript/PromScript.h"

namespace xmlpp { class Element; }
class PromNet;

class PromNode : public IPropertiesElement {
public:
	PromNet* net = NULL;
	PromScript* script = NULL;
	std::string script_filename;
public:
	PromNode() {}
	PromNode(PromNet* net, xmlpp::Element* node) : net(net) { read(node);}
	PromNode(PromNet* net, PromScript* script) : net(net), script(script) {	init(script); }
	virtual ~PromNode();

	void read(xmlpp::Element* node);
	void write(xmlpp::Element* node);

	void realize();

	void init(PromScript* script);
};

#endif /* PROMNODE_H_ */
