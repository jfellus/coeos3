/*
 * TagsForm.h
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#ifndef TagsForm_H_
#define TagsForm_H_

#include <widget/Browser.h>


using namespace libboiboites;
namespace coeos {


class TagsForm : public Browser {
public:
	TagsForm() : Browser("tags") {
		open("src/js/tags_form.html");
	}
	virtual ~TagsForm() {}


	virtual std::string answer(const std::string& request, const std::string& data);


	virtual void do_update() {
		if(bAnswering) return;
		if(is_loaded())	{
			script("update();");
		}
	}

	virtual void on_load() {update();}
};


}

#endif /* TagsForm_H_ */
