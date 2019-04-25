//Array for storing usernames (Dummy array until MySQL is utilized)
var usernameArray = ["david_mcclarty", "valentina_musat"];

// Displays the menu when clicking the menu icon and hides it when clicking again
function toggleMenu(){
    var x = document.getElementsByClassName("nav-menu")[0];
	if(x.style.display === "flex"){
		x.style.display = "";
	} else {
		x.style.display = "flex";
	}
}


// Displays the message box for password guidelines when clicking the password input box and hides it when click outside
function toggleMessageBox(){
	var x = document.getElementsByClassName("message-bubble")[0];
	if(x.style.display === "block"){
        x.style.display = "none";
    } else {
        x.style.display = "block";
    }
}


// INPUT VALIDATION functions ------------------------------------------------------------------------------------------

// The two functions below initialize gender checkboxes.
function uncheckMale() {
	document.getElementById("male").checked = false;
}

function uncheckFemale() {
	document.getElementById("female").checked = false;
}

function checkUsername(){
	var username = document.getElementById("username").value;
	var letter = new RegExp(/[a-z]/i);
	
	//If username matches any username in the username array, an error occurs.
	for (i = 0; i < usernameArray.length; i++) {
		if (username == usernameArray[i]){
			document.getElementById("errormessage").innerHTML = "Username is already taken.";
			return false;
		}
	}
	
	// If username isn't entered or has no letters, an error occurs.
	if (!letter.test(username)) {
		document.getElementById("errormessage").innerHTML = "Please enter a proper username.";
		return false;
	}
	
	else {
		return true;
	}
}

function checkEmail(){
	var email = document.getElementById("email").value;
	var pattEmail = new RegExp("^[^@]+@[^@]+\.com$"); // Correct email format.
	
	//If email isn't formatted "X@X.com", an error occurs.
	if (!(pattEmail.test(email))) {
		document.getElementById("errormessage").innerHTML = "Email isn't valid.";
		return false;
	}
	
	else {
		return true;
	}
}

function checkDOB(){
	var dayOfBirth = parseInt(document.getElementById("dayOfBirth").value); 
	var monthOfBirth = parseInt(document.getElementById("monthOfBirth").value);
	var yearOfBirth = parseInt(document.getElementById("yearOfBirth").value);	
	var dayOfBirthString = document.getElementById("dayOfBirth").value;
	var monthOfBirthString = document.getElementById("monthOfBirth").value;
	var yearOfBirthString = document.getElementById("yearOfBirth").value;
	var letter = new RegExp(/[a-z]/i);
	
	//If year of birth is null, or incorrectly typed, an error occurs.
	if (yearOfBirthString == "" || letter.test(yearOfBirthString)) {
		document.getElementById("errormessage").innerHTML = "Year of birth must be correctly entered.";
		return false;
	}
	
	//The oldest person in the world today was born 1898, so an error occurs if someone claims to be older than that.
	else if (yearOfBirth < 1898) {
		document.getElementById("errormessage").innerHTML = "Year of birth isn't valid.";
		return false;
	}
	
	//If month of birth is null, or incorrectly typed, an error occurs.
	else if (monthOfBirthString == "" || letter.test(monthOfBirthString)) {
		document.getElementById("errormessage").innerHTML = "Month of birth must be correctly entered.";
		return false;
	}
	
	//If month of birth isn't between 1 and 12, an error occurs.
	else if (1 > monthOfBirth || monthOfBirth > 12) {
		document.getElementById("errormessage").innerHTML = "Month of birth isn't valid.";
		return false;
	}
	
	//If year of birth is null, or incorrectly typed, an error occurs.
	else if (dayOfBirthString == "" || letter.test(dayOfBirthString)) {
		document.getElementById("errormessage").innerHTML = "Day of birth must be correctly entered.";
		return false;
	}
	
	//If day of birth isn't valid, while also considering the months and leap years, an error occurs.
	else if ((dayOfBirth < 1) 
	|| (dayOfBirth > 31) 
	|| (dayOfBirth == 31 && [2, 4, 6, 9, 11].indexOf(monthOfBirth) >= 0)
	|| (dayOfBirth == 30 && monthOfBirth == 2
	|| (dayOfBirth == 29 && monthOfBirth == 2
	// Fun Fact: Leap years happen every year divisible by 4, unless when divisible by 100 (unless when divisible by 400).
	&& (yearOfBirth % 4 != 0 || (yearOfBirth % 100 == 0 && yearOfBirth % 400 != 0))))
	){
		document.getElementById("errormessage").innerHTML = "Day of birth isn't valid.";
		return false;
	}
	
	else {
		return true;
	}
}

function checkPassword(){	
	var passWord = document.getElementById("password").value;
	var confirmPassword = document.getElementById("confirmPassword").value;
	
	// Checks to see if there is one letter or number in the password.
	var oneLetter; 
	var oneNumber;
	if (passWord == null) {
		oneLetter = false;
		oneNumber = false;
	}
	else {
		var patt1 = new RegExp(/[a-z]/i);
		var patt2 = new RegExp(/[0-9]/i);
		oneLetter = patt1.test(passWord);
		oneNumber = patt2.test(passWord);
	}
	
	
	//If password doesn't have at least one letter or number, an error occurs.
	if (!(oneLetter && oneNumber)){
		document.getElementById("errormessage").innerHTML = "Passwords must have at least one letter and one number.";
		return false;
	}
	
	//If password is less than 6 characters long, an error occurs.
	else if (passWord.length < 6) {
		document.getElementById("errormessage").innerHTML = "Passwords must be at least 6 characters long.";
		return false;
	}
	
	//If password and confirm password don't match, an error occurs.
	else if (passWord != confirmPassword){
		document.getElementById("errormessage").innerHTML = "'Confirm Password' does not match 'Password'.";
		return false;
	}
	
	else {
		return true;
	}
}


// Creates an account and returns a user to the search page IF the username, email, date of birth, gender, and password are valid and confirmed.
function validation(){
	
	// If the username, email, date of birth, OR password is wrong, returns false (Gender has been kept optional).
	if (!(checkUsername()) || !(checkEmail()) || !(checkDOB()) || !checkPassword()){
		return false;
	}
	
	// If the data entered is valid, function returns true and user is taken to the search page.
	else {
		return true;
	}
}

// MAPS, MARKERS, LOCATION functions -----------------------------------------------------------------------------------


/*
// dummy list of hotspots example
var hotspots = [
    {
        name: "Brisbane Botanic Gardens",
        address: "Mt Coot-tha Rd",
        suburb: "Toowong",
        lat: -27.47724,
        lng: 152.97599
    }
];
*/


// https://stackoverflow.com/questions/5384712/capture-a-form-submit-in-javascript
window.onload = function() {
    
	if (document.getElementById('searchForm') !== null) {
        document.getElementById('searchForm').onsubmit=function() {

            if (document.getElementById('myLocationButton').checked){
                // if geolocation is implemented by the browser, return the position
                if (navigator.geolocation) {
                    navigator.geolocation.getCurrentPosition(submitWithLocation, showError);
                    // if not return an error
                } else {
                    document.getElementById("errormessage").innerHTML="Geolocation is not supported by this browser.";
                }
                return false;  // do not submit form yet
            }
            else{
                return true; // let the submit process continue
            }
        }
	}

	if (document.getElementById('registerForm') !== null){
        document.getElementById('registerForm').onsubmit=function() {

            if (!(validation())){
                // if the registration isn't valid...
                return false;  // do not submit form yet
            }
            else{
                return true; // let the submit process continue
            }
        }
	}

}




function submitWithLocation(position){
	// set the value of own location
    document.getElementById('latitude').value = position.coords.latitude;
	document.getElementById('longitude').value = position.coords.longitude;

	//window.alert("[Test] Your coordinates are lat: "+position.coords.latitude+" long: "+position.coords.longitude);

	// submit the form after populating location details
	document.getElementById('searchForm').submit();
}


// Display map with hotspots
function initMap(hotspots, mapID, myLocDetails) {

	// if there are no hotspots and "my location" was not given, the center of the map will be the center of Brisbane
	if((hotspots.length === 0) && (myLocDetails === undefined || myLocDetails.length === 0)){
		centerLat = -27.4698;
		centerLng = 153.0251;
	}
	// if there are no hotspots, but "my location" was given, "my location" will be the center of the map
	else if(!(myLocDetails === undefined || myLocDetails.length === 0)){
        centerLat = parseFloat(myLocDetails.lat);
        centerLng = parseFloat(myLocDetails.lng);
	}
	// if there are hotspots, the first will be the center
	else{
        centerLat = parseFloat(hotspots[0].lat);
        centerLng = parseFloat(hotspots[0].lng);
	}

	// center the map to one of the hotspots
    var map = new google.maps.Map(document.getElementById(mapID), {
        zoom: 11,  // zooms into map
        center: {lat: centerLat, lng: centerLng}  //location of map center
    });

    for (i = 0; i < hotspots.length; i++) {
		// convert lat and lng from string to float
		lat = parseFloat(hotspots[i].lat);
    	lng = parseFloat(hotspots[i].lng);

    	// make new info window
       	var infowindow = new google.maps.InfoWindow();


        // add the markers to the map
    	var marker = new google.maps.Marker({
            position: {lat: lat, lng: lng},  //location of marker
            map: map,
            title: hotspots[i].name,  //title of marker displayed on map
			animation: google.maps.Animation.DROP,
        });

    	// add the details to marker
		// source adaptation: https://stackoverflow.com/questions/3059044/google-maps-js-api-v3-simple-multiple-marker-example
        google.maps.event.addListener(marker, 'click', (function(marker, i) {
            return function() {
                var contentString = "<p>Hotspot: "+hotspots[i].name+"</p><a href='"+hotspots[i].url+"'>Go to hotspot</a>";
            	infowindow.setContent(contentString);
                infowindow.open(map, marker);
            }
        })(marker, i));


        /*
        // make the markers bounce when clicked
        marker.addListener('click', function () {
            // if marker has animation -> do not animate it anymore
            if (this.getAnimation() !== null) {
                this.setAnimation(null);
            // if marker does not have animation -> make it bounce
            } else {
                this.setAnimation(google.maps.Animation.BOUNCE);
            }
        });
        */
    }
}

// Get own location and find neareast hotspots based on it
// Source adaptation: week5 tutorial
function getLocation() {
    // if geolocation is implemented by the browser, return the position
	if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(retrieveNearest, showError);
    // if not return an error
	} else {
        document.getElementById("errormessage").innerHTML="Geolocation is not supported by this browser.";
    }
}

// Error message to display when retrieving own location fails
// Source: week5 tutorial
function showError(error) {
    var msg = "";
    switch(error.code) {
        case error.PERMISSION_DENIED:
            msg = "User denied the request for Geolocation.";
            break;
        case error.POSITION_UNAVAILABLE:
            msg = "Location information is unavailable.";
            break;
        case error.TIMEOUT:
            msg = "The request to get user location timed out.";
            break;
        case error.UNKNOWN_ERROR:
            msg = "An unknown error occurred.";
            break;
    }
    document.getElementById("errormessage").innerHTML = msg;
}

