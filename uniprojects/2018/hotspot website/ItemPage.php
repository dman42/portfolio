<!DOCTYPE html>
<html>
<head>
    <title>Hot-spot</title> <!--Title of the page.-->
    <meta charset="utf-8">
    <?php
    // below the meta data for the Item page is generated
    include 'htmlGenerator.php';
    $imageSrc = (isset($_SERVER['HTTPS']) ? "https" : "http") . "://$_SERVER[HTTP_HOST]" . dirname($_SERVER['REQUEST_URI']) . "/img/hsmap.png";
    $url = (isset($_SERVER['HTTPS']) ? "https" : "http") . "://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]";
    genMetaData("Rate and review your favourite hot-spot. Let the others know your experience!",
        "Hot-spot", $imageSrc, $url);
    genHomeScreenData(); // generates meta data for home screens
    ?>
    <link href="fonts/font-awesome.min.css" rel="stylesheet"><!--Link to the FontAwesome font -->
    <link href="main.css" type="text/css" rel="stylesheet"> <!--Link to the main css stylesheet-->
    <link rel="stylesheet" type="text/css" href="ie.css"/><!--Link to css styling for Internet Explorer-->
    <script src="https://maps.googleapis.com/maps/api/js?key=AIzaSyAR0LF2vwEdAf-jQDa5FuDFi9B2H5SYMrU">
    </script>
    <script type="text/javascript" src="main.js"></script><!--Link to the main javascript code -->
</head>

<!-- When the body of the page finished loading, hotspotsForMap will contain the JSON repr of the hotspots, which will
be further used to initialize the map in main.js -->
<body onload="initMap(hotspotsForMap, 'map-hotspot');" >


<?php
session_start(); // make session
include 'TopOfPage.inc';  //The header of the page, containing the title, tagline, wi-fi icon, and username if logged in.
include 'operationsDB.php';

$errorsSendReview = array(); // will hold errors generated when attemting to send a review for a hotspot
$errorsReadReview = array(); // will hold errors generated when attemting to read reviews for a hotspot
$errorsItemDetails = array(); // will hold errors generated when attemting to read details of a hotspot

$db = connectToDB("item-details");
// if the connection to DB failed, load only the footer and an error
?>

<!--Main content area of the page. -->
<div class='item-details' itemprop='location' itemscope itemtype='http://schema.org/Place'>

    <?php
    $hotSpotExists = true;  // flag signals existence of hotspot in database

    if (isset($_GET['itemID'])) {
        $itemID = $_GET['itemID'];  // get the itemID of the hotspot from ResultsPage.php

        // if Submit Review button has been clicked, send the review to DB
        // done as early as possible, to update the average rating
        if(isset($_POST['submitReview'])){

            // check first if the user is logged in
            if(isset($_SESSION['loginUsername'])){
                sendReviewToDB($itemID, $errorsSendReview, $db);
            }
            else{
                // if user is not logged in, add as error
                $errorsSendReview['notLoggedIn'] = 'Please make sure you login before you add a review!';
            }
        }

        // get the hotspot from the database
        $hs = getHotspotFromDB($itemID,$hotSpotExists, $errorsItemDetails, $db);

        // display errors related to inexistent hotspot
        foreach ($errorsItemDetails as $error) {
            echo "<span class=\"error\"> $error </span> ";
        }

        // if the hotspot exists, $hs will hold its details
        if ($hotSpotExists) {
            $hotspotsForMap = array(); // (single) hotspot details for map
            $hotspot = array(
                "name" => $hs['name'],
                "address" => $hs['address'],
                "suburb" => $hs['suburb'],
                "lat" => $hs['latitude'],
                "lng" => $hs['longitude']
            ); // get hotspot details
            array_push($hotspotsForMap, $hotspot); // add hotspot to list
            $hotspotsToString = json_encode($hotspotsForMap);  // json representation of hotpots


            // get suburb and postal code (if exists) for microdata
            $postal = "";
            $suburb = "";
            $temp = explode(',', $hs['suburb']);
            if (sizeof($temp) == 1) {
                $suburb = $temp[0];
            } elseif (sizeof($temp) == 2) {
                $suburb = $temp[0];
                $postal = $temp[1];
            }

            // Hotpot details area -------------------------------------------------------------------------------------
            genDetailsArea($hs, $suburb, $postal);

            // Submit a review area ------------------------------------------------------------------------------------
            $alreadyTyped = "";
            // if there have been errors in submitting the review, keep the comment
            if(isset($_POST['textReview']) && (sizeof($errorsSendReview) !== 0)) {
                $alreadyTyped = $_POST['textReview'];
            }
            genSubmitReviewForm($hs, $alreadyTyped); // generate the submit review form containing comment and rating

            // display errors related to sending a review of the hotspot
            foreach ($errorsSendReview as $error) {
                echo "<span class=\"error\"> $error </span> ";
            }

            // Past reviews for a specific hotspot area ----------------------------------------------------------------
            echo "<div class='reviewsection'>
                    <h3>Reviews:</h3>";

            // read all past reviews
            $allReviews = readReviewFromDB($hs['itemID'], $errorsReadReview, $db);
            if ($allReviews !== null) {
                genPastReviewsArea($allReviews);
            }

            // display errors related to reading the reviews of the hotspot
            foreach ($errorsReadReview as $error) {
                echo "<span class=\"error\"> $error </span> ";
            }
            echo "</div>";
        }
    }
    ?>
</div>


<!--The footer of the page, containing authorship information.-->
<?php include 'footer.inc'; ?>


<!--Send (single) hotspot to map function-->
<script>
    // executed at the end, JS object hotspotsForMap will contain the JSON repr. of the hotspots
    var hotspotsForMap = <?php echo $hotspotsToString ?>;
</script>

</body>
</html>
