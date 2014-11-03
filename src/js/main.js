
function include(script) {	$.getScript(script, function() {alert("loaded");}); }

var PORT;
$(function() {	__init(document.URL); });
function __init(url) {
	var s = "server_port=";
	PORT = parseInt(url.substr(url.indexOf(s)+s.length));
}
