<?php

// generates HTML details-area according to given inputs for a specific hotspot
function genDetailsArea($row, $suburb, $postal)
{
    echo "<div id='details-area'>
        <div class='details'>

            <div id='item-title'>
                <h2>{$row['name']}</h2>
                <a href='ResultsPage.php'>&lt; Try Again</a>
            </div>

            <div itemprop='address' itemscope itemtype='http://schema.org/PostalAddress'>
            <h3>Address:</h3>
            <div itemprop='streetAddress'>{$row['address']}</div>
            <meta itemprop='addressLocality' content=$suburb>
            <meta itemprop='postalCode' content=$postal>
            </div>

            <div itemprop='geo' itemscope itemtype='http://schema.org/GeoCoordinates'>
                <meta itemprop='latitude' content='{$row['latitude']}' />
                <meta itemprop='longitude' content='{$row['longitude']}' />
            </div>

            <div>
                <h3>Average Rating:</h3>
                <div class='stars' itemprop='aggregateRating' itemscope itemtype='http://schema.org/AggregateRating'>
                    <meta itemprop='ratingValue' content='{$row['rating']}'/>
                    <div class='star-rating__wrap' title='{$row['rating']}' out of 5 stars>";

    // generate html of stars rating
    genFixedStars($row['rating']);

    echo "</div>
                </div>
            </div>
        </div>

        <!--This is the map of retrieved hotspot-->
        <div id='map-hotspot'>
        </div>
    </div>";
}


// generates HTML form for leaving a rating and a review for a specific hotspot, based on itemID
function genSubmitReviewForm($row, $alreadyTyped)
{

    echo "
    <form action='ItemPage.php?itemID={$row['itemID']}' method='post'>
    <div class='submitreview' >
        <textarea placeholder='Write a review!' name='textReview'>{$alreadyTyped}</textarea>
        <div class='submit-footer'>
            <div class='stars'>
                <div class='star-rating__wrap'>
                    <input class='star-rating__input' id='star-rating-1' type='radio' name='rating' value='1'>
                    <label class='star-rating__ico fa fa-star-o fa-lg' for='star-rating-1'
                           title='1 out of 5 stars'></label>
                    <input class='star-rating__input' id='star-rating-2' type='radio' name='rating' value='2'>
                    <label class='star-rating__ico fa fa-star-o fa-lg' for='star-rating-2'
                           title='2 out of 5 stars'></label>
                    <input class='star-rating__input' id='star-rating-3' type='radio' name='rating' value='3'>
                    <label class='star-rating__ico fa fa-star-o fa-lg' for='star-rating-3'
                           title='3 out of 5 stars'></label>
                    <input class='star-rating__input' id='star-rating-4' type='radio' name='rating' value='4'>
                    <label class='star-rating__ico fa fa-star-o fa-lg' for='star-rating-4'
                           title='4 out of 5 stars'></label>
                    <input class='star-rating__input' id='star-rating-5' type='radio' name='rating' value='5'>
                    <label class='star-rating__ico fa fa-star-o fa-lg' for='star-rating-5'
                           title='5 out of 5 stars'></label>
                </div>
            </div>

            <input type='submit' value='Submit review' name='submitReview'> <!--Button to submit review.-->


        </div>
        <p>You must login to rate/write a review</p>
    </div>
    </form>    
    ";

}


// generates the HTML consisting all the past reviews for a hotspot
function genPastReviewsArea($allReviews)
{

    foreach ($allReviews as $row) {
        echo "<div class='review' itemprop='review' itemscope itemtype='http://schema.org/Review'>
            <div class='stars' itemprop='reviewRating' itemscope itemtype='http://schema.org/Rating'>
                <meta itemprop='worstRating' content = '1'/>
                <meta itemprop='ratingValue' content='{$row['rating']}'/>
                <meta itemprop='bestRating' content='5'/>
                <div class='star-rating__wrap' title='{$row['rating']}' ' out of 5 stars'>";

        // generate html of stars
        genFixedStars($row['rating']);

        echo "</div>
    </div>

    <div class='comment'>
        <p itemprop='description' >{$row['commentText']}
        <sub><em itemprop='author' >{$row['username']}</em><span itemprop='datePublished' content='{$row['datePosted']}'></span> added on {$row['datePosted']}</sub>
        </p>
    </div>
    </div>";
    }
}


// generates the HTML of un-editable stars
function genFixedStars($ratingValue)
{

    // Counts the unfilled stars based on the hotspot's rating.
    for ($i = 0; $i < (5 - $ratingValue); $i++) {
        echo "<label class='star-static fa fa-star-o fa-lg'></label>";
    }

    // Counts the unfilled stars based on the hotspot's rating.
    for ($i = 0; $i < $ratingValue; $i++) {
        echo "<label class='star-static fa fa-star fa-lg'></label>";
    }

}


// wll generate the list of suburbs for the search page
function genSuburbList(&$db)
{
    $query = $db->query("SELECT suburb FROM Items GROUP BY suburb;");
    foreach ($query as $row) {
        echo "<option value={$row['suburb']}>{$row['suburb']}</option>";
    }
}


// function creates the meta data required for a specific page, given the inputs
function genMetaData($decription, $name, $image, $url)
{
    echo "
    <meta name='author' content='David McClarty, Musat Valentina'>
    <meta name='keywords' content='hot-spots, Brisbane, rate, review'>
    <meta name='robots' content='noindex'> <!-- do not index this page -->
    <!-- search engine -->
    <meta name='description' content='{$decription}'>
    <meta name='image' content='{$image}'>
    <!-- Google -->
    <meta itemprop='name' content='{$name}'>
    <meta itemprop='description' content='{$decription}'>
    <meta itemprop='image' content='{$image}'>
    <!-- Twitter -->
    <meta name='twitter:card' content='summary'>
    <meta name='twitter:title' content='{$name}'>
    <meta name='twitter:description' content='{$decription}'>
    <meta name='twitter:image:src' content='{$image}'>
    <!-- OG -->
    <meta name='og:title' content='{$name}'>
    <meta name='og:description' content='{$decription}'>
    <meta name='og:image' content='{$image}'>
    <meta name='og:url' content='{$url}'>
    <meta name='og:site_name' content='Brisbane Hot-Spot Finder'>
    <meta name='og:type' content='website'>    
    ";
}

function genHomeScreenData(){
    echo "
    <!-- Mobile meta tags for home screens-->   
    <link rel='icon' sizes='192x192' href='img/wifi.png'> 
    <link rel='icon' sizes='128x128' href='img/wifi.png'>
    <link rel='apple-touch-icon' sizes='128x128' href='img/wifi.png'>
    <link rel='apple-touch-icon-precomposed' sizes='128x128' href='img/wifi.png'>
    <meta name='apple-mobile-web-app-capable' content='yes' />
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    ";
}

?>