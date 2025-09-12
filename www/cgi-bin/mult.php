#!/usr/bin/php-cgi
<?php

// 1. For CGI, PRINT the header string directly to standard output.
// The double CRLF (\r\n\r\n) is crucial: it ends the header and adds the mandatory blank line
// that separates headers from the body. Your C++ server requires this.

// --- Script Logic ---
$result = null;
$error_message = null;

// Check if 'a' and 'b' are set and are numeric in the GET request.
if (isset($_GET['a']) && isset($_GET['b']) && is_numeric($_GET['a']) && is_numeric($_GET['b'])) {
    $val_a = (int)$_GET['a'];
    $val_b = (int)$_GET['b'];
    $result = $val_a * $val_b; // Multiplication as per filename
} else {
    // This block runs if parameters were missing or not valid numbers.
    $error_message = "Error: Please provide two numbers in the URL. Example: <strong>?a=5&b=10</strong>";
}

// --- HTML Output ---
?>
<html>
<head><title>CGI Big Number Multiplier</title></head>
<body style="display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0;">

<?php
// Check if an error occurred and display the appropriate message.
if ($error_message !== null) {
    echo "<p>{$error_message}</p>";
} else {
    // If successful, display the result with custom style.
    echo '<div style="font-size: 40px; font-weight: bold; color: blue; text-align: center;">';
    echo $result;
    echo '</div>';
}
?>

</body>
</html>