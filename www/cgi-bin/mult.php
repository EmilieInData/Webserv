#!/usr/bin/php-cgi
<?php
    // Set default values
    $result = null;
    $error_message = '';
    $val_a = null;
    $val_b = null;

    // Check if the required parameters 'a' and 'b' are present in the URL
    if (isset($_GET['a']) && isset($_GET['b'])) {
        $val_a = $_GET['a'];
        $val_b = $_GET['b'];

        // Check if both parameters are numeric
        if (is_numeric($val_a) && is_numeric($val_b)) {
            // Perform the multiplication
            $result = (float)$val_a * (float)$val_b;
        } else {
            // Set an error message for non-numeric input
            $error_message = 'Error: Both parameters must be numbers.';
        }
    } else {
        // Set an error message if parameters are missing
        $error_message = 'Please provide two numbers to multiply. Example: ?a=5&b=10';
    }

    // --- Start HTTP Output ---
    // This line is crucial for CGI; it separates headers from the body.
    // echo "Content-Type: text/html\r\n\r\n";
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>PHP Multiplier</title>
    <style>
        body { 
            font-family: Arial, sans-serif; 
            display: flex; 
            justify-content: center; 
            align-items: center; 
            height: 100vh; 
            margin: 0; 
            background-color: #f0f2f5; 
            color: #333;
            text-align: center;
        }
        .container {
            padding: 40px;
            border-radius: 10px;
            background-color: #fff;
            box-shadow: 0 4px 12px rgba(0,0,0,0.1);
        }
        .result {
            font-size: 5rem;
            font-weight: bold;
            color: #007bff;
        }
        .error {
            font-size: 1.2rem;
            color: #d9534f;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>PHP Multiplication Result</h1>
        
        <?php if ($result !== null): ?>
            <p class="result"><?php echo htmlspecialchars($result); ?></p>
            <p>Calculation: <?php echo htmlspecialchars($val_a) . ' × ' . htmlspecialchars($val_b); ?></p>
        
        <?php else: ?>
            <p class="error"><?php echo htmlspecialchars($error_message); ?></p>
        
        <?php endif; ?>

    </div>
</body>
</html>