
function include(script) {	$.getScript(script, function() {alert("loaded");}); }

var PORT;
$(function() {	__init(document.URL); });
function __init(url) {
	if(PORT) return;
	var s = "server_port=";
	PORT = parseInt(url.substr(url.indexOf(s)+s.length));
}




function __request(what, data) {
	if(!PORT) alert("WRONG PORT : ");
	var a = $.ajax({
	    url: "http://localhost:"+PORT+"/"+what,
	    beforeSend: function( xhr ) { xhr.overrideMimeType( "text/plain;" ); }, 
	    data: (data ? data : ""),
	    type: "POST", async: false,
	    dataType: "text"});
//	if(!a.responseText) alert("ERROR : null response for " + what + "(http://localhost:"+PORT+"/"+what+")" );
	return a.responseText;
}
