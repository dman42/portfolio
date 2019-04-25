<!DOCTYPE html>
<html>
<head>
    <title>Results Page</title> <!--Title of the page.-->
    <meta charset="utf-8">
    <?php
    // below the meta data for the Results page is generated
    include 'htmlGenerator.php';
    $imageSrc = (isset($_SERVER['HTTPS']) ? "https" : "http") . "://$_SERVER[HTTP_HOST]" . dirname($_SERVER['REQUEST_URI']) . "/img/hsmap.png";
    $url = (isset($_SERVER['HTTPS']) ? "https" : "http") . "://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]";
    genMetaData("Browse through the personalized list of hot-spots and find the one that suits you best!",
        "Results", $imageSrc, $url);
    genHomeScreenData(); // generates meta data for home screens
    ?>
    <link href="fonts/font-awesome.min.css" rel="stylesheet"> <!--Link to the FontAwesome font -->
    <link href="main.css" type="text/css" rel="stylesheet"><!--Link to the main css stylesheet-->
    <link rel="stylesheet" type="text/css" href="ie.css"><!--Link to css styling for Internet Explorer-->
    <script src="https://maps.googleapis.com/maps/api/js?key=AIzaSyAR0LF2vwEdAf-jQDa5FuDFi9B2H5SYMrU">
    </script>
    <script type="text/javascript" src="main.js"></script><!--Link to the main javascript code -->
    <!-- API AIzaSyAR0LF2vwEdAf-jQDa5FuDFi9B2H5SYMrU  -->
</head>
<body onload="initMap(hotspotsForMap, 'map-hotspots', myLocDetails)" ;>

<?php
session_start(); // make session
include 'TopOfPage.inc'; // The header of the page, containing the title, tagline, wi-fi icon, and username if logged in
include 'operationsDB.php';
$db = connectToDB("registration-area");
?>

<!--Main content area of the page. -->
<div class="search-results">

    <!-- Title of the results page, which will change depending on the query -->
    <div id="results-title">
        <?php
        $searchString = "<h2>Search results for: "; // The title of the page.

        // This boolean will check if commas need to be included in the title where appropiate.
        $firstInstance = true;

        // If the hotspot's suburb was selected, the name will register in the title.
        if (isset($_POST['suburb']) && $_POST['suburb'] != "none") {
            $searchString .= $_POST['suburb'];
            $firstInstance = false;
        }

        // If the hotspot's name was properly searched, with letters only,
        // the name will register in the title.
        if (isset($_POST['name']) &&  preg_match('/^[a-zA-Z]+$/', str_replace(' ', '', $_POST['name']))) {
            // If this isn't the first search parameter, adds a comma to the title for separation.
            if (!$firstInstance) {
                $searchString .= ", ";
            }
            $searchString .= $_POST['name'];
            $firstInstance = false;
        }
        // If a rating was selected the rating will register in the title.
        if (isset($_POST['rating'])) {
            // If this isn't the first search parameter, adds a comma to the title for separation.
            if (!$firstInstance) {
                $searchString .= ", ";
            }
            $searchString .= $_POST['rating'] . " star(s)";
            $firstInstance = false;
        }

        //If no search parameters have been specified, title says all items are shown.
        if ($firstInstance) {
            $searchString = "<h2>ALL ITEMS SHOWN";
        }

        // Specifies in title whether items are sorted by distance from user.
        if (isset($_POST['location'])) {
            $searchString .= " (up to 10km, sorted by distance)";
            $firstInstance = false;
        }

        $searchString .= "</h2>";
        echo "$searchString"; //Prints the string.
        ?>

        <a href="SearchPage.php">&lt; Try Again</a> <!--Returns to the Search Page -->
    </div>

    <div id="hotspots">

        <!-- This is the div that contains the information of all the retrieved hotspots-->
        <div class="retrieved-hotspots">

            <!-- Prints the entry for each search result, with the name in big letters,
            followed by the rating and address of the hotspot.-->

            <?php
            $nameDataPair = array(); // will hold name and value pairs for writing to DB

            if ($firstInstance) { //If no search parameters have been specified, shows all items.
                $sqlcode = "SELECT * FROM Items";
            } else {
                $moreThanOneSearch = false; // Checks if there is more than one search parameter.
                $sqlcode = "SELECT * FROM Items WHERE ";
                // Searches database for hotspots within specified suburb.
                if (isset($_POST['suburb']) && $_POST['suburb'] != "none") {
                    //$sqlcode .= "suburb LIKE '%" . $_POST['suburb'] . "%'";
                    $sqlcode .= "suburb LIKE CONCAT('%', :suburb, '%')";
                    array_push($nameDataPair, [':suburb', $_POST['suburb']]);
                    $moreThanOneSearch = true;
                }

                // Searches database for hotspots with part of the specified name.
                if (isset($_POST['name']) &&  preg_match('/^[a-zA-Z]+$/', str_replace(' ', '', $_POST['name']))) {
                    if ($moreThanOneSearch) { // Adds a connecting "AND" if neccesary.
                        $sqlcode .= " AND ";
                    }
                    //$sqlcode .= "name LIKE '%" . $_POST['name'] . "%'";
                    $sqlcode .= "name LIKE CONCAT('%', :name, '%')";
                    array_push($nameDataPair, [':name', $_POST['name']]);
                    $moreThanOneSearch = true;
                }

                // Searches database for hotspots with the specified rating.
                if (isset($_POST['rating'])) {
                    if ($moreThanOneSearch) { // Adds a connecting "AND" if neccesary.
                        $sqlcode .= " AND ";
                    }
                    //$sqlcode .= "rating=" . $_POST['rating'];
                    $sqlcode .= "rating=:rating";
                    array_push($nameDataPair, [':rating', $_POST['rating']]);
                }
            }


            // results sorted based on distance
            $myLocDetails = array();
            if (isset($_POST['location'])) {
                $myLat = $_POST['latitude'];
                $myLong = $_POST['longitude'];

                if ($moreThanOneSearch) { // Adds a connecting "AND" if neccesary.
                    $sqlcode .= " AND ";
                }
                // only hotspots within 10km
                //$sqlcode .=" calcDistance(" . $myLat . ", " . $myLong . ",items.latitude, items.longitude) < 10000";
                $sqlcode .=" calcDistance(:myLat, :myLong, items.latitude, items.longitude) < 10000";

                //$sqlcode .= " ORDER BY calcDistance(" . $myLat . ", " . $myLong . ",items.latitude, items.longitude)";
                $sqlcode .= " ORDER BY calcDistance(:myLat, :myLong, items.latitude, items.longitude)";

                array_push($nameDataPair, [':myLat', $myLat]);
                array_push($nameDataPair, [':myLong', $myLong]);

                $myLocDetails = array("lat" => $myLat, "lng" => $myLong);
            }
            $sqlcode .= ";"; // End of sql request.
            //echo "$sqlcode"; // Uncomment to test what sql is being sent to the database.
            $resultCount = 0; // Variable that counts the amount of results from the query.

            // bind values
            $stmt = $db->prepare($sqlcode);
            foreach ($nameDataPair as $pair){
                $stmt->bindValue($pair[0], $pair[1]);
            }
            $stmt->execute();
            //echo $sqlcode;

            // For each result from the query, the name, rating, suburb, and address are
            // formatted a certain way, linking to the appropriate pages.
            // $query_result = $db->query($sqlcode);
            $query_result = $stmt->fetchAll();
            foreach ($query_result as $row) {
                $linkToPage = "ItemPage.php?itemID=" . $row['itemID'];
                echo "<div class='entry'>
						<a href='" . $linkToPage . "'><h3>" . $row['name'] . "</h3></a>
						<div>
							<div class='stars'>
								<div class='star-rating__wrap' title='" . $row['rating'] . " out of 5 stars'>";
                // Counts the unfilled stars based on the hotspot's rating.
                for ($i = 0; $i < (5 - $row['rating']); $i++) {
                    echo "<label class='star-static fa fa-star-o fa-lg'></label>";
                }

                // Counts the unfilled stars based on the hotspot's rating.
                for ($i = 0; $i < $row['rating']; $i++) {
                    echo "<label class='star-static fa fa-star fa-lg'></label>";
                }

                echo "</div>
							</div>
							<p>" . $row['address'] . ", " . $row['suburb'] . "</p>
							</div>
						</div>";
                $resultCount++; // Result count increments.
            }

            // If the query returned no results, a "No results found" is
            // returned instead of a list.
            if ($resultCount == 0) {
                echo "<h2>No results found. :(  </h2>";
            }

            // populate map with what was retrieved
            $stmt->execute();
            $query_result = $stmt->fetchAll();
            $hotspotsForMap = array();
            foreach ($query_result as $row) {
                // get hotspot details
                $hotspot = array(
                    "name" => $row['name'],
                    "address" => $row['address'],
                    "suburb" => $row['suburb'],
                    "lat" => $row['latitude'],
                    "lng" => $row['longitude'],
                    "url" => "ItemPage.php?itemID=" . $row['itemID']
                );
                array_push($hotspotsForMap, $hotspot); // add hotspot to list
            }
            $hotspotsToString = json_encode($hotspotsForMap);
            $myLocDetailsToString = json_encode($myLocDetails);

            ?>
        </div>
        <!--This is the map of retrieved hotspots-->
        <div id="map-hotspots">
        </div>

        <!-- Error message that appears when a search error occurs. -->
        <div id="errormessage"></div>

    </div>


    <!--The footer of the page, containing authorship information.-->
    <?php include 'footer.inc'; ?>

    <!--Send hotspots list and my location to map function-->
    <script>
        var hotspotsForMap = <?php echo $hotspotsToString ?>;
        var myLocDetails = <?php echo $myLocDetailsToString?>;
    </script>

</body>
</html>