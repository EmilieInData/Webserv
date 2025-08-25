#!/usr/bin/php
<?php
// --- Script Logic ---
$result = null;
$error_message = null;
$params = [];

// Manually parse the query string from the environment variable.
if (isset($_SERVER['QUERY_STRING']) && !empty($_SERVER['QUERY_STRING'])) {
    parse_str($_SERVER['QUERY_STRING'], $params);
}

// Check if the parsed parameters 'a' and 'b' exist and are numeric.
if (isset($params['a']) && isset($params['b']) && is_numeric($params['a']) && is_numeric($params['b'])) {
    // Perform the multiplication.
    $val_a = (float)$params['a'];
    $val_b = (float)$params['b'];
    $result = $val_a * $val_b;
} else {
    // Set an error message if parameters are missing or invalid.
    $error_message = "Error: Please provide two numbers in the URL. Example: <strong>?a=8&b=7</strong>";
}

// --- HTTP Header & Output ---

header("Content-Type: text/html");
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
    <div style="font-size: 400px; font-weight: bold; color: blue; text-align: center;">
        <?php echo $result; ?>
    </div>
<?php endif; ?>

</body>
</html>