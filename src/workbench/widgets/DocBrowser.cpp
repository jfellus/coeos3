/*
 * DocBrowser.cpp
 *
 *  Created on: 24 oct. 2014
 *      Author: jfellus
 */

#include "DocBrowser.h"


using namespace libboiboites;
namespace coeos {

DocBrowser::DocBrowser() : Browser("doc") {

}

DocBrowser::~DocBrowser() {
	// TODO Auto-generated destructor stub
}

int _show_only_main_frame(void* p) {
	DocBrowser* b = (DocBrowser*)p;
	b->script("{document.getElementById('top').style.display='none';document.getElementById('side-nav').style.display='none';document.getElementById('doc-content').style.marginLeft=0;document.getElementById('doc-content').style.height='100%';}");
	return FALSE;
}
void DocBrowser::show_only_main_frame() {
	g_timeout_add(50, _show_only_main_frame, this);
}

}
