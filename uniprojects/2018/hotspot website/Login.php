<!DOCTYPE html>
<html>
<head>
    <title>Login</title> <!--Title of the page.-->
    <meta charset="utf-8">
    <?php
    // below the meta data for the Login page is generated
    include 'htmlGenerator.php';
    $imageSrc = (isset($_SERVER['HTTPS']) ? "https" : "http") . "://$_SERVER[HTTP_HOST]" . dirname($_SERVER['REQUEST_URI']) . "/img/wifi.png";
    $url = (isset($_SERVER['HTTPS']) ? "https" : "http") . "://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]";
    genMetaData("Login to Brisbane Hot-Spot Finder and leave a rating for your favourite hot-spot!",
        "Login", $imageSrc, $url);
    genHomeScreenData(); // generates meta data for home screens
    ?>
    <link href="fonts/font-awesome.min.css" rel="stylesheet"> <!--Link to the FontAwesome font -->
    <link href="main.css" type="text/css" rel="stylesheet"><!--Link to the main css stylesheet-->
    <link rel="stylesheet" type="text/css" href="ie.css"><!--Link to css styling for Internet Explorer-->
    <script type="text/javascript" src="main.js"></script><!--Link to the main javascript code -->
</head>
<body>
<!--The header of the page, containing the title,
    tagline, wi-fi icon, and username if logged in. -->
<?php
session_start(); // make session
include 'TopOfPage.inc';
include_once 'operationsDB.php';
include_once 'serverValidations.php';
?>

<!--Login logic-->
<?php
// for testing:
// username: valentina
// pass: vale123
$errorsLogin = array(); // holds errors generated when attemting to login
$loginUsername = "";

if (isset($_POST['submitLogin'])) {

    $loginUsername = $_POST['loginUsername']; // prefill username

    // check if username and password fields are set and non-empty
    if ((!isset($_POST['loginUsername'])) || (!isset($_POST['loginPassword'])) || empty($_POST['loginUsername']) || empty($_POST['loginPassword'])) {
        $errorsLogin['emptyField'] = 'Either username/password/both are missing.';
    } else {

        $db = connectToDB("login-area");

        // strip
        $cleanUsername = cleanInput($_POST['loginUsername']);


        // if the username of passwword have whitespaces inside, warn the user
        if (whiteSpaceInside($cleanUsername)) {
            $errorsLogin['whiteSpace'] = "Your username cannot contain white spaces";
        } else {
            // connection to DB was successful, so load the user data basd on username
            $readSql = $db->prepare("SELECT * FROM members WHERE username = :userName ");
            $readSql->bindValue(':userName', $_POST['loginUsername']);


            // check if query was successful
            if (!$readSql->execute()) {
                $errorsLogin['retrieveUserInfo'] = "There was a problem retrieving the information for this username.";
                // echo "There was a problem retrieving the information for this username.";

            } else {
                // query successful
                $userInfo = $readSql->fetch();  // get user's data

                // there exists a username so check if the encrypted password
                // matches the one belonging to the retrieved user
                $password = $_POST['loginPassword'] . $userInfo['salt'];
                $password = sha1($password);
                if (strcmp($userInfo['password'], $password) !== 0) {
                    $errorsLogin['noMatch'] = 'Username or password is invalid.';
                    // echo "Username or password are incorrect. Try again!";
                } else {
                    echo "Welcome!";
                    // source adaptation: https://www.codeproject.com/Tips/1076176/Login-logout-and-Session-Id-Cookies-in-PHP-for-Beg
                    $_SESSION['loginUsername'] = $_POST['loginUsername']; // write login to server storage
                    $_SESSION['loginUserID'] = $userInfo['memberID'];

                    // your folder below
                    header("Location: http://{$_SERVER['HTTP_HOST']}/Students/n9939768/SearchPage.php"); // redirect to Search page
                }
                // }
            }
        }
    }
}

?>

<!--Main content area of the page. -->
<form class="login-area" action="Login.php" method="POST">
    <h1>Login</h1>
    <div> <!--Username Field.-->
        <p>Username:</p>
        <input name="loginUsername" type="text" value="<?php echo $loginUsername; ?>">
    </div>

    <div> <!--Password Field.-->
        <p>Password:</p>
        <input name="loginPassword" type="password">
    </div>


    <button type="submit" value="Login" id="loginButton" name="submitLogin">Login</button> <!--Register button.-->


    <!-- Error message that appears when a validation error occurs when attempting to login. -->
    <div id="errormessage">
        <?php
        // display errors related to login
        foreach ($errorsLogin as $error) {
            echo "<span class=\"error\"> $error </span> ";
        }
        ?>
    </div>
</form>


<!--The footer of the page, containing authorship information.-->
<?php include 'footer.inc'; ?>

</body>
</html>
