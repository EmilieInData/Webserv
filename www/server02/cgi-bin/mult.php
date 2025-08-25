#!/usr/bin/php
<?php
// --- Script Logic ---

// Set default values.
$result = null;
$error_message = null;
$params = [];

// Check for parameters in both GET and POST requests.
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $params = $_POST;
} else {
    $params = $_GET;
}

// Check if parameters 'a' and 'b' are provided and are numeric.
if (isset($params['a']) && isset($params['b']) && is_numeric($params['a']) && is_numeric($params['b'])) {
    // Perform the multiplication.
    $val_a = (float)$params['a'];
    $val_b = (float)$params['b'];
    $result = $val_a * $val_b;
} else {
    // Set an error message if parameters are missing or invalid.
    $error_message = "Error: Please provide two numbers. Example: <strong>?a=7&b=6</strong>";
}

// --- HTTP Header ---

// The Content-Type header is mandatory for the server to understand the response.
header("Content-Type: text/html");
// The blank line below is also mandatory and separates headers from the body.

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>CGI Big Number Multiplier</title>
</head>
<body style="display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; font-family: sans-serif;">

<?php if ($error_message): ?>
    <p><?php echo $error_message; ?></p>
<?php else: ?>
    <div style="font-size: 200px; font-weight: bold; color: #4CAF50; text-align: center;">
        <?php echo $result; ?>
    </div>
<?php endif; ?>

</body>
</html>