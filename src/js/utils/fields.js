

////////////
// FIELDS //
////////////

$.fn.extend({
		set_editable: function() {
			this.attr("contentEditable", "true");
			this.on('keydown', function(event){
				this.old = $(this).html();
				if(event.which==27) {this.blur();	event.preventDefault();		return;	}
				if(!$(this).hasClass("multiline")) {
					if(event.which==13) {this.blur();	event.preventDefault(); return;	}
					if(event.which==10) {this.blur();	event.preventDefault(); return;	}
				}
			});
			this.on('input', function(event){
				if($(this).hasClass("numeric")) {
					if (!$.isNumeric($(this).html())) {
						//alert($(this).old);
						$(this).html(this.old);
						event.preventDefault();
						return false;
					} 
				}
			});
			this.on('blur', function(event) {$(this).triggerHandler("change");});
			return this;
		}
});


function make_fields(elts) {
	elts.set_editable();
	elts.filter(".group").on('focus', function(event) {		showGroupsDropdown(this);	});
	elts.filter(".group").on('input', function(event) {		showGroupsDropdown(this);	});
	elts.filter(".group").on('blur', function(event) {		hideGroupsDropdown(this);	});
}

