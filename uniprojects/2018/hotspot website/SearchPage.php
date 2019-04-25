<!DOCTYPE html>
<html>
<head>
    <title>Search Hot-spots</title> <!--Title of the page.-->
    <meta charset="utf-8">
    <?php
    // below the meta data for the Search page is generated
    include 'htmlGenerator.php';
    $imageSrc = (isset($_SERVER['HTTPS']) ? "https" : "http") . "://$_SERVER[HTTP_HOST]".dirname($_SERVER['REQUEST_URI'])."/img/wifi.png";
    $url = (isset($_SERVER['HTTPS']) ? "https" : "http") . "://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]";
    genMetaData("Our website allows you to explore Brisbane hotspots based on hotspot name, suburb, rating and even your own location.",
        "Search Hot-spots", $imageSrc, $url);
    genHomeScreenData(); // generates meta data for home screens
    ?>
    <link href="fonts/font-awesome.min.css" rel="stylesheet"> <!--Link to the FontAwesome font -->
    <link href="main.css" type="text/css" rel="stylesheet"><!--Link to the main css stylesheet-->
    <link rel="stylesheet" type="text/css" href="ie.css"><!--Link to css styling for Internet Explorer-->
    <script src="https://maps.googleapis.com/maps/api/js?key=AIzaSyAR0LF2vwEdAf-jQDa5FuDFi9B2H5SYMrU">
    </script>
    <script type="text/javascript" src="main.js"></script><!--Link to the main javascript code -->
</head>
<body>

<?php
session_start(); // make session
include 'TopOfPage.inc'; // The header of the page, containing the title, tagline, wi-fi icon, and username if logged in.
?>

<!--Main content area of the page. -->
<div class="search-options">

    <h1>Search Hot-spots </h1>

    <form id="searchForm" action="ResultsPage.php" method="POST">

        <!--Contains a drop-down list containing each searchable suburb.-->
        <div>
            <p>Suburb:</p>
            <select name="suburb">
                <option value="none">Please Select...</option>
                <?php
                include 'operationsDB.php';
                $db = connectToDB("");
                genSuburbList($db);
                ?>
            </select>
        </div>


        <!--Contains a search bar to search the name of a hotspot with.-->
        <div>
            <p>Name:</p>
            <input type="text" name="name" placeholder="Search..." maxlength="42">
        </div>


        <!--Contains a "star rating" radiomenu to search for hotspots with similar ratings.-->
        <div>
            <p>Rating:</p>
            <div class="stars">
                <div class="star-rating__wrap">
                    <input class="star-rating__input" id="star-rating-5" type="radio" name="rating" value="5">
                    <label class="star-rating__ico fa fa-star-o fa-lg" for="star-rating-5"
                           title="5 out of 5 stars"></label>
                    <input class="star-rating__input" id="star-rating-4" type="radio" name="rating" value="4">
                    <label class="star-rating__ico fa fa-star-o fa-lg" for="star-rating-4"
                           title="4 out of 5 stars"></label>
                    <input class="star-rating__input" id="star-rating-3" type="radio" name="rating" value="3">
                    <label class="star-rating__ico fa fa-star-o fa-lg" for="star-rating-3"
                           title="3 out of 5 stars"></label>
                    <input class="star-rating__input" id="star-rating-2" type="radio" name="rating" value="2">
                    <label class="star-rating__ico fa fa-star-o fa-lg" for="star-rating-2"
                           title="2 out of 5 stars"></label>
                    <input class="star-rating__input" id="star-rating-1" type="radio" name="rating" value="1">
                    <label class="star-rating__ico fa fa-star-o fa-lg" for="star-rating-1"
                           title="1 out of 5 stars"></label>
                </div>
            </div>

        </div>

        <!--Contains a button that will show the hotspots closest to you based on geolocation.-->
        <div>
            <p>Use My Location?</p>
            <input type="checkbox" name="location" id="myLocationButton"></input>
            <input type="text" name="latitude" id="latitude">  <!--Will later hold the lat if my location is used -->
            <input type="text" name="longitude" id="longitude">  <!--Will later hold the long if my location is used -->
        </div>

        <input type="submit" value="Search" id="searchButton"><!--Search button.-->

    </form>
    <!-- Error message that appears when a validation error occurs. -->
    <div id="errormessage"></div>

</div>


<!--The footer of the page, containing authorship information.-->
<?php include 'footer.inc'; ?>

</body>
</html>