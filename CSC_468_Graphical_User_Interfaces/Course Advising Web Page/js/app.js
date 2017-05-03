////////////////////////////////////////////////////////////////////////////////
// Document creation stuff we need to take care of
////////////////////////////////////////////////////////////////////////////////
$(document).ready(function() {
	$("#loadingAnimationID").hide();
	$.ajaxSetup({
		type: "POST",
		method: "POST"
	});
});

////////////////////////////////////////////////////////////////////////////////
// Global constants
////////////////////////////////////////////////////////////////////////////////
const animationEnd = 'webkitAnimationEnd mozAnimationEnd MSAnimationEnd oanimationend animationend';
const toastTime = 2000; // All toast animations take 2 seconds

////////////////////////////////////////////////////////////////////////////////
// Set our pjax callbacks 
////////////////////////////////////////////////////////////////////////////////
$(document).on('pjax:complete', function(evnt, xhr, opts) {
	$('#loadingAnimationID').fadeOut(400, function() {
		$('#bodyContainer').fadeIn(800);
	});
});

$(document).on('pjax:popstate', function(evnt) {
	if (evnt.direction == 'back') {
		location.reload();
	}
});

$(document).on('pjax:end', function(evnt, opts) {
	var resource = evnt.currentTarget.URL.split('/').pop();

	switch(resource) {
		case '': // Main page
		setupLogin();
		break;

		case 'classEntry.php':
		setupClassEntry();
		break;

		case 'advising.php':
		setupRecommendations();
		break;
	}
})

////////////////////////////////////////////////////////////////////////////////
// General Functions
////////////////////////////////////////////////////////////////////////////////

// Log in the user
function login() {
	// Fade the body of the document out
	$("#bodyContainer").fadeOut(800, function() {

		// Start our loading animation
		$("#loadingAnimationID").fadeIn(400);

		// Get our form data
		var formdata = $("#loginForm").serialize();

		// pjax load the next page
		$.pjax({
			container: "#bodyContainer",
			timeout: 2000,
			type: "POST",
			method: "POST",
			data: formdata,
			url: "classEntry.php",
			fragment: "#bodyContainer"
		});
	});

	// Disable page load after submit button is pressed
	return false;
}

// Submit the classes from the class entry page
function submitClasses() {
	// Start our AJAX request to validate their information
	$.ajax({
		data: $('#classEntryForm').serialize(),
		url: "validate.php",
		method: "POST",
		mimeType: "application/json",
		xhrFields: {
			withCredentials: true
		},

		// This error callback logs some helpful debug information
		error: function(xhr, status, err) {
			console.log(status);
           		console.log(xhr.responseText);
		},

		// This success callback will decide wether or not to let the next page
		// load based on the entered student data
		success: function(data, status, xhr) {
			// If the validator said we can go to the next page
			if (data.canSubmit) {

				// Fade out the body of the document
				$("#bodyContainer").fadeOut(800, function() {

					// Parse our form data
					var formdata = $("#classEntryForm").serialize();

					// Start our loading animation
					$("#loadingAnimationID").fadeIn(400);

					// pjax load the next page
					$.pjax({
						container: "#bodyContainer",
						type: "POST",
						method: "POST",
						data: formdata,
						timeout: 2000,
						xhrFields: {
							withCredentials: true
						},
						url: "advising.php",
						fragment: "#bodyContainer"
					});
				});

			// If the validator did not say we can go to the next page
			} else {
				// Shake our buttons and turn the erroronous rows red
				$('#submitButton').toggleClass("shake animated").one(animationEnd, function () {
					$(this).toggleClass("shake animated");
				});

				// Highlight rows as necessary
				for (var id in data.invalid) {
					$('#' + data.invalid[id]).css('border-color', 'red');
				}

				// Toast
				showToast("You entered incomplete data or prerequisits");
			}
		}
	});

	// Set all of the data rows to black before our AJAX request returns
	$('.row').each(function() {
		$(this).children().each(function() {
			$(this).children().css('border-color', 'black');
		});
	});

	// Disable page load after submit button is pressed
	return false;
}

////////////////////////////////////////////////////////////////////////////////
// Setup functions to initialize things based on the page that was just loaded
////////////////////////////////////////////////////////////////////////////////

// This setup function attaches event listeners to our submit button for the login form
function setupLogin() {
	$("#submitButton").off('click');
	$("#submitButton").click(function(e) {
		e.preventDefault();

		var invalidData = validateLogin();
		if(invalidData.length == 0) {
			login();
		} else {
			// If login was invalid, shake and toast
			$("#submitButton").toggleClass("shake animated").one(animationEnd, function () {
				$(this).toggleClass("shake animated");
			});

			// Toast message
			showToast(invalidData.join(', ') + ((invalidData.length == 1) ? ' is ' : ' are ') + 'invalid');
		}
	});

	$.ajaxSetup({
		type: "POST",
		method: "POST"
	});
}

// This setup function attaches some event listeners to our class entry form elements
function setupClassEntry() {
	// Setup our folding action
	$('[data-toggle="fold"]').off('change');
	$('[data-toggle="fold"]').change(function(e) {
		var base = '#' + $(this).attr('id').substring(4);
		$(base).slideToggle(400);
		$(base + 'Plus').toggleClass('rotate');
	})
	.each(function () {
		var base = '#' + $(this).attr('id').substring(4);
		$(base).hide(0);
		$(base + 'Plus').removeClass('rotate');
	});

	// Set up our submit button
	$('#submitButton').off('click');
	$('#submitButton').click(function(e) {
		e.preventDefault();

		if (validateClassEntry()) {
			submitClasses();
		} else {
			$("#submitButton").toggleClass("shake animated").one(animationEnd, function () {
				$(this).toggleClass("shake animated");
			});

			showToast("You entered invalid data");
		}
	});

	// I set MATH 101, MATH 102, MATH 115, and MATH 120 to "Ex" since the core curriculum
	// is designed for students entering Calc 1.
	const AssumedMathematics = ['#MATH101Grade', '#MATH102Grade', '#MATH115Grade', '#MATH120Grade'];
	$.each(AssumedMathematics, function (index, id) {
		$(id).val(6); // 'Ex' is the 6th option in the drop down list
	});

	// This code makes it so selecting a CSC or MATH elective selects the right class/name/credits 
	// in the drop down menus
	const sync = ['#CSElective0Number', '#CSElective0Name', '#CSElective1Number', '#CSElective1Name', '#CSElective2Number', '#CSElective2Name', '#CSElective3Number', '#CSElective3Name', '#MathElective0Number', '#MathElective0Name'];
	$.each(sync, function(index, id) {
		// I know this looks nasty, but trust me, it's really quite straight forward.  All I'm doing is
		//   1. Set the change listener to find the id of whichever drop down was clicked
		//   2. Set index = the number of the item clicked (i.e. the 3rd item down selected => index = 3)
		//   3. Select that for both the name and number field (since they are ordered in the HTML to match)
		//   4. CSC 415 and Math 423 have 4 credits instead of 3, so special case those
		$(id).change(function() {
			var idPrefix = $(this).attr('id');
			if (idPrefix[0] == 'C') {
				idPrefix = $(this).attr('id').substring(0,11);
			} else {
				idPrefix = $(this).attr('id').substring(0,13);
			}
			// var idPrefix = $(this).attr('id').substring(0,11);
			var index = $(this)[0].value;
			$('#' + idPrefix + 'Number').get(0).selectedIndex = index;
			$('#' + idPrefix + 'Name').get(0).selectedIndex = index;

			var number = $('#' + idPrefix + 'Number')[0].options[index];
			if(index == 0) {
				$('#' + idPrefix + 'Credits').val('');
			}
			else if(['415', '423'].includes(number.text)) { // Robotics or Advanced Calc
				$('#' + idPrefix + 'Credits').val('4');
			} else {
				$('#' + idPrefix + 'Credits').val('3');
			}
		});
	});

	// // We set each row to green if the grade is set to 'A', 'B', 'C', 'Ex', 'PI', or 'Taking'
	// // These integers map to those grades
	// const goodGrades = [1, 2, 3, 6, 7];
	// const badGrades = [4, 5];
	// const uglyGrades = [8];
	// $("select[name$='grade']").each(function(index) {
	// 	$(this).change(function() {
	// 		var selectedIndex = $(this).val();
	// 		if ($.inArray(selectedIndex, goodGrades)) {
	// 			$(this).css({'border-color' : 'green'});
	// 		} else if ($.inArray(selectedIndex, badGrades)) {
	// 			$(this).css({'border-color' : 'red'});
	// 		} else {
	// 			$(this).css({'border-color' : 'black'});
	// 		}
	// 	})
	// });

	$.ajaxSetup({
		type: "POST",
		method: "POST"
	});
}

function setupGrad() {

}

// Setup function to attach our event listeners to our elements
function setupRecommendations() {
	$('[data-toggle="fold"]').off('change');
	$('[data-toggle="fold"]').change(function(e) {
		var base = '#' + $(this).attr('id').substring(4);
		$(base).slideToggle(400);
		$(base + 'Plus').toggleClass('rotate');
	}).each(function () {
		var base = '#' + $(this).attr('id').substring(4);
		$(base + 'Plus').addClass('rotate');
	});

	// Progress Bar creation
	$('.progressBar').each(function() {
		$(this).find('.progressBarBar').animate({
			width: $(this).attr('data-percent')
		}, 3000);
	});
}

////////////////////////////////////////////////////////////////////////////////
// Validation functions
////////////////////////////////////////////////////////////////////////////////

// Validate the login
function validateLogin() {
	var stuid = $("#stuIDInput").val();
	var pass = $("#passIDInput").val();
	var valid = [];

	// Student ID's are 7 digits
	if ( !(/^\d{7}$/.test(stuid)) ) {
		$("#stuIDInput").css({'border-color' : 'red'});
		valid.push('Student ID');
	} else {
		$("#stuIDInput").css({'border-color' : 'black'});
	}

	// 8 char password minimum, because "security"
	if (pass.length < '8') {
		$("#passIDInput").css({'border-color' : 'red'});
		valid.push('Password');
	} else {
		$("#passIDInput").css({'border-color' : 'black'});
	}

	return valid;
}

// Validate our classentry page
function validateClassEntry() {
	var valid = true;
	
	// Basically, we're just looping over every input on the page right here
	// Probably an inefficient way to do this, but I already wrote it this way and
	// don't want to deal with writing it to attach the validator to each input
	// in the setup function.
	$('input').each( function(index, elem) {
		var id = elem.id;

		if (/Prefix$/.test(id)) {
			// Course prefixes are 3-4 character upper case
			if ($(elem).val() == "" || /^[A-Z]{3,4}$/.test($(elem).val())) {
				$(elem).css({'border-color' : 'black'});
			} else {
				valid = false;
				$(elem).css({'border-color' : 'red'});
			}
		}
		else if (/Number$/.test(id)) {
			// Numbers are 3 digits with an optional L at the end
			if ($(elem).val() == "" || /^[0-9]{3}L?$/.test($(elem).val())) {
				$(elem).css({'border-color' : 'black'});
			} else {
				valid = false;
				$(elem).css({'border-color' : 'red'});
			}
		}
		else if (/Name$/.test(id)) {
			// Names are alphanumeric with spaces
			if ($(elem).val() == '' || /^[a-zA-Z0-9 ]+$/.test($(elem).val())) {
				$(elem).css({'border-color' : 'black'});
			} else {
				valid = false;
				$(elem).css({'border-color' : 'red'});
			}
		}
		else if (/Credits$/.test(id)) {
			// Credits are a single digit
			if ($(elem).val() == '' || /^[0-9]{1}$/.test($(elem).val())) {
				$(elem).css({'border-color' : 'black'});
			} else {
				valid = false;
				$(elem).css({'border-color' : 'red'});
			}
		}
		else if (/Grade$/.test(id)) {
			// Grades are a single digit (representing the position in the list)
			// Seriously, if these ever get highlighted, something went WAYYY wrong
			if ($(elem).val() == '' || /^[0-9]{1}$/.test($(elem).val())) {
				$(elem).css({'border-color' : 'black'});
			} else {
				valid = false;
				$(elem).css({'border-color' : 'red'});
			}
		}		
	});

	return valid;
}

////////////////////////////////////////////////////////////////////////////////
// Utility functions
////////////////////////////////////////////////////////////////////////////////
function showToast(message) {
	var toaster = $('#toaster');

	toaster.html(message);

	toaster.addClass('toast');
    setTimeout(function() { toaster.removeClass('toast'); }, toastTime);
}
