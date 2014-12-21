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

using namespace libboiboites;
namespace xmlpp { class Element; }

namespace coeos {


class PromNet;

class PromNode : public IPropertiesElement, IPropertiesListener {
public:
	PromNet* net = NULL;
	PromScript* script = NULL;

	std::string path_symb;
	std::string path_script;
	std::string path_draw;
	std::string path_res;
	std::string path_config;
	std::string path_gcd;
	std::string path_dev;
	std::string path_prt;
	std::string path_deploy;

	bool bEmbedded = false;

	bool started = false;

public:
	PromNode(PromNet* net, PromScript* script) : net(net), script(script) {	init(script); }
	virtual ~PromNode();

	void read(xmlpp::Element* node);
	void write(xmlpp::Element* node);


	std::string get_filename();
	std::string get_absolute_filename();
	std::string get_absolute_path(const std::string& filename);

	void set_script_name(const std::string& name);


	void realize();

	void init(PromScript* script);

	virtual void on_property_change(IPropertiesElement* m, const std::string& name, const std::string& val);
};


}

#endif /* PROMNODE_H_ */
