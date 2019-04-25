<div class="header">

    <div>
    <img src="img/wifi.png" height="100" width="100">
    <h1>Brisbane Hot-Spot Finder</h1>
    </div>

    <?php
            // show welcome if logged in
			if (isset($_SESSION['loginUsername'])){
                $string = "<p>Hi ";
			    $string .= $_SESSION['loginUsername'];
                $string .= "!</p>";
                echo "$string";
			}
    ?>

</div>

<!--Line that divides header and navigation menu-->
<hr>


<!--Navigation menu icon-->
<div id="hamburger" onclick="toggleMenu();">
	<img src="img/hamburger.png" alt="menu icon" height="20" width="20">
</div>


<!--Navigation menu that contains links to all the other important php pages. -->
<div class="nav-menu">
    <a  class="menu-button" id="search" href="SearchPage.php">SEARCH</a>

    <?php
        // if the user is logged in, hide the reistration and login pages
        if (! isset($_SESSION['loginUsername'])){
           echo "                       
		        <a class='menu-button' id='register' href='Registration.php'>REGISTER</a>	
		        <a class='menu-button' id='login' href='Login.php'>LOGIN</a>";
        }
        else{
            echo "
		        <a class='menu-button' id='login' href='Logout.php'>LOGOUT</a>";
        }
    ?>
    
</div>