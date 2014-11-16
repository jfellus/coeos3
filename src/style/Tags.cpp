/*
 * Tags.cpp
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#include "Tags.h"
#include <module/Document.h>

Tag* Tags::isolated = NULL;
std::vector<Tag*> Tags::tags;



void Tag::isolate() {
	for(uint i=0; i<Document::cur()->modules.size(); i++) {
		Module* m = Document::cur()->modules[i];
		m->remove_class("isolated");
		m->add_class("nonisolated");
		m->lock();
	}
	for(uint i=0; i<Document::cur()->links.size(); i++) {
		Link* l = Document::cur()->links[i];
		l->remove_class("isolated");
		l->add_class("nonisolated");
		l->lock();
	}
	for(uint i=0; i<modules.size(); i++) {
		Module* m = modules[i];
		m->remove_class("nonisolated");
		m->add_class("isolated");
		m->unlock();
	}
	for(uint i=0; i<links.size(); i++) {
		Link* l = links[i];
		l->remove_class("nonisolated");
		l->add_class("isolated");
		l->unlock();
	}
	Tags::isolated = this;
}

void Tag::unisolate() {
	for(uint i=0; i<Document::cur()->modules.size(); i++) {
		Module* m = Document::cur()->modules[i];
		m->remove_class("isolated");
		m->remove_class("nonisolated");
		m->unlock();
	}
	for(uint i=0; i<Document::cur()->links.size(); i++) {
		Link* l = Document::cur()->links[i];
		l->remove_class("isolated");
		l->remove_class("nonisolated");
		l->unlock();
	}
	Tags::isolated = NULL;
}

void Tag::show() {
	if(bVisible) return;
	for(uint i=0; i<modules.size(); i++) modules[i]->show();
	for(uint i=0; i<links.size(); i++) links[i]->show();
	bVisible = true;
}

void Tag::hide() {
	if(!bVisible) return;
	for(uint i=0; i<modules.size(); i++) modules[i]->hide();
	for(uint i=0; i<links.size(); i++) links[i]->hide();
	bVisible = false;
}

void Tag::lock() {
	if(bLock) return;
	for(uint i=0; i<modules.size(); i++) modules[i]->lock();
	for(uint i=0; i<links.size(); i++) links[i]->lock();
	bLock = true;
}

void Tag::unlock() {
	if(!bLock) return;
	for(uint i=0; i<modules.size(); i++) modules[i]->unlock();
	for(uint i=0; i<links.size(); i++) links[i]->unlock();
	bLock = false;
}


void Tag::add(Module* m) {
	if(has(m)) return;
	modules.push_back(m);
	m->add_class(css_class);
	Tags::update_isolated();
}

void Tag::remove(Module* m) {
	vector_remove(modules, m);
	m->remove_class(css_class);
	Tags::update_isolated();
}

void Tag::add(Link* l) {
	if(has(l)) return;
	links.push_back(l);
	l->add_class(css_class);
	Tags::update_isolated();
}


void Tag::remove(Link* l) {
	vector_remove(links, l);
	l->remove_class(css_class);
	Tags::update_isolated();
}

