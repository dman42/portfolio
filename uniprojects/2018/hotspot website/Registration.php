<!DOCTYPE html>
<html>
<head>
    <title>Register</title> <!--Title of the page.-->
    <meta charset="utf-8">
    <?php
    // below the meta data for the Registration page is generated
    include 'htmlGenerator.php';
    $imageSrc = (isset($_SERVER['HTTPS']) ? "https" : "http") . "://$_SERVER[HTTP_HOST]" . dirname($_SERVER['REQUEST_URI']) . "/img/wifi.png";
    $url = (isset($_SERVER['HTTPS']) ? "https" : "http") . "://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]";
    genMetaData("Register to Brisbane Hot-Spot Finder and contribute to the amazing database of ratings and reviews!",
        "Register", $imageSrc, $url);
    genHomeScreenData(); // generates meta data for home screens
    ?>
    <link href="fonts/font-awesome.min.css" rel="stylesheet"> <!--Link to the FontAwesome font -->
    <link href="main.css" type="text/css" rel="stylesheet"><!--Link to the main css stylesheet-->
    <link rel="stylesheet" type="text/css" href="ie.css"><!--Link to css styling for Internet Explorer-->
    <script type="text/javascript" src="main.js"></script><!--Link to the main javascript code -->
</head>
<body>

<!--Connects to the database and either:
1. Adds the newly created account to the database if the user submitted from registration page,
2. Updates the status of an account if the person submitted from login page.-->


<?php
session_start(); // make session
include  'TopOfPage.inc'; // The header of the page, containing the title, tagline, wi-fi icon, and username if logged in
include_once 'serverValidations.php'; // contains functions for validating the registration input
include_once 'operationsDB.php';
$errorsUsername = array(); // holds errors generated when attemting to send username to DB
$errorsEmail = array(); // holds errors generated when attemting to send email to DB
$errorsDOB = array(); // holds errors generated when attemting to send date of birth to DB
$errorsGender = array(); // holds errors generated when attemting to send gender to DB
$errorsPassword = array(); // holds errors generated when attemting to send password to DB

// if validations are not passed, prefill boxes
$username = $email = $day = $month = $year = $gender = $password = $confpass = "";

// If the user attempted to register ...
if (isset($_POST['submitRegistration'])) {

    // try connect to Db
    $db = connectToDB("registration-area");

    // The account information is then put into the database and the account is automatically active.
    $date = $_POST['year'] . "-" . $_POST['month'] . "-" . $_POST['day'];
    $query = $db->prepare("INSERT INTO Members(username, password, email, DOB, gender, salt) 
		VALUES(:username, :password, :email, :date, :gender, :salt)");

    // prefill
    if(isset($_POST['username'])){
        $username = $_POST['username'];
    }
    if(isset($_POST['email'])){
        $email = $_POST['email'];
    }
    if(isset($_POST['day'])){
        $day = $_POST['day'];
    }
    if(isset($_POST['month'])){
        $month = $_POST['month'];
    }
    if(isset($_POST['year'])){
        $year = $_POST['year'];
    }
    if(isset($_POST['gender'])){
        $gender = $_POST['gender'];
    }
    if(isset($_POST['password'])){
        $password = $_POST['password'];
    }
    if(isset($_POST['confirmPassword'])){
        $confpass = $_POST['confirmPassword'];
    }



    // make sure validation() from main.js returns true even if the data is not valid
    // to be able to test php validation
    $registration = ( validateUsername($username, $errorsUsername, $db) &&
    validateEmail($email, $errorsEmail,$db) &&
    validateDOB($day, $_POST['month'], $_POST['year'], $errorsDOB,$db)&&
    validateGender($month, $errorsGender, $db)&&
    validatePassword($password, $confpass, $errorsPassword, $db));

    // enter this branch only if validation returns true
    if($registration){
        // encrypt
        $salt = substr(md5(uniqid()), 0, 20);
        $password = $_POST['password'] . $salt;

        $query->execute(array(
            "username" => $_POST['username'],
            "password" => sha1($password),
            "email" => $_POST['email'],
            "date" => $date,
            "gender" => $_POST['gender'],
            "salt" => $salt
        ));

        // make session
        $_SESSION['loginUsername'] = $_POST['username'];

        // if all validations are successful and he is addded to DB, take user to search page
        // your folder below
        header("Location: http://{$_SERVER['HTTP_HOST']}/Students/n9939768/SearchPage.php");
    }

}

?>


<!--Main content area of the page. -->
<div class="registration-area">
	<h1>Register</h1>
	
	<form id="registerForm" action="Registration.php" method="POST">
		<div> <!--Username Field.-->
			<p>Username:</p>
			<input id="username" name="username" type="text" maxlength="40" value="<?php echo $username; ?>">
            <?php
            // display errors related to username
            foreach ($errorsUsername as $error) {
                echo "<br>";
                echo "<span class=\"error\"> $error </span> ";
            }
            ?>
		</div>
		
		<div> <!--Email Field.-->
			<p>Email:</p>
			<input id="email" name="email" type="text" placeholder="name@domain.com" value="<?php echo $email; ?>">
            <?php
            // display errors related to email
            foreach ($errorsEmail as $error) {
                echo "<br>";
                echo "<span class=\"error\"> $error </span> ";
            }
            ?>
		</div>
		
		<div> <!--DOB Field.-->
			<p>Date of Birth:</p>
			<div id="DOB">
				<input id="dayOfBirth" name="day" class="birthfield" type="text" placeholder="DD" value="<?php echo $day; ?>">
				<div>/</div>
				<input id="monthOfBirth" name="month" class="birthfield" type="text" placeholder="MM" value="<?php echo $month; ?>">
				<div>/</div>
				<input id="yearOfBirth" name="year" class="birthfield" type="text" placeholder="YYYY" value="<?php echo $year; ?>">
			</div>
            <?php
            // display errors related to DOB
            foreach ($errorsDOB as $error) {
                echo "<br>";
                echo "<span class=\"error\"> $error </span> ";
            }
            ?>
		</div>
		
		<div> <!--Gender Field-->
			<p>Gender:</p>
			<div id="gender">
				<h6>Male:</h6>
					<input type="radio" id="male" value="M" name="gender" onclick="uncheckFemale()">
				<h6>Female:</h6>
					<input type="radio" id="female" value="F" name="gender" onclick="uncheckMale()">
			</div>
            <?php
            // display errors related to gender
            foreach ($errorsGender as $error) {
                echo "<br>";
                echo "<span class=\"error\"> $error </span> ";
            }
            ?>
		</div>


		<div> <!--Password Field.-->
			<p>Password:</p>
			<input id="password" name="password" type="password" onclick="toggleMessageBox();" onblur="toggleMessageBox();">
            <?php
            // display errors related to email
            foreach ($errorsPassword as $error) {
                echo "<br>";
                echo "<span class=\"error\"> $error </span> ";
            }
            ?>
		</div>

		<!-- Holds the message describing the requirements for password-->
		<div class="message-bubble">
			<p>Should be at least 6 characters, containing both letters and numbers.</p>
		</div>

		<div> <!--Confirm Password Field.-->
			<p>Confirm Password:</p>
			<input id="confirmPassword" name="confirmPassword" type="password">
		</div>

		<input type="submit" value="Register" id="searchButton"  name="submitRegistration" onclick="validation()"><!--Register Button-->
	</form>
	
	<!-- Error message that appears when a validation error occurs. -->
	<div id="errormessage"></div> 
	
</div>


<!--The footer of the page, containing authorship information.-->
<?php include 'footer.inc'; ?>

</body></html>
