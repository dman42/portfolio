<?php
include_once 'serverValidations.php';

// this function connects to the database, returns it if successful, null otherwise
function connectToDB($className)
{
    // Checks to see if the database has successfully connected or not.
    try {
        // Database is created below.
        //$db = new PDO('mysql:host=localhost;dbname=hotspotDatabase;charset=utf8mb4', 'root', '');
        $db = new PDO('mysql:host=localhost;dbname=n9939768;charset=utf8mb4', 'n9939768', 'CAB230unsafe');
        // set the PDO error mode to exception
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        return $db;

    } catch (PDOException $e)  // if the connection to DB failed, load only the footer and an error
    {
        echo "Connection failed: " . $e->getMessage();  //temporarily
        echo "<div class={$className}>Could not connect to database. Sorry...</div>"; // make empty div to fill page and push footer
        include 'footer.inc';
        die();  // stop execution of .php
    }
}


// this function validates the review comment and rating and writes it to the database
// it also calls the updateAverageRating() function to update the average review considering
function sendReviewToDB($itemID, &$errors, &$db)
{
    // if comment and rating have been given
    if (isset($_POST['textReview']) && strlen($_POST['textReview']) != 0 && isset($_POST['rating'])) {

        // clean comment to avoid cross scripting
        $commentText = cleanInput($_POST['textReview']);
        // check if unallowed characters, group of characters exist
        if (containsUnallowed($commentText)) {
            $errors['reviewError'] = 'The review contains links to other websites!';
            return;
        }

        // check if rating is an integer between 1 and 5
        if ((!is_numeric($_POST['rating'])) || ($_POST['rating'] < 1) || ($_POST['rating'] > 5)) {
            $errors['ratingError'] = 'The rating is not an elegible integer!';
            return;
        }
        $rating = 6 - $_POST['rating'];
        $writeSql = $db->prepare("INSERT INTO reviews (commentText, rating, datePosted, memberID, itemID) VALUES (:commentText, :rating, CURDATE(), :memberID, :itemID)");
        $writeSql->bindValue(':commentText', $commentText);
        $writeSql->bindValue(':rating', $rating);
        $writeSql->bindValue('memberID', $_SESSION['loginUserID']);
        $writeSql->bindValue(':itemID', $itemID);
        $writeSql->execute();
        //$writeSql->debugDumpParams();
        //echo "New review added successfully";


        echo "";  // add review from mobile fix ???
        // I honestly do not know why trying to add a review from mobile only works if there is an echo statement here
        // if there isn't, the review text is not submitted, but the rating is, and the average rating is correctly updated
        // this only happens in mobile mode

        // update the average rating of the hotspot
        updateAverageRating($itemID, $db);

    } else {
        $errors['submitError'] = "Either rating or review is missing...";
    }
}


// this function updates the average rating of a hotspot
function updateAverageRating($itemID, &$db)
{
    // sql string for reading all reviews of a hotspot
    $readSql = $db->prepare("SELECT rating FROM reviews WHERE itemID = :itemID");
    $readSql->bindValue(':itemID', $itemID);

    // fetch reviews of hotspot based on itemID
    if (!$readSql->execute()) {
        return;
    }
    $allRatings = $readSql->fetchAll();

    $totalRating = 0.0;
    $numberRatings = 0;
    $avgRating = 0;

    // count total rating and compute average rating
    foreach ($allRatings as $rat) {
        $totalRating = $totalRating + $rat['rating'];
        $numberRatings++;
    }
    if ($numberRatings > 0) {
        $avgRating = round($totalRating / $numberRatings);
    }

    // update the average rating of the hotspot
    $writeSql = $db->prepare("UPDATE items SET rating= :avgRating WHERE itemID= :itemID");
    $writeSql->bindValue(':avgRating', $avgRating);
    $writeSql->bindValue(':itemID', $itemID);
    $writeSql->execute();
    //echo " Average review updated successfully";
}


// this function retrieves past reviews of a hotspot from the database, and returns them
function readReviewFromDB($itemID, &$errors, &$db)
{
    // sql string for retrieving previous reviews
    // since the username of reviewer is needed, memberID is used to join the two tables
    $readSql = $db->prepare("SELECT * FROM reviews INNER JOIN members ON reviews.memberID=members.memberID WHERE itemID = :itemID ORDER BY datePosted DESC");
    $readSql->bindValue(':itemID', $itemID);
   // $readSql->execute();

    // if query was not successful, add error
    if (!$readSql->execute()) {
        $errors['retrieveReviewError'] = "There was a problem retrieving the reviews for this hotspot.";
        return;
    }

    $allReviews = $readSql->fetchAll();
    // if the query did not return any review, add as error
    if (sizeof($allReviews) == 0) {
        $errors['noReviewsError'] = "No one submitted a review for this hotspot yet. Be the first to do it!";
        return;
    }

    return $allReviews;
}


// retrieve hotspot details from the database based on itemID, returns false if not successful
function getHotspotFromDB($itemID, &$hotSpotExists, &$errorsItemDetails, &$db){
    // prepare sql string
    $readSql = $db->prepare("SELECT * FROM Items WHERE itemID = :itemID");
    $readSql->bindValue(':itemID', $itemID);

    // if there was an error executing the query, include the error
    if (!$readSql->execute()) {
        $errorsItemDetails['noDetailsError'] = "Could not find the details for this hotspot.";
        $hotSpotExists = false;
    }

    // if nothing was retrieved from the database, include the error
    $row = $readSql->fetch();
    if ($row === false || sizeof($row) == 0) {
        $errorsItemDetails['noHotspotError'] = "This hotspot does not exist";
        $hotSpotExists = false;
    }

    return $row;
}

?>