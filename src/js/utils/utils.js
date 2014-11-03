
//////////////
// REQUESTS //
//////////////

function __request(what, data) {
	return $.ajax({
	    url: "http://localhost:"+PORT+"/"+what,
	    beforeSend: function( xhr ) { xhr.overrideMimeType( "text/plain;" ); }, 
	    data: data ? data : "-",
	    type: "POST", async: false,
	    dataType: "text"}).responseText;
}

function request_set_property(property, value) {
	return __request("properties/set/"+property,value);
}
