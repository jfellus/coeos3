
//////////////
// REQUESTS //
//////////////

function request_set_property(property, value) {
	return __request("properties/set/"+property,value);
}
