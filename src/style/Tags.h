/*
 * Tags.h
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#ifndef TAGS_H_
#define TAGS_H_

#include "../modules/ModulePromGroup.h"
#include "../modules/LinkPromLink.h"

using namespace libboiboites;
namespace coeos {



class Tag {
public:
	std::string name;
	bool bEnabled = false;
	std::vector<Module*> modules;
	std::vector<Link*> links;
	bool bVisible = true;
	bool bLock = false;
	bool bBlur = false;

	std::string css_class = "";
public:
	Tag(const std::string& name) : name(name) {}
	virtual ~Tag() {}

	void add(Module* m);

	void remove(Module* m);

	void add(Link* l);


	void remove(Link* l);

	bool has(Link* l) {
		for(uint i=0; i<links.size(); i++) { if(links[i]==l) return true; }
		return false;
	}

	bool has(Module* l) {
		for(uint i=0; i<modules.size(); i++) { if(modules[i]==l) return true; }
		return false;
	}

	void on() {
		if(bEnabled) return;
		for(uint i=0; i<modules.size(); i++) modules[i]->add_class(css_class);
		for(uint i=0; i<links.size(); i++) links[i]->add_class(css_class);
		bEnabled = true;
	}

	void off() {
		if(!bEnabled) return;
		for(uint i=0; i<modules.size(); i++) modules[i]->remove_class(css_class);
		for(uint i=0; i<links.size(); i++) links[i]->remove_class(css_class);
		bEnabled = false;
	}

	void set_css_class(const std::string& css_class) {
		off();
		this->css_class = css_class;
		on();
	}

	void isolate();
	void unisolate();

	void show();
	void hide();

	void lock();
	void unlock();

	void blur();
	void unblur();
};


class Tags {
public:
	static Tag* isolated;
	static std::vector<Tag*> tags;
public:
	Tags();
	virtual ~Tags();

	static void add(Tag* tag) { tags.push_back(tag); }
	static void remove(Tag* tag) { vector_remove(tags,tag); }
	static void remove(const std::string& tagname) { vector_remove(tags,get(tagname)); }

	static Tag* get_by_class(const std::string& css) {
		for(uint i=0; i<tags.size(); i++) {
			if(tags[i]->css_class == css) return tags[i];
		}
		return NULL;
	}

	static std::string new_tagname() {
		for(uint i=1; ; i++) {
			std::string newname = TOSTRING("New Tag " << i);
			if(!get(newname)) return newname;
		}
		return "New Tag";
	}

	static Tag* get(const std::string& name) {
		for(uint i=0; i<tags.size(); i++) {
			if(tags[i]->name == name) return tags[i];
		}
		return NULL;
	}

	static void isolate(const std::string& tagname) {unisolate(); get(tagname)->isolate();}
	static void isolate(Tag* tag) {tag->isolate();}

	static void unisolate() {
		if(isolated) isolated->unisolate();
	}


	static void set_css(const std::string& tagname, const std::string& css_class) {
		Tag* t = get(tagname);
		if(!t) return;
		t->set_css_class(css_class);
	}

	static void update_isolated() {
		Tag* t = Tags::isolated;
		if(t) {
			unisolate();
			isolate(t);
		}
	}
};

}

#endif /* TAGS_H_ */
