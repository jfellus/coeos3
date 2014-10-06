/*
 * Component.cpp
 *
 *  Created on: 2 oct. 2014
 *      Author: jfellus
 */



#include "../widget/ZoomableDrawingArea.h"
#include "Component.h"
#include "BoundingBox.h"



void Component::repaint() {
	if(canvas) canvas->repaint();
}

void Component::select() {
	if(!selectionBox) selectionBox = new BoundingBox(this);
	if(!selected) canvas->add_selection(this);
	selected = true;
}

void Component::toggle_select() {
	if(selected) unselect();
	else select();
}

void Component::unselect() {
	if(selected) canvas->remove_selection(this);
	selected = false;
}

void Component::set_selectable(bool b) {
	if(b) canvas->add_selectable(this);
	else canvas->remove_selectable(this);
}

void Component::draw(Graphics& g) {
	g.cr->save();
	transform(g);
	if(canvas && canvas->selectionRenderingMode==SELECTION_Colored && is_selected())
		g.map_color(RGB_BLACK, RGB_RED);
	render(g);
		g.unmap_color(RGB_BLACK);
	g.cr->restore();
	if(canvas && canvas->selectionRenderingMode==SELECTION_ShowComponentsBoundingBoxes && is_selected())
		selectionBox->draw(g);
}
