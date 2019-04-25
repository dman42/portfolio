<?php
session_start(); // make session
unset($_SESSION['loginUsername']);

?>


<!DOCTYPE html>
<html><head>
    <title>Login</title> <!--Title of the page.-->
    <meta charset="utf-8">
    <?php
    include 'htmlGenerator.php';
    genHomeScreenData(); // generates meta data for home screens
    ?>
    <link href="fonts/font-awesome.min.css" rel="stylesheet"> <!--Link to the FontAwesome font -->
    <link href="main.css" type="text/css" rel="stylesheet"><!--Link to the main css stylesheet-->
    <link rel="stylesheet" type="text/css" href="ie.css"><!--Link to css styling for Internet Explorer-->
    <script type="text/javascript" src="main.js"></script><!--Link to the main javascript code -->
</head>
<body>


<!--The header of the page, containing the title, tagline, wi-fi icon, and username if logged in. -->
<?php include 'TopOfPage.inc'; ?>


<!--Main content area of the page. -->
<div class="logout-area">
    <h1>You have been logged out!</h1>
</div>


<!--The footer of the page, containing authorship information.-->
<?php include 'footer.inc'; ?>

</body></html>

