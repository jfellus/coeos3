

////////////
// FIELDS //
////////////

function make_fields(elts) {
	elts.on('keydown', function(event){
		this.old = $(this).html();
		if(event.which==27) {this.blur();	event.preventDefault();		return;	}
		if(!$(this).hasClass("multiline")) {
			if(event.which==13) {this.blur();	event.preventDefault(); return;	}
		}
	});
	elts.on('input', function(event){
		if($(this).hasClass("numeric")) {
			if (!$.isNumeric($(this).html())) {
				//alert($(this).old);
				$(this).html(this.old);
				event.preventDefault();
				return false;
			} 
		}
	});
	elts.on('blur', function(event) {$(this).triggerHandler("change");});
	
	elts.filter(".group").on('focus', function(event) {		showGroupsDropdown(this);	});
	elts.filter(".group").on('input', function(event) {		showGroupsDropdown(this);	});
	elts.filter(".group").on('blur', function(event) {		hideGroupsDropdown(this);	});
}

