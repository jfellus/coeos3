
/////////////////////
// GROUPS DROPDOWN //
/////////////////////

var GROUPS_LIST = [];
var cur_group_list_field = null;
var cur_group_list_index = -1;
var cur_group_list_field_is_hidden = true;

function fillGroupsDropdown() {
	var a = $("#groups_dropdown");
	a.hide();
	cur_group_list_field_is_hidden = true;
	a.addClass("dropdown");
	var list = $("<ul></ul>");
	for(var i = 0; i < GROUPS_LIST.length; i++) {
		list.append("<li>"+GROUPS_LIST[i]+"</li>");
	}
	a.append(list);
	$("#groups_dropdown li").click(function(e) {
		$(cur_group_list_field).text($(this).text());
		$(cur_group_list_field).triggerHandler("change");
		$(cur_group_list_field).blur();
		cur_group_list_field = null;
	});
	
	cur_group_list_callback = function(e) {
		if(e.which==40) {
			cur_group_list_index++;
			if(cur_group_list_index >= $("#groups_dropdown").find("li.available").length) cur_group_list_index = 0;
			$("#groups_dropdown").find("li").removeClass("selected");
			$("#groups_dropdown").find("li.available").eq(cur_group_list_index).addClass("selected");
			e.preventDefault();
		} else if(e.which==38) {
			cur_group_list_index--;
			if(cur_group_list_index <= -1) cur_group_list_index = $("#groups_dropdown").find("li.available").length-1;
			$("#groups_dropdown").find("li").removeClass("selected");
			$("#groups_dropdown").find("li.available").eq(cur_group_list_index).addClass("selected");
			e.preventDefault();
		} else if(e.which==13) {
			if($("#groups_dropdown li.selected").is(":visible"))
				$(cur_group_list_field).text($("#groups_dropdown li.selected").text());
			$(cur_group_list_field).triggerHandler("change");
			$(cur_group_list_field).blur();
			e.preventDefault();
		} else if(e.which==9) {
			if($("#groups_dropdown li.selected").is(":visible"))
				$(cur_group_list_field).text($("#groups_dropdown li.selected").text());
			$(cur_group_list_field).triggerHandler("change");
			$(cur_group_list_field).blur();
		}
	}
}

function filterGroupsDropdown(val) {
	$("#groups_dropdown .nothing").remove(); 
	$("#groups_dropdown > ul").children().each(function(i) {
		if(!$(this).text().toLowerCase().startsWith(val.toLowerCase())) {$(this).hide(); $(this).removeClass("available");}
		else {$(this).show(); $(this).addClass("available");}
	});
	if($("#groups_dropdown").find("li.available").length==0) {
		$("#groups_dropdown > ul").append("<li class='nothing'>No such group</li>");
	} else {
		$("#groups_dropdown").find("li").removeClass("selected");
		$("#groups_dropdown").find("li.available").eq(cur_group_list_index).addClass("selected");
	}
}

function showGroupsDropdown(e) {
	if($("#groups_dropdown").length==0) {
		$("body").append("<div id='groups_dropdown'></div>");
		fillGroupsDropdown();
	}
	
	cur_group_list_index = 0;
	filterGroupsDropdown($(e).text());

	var a = $("#groups_dropdown");
	var pos = $(e).offset();
	a.css({"top":pos.top + $(e).height(), "left":pos.left});
	a.css({"min-width":  $(e).width() + "px"});

	if(cur_group_list_field_is_hidden) {
		a.slideDown();
		$(e).keydown(cur_group_list_callback);
	}
	if(cur_group_list_field!=e) {
		if(cur_group_list_field!=null) $(cur_group_list_field).unbind("keydown");
	}
	
	cur_group_list_field = e;
	cur_group_list_field_is_hidden = false;
}

function hideGroupsDropdown(e) {
	$("#groups_dropdown").slideUp();
	$(cur_group_list_field).unbind("keydown", cur_group_list_callback);
	cur_group_list_field_is_hidden = true;
}


/////////////////////////
// LINK TYPES DROPDOWN //
/////////////////////////

var LINKS_LIST = [
          		"one to one modifiable",
        		"one to random (modifiable)",
        		"one to neighborhood (modifable)",
        		"one to one (NON modifiable)",
        		"one to all (modifiable)",
        		"algorithmic link (virtual)",
        		"one to one blocking (vig. control)",
        		"one to neighborhood (NON modifiable)",
        		"global neuro modulation link (virtual)"                  
];
var cur_link_list_field = null;
var cur_link_list_index = -1;
var cur_link_list_field_is_hidden = true;

function fillLinksDropdown() {
	var a = $("#links_dropdown");
	a.hide();
	cur_link_list_field_is_hidden = true;
	a.addClass("dropdown");
	var list = $("<ul></ul>");
	for(var i = 0; i < LINKS_LIST.length; i++) {
		list.append("<li>"+LINKS_LIST[i]+"</li>");
	}
	a.append(list);
	$("#links_dropdown li").click(function(e) {
		$(cur_link_list_field).text($(this).text());
		$(cur_link_list_field).triggerHandler("change");
		$(cur_link_list_field).blur();
		cur_link_list_field = null;
	});
	
	cur_link_list_callback = function(e) {
		if(e.which==40) {
			cur_link_list_index++;
			if(cur_link_list_index >= $("#links_dropdown").find("li.available").length) cur_link_list_index = 0;
			$("#links_dropdown").find("li").removeClass("selected");
			$("#links_dropdown").find("li.available").eq(cur_link_list_index).addClass("selected");
			e.preventDefault();
		} else if(e.which==38) {
			cur_link_list_index--;
			if(cur_link_list_index <= -1) cur_link_list_index = $("#links_dropdown").find("li.available").length-1;
			$("#links_dropdown").find("li").removeClass("selected");
			$("#links_dropdown").find("li.available").eq(cur_link_list_index).addClass("selected");
			e.preventDefault();
		} else if(e.which==13) {
			if($("#links_dropdown li.selected").is(":visible"))
				$(cur_link_list_field).text($("#links_dropdown li.selected").text());
			$(cur_link_list_field).triggerHandler("change");
			$(cur_link_list_field).blur();
			e.preventDefault();
		} else if(e.which==9) {
			if($("#links_dropdown li.selected").is(":visible"))
				$(cur_link_list_field).text($("#links_dropdown li.selected").text());
			$(cur_link_list_field).triggerHandler("change");
			$(cur_link_list_field).blur();
		}
	}
}

function filterLinksDropdown(val) {
	$("#links_dropdown .nothing").remove(); 
	$("#links_dropdown > ul").children().each(function(i) {
		if(!$(this).text().toLowerCase().startsWith(val.toLowerCase())) {$(this).hide(); $(this).removeClass("available");}
		else {$(this).show(); $(this).addClass("available");}
	});
	if($("#links_dropdown").find("li.available").length==0) {
		$("#links_dropdown > ul").append("<li class='nothing'>No such link</li>");
	} else {
		$("#links_dropdown").find("li").removeClass("selected");
		$("#links_dropdown").find("li.available").eq(cur_link_list_index).addClass("selected");
	}
}

function showLinksDropdown(e) {
	if($("#links_dropdown").length==0) {
		$("body").append("<div id='links_dropdown'></div>");
		fillLinksDropdown();
	}
	
	cur_link_list_index = 0;
	filterLinksDropdown($(e).text());

	var a = $("#links_dropdown");
	var pos = $(e).offset();
	a.css({"top":pos.top + $(e).height(), "left":pos.left});
	a.css({"min-width":  $(e).width() + "px"});

	if(cur_link_list_field_is_hidden) {
		a.slideDown();
		$(e).keydown(cur_link_list_callback);
	}
	if(cur_link_list_field!=e) {
		if(cur_link_list_field!=null) $(cur_link_list_field).unbind("keydown");
	}
	
	cur_link_list_field = e;
	cur_link_list_field_is_hidden = false;
}

function hideLinksDropdown(e) {
	$("#links_dropdown").slideUp();
	$(cur_link_list_field).unbind("keydown", cur_link_list_callback);
	cur_link_list_field_is_hidden = true;
}