
///////////
// UTILS //
///////////

function isControlKey(e) {
	var keycode = e.keyCode;
	var valid = 
	    (keycode > 47 && keycode < 58)   || // number keys
	    keycode == 32 || keycode == 13   || // spacebar & return key(s) (if you want to allow carriage returns)
	    (keycode > 64 && keycode < 91)   || // letter keys
	    (keycode > 95 && keycode < 112)  || // numpad keys
	    (keycode > 185 && keycode < 193) || // ;=,-./` (in order)
	    (keycode > 218 && keycode < 223);   // [\]' (in order)
	return !valid;
}


if (typeof String.prototype.startsWith != 'function') {
	  String.prototype.startsWith = function (str){
	    return this.indexOf(str) == 0;
	  };
	}

if (typeof String.prototype.contains != 'function') {
	  String.prototype.contains = function (str){
	    return this.indexOf(str) !== -1;
	  };
	}

if (typeof String.prototype.after != 'function') {
	  String.prototype.after = function (str){
		  var i = this.indexOf(str);
		  if(i==-1) return "";
	    return this.substr(i+str.length);
	  };
	}

if (typeof String.prototype.before != 'function') {
	  String.prototype.before = function (str){
		  var i = this.indexOf(str);
		  if(i==-1) return this;
	    return this.substr(0,i);
	  };
	}

if (typeof String.prototype.between != 'function') {
	  String.prototype.between = function (prefix, suffix){
		  return this.after(prefix).before(suffix);
	  };
	}

if (typeof String.prototype.replaceAll != 'function') {
	  String.prototype.replaceAll = function (search, replace){
		  return this.replace(new RegExp(search, 'g'), replace);
	  };
	}

function uri_encode(s) {
	var ss = s.replaceAll("<br>", "|##|");
	var encoded = $("<div />").html(ss).text().replaceAll("\\|\\#\\#\\|", "\n");
	alert(escape(encoded));
	return escape(encoded);
}

function post_encode(s) {
	var ss = s.replaceAll("<br>", "|##|");
	var encoded = $("<div />").html(ss).text().replaceAll("\\|\\#\\#\\|", "\n");
	return encoded;
}
