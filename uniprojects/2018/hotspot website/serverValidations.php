<?php

// strip input text to avoid cross-scripting
function cleanInput($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}


// checks if the text contains links to other websites, returns true if it does
function containsUnallowed($inputText){

    $unallowedChars = array("www.", "http", ".com", "@");

    // check if one of the combinations exist in the text
    for ($i = 0; $i < sizeof($unallowedChars); $i++){
        if (strpos($inputText, $unallowedChars[$i]) !== false) {
            return true;
        }
    }
    return false;
}


// check if the input test contains white spaces inside
function whiteSpaceInside($inputText){
    if( preg_match('/\s/',$inputText) ){
        return true;
    }
}


// below function checks the username
// receives the pointer to the variable that should capture errors related to the username
function validateUsername($username, &$errorsUsername, &$db){
    $usernameOK = true;

    // check if the username has empty spaces, slashes or special characters
    $cleanedUsername = cleanInput($username);
    if((strcmp($username, $cleanedUsername) !== 0) || whiteSpaceInside($username)){
        $errorsUsername['emptySpace'] = 'The username entered cannot contain empty spaces, slashes or special characters!. Please select another one';
        $usernameOK = false;
    }

    if (! checkIfUnique($username, "username",$db )){
        $errorsUsername['notUniqueUser'] = 'The username entered is already taken!. Please select another one';
        $usernameOK = false;
    }
    // check if it already exist in the database
    // handle errors

    return $usernameOK;
}

function checkIfUnique($toCheck, $type, &$db){
    $query = $db->query("SELECT * FROM Members WHERE {$type} = '{$toCheck}'");

    foreach ($query as $row) {
        return false;
    }
    return true;
}

// below function checks the email
// receives the pointer to the variable that should capture errors related to the email
function validateEmail($email, &$errorsEmail, &$db){
    $cleanedEmail = cleanInput($email);
    $emailOK = true;

    $pattEmail = "/^[^@]+@[^@]+\.com$/"; // Correct email format.

    if(strlen($email) <= 0){

    }

    if(strcmp($email, $cleanedEmail) !== 0){
        $errorsEmail['emptySpace'] = 'The email entered cannot contain empty spaces, slashes or special characters!. Please select another one';
        $emailOK = false;
    }

    if (! checkIfUnique($email, "email",$db )){
        $errorsEmail['notUniqueEmail'] = 'The email entered is already taken!. Please select another one';
        $emailOK = false;
    }

    //If email isn't formatted "X@X.com", an error occurs.
    if (!preg_match($pattEmail, $email)) {
        $errorsEmail['notValidEmail'] = 'The email entered is not valid!. Please select another one';
        $emailOK = false;
    }

   return $emailOK;
}

// below function checks the date of birth
// receives the pointer to the variable that should capture errors related to the DOB
function validateDOB($day, $month, $year, &$errorsDOB, &$db){
    $dateOK = true;

    settype($day, "integer");
    settype($month, "integer");
    settype($year, "integer");

    $cleanedDay = cleanInput($day);
    $cleanedMonth = cleanInput($month);
    $cleanedYear = cleanInput($year);

    if((strcmp($day, $cleanedDay) !== 0) || (strcmp($month, $cleanedMonth) !== 0) || (strcmp($year, $cleanedYear) !== 0)){
        $errorsDOB['emptySpace'] = 'The date entered cannot contain empty spaces, slashes or special characters!. Please select another one';
        $dateOK = false;
    }

    //The oldest person in the world today was born 1898, so an error occurs if someone claims to be older than that.
    else if ($year < 1898) {
        $errorsDOB['invalidYear'] = 'You cant\'t be that old!. Please select another one';
        $dateOK = false;
    }

    //If month of birth isn't between 1 and 12, an error occurs.
    else if (1 > $month || $month > 12) {
        $errorsDOB['invalidMonth'] = 'The month you\'ve entered isn\'t valid!. Please select another one';
        $dateOK = false;
    }

    //If day of birth isn't valid, while also considering the months and leap years, an error occurs.
    else if (($day < 1)
        || ($day > 31)
        || ($day == 31 && ($month == 2) || ($month == 4) || ($month == 6) || ($month == 9) || ($month == 11))
        || ($day == 30 && $month == 2
            || ($day == 29 && $month == 2
                // Fun Fact: Leap years happen every year divisible by 4, unless when divisible by 100 (unless when divisible by 400).
                && ($year % 4 != 0 || ($year % 100 == 0 && $year % 400 != 0))))
    ){
        $errorsDOB['invalidDay'] = 'The day you\'ve entered isn\'t valid!. Please select another one';
        $dateOK = false;
    }

    return $dateOK; // if valid
}

// below function checks the gender
// receives the pointer to the variable that should capture errors related to the gender
function validateGender($gender, &$errorsGender, &$db){

    $cleanedGender = cleanInput($gender);
    if(strcmp($gender, $cleanedGender) !== 0){
        $errorsGender['invalidGender'] = 'Trying to hack our website, aren\'t you?';
        return false;
    }
    return true; // if valid
}

// below function checks the password
// receives the pointer to the variable that should capture errors related to the password and confirmation
function validatePassword($pass, $confirmedPass, &$errorsPassword, &$db){
    $passwordOK = true;

    // Checks to see if there is one letter or number in the password.
    $patt1 = "/[a-z]/i";
    $patt2 = "/[0-9]/i";
    $oneLetter = preg_match($patt1, $pass);
    $oneNumber = preg_match($patt2, $pass);

    //If password doesn't have at least one letter or number, an error occurs.
    if (!($oneLetter && $oneNumber)){
        $errorsPassword['letNumError'] = 'The password should contain at least one letter and one number!';
        $passwordOK = false;
    }

    //If password is less than 6 characters long, an error occurs.
    else if (strlen($pass) < 6) {
        $errorsPassword['passLenError'] = 'The password should contain at least 6 characters!';
        $passwordOK = false;
    }

    //If password and confirm password don't match, an error occurs.
    else if (strcmp($pass, $confirmedPass) !== 0){
        $errorsPassword['mismatchError'] = 'The password and confirmed password do not match!';
        $passwordOK = false;
    }

    return $passwordOK;
}

?>